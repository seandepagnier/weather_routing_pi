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
#include <wx/object.h>
#include <wx/weakref.h>

#include <list>

#include "ODAPI.h"
#include "GribRecordSet.h"

struct RouteMapConfiguration;
class IsoRoute;

typedef std::list<IsoRoute*> IsoRouteList;

class PlotData;

class RoutePoint {
public:
    RoutePoint(double latitude = 0., double longitude = 0., int sp = -1, int t=0, bool d = false) :
        lat(latitude), lon(longitude), polar(sp), tacks(t), grib_is_data_deficient(d) {}

    virtual ~RoutePoint() {};

    double lat;
    double lon;
    int polar; /* which polar in the boat we are using */
    int tacks; /* how many times we have tacked to get to this position */

    bool grib_is_data_deficient;

    bool GetPlotData(RoutePoint *next, double dt, RouteMapConfiguration &configuration, PlotData &data);
    // @brief Return the wind data at the route point.
    bool GetWindData(RouteMapConfiguration &configuration, double &W, double &VW, int &data_mask);
    // @brief Return the current data at the route point.
    bool GetCurrentData(RouteMapConfiguration &configuration, double &C, double &VC, int &data_mask);

    // @brief Return true if the route point crosses land.
    bool CrossesLand(double dlat, double dlon);
    // @brief Return true if the route point enters a boundary.
    bool EntersBoundary(double dlat, double dlon);
    // @brief Propagate the route point to a specific point.
    double PropagateToPoint(double dlat, double dlon, RouteMapConfiguration &cf, double &H, int &data_mask, bool end = true);
};

/*
 * A RoutePoint that has a time associated with it, along with navigation and weather data.
*/
class PlotData : public RoutePoint
{
public:
    wxDateTime time; // The time when the boat reaches this position, based on the route calculation.
    double delta;    // The time in seconds from the previous position to this position.
    double VBG;      // Velocity of boat over ground, in knots.
    double BG;       // Boat direction over ground.
    double VB;       // Velocity of boat over water, in knots.
    double B;        // Boat direction over water.
    double VW;       // Velocity of wind over water, in knots.
    double W;        // Wind direction over water.
    double VWG;      // Velocity of wind over ground, in knots.
    double WG;       // Wind direction over ground.
    double VC;       // Velocity of current over ground, in knots.
    double C;        // Sea current direction over ground.
    double WVHT;     // Swell height, in meters.
    double VW_GUST;  // Gust wind speed, in knots.
};

class SkipPosition;

/* circular linked list node for positions which take equal time to reach */
class Position: public RoutePoint
{
public:
    Position(double latitude, double longitude, Position *p=NULL,
             double pheading=NAN, double pbearing=NAN, int sp=-1,
             int t=0, int dm=0, bool df = false);
    Position(Position *p);

    SkipPosition *BuildSkipList();

    bool Propagate(IsoRouteList &routelist, RouteMapConfiguration &configuration);

    double Distance(Position *p);
    int SailChanges();
    double PropagateToEnd(RouteMapConfiguration &configuration, double &H, int &data_mask);
   
    double parent_heading; /* angle relative to true wind we sailed from parent to this position */
    double parent_bearing; /* angle relative to north */

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

/* A closed loop of isochrone positions */
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

// -----------------
class WR_GribRecordSet {
public:
    WR_GribRecordSet(unsigned int id) : m_Reference_Time(-1), m_ID(id) {
        for(int i=0; i<Idx_COUNT; i++) {
            m_GribRecordPtrArray[i] = 0;
            m_GribRecordUnref[i] = false;
        }
    }

    virtual ~WR_GribRecordSet()
    {
         RemoveGribRecords();
    }

    /* copy and paste by plugins, keep functions in header */
    void SetUnRefGribRecord(int i, GribRecord *pGR ) { 
        assert (i >= 0 && i < Idx_COUNT);
        if (m_GribRecordUnref[i] == true) {
            delete m_GribRecordPtrArray[i];
        }
        m_GribRecordPtrArray[i] = pGR;
        m_GribRecordUnref[i] = true;
    }

