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

#include "Polar.h"

class Boat
{
public:
    Boat();
    ~Boat();

    wxString OpenXML(wxString filename, bool shortcut=true);
    wxString SaveXML(wxString filename);

    std::vector<Polar> Polars;

    int TrySwitchPolar(int curpolar, double VW, double H, double Swell);
    bool FastestPolar(int p, float H, float VW);
    void GenerateCrossOverChart(void *arg=0, void (*status)(void *, int, int)=0);

private:
    Point Interp(const Point &p0, const Point &p1, int q, bool q0, bool q1);
    void NewSegment(Point &p0, Point &p1, std::list<Segment> &segments);
    void GenerateSegments(float H, float VW, float step, bool q[4],
                          std::list<Segment> &segments, int p);

    wxString   m_last_filename;
    wxDateTime m_last_filetime;
};
