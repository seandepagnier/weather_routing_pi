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

#include "wx/datetime.h"

#include <list>

#include "ODAPI.h"

struct RouteMapConfiguration;
class IsoRoute;
class GribRecordSet;

typedef std::list<IsoRoute*> IsoRouteList;

struct PlotData
{
    wxDateTime time;
    double lat, lon;
    double VBG, BG, VB, B, VW, W, VWG, WG, VC, C, WVHT;
    int tacks;
};

class SkipPosition;

/* circular linked list node for positions which take equal time to reach */
class Position
{
public:
    Position(double latitude, double longitude, Position *p=NULL,
             double pheading=NAN, double pbearing=NAN, int sp=-1, int t=0, int dm=0);
    Position(Position *p);

    SkipPosition *BuildSkipList();

    void GetPlotData(Position *next, double dt,
                     RouteMapConfiguration &configuration, PlotData &data);
    void GetWindData(RouteMapConfiguration &configuration, double &W, double &VW, int &data_mask);
    void GetCurrentData(RouteMapConfiguration &configuration, double &C, double &VC, int &data_mask);

    bool Propagate(IsoRouteList &routelist, RouteMapConfiguration &configuration);
    double PropagateToEnd(RouteMapConfiguration &configuration, double &H, int &data_mask);

    double Distance(Position *p);
    bool CrossesLand(double dlat, double dlon);
    int SailChanges();
    bool EntersBoundary(double dlat, double dlon);
    
    double lat, lon;

    double parent_heading; /* angle relative to true wind we sailed from parent to this position */
    double parent_bearing; /* angle relative to north */
    int polar; /* which polar in the boat we are using */
    int tacks; /* how many times we have tacked to get to this position */

    Position *parent; /* previous position in time */
    Position *prev, *next; /* doubly linked circular list of positions */

    bool propagated;
    bool drawn, copied;

    // used for rendering
    enum DataMask { GRIB_WIND=1, CLIMATOLOGY_WIND=2, DATA_DEFICIENT_WIND=4,
                    GRIB_CURRENT=8, CLIMATOLOGY_CURRENT=16, DATA_DEFICIENT_CURRENT=32 };
    int data_mask;
};

/* circular skip list of positions which point to where we
   change direction of quadrant.
   That way we can skip past a long string of positions very quickly
   when they all go in the same direction.  */
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

    void MinimizeLat();
    int IntersectionCount(Position &pos);
    int Contains(Position &pos, bool test_children);

    bool CompletelyContained(IsoRoute *r);
    bool ContainsRoute(IsoRoute *r);

    void ReduceClosePoints();
//    bool ApplyCurrents(GribRecordSet *grib, wxDateTime time, RouteMapConfiguration &configuration);
    void FindIsoRouteBounds(double bounds[4]);

    void RemovePosition(SkipPosition *s, Position *p);
    Position *ClosestPosition(double lat, double lon, double *dist=0);
    bool Propagate(IsoRouteList &routelist, RouteMapConfiguration &configuration);
    void PropagateToEnd(RouteMapConfiguration &configuration, double &mindt,
                        Position *&endp, double &minH, bool &mintacked, int &mindata_mask);

    int SkipCount();
    int Count();
    void UpdateStatistics(int &routes, int &invroutes, int &skippositions, int &positions);
    void ResetDrawnFlag();
    
    SkipPosition *skippoints; /* skip list of positions */

    int direction; /* 1 or -1 for inverted region */
    
    IsoRoute *parent; /* outer region if a child */
    IsoRouteList children; /* inner inverted regions */
};

/* list of routes with equal time to reach */
class IsoChron
{
public:
    IsoChron(IsoRouteList r, wxDateTime t, GribRecordSet *g, bool grib_is_data_deficient);
    ~IsoChron();

    void PropagateIntoList(IsoRouteList &routelist, RouteMapConfiguration &configuration);
    bool Contains(Position &p);
    bool Contains(double lat, double lon);
    Position *ClosestPosition(double lat, double lon, double *dist=0);
    void ResetDrawnFlag();

    IsoRouteList routes;
    wxDateTime time;
    GribRecordSet *m_Grib;
    bool m_Grib_is_data_deficient;
};

typedef std::list<IsoChron*> IsoChronList;

struct RouteMapPosition {
    RouteMapPosition(wxString n, double lat0, double lon0)
    : Name(n), lat(lat0), lon(lon0) {}

    wxString Name;
    double lat, lon;
};

