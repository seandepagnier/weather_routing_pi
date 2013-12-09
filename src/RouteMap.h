/***************************************************************************
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

struct RouteMapConfiguration;
class IsoRoute;
class GribRecordSet;

typedef std::list<IsoRoute*> IsoRouteList;

struct PlotData
{
    wxDateTime time;
    double lat, lon;
    double VBG, BG, VB, B, VW, W, VWG, WG, VC, C, WVHT;
};

class SkipPosition;

/* circular linked list node for positions which take equal time to reach */
class Position
{
public:
    Position(double latitude, double longitude, int sp=0, int t=0, Position *p=NULL);
    Position(Position *p);

    SkipPosition *BuildSkipList();

    bool GetPlotData(GribRecordSet *grib, double dt,
                     RouteMapConfiguration &configuration, PlotData &data);
    bool Propagate(IsoRouteList &routelist, GribRecordSet *Grib, wxDateTime &time,
                   RouteMapConfiguration &configuration);
    double Distance(Position *p);
    bool CrossesLand(double dlat, double dlon);

    double lat, lon;
    int sailplan; /* which sail plan in the boat we are using */
    int tacks; /* how many times we have tacked to get to this position */
    Position *parent; /* previous position in time */
    Position *prev, *next; /* doubly linked circular list of positions */

    bool propagated;
};

/* circular skip list of positions which point to where we
   change direction of quadrant.
   That way we can eliminate a long string of positions very quickly
   all go in the same direction.  */
class SkipPosition
{
public:
    SkipPosition(Position *p, int q);

    void Remove();
    SkipPosition *Copy();

    Position *point;
    SkipPosition *prev, *next;
    int quadrant;
};

/* a closed loop of positions */
class IsoRoute
{
public:
    IsoRoute(SkipPosition *p, int dir = 1);
    IsoRoute(IsoRoute *r, IsoRoute *p=NULL);
    ~IsoRoute();

    void Print(); /* for debugging */
    void PrintSkip();

    int IntersectionCount(Position &pos);
    int Contains(Position &pos, bool test_children);

    bool CompletelyContained(IsoRoute *r);
    bool ContainsRoute(IsoRoute *r);

    bool ApplyCurrents(GribRecordSet *grib, wxDateTime time, RouteMapConfiguration &configuration);
    void FindIsoRouteBounds(double bounds[4]);
    void RemovePosition(SkipPosition *s, Position *p);
    Position *ClosestPosition(double lat, double lon, double *dist=0);
    bool Propagate(IsoRouteList &routelist, GribRecordSet *Grib,
                   wxDateTime &time, RouteMapConfiguration &configuration);

    int SkipCount();
    int Count();
    void UpdateStatistics(int &routes, int &invroutes, int &skippositions, int &positions);
    
    SkipPosition *skippoints; /* skip list of positions */

    int direction; /* 1 or -1 for inverted region */
    
    IsoRoute *parent; /* outer region if a child */
    IsoRouteList children; /* inner inverted regions */
};

/* list of routes with equal time to reach */
class IsoChron
{
public:
    IsoChron(IsoRouteList r, wxDateTime t, GribRecordSet *g);
    ~IsoChron();

    void PropagateIntoList(IsoRouteList &routelist, GribRecordSet *grib,
                           wxDateTime &time, RouteMapConfiguration &configuration);
    bool Contains(Position &p);
    bool Contains(double lat, double lon);
    Position* ClosestPosition(double lat, double lon, double *dist=0);
  
    IsoRouteList routes;
    wxDateTime time;
    GribRecordSet *m_Grib;
};

typedef std::list<IsoChron*> IsoChronList;

struct RouteMapPosition {
    RouteMapPosition(wxString n, double lat0, double lon0)
    : Name(n), lat(lat0), lon(lon0) {}

    wxString Name;
    double lat, lon;
};

