/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "ocpn_plugin.h"
#include "LineBufferOverlay.h"

void LineBuffer::pushLine( float x0, float y0, float x1, float y1 )
{
    buffer.push_back(x0);
    buffer.push_back(y0);
    buffer.push_back(x1);
    buffer.push_back(y1);
}

void LineBuffer::Finalize()
{
    count = buffer.size() / 4;
    delete [] lines;
    lines = new float[buffer.size()];
    int i = 0;
    for(std::list <float>::iterator it = buffer.begin(); it != buffer.end(); it++)
        lines[i++] = *it;

    buffer.clear();
}

void LineBuffer::pushTransformedBuffer(LineBuffer &buffer, int x, int y, double ang, bool south, bool head, int lineWidth)
{
    // transform vertexes by angle
    float sa = sinf( ang ), ca = cosf( ang );

    float m[2][2] = {{ ca, -sa},
                     { sa,  ca}};

    if(south) {
        m[0][0] = -m[0][0];
        m[1][0] = -m[1][0];
    }
    if(head) {
        // Calculate the offset to put the head of the arrow on the point
        // (and not the middle of the arrow taking into account lineWidth).
        const int windArrowSize = (int)(DEFAULT_WIND_ARROW_SIZE * (lineWidth/DEFAULT_WIND_ARROW_SIZE_FACTOR));
        x += (int)((windArrowSize / 2 + lineWidth) * sa);
        y -= (int)((windArrowSize / 2 + lineWidth) * ca);
    }

    for(int i=0; i < 2*buffer.count; i+=2) {
        float *k = buffer.lines + 2*i;
        pushLine(k[0]*m[0][0] + k[1]*m[0][1] + x , k[0]*m[1][0] + k[1]*m[1][1] + y,
                 k[2]*m[0][0] + k[3]*m[0][1] + x, k[2]*m[1][0] + k[3]*m[1][1] + y);
    }
}

void LineBuffer::draw(wxDC *dc)
{
    if( dc ) {
        for(int i=0; i < count; i++) {
            float *l = lines + 4*i;
#if wxUSE_GRAPHICS_CONTEXT && 0
            if( m_hiDefGraphics && m_gdc )
                m_gdc->StrokeLine( l[0], l[1], l[2], l[3] );
            else
#endif
                dc->DrawLine( l[0], l[1], l[2], l[3] );
        }
    } else {                       // OpenGL mode
#if defined(ocpnUSE_GL) && !defined(__OCPN__ANDROID__)
        glVertexPointer(2, GL_FLOAT, 2*sizeof(float), lines);
        glDrawArrays(GL_LINES, 0, 2*count);
#endif
    }
}

void WindBarbLineBuffer::pushPetiteBarbule( int b, int lineWidth )
{
    pushLine( 0, b, -(5 * (lineWidth/DEFAULT_WIND_ARROW_SIZE_FACTOR)), b - 2 );
}

void WindBarbLineBuffer::pushGrandeBarbule( int b, int lineWidth )
{
    pushLine( 0, b, -(10 * (lineWidth/DEFAULT_WIND_ARROW_SIZE_FACTOR)), b - 4 );
}

void WindBarbLineBuffer::pushTriangle( int b, int lineWidth )
{
    pushLine( 0, b,     -(10 * (lineWidth/DEFAULT_WIND_ARROW_SIZE_FACTOR)), b - 4 );
    pushLine( 0, b - 8, -(10 * (lineWidth/DEFAULT_WIND_ARROW_SIZE_FACTOR)), b - 4 );
}


LineBufferOverlay g_LineBufferOverlay;
// Customization WindBarbsOnRoute
LineBufferOverlay g_barbsOnRoute_LineBufferOverlay;

LineBufferOverlay::LineBufferOverlay()
{
    m_lineWidth = 2;
    setLineBuffer();
}

void LineBufferOverlay::setLineWidth(int lineWidth)
{
    m_lineWidth = lineWidth;
    setLineBuffer();
}

