/******************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *   sean@depagnier.com                                                    *
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

#include <list>

class PlugIn_ViewPort;

class Route;
class RouteHeap;
class RouteMap;
typedef std::list<Route*> RouteList;

/* list of positions which take equal time to reach */
class Position
{
public:
    Position(double latitude, double longitude, Position *p=NULL);
    Position(Position *p);
    void Propagate(RouteHeap &routeheap, RouteMap &map, wxDateTime time);
    double Distance(Position *p);
    bool CrossesLand(double dlat, double dlon);
    Position *Copy();

    double lat, lon;
    Position *parent; /* previous position in time */
    Position *prev, *next; /* doubly linked circular list of positions */

    bool propagated;
};

/* a closed loop of positions */
class Route
{
public:
    Route(Position *p, int cnt, int dir);
    Route(Route *r, Route *p=NULL);
    ~Route();

    void Print();
    bool Contains(Route *r);
    void Render(PlugIn_ViewPort *vp);
    void ApplyCurrent(RouteMap &routemap, wxDateTime time);
    bool FindRouteBounds(double bounds[4]);
    void RemovePosition(Position *p);
    RouteList Normalize(int level);
    Position *ClosestPosition(double lat, double lon);
    void Propagate(RouteHeap &routeheap, RouteMap &routemap, wxDateTime time);
    
    Position *points; /* pointer to point with maximum latitude */
    int count;
  
    int direction; /* 1 or -1 for inverted region */
    
    Route *parent; /* outer region if a child */
    RouteList children; /* inner inverted regions */
};

/* contain routes in a heap to process smallest first (to minimize O(n^2) runtime of merging) */
class RouteHeap
{
public:
    RouteHeap();
    ~RouteHeap();
    void Insert(Route *r);
    Route *Remove();
    int Size() { return size; }

private:
    int size, maxsize;
    Route **Heap;
    void expand();
    int parent(int a) { return (a-!(a&1)) >> 1; }
    int child(int a) { return (a<<1) + 1; }
};

/* list of routes with equal time to reach */
class RouteIso
{
public:
  RouteIso(Position *p, wxDateTime t);
  RouteIso(RouteList r, wxDateTime t);
  ~RouteIso();

  RouteIso *Propagate(RouteMap &routemap);
  void Render(PlugIn_ViewPort *vp);

  RouteList routes;
  wxDateTime time;
};

typedef std::list<RouteIso*> RouteIsoList;

/* contains pointers to reach
   complete datastructure describing
   the optimal sailing route */
class RouteMap
{
public:
    static void Wind(double &windspeed, double &winddir,
                     double lat, double lon, wxDateTime time);
    RouteMap(BoatSpeed &b);
    ~RouteMap();

    void Propagate();
    Position *ClosestPosition(double lat, double lon);
    void Render(PlugIn_ViewPort *vp);
    void Reset(double lat, double lon, wxDateTime time);
    void Clear();
    bool SetCursorLatLon(double lat, double lon);

    double EndLat, EndLon;

    wxTimeSpan dt; /* time in seconds between propagations */

    std::list<double> DegreeSteps;

    double MaxDivertedCourse, MaxWindKnots, MaxSwellMeters;
    double MaxLatitude, TackingTime;

    int SubSteps;

    bool DetectLand, InvertedRegions, Anchoring, AllowDataDeficient;

    BoatSpeed &boat;

    RouteIsoList origin; /* initial route iso */

private:

    Position *last_cursor_position;
};