    void RemoveGribRecords( ) { 
        for(int i=0; i<Idx_COUNT; i++) {
            if (m_GribRecordUnref[i] == true) {
                delete m_GribRecordPtrArray[i];
            }
        }
    }

    time_t m_Reference_Time;
    unsigned int m_ID;

    GribRecord *m_GribRecordPtrArray[Idx_COUNT];
private:
    // grib records files are stored and owned by reader mapGribRecords
    // interpolated grib are not, keep track of them
    bool        m_GribRecordUnref[Idx_COUNT];
};

// ------
class Shared_GribRecordSetData: public wxRefCounter
{
public:
    Shared_GribRecordSetData( WR_GribRecordSet *gribset = 0 ) : m_GribRecordSet(gribset) { }
    Shared_GribRecordSetData( const Shared_GribRecordSetData& data ) : m_GribRecordSet(data.m_GribRecordSet) { }

    void SetGribRecordSet( WR_GribRecordSet *gribset )  { m_GribRecordSet = gribset; }
    WR_GribRecordSet * GetGribRecordSet() const { return m_GribRecordSet; }

    ~Shared_GribRecordSetData();

protected:
     WR_GribRecordSet *m_GribRecordSet;
                    
};

// ------
class Shared_GribRecordSet: public wxTrackable
{
public:
    // initializes this, assigning to the
    // internal data pointer a new instance of Shared_GribRecordSetData
    Shared_GribRecordSet( WR_GribRecordSet * ptr = 0 ) : m_data( new Shared_GribRecordSetData(ptr) )
    {
    }
    Shared_GribRecordSet& operator =( const Shared_GribRecordSet& tocopy )
    {
        // shallow copy: this is just a fast copy of pointers; the real
        // memory-consuming data which typically is stored inside
        m_data = tocopy.m_data;
        return *this;
    }

    void SetGribRecordSet( WR_GribRecordSet * ptr )
    {
        // make sure changes to this class do not affect other instances
        // currently sharing our same refcounted data:
        UnShare();
        m_data->SetGribRecordSet( ptr );
    }

    WR_GribRecordSet * GetGribRecordSet() const
    {
       return m_data->GetGribRecordSet();
    }

    bool operator == ( const Shared_GribRecordSet& other ) const
    {
        if (m_data.get() == other.m_data.get())
            return true; // this instance and the 'other' one share the same data...
        return (m_data->GetGribRecordSet() == other.m_data->GetGribRecordSet());
    }

    wxObjectDataPtr<Shared_GribRecordSetData> m_data;

protected:
    void UnShare()
    {
        if (m_data->GetRefCount() == 1)
            return;
        m_data.reset( new Shared_GribRecordSetData( *m_data ) );
    }
};

/* list of routes with equal time to reach */
class IsoChron
{
public:
    IsoChron(IsoRouteList r, wxDateTime t, double d, Shared_GribRecordSet &g, bool grib_is_data_deficient);
    ~IsoChron();

    void PropagateIntoList(IsoRouteList &routelist, RouteMapConfiguration &configuration);
    bool Contains(Position &p);
    bool Contains(double lat, double lon);
    Position *ClosestPosition(double lat, double lon, wxDateTime *t = 0, double *dist=0);
    void ResetDrawnFlag();

    IsoRouteList routes;
    wxDateTime time;
    double delta;
    Shared_GribRecordSet m_SharedGrib;
    WR_GribRecordSet *m_Grib;
    bool m_Grib_is_data_deficient;
};

typedef std::list<IsoChron*> IsoChronList;

struct RouteMapPosition {
    RouteMapPosition(wxString n, double lat0, double lon0, wxString guid = wxEmptyString)
    : Name(n), GUID(guid), lat(lat0), lon(lon0) {ID = ++s_ID;}