void LineBufferOverlay::setLineBuffer()
{
    // Set arrow size
    // which is proportional to the line width of the barbs
    const int windArrowSize = (int)(DEFAULT_WIND_ARROW_SIZE * (m_lineWidth/DEFAULT_WIND_ARROW_SIZE_FACTOR));

    // Generate the wind arrow cache
    
    // Wind is very light, draw a circle
    // also proportional to line width
    int r = 2 * m_lineWidth;
    int i=0;
    double s = 2 * M_PI / 10.;
    for( double a = 0; a < 2 * M_PI; a += s )
        m_WindArrowCache[0].pushLine(r*sin(a), r*cos(a), r*sin(a+s), r*cos(a+s));

    int dec = windArrowSize / 2;

    // the barbed arrows
    for(i=1; i<14; i++) {
        LineBuffer &arrow = m_WindArrowCache[i];

        arrow.pushLine( 0, dec,  0, dec - windArrowSize );   // hampe
        // Right arrow
        arrow.pushLine(0, dec - ceil(m_lineWidth / 2 * sqrt(2)), round(dec/4 + (m_lineWidth / 2 * sqrt(2))), round(dec/2.));
        // Left arrow
        arrow.pushLine(0, dec - ceil(m_lineWidth / 2 * sqrt(2)), round(-dec/4 - (m_lineWidth / 2 * sqrt(2))), round(dec/2.));
    }

    float alpha = 0.38;
    int b1 = dec - windArrowSize + (2 * m_lineWidth) + ceil((m_lineWidth/2) * sin(alpha));  // Position of 1st barb
    int b2 = dec - windArrowSize + round((m_lineWidth/2) * sin(alpha));                    // Position of 2nd barb if >= 10 knts

    // 5 ktn
    m_WindArrowCache[1].pushPetiteBarbule( b1, m_lineWidth );
    // 10 ktn
    m_WindArrowCache[2].pushGrandeBarbule( b2, m_lineWidth );
    // 15 ktn
    m_WindArrowCache[3].pushGrandeBarbule( b2, m_lineWidth );
    m_WindArrowCache[3].pushPetiteBarbule( b2 + (2 * m_lineWidth), m_lineWidth );
    // 20 ktn
    m_WindArrowCache[4].pushGrandeBarbule( b2, m_lineWidth );
    m_WindArrowCache[4].pushGrandeBarbule( b2 + (2 * m_lineWidth), m_lineWidth );
    // 25 ktn
    m_WindArrowCache[5].pushGrandeBarbule( b2, m_lineWidth );
    m_WindArrowCache[5].pushGrandeBarbule( b2 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[5].pushPetiteBarbule( b2 + (4 * m_lineWidth), m_lineWidth );
    // 30 ktn
    m_WindArrowCache[6].pushGrandeBarbule( b2, m_lineWidth );
    m_WindArrowCache[6].pushGrandeBarbule( b2 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[6].pushGrandeBarbule( b2 + (4 * m_lineWidth), m_lineWidth );
    // 35 ktn
    m_WindArrowCache[7].pushGrandeBarbule( b2, m_lineWidth );
    m_WindArrowCache[7].pushGrandeBarbule( b2 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[7].pushGrandeBarbule( b2 + (4 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[7].pushPetiteBarbule( b2 + (6 * m_lineWidth), m_lineWidth );
    // 40 ktn
    m_WindArrowCache[8].pushGrandeBarbule( b2, m_lineWidth );
    m_WindArrowCache[8].pushGrandeBarbule( b2 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[8].pushGrandeBarbule( b2 + (4 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[8].pushGrandeBarbule( b2 + (6 * m_lineWidth), m_lineWidth );
    // 50 ktn
    m_WindArrowCache[9].pushTriangle( b1 + (2 * m_lineWidth), m_lineWidth );
    // 60 ktn
    m_WindArrowCache[10].pushTriangle( b1 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[10].pushGrandeBarbule( b1 + (4 * m_lineWidth), m_lineWidth );
    // 70 ktn
    m_WindArrowCache[11].pushTriangle( b1 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[11].pushGrandeBarbule( b1 + (4 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[11].pushGrandeBarbule( b1 + (6 * m_lineWidth), m_lineWidth );
    // 80 ktn
    m_WindArrowCache[12].pushTriangle( b1 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[12].pushGrandeBarbule( b1 + (4 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[12].pushGrandeBarbule( b1 + (6 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[12].pushGrandeBarbule( b1 + (8 * m_lineWidth), m_lineWidth );
    // > 90 ktn
    m_WindArrowCache[13].pushTriangle( b1 + (2 * m_lineWidth), m_lineWidth );
    m_WindArrowCache[13].pushTriangle( b1 + (6 * m_lineWidth), m_lineWidth );

    for(i=0; i<14; i++)
        m_WindArrowCache[i].Finalize();

    for(i=0; i< 14; i++) {
        int arrowSize = 10  +i;
        dec = arrowSize / 2;
        int h = (i > 7)?6:4 ;
        if (i >= 10) {
            m_SingleArrow[i].pushLine( 0, dec, /* */ dec, dec - arrowSize );
            m_SingleArrow[i].pushLine( 0, dec, /* */ -dec, dec - arrowSize );
            m_SingleArrow[i].pushLine( dec, dec -arrowSize, /* */ 0, dec -arrowSize +h);
            m_SingleArrow[i].pushLine( -dec, dec -arrowSize, /* */ 0, dec -arrowSize +h);
        }
        else {
            m_SingleArrow[i].pushLine( 0, dec, 0, dec - arrowSize );
            m_SingleArrow[i].pushLine( 0, dec,  h, dec - (h -1) );    // flèche
            m_SingleArrow[i].pushLine( 0, dec, -h, dec - (h -1) );   // flèche
        }
    }

    for(i=0; i<14; i++)
        m_SingleArrow[i].Finalize();

}

void LineBufferOverlay::pushWindArrowWithBarbs(LineBuffer &buffer, int x, int y, double vkn, double ang, bool south, bool head)
{
    int cacheidx;

    if( vkn < 1 )
        cacheidx = 0;
    else if( vkn < 2.5)
        cacheidx = 1;
    else if( vkn < 40 )
        cacheidx = (int)(vkn + 2.5) / 5;
    else if( vkn < 90 )
        cacheidx = (int)(vkn + 5) / 10 + 4;
    else if (vkn < 400 ) 
        cacheidx = 13;
    else 
        return;
    buffer.pushTransformedBuffer(m_WindArrowCache[cacheidx], x, y, ang, south, head, m_lineWidth);
}

void LineBufferOverlay::pushSingleArrow( LineBuffer &buffer, int x, int y, double vkn, double ang, bool south)
{
    int cacheidx = 13;
    int v = (int)(vkn*2. +0.5);
    
    if( v <= 0 )
        return;
    else if( v <= 13 )
        cacheidx = v;
    else if (v > 40)
        return;
    buffer.pushTransformedBuffer(m_SingleArrow[cacheidx], x, y, ang, south);
}

