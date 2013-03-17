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

#include "wx/datetime.h"

#include <list>

class RouteMapOptions;
class Route;
class RouteHeap;
class GribRecordSet;

typedef std::list<Route*> RouteList;

/* circular linked list node for positions which take equal time to reach */
class Position
{
public:
    Position(double latitude, double longitude, Position *p=NULL);
    Position(Position *p);
    void Propagate(RouteHeap &routeheap, GribRecordSet &Grib, RouteMapOptions &options);
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
    Route(Position *p, int cnt = 1, int dir = 1);
    Route(Route *r, Route *p=NULL);
    ~Route();

    void Print();
    bool Contains(Route *r);
    void ApplyCurrent(GribRecordSet &grib, RouteMapOptions &options);
    bool FindRouteBounds(double bounds[4]);
    void RemovePosition(Position *p);
    RouteList Normalize(int level, bool inverted_regions);
    Position *ClosestPosition(double lat, double lon);
    void Propagate(RouteHeap &routeheap, GribRecordSet &Grib, RouteMapOptions &options);
    
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

    void PropagateIntoHeap(RouteHeap &routeheap, GribRecordSet &grib, RouteMapOptions &options);
    bool Contains(double lat, double lon);
  
    RouteList routes;
    wxDateTime time;
};

typedef std::list<RouteIso*> RouteIsoList;

struct RouteMapOptions {
    double EndLat, EndLon;
    double dt; /* time in seconds between propagations */

    std::list<double> DegreeSteps;
    
    double MaxDivertedCourse, MaxWindKnots, MaxSwellMeters;
    double MaxLatitude, TackingTime;
    
    int SubSteps;
    bool DetectLand, InvertedRegions, Anchoring, AllowDataDeficient;

    BoatSpeed *boat;
};

class RouteMap
{
public:
    static void Wind(double &windspeed, double &winddir,
                     double lat, double lon, wxDateTime time);
    RouteMap();
    ~RouteMap();

    void Reset(double lat, double lon, wxDateTime time);

#define LOCKING_ACCESSOR(name, flag) bool name() { Lock(); bool ret = flag; Unlock(); return ret; }
    LOCKING_ACCESSOR(Finished, m_bFinished)
    LOCKING_ACCESSOR(ReachedDestination, m_bReachedDestination)
    LOCKING_ACCESSOR(GribFailed, m_bGribFailed)

    bool Empty() { Lock(); bool empty = origin.size() == 0; Unlock(); return empty; }
    bool NeedsGrib() { Lock(); bool needsgrib = m_bNeedsGrib; Unlock(); return needsgrib; }
    void SetNewGrib(GribRecordSet *grib) { Lock(); m_NewGrib = grib; m_bNeedsGrib = false; Unlock(); }
    wxDateTime NewGribTime() { Lock(); wxDateTime time =  m_NewGribTime; Unlock(); return time; }
    bool HasGrib() { return m_NewGrib; }

    void SetOptions(RouteMapOptions &o) { Lock(); m_Options = o; Unlock(); }
    RouteMapOptions GetOptions() { Lock(); RouteMapOptions o = m_Options; Unlock(); return o; }
    void SetBoat(BoatSpeed &b) {
        Lock();
        m_boats[m_currentboat] = b;
        m_bUpdateBoat = true;
        Unlock();
    }

protected:
    RouteList ReduceHeap(RouteHeap &routeheap, RouteMapOptions &options);
    bool Propagate();
    Position *ClosestPosition(double lat, double lon);

    /* protect any member variables with mutexes if needed */
    virtual void Lock() = 0;
    virtual void Unlock() = 0;

    RouteIsoList origin; /* list of route isos in order of time */

private:
    virtual void Clear();

    RouteMapOptions m_Options;
    bool m_bNeedsGrib, m_bFinished, m_bReachedDestination, m_bGribFailed;

    GribRecordSet *m_NewGrib;
    wxDateTime m_NewGribTime;

    bool m_bUpdateBoat;
    BoatSpeed m_boats[2]; /* page flip boats to allow safe threaded access
                             without copying the whole boat very often */
    int m_currentboat;
};
