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

class LineBuffer {
public:
    LineBuffer() { count = 0; lines = NULL; }
    ~LineBuffer() { delete [] lines; }

    void pushLine( float x0, float y0, float x1, float y1 );
    void Finalize();

    void pushTransformedBuffer(LineBuffer &buffer, int x, int y, double ang, bool south=false);
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
    void pushPetiteBarbule( int b );
    void pushGrandeBarbule( int b );
    void pushTriangle( int b );
};

class LineBufferOverlay
{
public:
    LineBufferOverlay();
    void pushWindArrowWithBarbs(LineBuffer &buffer, int x, int y, double vkn, double ang, bool south=false);
    void pushSingleArrow( LineBuffer &buffer, int x, int y, double vkn, double ang, bool south=false);
private:

    WindBarbLineBuffer m_WindArrowCache[14];
    LineBuffer m_SingleArrow[14];
};

extern LineBufferOverlay g_LineBufferOverlay;
