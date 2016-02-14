/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
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

#include <string>
#include <list>
#include "libtess2/tess.h"

struct Point
{
    Point() {}
    Point(float _x, float _y) : x(_x), y(_y) {}
    float x, y;

    bool operator==(const Point& point) const { return point.x == x && point.y == y; }
};

struct Segment
{
    Segment() { p[0] = Point(0, 0), p[1] = Point(0, 0); }
    Segment(const Point &p0, const Point &p1) {
            p[0] = p0, p[1] = p1;
        }
    Point p[2];
};

struct Contour
{
    Contour(const Contour &contour) { Init(contour.points, contour.n); }
    Contour(const float *p, int n) { Init(p, n); }
    Contour(const std::list<Point> &points);
    ~Contour() { delete [] points; }

    Contour operator=(const Contour& contour) { delete [] points; Init(contour.points, contour.n); return *this; }

    void Init(const float *p, int c);
    bool CCW();
    void Reverse();

    void Simplify(float epsilon=1e-6);
    
    float *points;
    int n;
};

class PolygonRegion
{
public:
    PolygonRegion() { InitMem(); }
    PolygonRegion(int n, float *points);
    PolygonRegion(std::list<Segment> &segments);
    PolygonRegion(const std::string &str);
    ~PolygonRegion() { FreeMem(); }

    void Clear() { contours.clear(); }
    bool Empty() const { return contours.empty(); }
    void Print();

    std::string toString();

    bool Contains(float x, float y);

    void Intersect(PolygonRegion &region);
    void Union(PolygonRegion &region);
    void Subtract(PolygonRegion &region);

    void Simplify(float epsilon=1e-6);

    TESStesselator* Tesselate(bool triangles);
    
private:
    void Put( const PolygonRegion& region, int winding_rule, bool reverse);
    void PutContours(TESStesselator *tess, bool reverse) const;
    void InitMem() { memsize = 0, mem = NULL; }
    void AllocateMem();
    void FreeMem() { /*delete [] mem;*/ }

    std::list<Contour> contours;
    unsigned char *mem;
    int memsize;
};