    wxString Name;
    wxString GUID;
    double lat, lon;
    long  ID;
    static long s_ID;
};


/* Configuration parameters for a route between two points. */
struct RouteMapConfiguration {
    RouteMapConfiguration () : StartLon(0), EndLon(0), 
          grib(nullptr), grib_is_data_deficient(false) {} /* avoid waiting forever in update longitudes */
    bool Update();

    wxString RouteGUID;       /* Route GUID if any */
    wxString Start;           /* The name of starting position, which is resolved to StartLat/StartLon. */
    wxString StartGUID;

    wxString End;             /* The name of the destination position, which is resolved to EndLat/EndLon. */
    wxString EndGUID;

    wxDateTime StartTime;     /* The time when the boat leaves the starting position. */

    double DeltaTime; /* default time in seconds between propagations */
    double UsedDeltaTime; /* time in seconds between propagations */

    Boat boat;                /* The polars of the boat, used for the route calculation. */
    wxString boatFileName;    /* The name of the boat XML file referencing polars. */
    
    enum IntegratorType { NEWTON, RUNGE_KUTTA } Integrator;

    // The maximum angle the boat can be diverted from the bearing to the destination,
    // at each step of the route calculation, in degrees.
    // This represents the angle away from Start to End bearing (StartEndBearing).
    // The normal setting is 100 degrees, which speeds up calculations.
    // If the route needs to go around land, islands or peninsulas, the user can increase the value.
    // E.g. the boat may have to go in the opposite direction then back to the destination bearing.
    double MaxDivertedCourse;
    // The maximum deviation away from the direct route.
    double MaxCourseAngle;
    // The maximum angle at each step of the route calculation.
    double MaxSearchAngle;
    // The calculated route will avoid a path where the true wind is above this value in knots.
    double MaxTrueWindKnots;
    // The calculated route will avoid a path where the gust wind is above this value in knots.
    double MaxApparentWindKnots;

    // The calculated route will avoid swells larger than this value in meters.
    // If the grib data does not contain swell information, the maximum swell value is ignored.
    // If there is no route within the maximum swell value, the route calculation will fail.
    double MaxSwellMeters;
    // The calculated route will not go beyond this latitude, as an absolute value.
    // If the starting or destination position is beyond this latitude, the route calculation will fail.
    double MaxLatitude;
    // The penalty time to tack the boat, in seconds.
    // The penalty time is added to the route calculation for each tack.
    double TackingTime;
    // Balance the influence of the wind and the ocean current on the route calculation.
    double WindVSCurrent;
    // The minimum safety distance to land, in nautical miles.
    // The calculated route will avoid land within this distance.
    double SafetyMarginLand;

    bool AvoidCycloneTracks;
    // Avoid cyclone tracks within ( 30*CycloneMonths + CycloneDays ) days of climatology data.
    int CycloneMonths;
    int CycloneDays;

    bool UseGrib;
    enum ClimatologyDataType {DISABLED, CURRENTS_ONLY, CUMULATIVE_MAP, CUMULATIVE_MINUS_CALMS, MOST_LIKELY, AVERAGE};
    enum ClimatologyDataType ClimatologyType;
    bool AllowDataDeficient;
    double WindStrength; // wind speed multiplier. 1.0 is 100% of the wind speed in the grib.

    // If true, the route calculation will avoid land, outside the SafetyMarginLand.
    bool DetectLand;
    // If true, the route calculation will avoid exclusion boundaries.
    bool DetectBoundary;
    // If true and grib data contains ocean currents, the route calculation will use ocean current data.
    bool Currents;
    // If true, avoid polar dead zones.
    // If false, avoid upwind course (polar angle too low) or downwind no-go zone (polar angle too high).
    bool OptimizeTacking;
    bool InvertedRegions;
    bool Anchoring;

    // Do not go below this minimum True Wind angle at each step of the route calculation.
    // The default value is 0 degrees.
    double FromDegree;
    // Do not go above this maximum True Wind angle at each step of the route calculation.
    // The default value is 180 degrees.
    double ToDegree;
    // The angular resolution at each step of the route calculation, in degrees.
    // Lower values provide finer resolution but increase computation time.
    // Higher values provide coarser resolution, but faster computation time.
    // The allowed range of resolution is from 0.1 to 60 degrees.
    // The default value is 5 degrees.
    double ByDegrees;


