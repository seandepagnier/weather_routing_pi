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

void LineBuffer::pushTransformedBuffer(LineBuffer &buffer, int x, int y, double ang, bool south)
{
    // transform vertexes by angle
    float sa = sinf( ang ), ca = cosf( ang );

    float m[2][2] = {{ ca, -sa},
                     { sa,  ca}};

    if(south)
        m[0][0] = -m[0][0], m[1][0] = -m[1][0];

    for(int i=0; i < 2*buffer.count; i+=2) {
        float *k = buffer.lines + 2*i;
        pushLine(k[0]*m[0][0] + k[1]*m[0][1] + x, k[0]*m[1][0] + k[1]*m[1][1] + y,
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
#ifdef ocpnUSE_GL
        glVertexPointer(2, GL_FLOAT, 2*sizeof(float), lines);
        glDrawArrays(GL_LINES, 0, 2*count);
#endif
    }
}

void WindBarbLineBuffer::pushPetiteBarbule( int b )
{
    pushLine( 0, b, -5, b - 2 );
}

void WindBarbLineBuffer::pushGrandeBarbule( int b )
{
    pushLine( 0, b, -10, b - 4 );
}

void WindBarbLineBuffer::pushTriangle( int b )
{
    pushLine( 0, b,     -10, b - 4 );
    pushLine( 0, b - 8, -10, b - 4 );
}

LineBufferOverlay g_LineBufferOverlay;

LineBufferOverlay::LineBufferOverlay()
{
    const int windArrowSize = 26;   //set arrow size

    // Generate the wind arrow cache

    int r = 5, i=0;     // wind is very light, draw a circle
    double s = 2 * M_PI / 10.;
    for( double a = 0; a < 2 * M_PI; a += s )
        m_WindArrowCache[0].pushLine(r*sin(a), r*cos(a), r*sin(a+s), r*cos(a+s));

    int dec = windArrowSize / 2;

    // the barbed arrows
    for(i=1; i<14; i++) {
        LineBuffer &arrow = m_WindArrowCache[i];

        arrow.pushLine( 0, dec,  0, dec - windArrowSize );   // hampe
        arrow.pushLine( 0, dec,  2, dec - 5 );    // flèche
        arrow.pushLine( 0, dec, -2, dec - 5 );   // flèche
    }

    int b1 = dec - windArrowSize + 4;  // position de la 1ère barbule
    int b2 = dec - windArrowSize;  // position de la 1ère barbule si >= 10 noeuds

    // 5 ktn
    m_WindArrowCache[1].pushPetiteBarbule( b1 );
    // 10 ktn
    m_WindArrowCache[2].pushGrandeBarbule( b2 );
    // 15 ktn
    m_WindArrowCache[3].pushGrandeBarbule( b2 );
    m_WindArrowCache[3].pushPetiteBarbule( b2 + 4 );
    // 20 ktn
    m_WindArrowCache[4].pushGrandeBarbule( b2 );
    m_WindArrowCache[4].pushGrandeBarbule( b2 + 4 );
    // 25 ktn
    m_WindArrowCache[5].pushGrandeBarbule( b2 );
    m_WindArrowCache[5].pushGrandeBarbule( b2 + 4 );
    m_WindArrowCache[5].pushPetiteBarbule( b2 + 8 );
    // 30 ktn
    m_WindArrowCache[6].pushGrandeBarbule( b2 );
    m_WindArrowCache[6].pushGrandeBarbule( b2 + 4 );
    m_WindArrowCache[6].pushGrandeBarbule( b2 + 8 );
    // 35 ktn
    m_WindArrowCache[7].pushGrandeBarbule( b2 );
    m_WindArrowCache[7].pushGrandeBarbule( b2 + 4 );
    m_WindArrowCache[7].pushGrandeBarbule( b2 + 8 );
    m_WindArrowCache[7].pushPetiteBarbule( b2 + 12 );
    // 40 ktn
    m_WindArrowCache[8].pushGrandeBarbule( b2 );
    m_WindArrowCache[8].pushGrandeBarbule( b2 + 4 );
    m_WindArrowCache[8].pushGrandeBarbule( b2 + 8 );
    m_WindArrowCache[8].pushGrandeBarbule( b2 + 12 );
    // 50 ktn
    m_WindArrowCache[9].pushTriangle( b1 + 4 );
    // 60 ktn
    m_WindArrowCache[10].pushTriangle( b1 + 4 );
    m_WindArrowCache[10].pushGrandeBarbule( b1 + 8 );
    // 70 ktn
    m_WindArrowCache[11].pushTriangle( b1 + 4 );
    m_WindArrowCache[11].pushGrandeBarbule( b1 + 8 );
    m_WindArrowCache[11].pushGrandeBarbule( b1 + 12 );
    // 80 ktn
    m_WindArrowCache[12].pushTriangle( b1 + 4 );
    m_WindArrowCache[12].pushGrandeBarbule( b1 + 8 );
    m_WindArrowCache[12].pushGrandeBarbule( b1 + 12 );
    m_WindArrowCache[12].pushGrandeBarbule( b1 + 16 );
    // > 90 ktn
    m_WindArrowCache[13].pushTriangle( b1 + 4 );
    m_WindArrowCache[13].pushTriangle( b1 + 12 );

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

void LineBufferOverlay::pushWindArrowWithBarbs(LineBuffer &buffer, int x, int y, double vkn, double ang, bool south)
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
    buffer.pushTransformedBuffer(m_WindArrowCache[cacheidx], x, y, ang, south);
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

