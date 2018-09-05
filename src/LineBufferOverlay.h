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

#include "wx/wx.h"

#include <list>

#define DEFAULT_WIND_ARROW_SIZE 26
#define DEFAULT_WIND_ARROW_SIZE_FACTOR 2.3
#define DEFAULT_WIND_ARROW_LINE_WIDTH 2

class LineBuffer {
public:
    LineBuffer() { count = 0; lines = NULL; }
    ~LineBuffer() { delete [] lines; }

    void pushLine( float x0, float y0, float x1, float y1 );
    void Finalize();

    void pushTransformedBuffer(LineBuffer &buffer, int x, int y, double ang, bool south=false, bool head=false, int lineWidth=DEFAULT_WIND_ARROW_SIZE);
    void draw(wxDC *dc);
    void drawTransformed(wxDC *dc, wxPoint offset, double ang);

    int count;
    float *lines;

private:
    std::list <float> buffer;
};

class WindBarbLineBuffer : public LineBuffer
{
public:
    void pushPetiteBarbule( int b, int lineWidth=DEFAULT_WIND_ARROW_SIZE );
    void pushGrandeBarbule( int b, int lineWidth=DEFAULT_WIND_ARROW_SIZE );
    void pushTriangle( int b, int lineWidth=DEFAULT_WIND_ARROW_SIZE );
};

class LineBufferOverlay
{
public:
    LineBufferOverlay();
    void setLineBuffer();
    void setLineWidth(int lineWidth);
    void pushWindArrowWithBarbs(LineBuffer &buffer, int x, int y, double vkn, double ang, bool south=false, bool head=false);
    void pushSingleArrow( LineBuffer &buffer, int x, int y, double vkn, double ang, bool south=false);
private:

    WindBarbLineBuffer m_WindArrowCache[14];
    LineBuffer m_SingleArrow[14];
    int m_lineWidth;
};

extern LineBufferOverlay g_LineBufferOverlay;
extern LineBufferOverlay g_barbsOnRoute_LineBufferOverlay;