    /* computed values */
    std::list<double> DegreeSteps;
    double StartLat, StartLon; // The latitude and longitude of the starting position.
    double EndLat, EndLon; // The latitude and longitude of the destination position.

    /*
     * The initial bearing from Start position to End position, following the Great Circle
     * route and taking into consideration the ellipsoidal shape of the Earth.
     * Note: a boat sailing the great circle route will gradually change the bearing to the destination.
    */
    double StartEndBearing; 
    bool positive_longitudes; /* longitudes are either 0 to 360 or -180 to 180,
    this means the map cannot cross both 0 and 180 longitude.
    To fully support this requires a lot more logic and would probably slow the algorithm
    by about 8%.  Is it even useful?  */

    // parameters
    WR_GribRecordSet *grib;
    // The time of the current isochrone step in the route calculation.
    // The time starts at StartTime and is incremented for each step until the destination is reached,
    // or the route calculation fails.
    wxDateTime time;
    bool grib_is_data_deficient, polar_failed, wind_data_failed;
    bool land_crossing, boundary_crossing;
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
    LOCKING_ACCESSOR(LandCrossing, m_bLandCrossing)
    LOCKING_ACCESSOR(BoundaryCrossing, m_bBoundaryCrossing)

    bool Empty() { Lock(); bool empty = origin.size() == 0; Unlock(); return empty; }
    bool NeedsGrib() { Lock(); bool needsgrib = m_bNeedsGrib; Unlock(); return needsgrib; }
    void RequestedGrib() { Lock(); m_bNeedsGrib=false; Unlock(); }
    void SetNewGrib(GribRecordSet *grib);
    void SetNewGrib(WR_GribRecordSet *grib);
    wxDateTime NewTime() { Lock(); wxDateTime time =  m_NewTime; Unlock(); return time; }
    wxDateTime StartTime() { Lock(); wxDateTime time = m_Configuration.StartTime;
        Unlock(); return time; }

    void SetConfiguration(const RouteMapConfiguration &o) { Lock();
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

    // XXX Isn't wxString refcounting thread safe?
    wxString GetError() { Lock(); wxString ret = m_ErrorMsg; Unlock(); return ret; }

    void SetError(wxString msg) {
       Lock();
       m_ErrorMsg = msg;
       m_bValid = false;
       m_bFinished = false;
       Unlock();
    }

protected:

    void SetFinished(bool destination ) {
       m_bReachedDestination = destination;
       m_bFinished = true;
    }

    void UpdateStatus(const RouteMapConfiguration &configuration) {
       if(configuration.polar_failed)
          m_bPolarFailed = true;

        if(configuration.wind_data_failed)
           m_bNoData = true;

        if(configuration.boundary_crossing)
           m_bBoundaryCrossing = true;

        if(configuration.land_crossing)
           m_bLandCrossing = true;
    }

    virtual void Clear();
    bool ReduceList(IsoRouteList &merged, IsoRouteList &routelist, RouteMapConfiguration &configuration);
    Position *ClosestPosition(double lat, double lon, wxDateTime *t=0, double *dist=0);

    /* protect any member variables with mutexes if needed */
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    virtual bool TestAbort() = 0;

    IsoChronList origin; /* list of route isos in order of time */
    bool m_bNeedsGrib;
    Shared_GribRecordSet m_SharedNewGrib;
    WR_GribRecordSet *m_NewGrib;

private:
 
    RouteMapConfiguration m_Configuration;
    bool m_bFinished, m_bValid;
    bool m_bReachedDestination, m_bGribFailed, m_bPolarFailed, m_bNoData;
    bool m_bLandCrossing, m_bBoundaryCrossing;

    wxString m_ErrorMsg;

    wxDateTime m_NewTime;
};