struct RouteMapConfiguration {
    RouteMapConfiguration () : StartLon(0), EndLon(0) {} /* avoid waiting forever in update longitudes */
    bool Update();

    wxString Start, End;
    wxDateTime StartTime;

    double dt; /* time in seconds between propagations */

    Boat boat;
    wxString boatFileName;
    
    double MaxDivertedCourse, MaxWindKnots, MaxSwellMeters;
    double MaxLatitude, MaxTacks, TackingTime;

    bool UseGrib, UseClimatology;
    bool AllowDataDeficient;
    bool DetectLand, Currents, InvertedRegions, Anchoring;

    std::list<double> DegreeSteps;

    bool (*Climatology())(int setting, wxDateTime &, double, double, double &, double &);

/* computed values */
    double StartLat, StartLon, EndLat, EndLon;

    double StartEndBearing; /* calculated from start and end */
    bool positive_longitudes; /* longitudes are either 0 to 360 or -180 to 180,
    this means the map cannot cross both 0 and 180 longitude.
    To fully support this requires a lot more logic and would probably slow the algorithm
    by about 8%.  Is it even useful?  */
};

bool operator!=(const RouteMapConfiguration &c1, const RouteMapConfiguration &c2);

class RouteMap
{
public:
    RouteMap();
    virtual ~RouteMap();

    static void PositionLatLon(wxString Name, double &lat, double &lon);

    void Reset();

#define LOCKING_ACCESSOR(name, flag) bool name() { Lock(); bool ret = flag; Unlock(); return ret; }
    LOCKING_ACCESSOR(Finished, m_bFinished)
    LOCKING_ACCESSOR(ReachedDestination, m_bReachedDestination)
    LOCKING_ACCESSOR(Valid, m_bValid)
    LOCKING_ACCESSOR(GribFailed, m_bGribFailed)
    LOCKING_ACCESSOR(ClimatologyFailed, m_bClimatologyFailed)
    LOCKING_ACCESSOR(NoData, m_bNoData)

    bool Empty() { Lock(); bool empty = origin.size() == 0; Unlock(); return empty; }
    bool NeedsGrib() { Lock(); bool needsgrib = m_bNeedsGrib; Unlock(); return needsgrib; }
    void SetNewGrib(GribRecordSet *grib) { Lock(); m_bNeedsGrib = !(m_NewGrib = grib); Unlock(); }
    wxDateTime NewTime() { Lock(); wxDateTime time =  m_NewTime; Unlock(); return time; }
    wxDateTime StartTime() { Lock(); wxDateTime time; if(origin.size()) time = origin.front()->time;
        Unlock(); return time; }
    void SetConfiguration(RouteMapConfiguration &o) { Lock();
        m_Configuration = o;
        m_bValid = m_Configuration.Update();
        m_bFinished = false;
        Unlock(); }
    RouteMapConfiguration GetConfiguration() { Lock(); RouteMapConfiguration o = m_Configuration; Unlock(); return o; }
    void GetStatistics(int &isochrons, int &routes, int &invroutes, int &skippositions, int &positions);
    bool Propagate();
    wxDateTime EndDate();

    static bool (*ClimatologyData)(int setting, wxDateTime &, double, double, double &, double &);
    static std::list<RouteMapPosition> Positions;

protected:
    virtual void Clear();
    bool ReduceList(IsoRouteList &merged, IsoRouteList &routelist, RouteMapConfiguration &configuration);
    Position *ClosestPosition(double lat, double lon, double *dist=0, bool before_last=false);

    /* protect any member variables with mutexes if needed */
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    virtual bool TestAbort() = 0;

    IsoChronList origin; /* list of route isos in order of time */
    bool m_bNeedsGrib;
    GribRecordSet *m_NewGrib;

private:
    RouteMapConfiguration m_Configuration;
    bool m_bFinished, m_bValid,
        m_bReachedDestination, m_bGribFailed, m_bClimatologyFailed, m_bNoData;

    wxDateTime m_NewTime;
};