struct RouteMapConfiguration {
    RouteMapConfiguration () : StartLon(0), EndLon(0), grib_is_data_deficient(false) {} /* avoid waiting forever in update longitudes */
    bool Update();

    wxString Start, End;
    wxDateTime StartTime;

    double dt; /* time in seconds between propagations */

    Boat boat;
    wxString boatFileName;
    
    enum IntegratorType { NEWTON, RUNGE_KUTTA } Integrator;

    double MaxDivertedCourse, MaxCourseAngle, MaxSearchAngle, MaxTrueWindKnots, MaxApparentWindKnots;
    double MaxSwellMeters, MaxLatitude, TackingTime, WindVSCurrent;

    bool AvoidCycloneTracks;
    int CycloneMonths, CycloneDays;

    bool UseGrib;
    enum ClimatologyDataType {DISABLED, CURRENTS_ONLY, CUMULATIVE_MAP, CUMULATIVE_MINUS_CALMS, MOST_LIKELY, AVERAGE};
    enum ClimatologyDataType ClimatologyType;
    bool AllowDataDeficient;
    double WindStrength; // wind speed multiplier

    bool DetectLand, DetectBoundary, Currents, InvertedRegions, Anchoring;

    double FromDegree, ToDegree, ByDegrees;

    /* computed values */
    std::list<double> DegreeSteps;
    double StartLat, StartLon, EndLat, EndLon;

    double StartEndBearing; /* calculated from start and end */
    bool positive_longitudes; /* longitudes are either 0 to 360 or -180 to 180,
    this means the map cannot cross both 0 and 180 longitude.
    To fully support this requires a lot more logic and would probably slow the algorithm
    by about 8%.  Is it even useful?  */

    // parameters
    GribRecordSet *grib;
    wxDateTime time;
    bool grib_is_data_deficient, polar_failed, wind_data_failed;
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
    LOCKING_ACCESSOR(PolarFailed, m_bPolarFailed)
    LOCKING_ACCESSOR(NoData, m_bNoData)

    bool Empty() { Lock(); bool empty = origin.size() == 0; Unlock(); return empty; }
    bool NeedsGrib() { Lock(); bool needsgrib = m_bNeedsGrib; Unlock(); return needsgrib; }
    void RequestedGrib() { Lock(); m_bNeedsGrib=false; Unlock(); }
    void SetNewGrib(GribRecordSet *grib);
    wxDateTime NewTime() { Lock(); wxDateTime time =  m_NewTime; Unlock(); return time; }
    wxDateTime StartTime() { Lock(); wxDateTime time; if(origin.size()) time = origin.front()->time;
        Unlock(); return time; }

    void SetConfiguration(RouteMapConfiguration &o) { Lock();
        m_Configuration = o;
        m_bValid = m_Configuration.Update();
        m_bFinished = false;
        Unlock(); }
    RouteMapConfiguration GetConfiguration() {
        Lock(); RouteMapConfiguration o = m_Configuration; Unlock(); return o; }

    void GetStatistics(int &isochrons, int &routes, int &invroutes, int &skippositions, int &positions);
    bool Propagate();

    static bool (*ClimatologyData)(int setting, const wxDateTime &, double, double, double &, double &);
    static bool (*ClimatologyWindAtlasData)(const wxDateTime &, double, double, int &count,
                                            double *, double *, double &, double &);
    static int (*ClimatologyCycloneTrackCrossings)(double lat1, double lon1, double lat2, double lon2,
                                                   const wxDateTime &date, int dayrange);

    static OD_FindClosestBoundaryLineCrossing ODFindClosestBoundaryLineCrossing;
    
    static std::list<RouteMapPosition> Positions;
    void Stop() { Lock(); m_bFinished = true; Unlock(); }
    void ResetFinished() { Lock(); m_bFinished = false; Unlock(); }
    wxString LoadBoat() { return m_Configuration.boat.OpenXML(m_Configuration.boatFileName); }

protected:
    virtual void Clear();
    bool ReduceList(IsoRouteList &merged, IsoRouteList &routelist, RouteMapConfiguration &configuration);
    Position *ClosestPosition(double lat, double lon, double *dist=0);

    /* protect any member variables with mutexes if needed */
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    virtual bool TestAbort() = 0;

    IsoChronList origin; /* list of route isos in order of time */
    bool m_bNeedsGrib;
    GribRecordSet *m_NewGrib;

private:

    RouteMapConfiguration m_Configuration;
    bool m_bFinished, m_bValid;
    bool m_bReachedDestination, m_bGribFailed, m_bPolarFailed, m_bNoData;

    wxDateTime m_NewTime;
};
