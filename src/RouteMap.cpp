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

/* generate a datastructure which contains positions for
   isochron line segments which describe the position of the boat at a given time..

   Starting at a given location, propagate outwards in all directions.
   the outward propagation is guarenteed a closed region, and circular linked
   lists are used. If the route comes upon a boundary or reason to stop
   searching, then the point is tagged as propagated so that it is not
   propagated any further.

   To merge regions requires virtually the same algorithm for descrambling
   (normalizing) a single region.

   To normalize a region means that no two line segments intersect.

   For each segment go through and see if it intersects
   with any other line segment.  When it does the old route will follow
   the correct direction of the intersection on the intersected route,
   and the new region generated will be recursively normalized and then
   merged.

   A positive intersection comes in from the right.  Negative intersections
   signal negative regions.

   For each segment in a given route
   If the intersection occurs with the route and itself
   a new region is created with the same sign as the intersection
   and added to the list of either positive or negative subregions
   otherwise if the intersection occurs on different routes
   the intersecting route is merged into this one,
   swapping their connections

   Once we reach the end of the route, we can declare that it is complete,
   so in turn recursively normalize each inner subroute.  The subregions
   with the same sign are inner routes.  Once these regions are all normalized,
   the remaining regions with a different sign are the perminent subregions.
   Any inner routes remaining with matching sign can be discarded.
   
   Any outer subregions are also normalized to give outer regions
   with both signs which can be appended to the incomming lists

   Any remaining routes should be tested to ensure they are outside this one,
   Any inside routes may be discarded leaving only inverted subroutes
*/

#include <wx/wx.h>

#include "../../grib_pi/src/GribRecordSet.h"

#include "Boat.h"
#include "RouteMap.h"
#include "Utilities.h"

#include "georef.h"

static double Swell(GribRecordSet &grib, double lat, double lon)
{
    GribRecord *grh = grib.m_GribRecordPtrArray[Idx_HTSIGW];
    if(!grh)
        return 0;

    double height = grh->getInterpolatedValue(lon, lat, true );
    if(height == GRIB_NOTDEF)
        return 0;
    return height;
}

static bool Wind(GribRecordSet &grib, double &winddirground, double &windspeedground,
                 double lat, double lon)
{
    GribRecord *grx = grib.m_GribRecordPtrArray[Idx_WIND_VX];
    GribRecord *gry = grib.m_GribRecordPtrArray[Idx_WIND_VY];

    if(!grx || !gry)
        return false;

    double vx = grx->getInterpolatedValue(lon, lat, true);
    double vy = gry->getInterpolatedValue(lon, lat, true);

    if(vx == GRIB_NOTDEF || vy == GRIB_NOTDEF)
        return false;

    windspeedground = hypot(vy, vx) * 3.6 / 1.852;
    winddirground = positive_degrees(rad2deg(atan2(-vx, -vy)));
    return true;
}

static bool Current(GribRecordSet &grib,
                    double &currentspeed, double &currentdir,
                    double lat, double lon)
{
    GribRecord *grx = grib.m_GribRecordPtrArray[Idx_SEACURRENT_VX];
    GribRecord *gry = grib.m_GribRecordPtrArray[Idx_SEACURRENT_VY];

    if(!grx || !gry)
        return false;

    double vx = grx->getInterpolatedValue(lon, lat, true);
    double vy = grx->getInterpolatedValue(lon, lat, true);

    if(vx == GRIB_NOTDEF || vy == GRIB_NOTDEF)
        return false;

    currentspeed = hypot(vy, vx) * 3.6 / 1.852;
    currentdir = positive_degrees(rad2deg(atan2(-vx, -vy)));
    return true;
}

/* Often localized currents can be strong enough to create
   a breeze which can be sailed off.  The wind data is
   calculated from the ground not the sea, it is converted

   WG  - Wind direction over ground
   VWG - Velocity of wind over ground
   C   - Sea Current Direction over ground
   VC  - Velocity of Current
   WA  - Angle of wind relative to true north
   VW - velocity of wind over water
*/

void OverWater(double C, double VC, double WG, double VWG, double &WA, double &VW)
{
    double Cx = VC * cos(deg2rad(C));
    double Cy = VC * sin(deg2rad(C));
    double Wx = VWG * cos(deg2rad(WG)) - Cx;
    double Wy = VWG * sin(deg2rad(WG)) - Cy;
    VW = hypot(Wx, Wy);
    WA = rad2deg(atan2(Wy, Wx));
}

/* provisions to compute boat movement over ground

   BG  - boat direction over ground
   BGV - boat speed over ground (gps velocity)
*/

void BoatOverGround(double B, double VB, double C, double VC, double &BG, double &VBG)
{
    double Cx = VC * cos(deg2rad(C));
    double Cy = VC * sin(deg2rad(C));
    double BGx = VB * cos(deg2rad(B)) + Cx;
    double BGy = VB * sin(deg2rad(B)) + Cy;

    BG  = rad2deg(atan2(BGy, BGx));
    VBG = hypot(BGx, BGy);
}

/* find intersection of two line segments
   if no intersection return 0, otherwise, 1 if the
   second line crosses from right to left, or -1 for left to right

   If the math breaks down and for contrived cases, -2 or 2 is returned
   to specify if the first or second segment is the problem

   Truth equations to calculate intersection (x, y)
   (y-y1) * (x2-x1) = (y2-y1) * (x-x1)
   (y-y3) * (x4-x3) = (y4-y3) * (x-x3)
*/

#define EPSILON (2e-10)
#define EPSILON2 (2e-20)
int TestIntersectionGoodButSlow(Position *p1, Position *p2, Position *p3, Position *p4)
{
    double x1 = p1->lon, x2 = p2->lon, x3 = p3->lon, x4 = p4->lon;
    double y1 = p1->lat, y2 = p2->lat, y3 = p3->lat, y4 = p4->lat;

    /* right way around world */
    if(fabs(x1 - x2) > 180) {
        if(x1 < 0)      x1 += 360;
        else if(x2 < 0) x2 += 360;
        /* put other segment in our coords */
        if(x3 < 0)
            x3 += 360, x4 += 360;
    }

    /* right way around for this segment */
    if(fabs(x3 - x4) > 180) {
        if(x3 < 0)      x3 += 360; else if(x3 >= 360) x3 -= 360;
        else if(x4 < 0) x4 += 360; else if(x4 >= 360) x4 -= 360;
        if(x1 < 0)
            x1 += 360, x2 += 360;
    }

    /* quick test to avoid calculations if segments are far apart */
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;

    /* error for zero segments */
    if((x1 == x2 && y1 == y2))
        return -2;
    if((x3 == x4 && y3 == y4))
        return 2;

    double dem = (x1*(y4-y3)+x2*(y3-y4)+(x4-x3)*y2+(x3-x4)*y1);

    if(fabs(dem) < EPSILON2) /* parallel or really close to parallel */
        return 2;

    /* find intersection point */
    double x = (x1*(x3*y4-x4*y3+y2*(x4-x3)) + x2*(x4*y3-x3*y4+y1*(x3-x4)))/dem;
    double y = (y1*(x3*y4-x4*y3+x2*(y3-y4)) + y2*(x4*y3-x3*y4+x1*(y4-y3)))/dem;

    double xa, ya, xb, yb, xc, yc, da, db, dc;
    xb = x1 - x , yb = y1 - y , db = xb*xb + yb*yb;
    xc = x2 - x , yc = y2 - y , dc = xc*xc + yc*yc;

    if(db < EPSILON2 || dc < EPSILON2)
        return 2; /* -2 or 2? */

    xa = x1 - x2, ya = y1 - y2, da = xa*xa + ya*ya;

    if(db > da || dc > da)
        return 0;

    xb = x3 - x , yb = y3 - y , db = xb*xb + yb*yb;
    xc = x4 - x , yc = y4 - y , dc = xc*xc + yc*yc;

    if(db < EPSILON2 || dc < EPSILON2)
        return 2;

    xa = x3 - x4, ya = y3 - y4, da = xa*xa + ya*ya;

    if(db > da || dc > da)
        return 0;

    return dem < 0 ? 1 : -1;
}

int TestIntersection(Position *p1, Position *p2, Position *p3, Position *p4)
{
    double x1 = p1->lon, x2 = p2->lon, x3 = p3->lon, x4 = p4->lon;
    double y1 = p1->lat, y2 = p2->lat, y3 = p3->lat, y4 = p4->lat;

    /* right way around world */
    if(fabs(x1 - x2) > 180) {
        if(x1 < 0)      x1 += 360;
        else if(x2 < 0) x2 += 360;
        /* put other segment in our coords */
        if(x3 < 0)
            x3 += 360, x4 += 360;
    }

    /* right way around for this segment */
    if(fabs(x3 - x4) > 180) {
        if(x3 < 0)      x3 += 360; else if(x3 >= 360) x3 -= 360;
        else if(x4 < 0) x4 += 360; else if(x4 >= 360) x4 -= 360;
        if(x1 < 0)
            x1 += 360, x2 += 360;
    }

#if 0
    /* quick test to avoid calculations if segments are far apart */
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;
#endif

    double ax = x2 - x1, ay = y2 - y1;
    double bx = x3 - x4, by = y3 - y4;
    double cx = x1 - x3, cy = y1 - y3;

    double denom = ay * bx - ax * by;

#define EPS 2e-10
#define EPS2 2e-4
    if(fabs(denom) < EPS) { /* parallel or really close to parallel */
        if(fabs((y1 + by*ax*x3) - (y3 + ay*bx*x1)) > EPS2)
            return 0; /* different intercepts, no intersection */

        /* we already know from initial test we are overlapping,
           for parallel line segments, there is no way to tell
           which direction the intersection occurs */
        if(!ax && !ay) /* first segment is a zero segment */
            return -2;
        else
            return 2;
    }

    double recip = 1 / denom;
    double na = (by * cx - bx * cy) * recip;
    if(na < -EPS2 || na > 1 + EPS2)
        return 0;

    double nb = (ax * cy - ay * cx) * recip;
    if(nb < -EPS2 || nb > 1 + EPS2)
        return 0;

    /* too close to call.. floating point loses bits with arithmetic so
       in this case we must avoid potential false guesses */
    if(na < EPS2 || na > 1 - EPS2 ||
       nb < EPS2 || nb > 1 - EPS2)
        return 2;

    return denom < 0 ? -1 : 1;
}


inline int TestIntersectionXY(double x1, double y1, double x2, double y2,
                              double x3, double y3, double x4, double y4)
{
#if 0
    /* quick test to avoid calculations if segments are far apart */
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;
#endif

    double ax = x2 - x1, ay = y2 - y1;
    double bx = x3 - x4, by = y3 - y4;
    double cx = x1 - x3, cy = y1 - y3;

    double denom = ay * bx - ax * by;

#undef EPS
#undef EPS2
#define EPS 2e-10
#define EPS2 2e-5
    if(fabs(denom) < EPS) { /* parallel or really close to parallel */
        if(fabs((y1 + by*ax*x3) - (y3 + ay*bx*x1)) > EPS2)
            return 0; /* different intercepts, no intersection */

        /* we already know from initial test we are overlapping,
           for parallel line segments, there is no way to tell
           which direction the intersection occurs */
        if(!ax && !ay) /* first segment is a zero segment */
            return -2;
        else
            return 2;
    }

    double recip = 1 / denom;
    double na = (by * cx - bx * cy) * recip;
    if(na < -EPS2 || na > 1 + EPS2)
        return 0;

    double nb = (ax * cy - ay * cx) * recip;
    if(nb < -EPS2 || nb > 1 + EPS2)
        return 0;

    /* too close to call.. floating point loses bits with arithmetic so
       in this case we must avoid potential false guesses */
    if(na < EPS2 || na > 1 - EPS2 ||
       nb < EPS2 || nb > 1 - EPS2)
        return 2;

    return denom < 0 ? -1 : 1;
}

Position::Position(double latitude, double longitude, int sp, Position *p)
    : lat(latitude), lon(longitude), sailplan(sp), parent(p), propagated(false)
{
    lat -= fmod(lat, EPSILON);
    lon -= fmod(lon, EPSILON);

    prev = next = this;
}

Position::Position(Position *p)
    : lat(p->lat), lon(p->lon), sailplan(p->sailplan), parent(p->parent),
      propagated(p->propagated)
{
}

SkipPosition *Position::BuildSkipList()
{
    /* build skip list */
    SkipPosition *skippoints = NULL;
    Position *p = points;
    int firstquadrant, lastquadrant = -1;
    do {
        Position *q = p->next;
        int quadrant = 0;
        if(q->lat < p->lat)
            quadrant += 2;

        double diff = p->lon - q->lon;
        if(diff > 0) {
            if(diff < 180)
                quadrant += 1;
        } else if(diff < -180)
            quadrant += 1;

        if(lastquadrant == -1) {
            firstquadrant = lastquadrant = quadrant;
        } else
        if(quadrant != lastquadrant) {
            SkipPosition *rs = new SkipPosition(p);
            if(skippoints) {
                rs->prev=skippoints->prev;
                rs->next=skippoints;
                skippoints->prev->next = sp;
                skippoints->prev = sp;
            } else
                skippoints = sp;
        }
    } while(p != points);

    if(quadrant != firstquadrant) {
        SkipPosition *rs = new SkipPosition(p);
        if(!skippoints) {
            printf("no skippoints and we are done... how?\n");
            exit(1);
        }

        rs->prev=skippoints->prev;
        rs->next=skippoints;
        skippoints->prev->next = sp;
        skippoints->prev = sp;
    }
}

bool Position::GetPlotData(GribRecordSet &grib, PlotData &data, double dt)
{
    data.WVHT = Swell(grib, lat, lon);
    if(!Wind(grib, data.WG, data.VWG, lat, lon))
        return false;

    if(!Current(grib, data.VC, data.C, lat, lon))
        data.C = data.VC = 0;

    OverWater(data.C, data.VC, data.WG, data.VWG, data.W, data.VW);

    if(parent) {
        ll_gc_ll_reverse(parent->lat, parent->lon, lat, lon, &data.BG, &data.VBG);
        data.VBG *= 3600 / dt;

        OverWater(data.C, data.VC, data.BG, data.VBG, data.B, data.VB);

        return true;
    }
    return false;
}

/* create a looped route by propagating from a position by computing
   the location the boat would be in if sailed at every angle
   relative to the true wind. */
bool Position::Propagate(IsoRouteList &routelist, GribRecordSet &grib, RouteMapOptions &options)
{
    /* already propagated from this position, don't need to again */
    if(propagated)
        return false;

    propagated = true;

    Position *points = NULL;
    /* through all angles relative to wind */
    int count = 0;

    double S = Swell(grib, lat, lon);
    if(S > options.MaxSwellMeters)
        return false;

    if(fabs(lat) > options.MaxLatitude)
        return false;

    double WG, VWG;
    Position *p = this; /* get wind from parent position if not available */
    while(!Wind(grib, WG, VWG, p->lat, p->lon)) {
        p = p->parent; /* should we use parent's time here ? */
        if(!options.AllowDataDeficient || !p)
            return false;
    }

    if(VWG > options.MaxWindKnots)
        return false;

    double C, VC;
    if(!Current(grib, VC, C, lat, lon))
        C = VC = 0;

    double W, VW;
    OverWater(C, VC, WG, VWG, W, VW);

    double bearing = 0, parentbearing = NAN;
    if(parent && options.TackingTime)
        ll_gc_ll_reverse(parent->lat, parent->lon, lat, lon, &parentbearing, 0);
    else if(options.MaxDivertedCourse == 180)
        goto skipbearingcomputation;

    ll_gc_ll_reverse(lat, lon, options.EndLat, options.EndLon, &bearing, 0);
skipbearingcomputation:

    double timeseconds = options.dt;
    for(std::list<double>::iterator it = options.DegreeSteps.begin();
        it != options.DegreeSteps.end(); it++) {
        double H = *it;
        double B, VB;

        int sailplan = options.boat.TrySwitchBoatPlan(sailplan, VW, H, S);

        B = W + H; /* rotated relative to wind */

        /* avoid propagating from positions which go in a direction too much
           diverted from the correct course.  */
        if(fabs(heading_resolve(B - bearing)) > options.MaxDivertedCourse)
            continue;

        VB = options.boat.Plans[sailplan]->Speed(H, VW);

        /* failed to determine speed.. I guess we can always drift? */
        if(isnan(B) || isnan(VB))
            B = VB = 0;

        /* compound boatspeed with current */
        double BG, VBG;
        BoatOverGround(B, VB, C, VC, BG, VBG);

        /* did we tack? apply penalty */
        if(!isnan(parentbearing)) {
            double hrpb = heading_resolve(parentbearing), hrb = heading_resolve(bearing);
            if(hrpb*hrb < 0 && fabs(hrpb - hrb) < 180)
                timeseconds -= options.TackingTime;
        }

        /* distance over ground */
        double dist = VBG * timeseconds / 3600.0;

        double dlat, dlon;
        ll_gc_ll(lat, lon, BG, dist, &dlat, &dlon);
        
        bool hitland = options.DetectLand ? CrossesLand(dlat, dlon) : false;
        if(!hitland && dist) {
            Position *rp = new Position(dlat, dlon, sailplan, this);

            if(points) {
                rp->prev=points->prev;
                rp->next=points;
                points->prev->next = rp;
                points->prev = rp;
            } else
                points = rp;
            count++;
        }
    }

    if(count < 3) /* tested in normalize, but save the extra steps */
        return false;

    SkipPositon *skippoints = BuildSkipList();

    routelist = (new IsoRoute(skippoints))->Normalize(0, options.InvertedRegions);
    return true;
}

double Position::Distance(Position *p)
{
    return DistGreatCircle(lat, lon, p->lat, p->lon);
}

#include "../../../include/chcanv.h"
#include "../../../include/gshhs.h"
bool Position::CrossesLand(double dlat, double dlon)
{
    static GshhsReader *reader;
    static Projection proj;
    if(reader == NULL)
        reader = new GshhsReader(&proj);

    QLineF trajectWorld(positive_degrees(lon), lat,
                        positive_degrees(dlon), dlat);

    return reader->crossing2(trajectWorld);
}

SkipPositon::SkipPosition(Position *p)
point(p), prev(this), next(this), quadrant(0)
{
}

/* copy a skip list along with it's position list to new lists */
SkipPosition* SkipPosition::Copy(bool skipinvalidated)
{
    SkipPosition *s = this;
    Position *p = s->point;
    Position *np = new Position(p);
    Position *fp = np;

    SkipPosition *ns = new SkipPosition(np, s->quadrant);
    SkipPosition *fs = ns;

    while(s->next != this) {
        Position *p = s->point;
        /* copy all positions between skip positions */
        do {
            Position *nnp = new Position(p);
            np->next = nnp;
            nnp->prev = np;
            np = nnp;
            p = p->next;
        } while(p != s->next->point);

        s = s->next;
        Position *nns = new SkipPosition(np, s->quadrant);
        ns->next = nns;
        nns->prev = ns;
        ns = nns;
    }
    fs->prev = ns;
    ns->next = fs;

    fp->prev = np;
    np->next = fp;
    return fs;
}

IsoRoute::IsoRoute(SkipPosition *p, int dir)
    : skippoints(p), direction(dir), parent(NULL)
{
}

/* copy constructor */
IsoRoute::IsoRoute(IsoRoute *r, IsoRoute *p)
    : points(r->skippoints->Copy()), direction(r->direction), parent(p)
{
}

IsoRoute::~IsoRoute()
{
    for(IsoRouteList::iterator it = children.begin(); it != children.end(); ++it)
        delete *it;

    if(!points)
        return;

    Position *p = skippoints->point;
    do {
        Position *dp = p;
        p = p->next;
        delete dp;
    } while(p != skippoints->point);

    DeleteSkipPoints();
}

void IsoRoute::DeleteSkipPoints()
{
    SkipPosition *s = skippoints;
    do {
        SkipPosition *ds = s;
        s = s->next;
        delete ds;
    } while(s != skippoints);
}

void IsoRoute::Print()
{
    if(!skippoints)
        printf("Empty IsoRoute\n");
    else {
        Position *p = skippoints->point;
        do {
            printf("%.4f %.4f\n", p->lon, p->lat);
            p = p->next;
        } while(p != skippoints->point);
        printf("\n");
    }
}

/* how many times do we cross this route going from this point to infinity,
   return -1 if inconclusive */
int IsoRoute::IntersectionCount(Position *pos)
{
    int numintsct = 0;
    double lat = pos->lat, lon = pos->lon;

    SkipPosition *s1 = skippoints;
    do {
        SkipPosition *s2 = skippoints->next;
        double s1plon = s1->point->lon, s2plon = s2->point->lon;

        /* we could avoid completely recomputing the state every time,
           and use all 9 states, cutting compariison tests and switches
           in half (roughly double the speed of this routine)
           but it would complicate things.. how often is this called? */
        int state = 0;
        if(lon > s1plon) state++;
        if(lon > s2plon) state++;
        if(state == 1) {
            double s1plat = s1->point->lat, s2plat = s2->point->lat;
            state = 0;
            if(lat > s1plat) state++;
            if(lat > s2plat) state++;

            switch(state) {
            case 1: /* must test every point in this case as point falls in boundaries of skip list */
            {
                Position *p1 = s1->point;
                do {
                    Position *p2 = p1->next;
                    double p1lon = p1->point->lon, p2lon = p2->point->lon;
                    state = 0;
                    if(lon > p1lon) state++;
                    if(lon > p2lon) state++;
                    if(state == 1) {
                        double p1lat = p1->lat, p2lat = p2->lat;
                        state = 0;
                        if(lat > p1lon) state++;
                        if(lat > p2lon) state++;
                        switch(state) {
                        case 1: /* must perform exact intersection test */
                        {
                            int dir = TestIntersectionXY(ppx, ppy, qpx, qpy, rpx, rpy, spx, spy);
                            switch(dir) {
                            case -2: case 2: return -1;
                            case 1: case -1: goto intersects;
                            }
                        } break;
                        case 2: /* must intersect, we are below */
                            goto intersects;
                        }
                    }
                    p1 = p2;
                } while(p1 != s2->point);
            } break;
            case 2: /* must intersect, we are below skip segment out of bounds */
            intersects:
                numintsct++;
            }

            s1 = s2;
    } while(s1 != skippoints->point);

    return numintsct;
}

/* determine if a route contains a position
   0 for outside, 1 for inside, -1 for inconclusive (on border or really close) */
int IsoRoute::Contains(Position *pos, bool test_children)
{
    int numintsct = IntersectionCount(pos);
    if(numintsct == -1)
        return -1;

    if(test_children)
        for(IsoRouteList::iterator it = children.begin();
            it != children.end(); it++) {
            int cnumintsct = (*it)->Contains(pos, test_children);
            if(cnumintsct == -1)
                return -1;
            numintsct += cnumintsct;
        }

    return numintsct&1; /* odd */
}

/* This function is very slow, and should probably be removed
   or replaced with something else.. see how often it is called */
bool IsoRoute::CompletelyContained(IsoRoute *r)
{
    Position *pos = r->skipoints->point;
    do {
        if(Contains(pos, false) != 1)
            return false;

        pos = pos->next;
    } while(pos != r->skipoints->point);
    return true;
}

/* Determine if one route contains another,
   only test first point, but if it fails try other points */
bool IsoRoute::ContainsRoute(IsoRoute *r)
{
    Position *pos = r->skipoints->point;

    do {
        switch(Contains(pos, false)) {
        case 0: return false;
        case 1: return true;
        }

        pos = pos->next;
    } while(pos != r->skipoints->point); /* avoid deadlock.. lets hope we dont do this often */

    return true; /* probably good to say it is contained in this rare case */
}

/* apply current to given route, and return if it changed at all */
bool IsoRoute::ApplyCurrents(GribRecordSet &grib, RouteMapOptions &options)
{
    if(!skippoints)
        return false;

    bool ret = false;
    Position *p = skippoints->point;
    double timeseconds = options.dt;
    do {
        double C, VC;
        if(Current(grib, VC, C, p->lat, p->lon)) {
            /* drift distance over ground */
            double dist = VC * timeseconds / 3600.0;
            if(dist)
                ret = true;
            ll_gc_ll(p->lat, p->lon, C, dist, &p->lat, &p->lon);
        }

        p = p->next;
    } while(p != skippoints->point);

    /* if we moved we need to rebuild the skip list */
    if(ret) {
        Position *points = skippoints->point;
        DeleteSkipPoints();
        skippoints = points->BuildSkipList();
    }

    return ret;
}

enum { MINLON, MAXLON, MINLAT, MAXLAT };

/* return false if longitude is possibly invalid
   could cache these bounds to avoid recomputing all the time */
bool IsoRoute::FindIsoRouteBounds(double bounds[4])
{
    SkipPosition *maxlat = skippoints;
    Position *p = s->point;
    bounds[MINLAT] = bounds[MAXLAT] = p->lat;
    bounds[MINLON] = bounds[MAXLON] = p->lon;

    SkipPosition *s = skippoints->next;
    while(s != skippoints) {
        p = s->point;
        bounds[MINLAT] = MIN(p->lat, bounds[MINLAT]);
        bounds[MAXLAT] = MAX(p->lat, bounds[MAXLAT]);
        bounds[MINLON] = MIN(p->lon, bounds[MINLON]);
        bounds[MAXLON] = MAX(p->lon, bounds[MAXLON]);
            
        if(p->lat == bounds[MAXLAT])
            maxlat = s;
        s = s->next;
    }
    skippoints = maxlat; /* set to max lat for merging to keep outside */
    
    /* cross date line? */
    if(bounds[MAXLON] - bounds[MINLON] > 180)
        return false; /* possibly invalid for longitude */
    return true;
}

/* remove and delete a position given it's last skip position,
   we need to update the skip list if this point falls on a skip position*/
void IsoRoute::RemovePosition(SkipPosition *s, Position *p)
{
    if(skippoints == skippoints->next) {
        delete skippoints->point;
        delete skippoints;
        skippoints = NULL;
    } else {
        p->next->prev = p->prev;
        p->prev->next = p->next;
        if(p == points)
            points = p->next;
    }

    if(s->point == p) {
#if 0
    /* possible optimization.. is this really useful? */
    /* is this correct? */
        int pquadrant = s->prev->quadrant;
        int quadrant = FindQuadrant(p->prev, p->next);
        if(quadrant == pquadrant) {
            s->point = p->next;
            if(s->point == s->next->point || quadrant == s->quadrant) {
                s->prev->next = s->next;
                s->next->prev = s->prev;
                delete s;
            }
        } else {
            s->point = p->prev;
            if(s->point == s->prev->point || quadrant = s->next->quadrant) {
                s->next->point = s->point;
                s->prev->next = s->next;
                s->next->prev = s->prev;
                delete s;
            }
            s->quadrant = quadrant;
        }
#else
        Position *points = skippoints->points;
        if(p == points)
            points = points->next;
        DeleteSkipPoints();
        skippoints = points->BuildSkipList();
#endif
    }

    delete p;
}

int FindIntersections(SkipPosition *sp1, SkipPosition *sp2)
{
    SkipPosition *p = sp1;
    SkipPosition *q = p->next;

    do {
        SkipPosition *r = sp2;
        SkipPosition *s = r->next;

        double px = p->point->lon, qx = q->point->lon, py = p->point->lat, qy = q->point->lat;
        
        double minx, maxx, miny, maxy;
        if(px > qx) minx = qx, maxx = px; else minx = px, maxx = qx;
        if(py > qy) miny = qy, maxy = py; else miny = py, maxy = qy;
        
        double rx = r->point->lon, ry = r->point->lat;

        int state = 0;
        if(rx > minx) state++;
        if(rx > maxx) state++;
        if(ry > miny) state+=3;
        if(ry > maxy) state+=3;

        do {
            double sx = s->lon, sy = s->lat;

            int dir;
            /*  0 1 2
                3 4 5
                6 7 8 */
            switch(state) {
            case 0:
                if(sx < minx) {
                    if(sy > miny) {
                        if(sy > maxy)
                            state = 6;
                        else state = 3;
                    }
                    goto skipit;
                }
                if(sy < miny) {
                    if(sx > maxx)
                        state = 2;
                    else
                        state = 1;
                    goto skipit;
                } break;
            case 1:
                if(sy < miny) {
                    if(sx < minx)
                        state = 0;
                    else if(sx > maxx)
                        state = 2;
                    goto skipit;
                } break;
            case 2:
                if(sx > maxx) {
                    if(sy > miny) {
                        if(sy > maxy)
                            state = 8;
                        else
                            state = 5;
                    }
                    goto skipit;
                }
                if(sy < miny) {
                    if(sx < minx)
                        state = 0;
                    else
                        state = 1;
                    goto skipit;
                } break;
            case 3:
                if(sx < minx) {
                    if(sy < miny)
                        state = 0;
                    else if(sy > maxy)
                        state = 6;
                    goto skipit;
                } break;
            case 5:
                if(sx > maxx) {
                    if(sy < miny)
                        state = 2;
                    else if(sy > maxy)
                        state = 8;
                    goto skipit;
                } break;
            case 6:
                if(sx < minx) {
                    if(sy < maxy) {
                        if(sy < miny)
                            state = 0;
                        else
                            state = 3;
                    }
                    goto skipit;
                }
                if(sy > maxy) {
                    if(sx > maxx)
                        state = 8;
                    else
                        state = 7;
                    goto skipit;
                } break;
            case 7:
                if(sy > maxy) {
                    if(sx < minx)
                        state = 6;
                    else if(sx > maxx)
                        state = 8;
                    goto skipit;
                } break;
            case 8:
                if(sx > maxx) {
                    if(sy < maxy) {
                        if(sy < miny)
                            state = 2;
                        else
                            state = 5;
                    }
                    goto skipit;
                }
                if(sy > maxy) {
                    if(sx < minx)
                        state = 6;
                    else
                        state = 7;
                    goto skipit;
                } break;
            }

#if 0
            /* TODO: test bounds efficiently with states */
            double rminx, rmaxx, rminy, rmaxy;
            if(rx > sx) rminx = sx, rmaxx = rx; else rminx = rx, rmaxx = sx;
            if(ry > sy) rminy = sy, rmaxy = ry; else rminy = ry, rmaxy = sy;

            if(ppx < rminx && qpx < rminx) continue;
            if(ppx > rmaxx && qpx > rmaxx) continue;
            if(ppy < rminy && qpy < rminy) continue;
            if(ppy > rmaxy && qpy > rmaxy) continue;
#endif

            for(Positon *pp = p->points; pp != q->points; pp=pp->next) {
                double ppx = pp->lon, ppy = pp->lat;
                Position *qp = pp->next;
                double qpx = qp->lon, qpy = qp->lat;
                for(Positon *rp = r->points; rp != s->points; rp=rp->next) {
                    /* dont test adjacent segments, so in the case p first, we skip the last segment */
                    if((p == r) && (pp == rp || pp->next == rp))
                        break;

                    double rpx = rp->x, rpy = rp->y;
                    Position *sp = rp->next;
                    double spx = sp->lon, spy = sp->lat;

                    if((dir = TestIntersectionXY(ppx, ppy, qpx, qpy, rpx, rpy, spx, spy)))
                        return dir;
                }
            }
        }

        state = 0; /* recompute state */
        if(sx > minx) state++;
        if(sx > maxx) state++;
        if(sy > miny) state+=3;
        if(sy > maxy) state+=3;
            
#if 1
    skipit:
        rx = sx, ry = sy;
#endif
    }

    return 0;
}

/* Take a route which may overlaps with itself and convert to a list
   of normalized routes which no longer have overlaps */
IsoRouteList IsoRoute::Normalize(int level, bool inverted_regions)
{
    static int ncount;
    ncount++;

    IsoRouteList ret;
reset:
    if(!skippoints) /* no points.. this route is gone */
        delete this;
        return ret;
    }

    SkipPosition *sp1 = skippoints, *sp2 = skippoints;
    while(FindIntersections(sp1, sp2)) {
        /* this can change points, so reset to be safe
           in the future maybe we can avoid this and not reset */
        switch(dir) {
        case -2: /* too close to call, delete and try next point */
            RemovePosition(q);
            goto reset;
        case 2:
            RemovePosition(s);
            goto reset;
        case -1:
        case 1:
            pp->next = s;
            sp->prev = p;
            rp->next = q;
            qp->prev = r;

            /* update skip list properly */

            if(level == 0) {
                if(dir != direction) {
                    /* slight numerical error, or outer inversion */
                    FreePoints(q);
                } else {
                    IsoRoute *x = new IsoRoute(q, dir);
                    IsoRouteList sub = x->Normalize(level + 1, inverted_regions);
                    if(inverted_regions) {
                        for(IsoRouteList::iterator it = sub.begin(); it != sub.end(); ++it) {
                            if((*it)->children.size()) {
                                printf("grandchild detected\n");
                                delete *it;
                            } else if(direction == (*it)->direction) {
                                ret.push_back(*it); /* sibling */
                            } else if((*it)->Count() < 16) {
                                printf("too small to be a useful child: %d\n", (*it)->count);
                                delete *it;
                            } else if(!CompletelyContained(*it)) {
                                printf("not correct to be child: %d\n", (*it)->count);
                                delete *it;
                            } else { /* different direction contained.. it is a child */
                                /* we should merge it with the other children here */
                                printf("Child route: %d\n", (*it)->Count());
                                IsoRoute *child = *it;
                                child->parent = this;
                                children.push_back(child);
                            }
                        }
                    } else { /* no inverted regions mode */
                        for(IsoRouteList::iterator it = sub.begin(); it != sub.end(); ++it) {
                            if(direction == (*it)->direction) {
                                ret.push_back(*it); /* sibling */
                            } else
                                delete *it; /* inversion */
                        }
                    }
                }
            } else { /* all subregions are siblings for inner levels */
                IsoRoute *x = new IsoRoute(q, dir);
                IsoRouteList sub = x->Normalize(level + 1, inverted_regions);
                ret.splice(ret.end(), sub);
            }
#if 0
            /* TODO: must make FindIntersections not lose state
               so a full reset is not needed */
            goto outer_continue;
#else
            goto reset;
#endif
        }

    ret.push_back(this);
    return ret;
}

/* take two routes that may overlap, and combine into a list of non-overlapping routes */
bool Merge(IsoRouteList &rl, IsoRoute *route1, IsoRoute *route2, bool inverted_regions, int level)
{
    static int mergecnt;
    mergecnt++;

    if(route1->direction == -1 && route2->direction == -1) {
        printf("cannot merge two inverted routes yet\n");
        exit(1);
    }

    /* quick test to make sure we could possibly intersect with bounds */
    double bounds1[4], bounds2[4];
    bool r1bv = route1->FindIsoRouteBounds(bounds1);
    bool r2bv = route2->FindIsoRouteBounds(bounds2);
    if(bounds1[MINLAT] > bounds2[MAXLAT] || bounds1[MAXLAT] < bounds2[MINLAT])
        return false;
    if(r1bv && r2bv && (bounds1[MINLON] > bounds2[MAXLON] || bounds1[MAXLON] < bounds2[MINLON]))
        return false;

    /* make sure we start on the outside */
    if(route2->skippoints->point->lat > route1->skippoints->point->lat) {
        IsoRoute *t = route1;
        route1 = route2;
        route2 = t;
    }

reset:
    SkipPosition *sp1 = route1->skippoints, *sp2 = route2->skippoints;
    int dir;
    while((dir = FindIntersections(sp1, sp2))) {
        switch(dir) {
            /* too close to call, delete s and try next point */
        case -2:
            route1->RemovePosition(q);
            if(!route1->skippoints) { /* nothing left to merge */
                delete route1;
                rl.push_back(route2); /* no need to normalize */
                return true;
            }
#if 0
            q = p->next;
            goto outer_continue;
#else
            goto reset;
#endif
        case 2:
            route2->RemovePosition(s);
            if(!route2->skippoints) { /* nothing left to merge */
                delete route2;
                rl.push_back(route1); /* no need to normalize */
                return true;
            }
#if 0
            s = r->next;
            continue;
#else
            goto reset;
#endif
        case -1:        
            /* sanity check */
            if(route1->direction != 1 || route2->direction != -1) {
                /* we intersected at the wrong side, skip this intersection
                   and continue to find the intersection we want,  this occurs
                   when a line segment passes completely through a region.
                   could possibly merge here anyway and
                   still get the right result...  */
                goto skip;
            }
            goto ok;
        case 1:
            if(route1->direction == 1 && route2->direction == -1)
                goto skip;
        ok:
            /* swap intersecting connections */
            p->next = s;
            s->prev = p;
            r->next = q;
            q->prev = r;
            
            for(IsoRouteList::iterator it = route2->children.begin();
                it != route2->children.end(); it++)
                (*it)->parent = route1;
                
            /* merge children (append is currently incorrect)
               the children need to be merged, and any overlapping regions
               incremented so they don't get removed if contained */
            int sc1 = route1->children.size();
            int sc2 = route2->children.size();
            if(sc1 && sc2)
                printf("both have children: %d %d\n", sc1, sc2);
            
            route1->children.splice(route1->children.end(), route2->children);
            
            route2->points = NULL; /* all points are now in route1 */
            delete route2;
            
            IsoRouteList nrl = route1->Normalize(level, inverted_regions);
            rl.splice(rl.end(), nrl);
            return true;
        }
    }

    /* no intersection found, test if the second route is completely
       inside the first */
    if(route1->ContainsRoute(route2)) {
        if(inverted_regions) {
            if(route1->direction == 1 && route2->direction == 1) {

                int sc1 = route1->children.size();
                int sc2 = route2->children.size();
                    /* if both region have children, they should get merged
                       correctly here instead of this */
                if(sc1 && sc2)
                    printf("both have children contains: %d %d\n", sc1, sc2);

                /* remove all of route2's children, route1 clears them
                   (unless they interected with route1 children which we don't handle yet */
                for(IsoRouteList::iterator it2 = route2->children.begin();
                    it2 != route2->children.end(); it2++)
                    delete *it2;
                route2->children.clear();
                
                /* now determine if route2 affects any of route1's children,
                   if there are any intersections, it should mask away that area.
                   once completely merged, all the masks are removed and children
                   remain */
                IsoRouteList childrenmask; /* non-inverted */
                IsoRouteList mergedchildren; /* inverted */
                childrenmask.push_back(route2);
                while(childrenmask.size() > 0) {
                    IsoRoute *r1 = childrenmask.front();
                    childrenmask.pop_front();
                    while(route1->children.size() > 0) {
                        IsoRoute *r2 = route1->children.front();
                        route1->children.pop_front();
                        IsoRouteList childrl; /* see if there is a merge */

                        if(Merge(childrl, r1, r2, true, 1)) { 
                            for(IsoRouteList::iterator cit = childrl.begin(); cit != childrl.end(); cit++)
                                if((*cit)->direction == route1->direction)
                                    childrenmask.push_back(*cit);
                                else {
                                    IsoRoute *child = *cit;
                                    child->parent = route1;
                                    route1->children.push_back(child);
                                }
                            goto remerge_children;
                        } else
                            mergedchildren.push_back(r2);
                    }
                    delete r1; /* all children have been tried, so done with this mask */
                    
                remerge_children:
                    route1->children.splice(route1->children.end(), mergedchildren);
                }
            } else if(route1->direction == -1 && route2->direction == -1) {
                delete route1; /* keep smaller region if both inverted */
                route1 = route2;
            } else if(route1->direction == 1 && route2->direction == -1) {
                delete route2;
            } else {
                /* this is a child route with a normal route completely inside..
                   a contrived situation it is, should not get here often */
                printf("contrived delete: %d, %d\n", route1->Count(), route2->Count());
                delete route2;
            }
        } else /* no inverted regions mode */
            delete route2; /* it covers a sub area, delete it */

        rl.push_back(route1); /* no need to normalize */
        return true;
    }

    /* routes close enough to pass initial rectangle test but no
       actual intersection or overlap occurs so no merge takes places */
    return false;
}

Position *IsoRoute::ClosestPosition(double lat, double lon)
{
    Position pos(lat, lon), *minpos = NULL;
    double mindist = 0;
    Position *p = skippoints->point;
    do {
        double dist = pos.Distance(p);
        if(dist < mindist || mindist == 0) {
            minpos = p;
            mindist = dist;
        }
        p = p->next;
    } while(p != points);

    /* now try children */
    for(IsoRouteList::iterator it = children.begin(); it != children.end();  it++) {
        p = (*it)->ClosestPosition(lat, lon);
        if(p) {
            double dist = pos.Distance(p);
            if(dist < mindist || mindist == 0) {
                minpos = p;
                mindist = dist;
            }
        }
    }

    return minpos;
}

bool IsoRoute::Propagate(IsoRouteList &routelist, GribRecordSet &grib, RouteMapOptions &options)
{
    Position *p = skippoints->point;
    bool ret = false;
    if(p)
        do {
            if(p->Propagate(routelist, grib, options))
                ret = true;
            p = p->next;
        } while(p != skippoints->point);
    return ret;
}

int IsoRoute::Count()
{
    Position *p = skippoints->point;
    int count = 0;
    if(p)
        do {
            count++;
            p = p->next;
        } while(p != skippoints->point);
    return count;
}

void IsoRoute::UpdateStatistics(int &routes, int &invroutes, int &positions)
{
    invroutes += children.size();
    routes += children.size() + 1;

    for(IsoRouteList::iterator it = children.begin(); it != children.end(); it++)
        positions += (*it)->Count();

    positions += Count();
}

IsoChron::IsoChron(IsoRouteList r, wxDateTime t, GribRecordSet *g)
    : routes(r), time(t), m_Grib(g)
{
}

IsoChron::~IsoChron()
{
    for(IsoRouteList::iterator it = routes.begin(); it != routes.end(); ++it)
        delete *it;

    /* done with grib, so free memory and request next one */
    for(int i=0; i<Idx_COUNT; i++)
        delete m_Grib->m_GribRecordPtrArray[i];

    delete m_Grib;
}

void IsoChron::PropagateIntoList(IsoRouteList &routelist, GribRecordSet &grib, RouteMapOptions &options)
{
    for(IsoRouteList::iterator it = routes.begin(); it != routes.end(); ++it) {
        bool propagated = false;

        /* build up a list of iso regions for each point
           in the current iso */
        if((*it)->Propagate(routelist, grib, options))
            propagated = true;

        IsoRoute *x = new IsoRoute(*it);
        for(IsoRouteList::iterator cit = (*it)->children.begin();
            cit != (*it)->children.end(); cit++)
            if((*cit)->Propagate(routelist, grib, options)) {
                x->children.push_back(new IsoRoute(*cit, x)); /* copy child */
                propagated = true;
            }

        /* if any propagation occured even for children, then we clone this route
           this prevents backtracking, otherwise, we don't need this route
           (it's a dead end) but update it to drift with the current */
        if(propagated) {
            if(options.Currents) {
                IsoRoute *y = NULL;
                if(options.Anchoring) {
                    y = new IsoRoute(x);
                    for(IsoRouteList::iterator cit = x->children.begin();
                        cit != x->children.end(); cit++)
                        y->children.push_back(new IsoRoute(*cit, y)); /* copy child */
                    if(x->ApplyCurrents(grib, options))
                        routelist.push_back(y);
                    else
                        delete y; /* I guess we didn't need it after all */
                } else
                    x->ApplyCurrents(grib, options);
            }
            routelist.push_back(x);
        } else
            delete x;
    }
}

bool IsoChron::Contains(double lat, double lon)
{
    Position p(lat, lon);
    for(IsoRouteList::iterator it = routes.begin(); it != routes.end(); ++it)
        if((*it)->Contains(&p, true))
            return true;

    return false;
}

RouteMap::RouteMap()
{
}

RouteMap::~RouteMap()
{
    Clear();
}

bool RouteMap::ReduceList(IsoRouteList &merged,
                          IsoRouteList &routelist,
                          RouteMapOptions &options)
{
    /* once we have multiple worker threads, we can delegate a workers here
       to merge routes. */
               
    IsoRouteList unmerged;
    while(routelist.size()) {
        if(TestAbort())
            return false;

        IsoRoute *r1 = routelist.front();
        routelist.pop_front();
        while(routelist.size()) {
            IsoRoute *r2 = routelist.front();
            routelist.pop_front();
            IsoRouteList rl;
            if(Merge(rl, r1, r2, options.InvertedRegions, 0)) {
#if 0
                for(IsoRouteList::iterator it = rl.begin(); it != rl.end(); ++it)
                    routelist.push_back(*it);
#else /* merge new routes with each other right away before hitting the main list,
         does this still offer a speed improvement? if so make this a recursive function instead */
                IsoRouteList unmerged2;
                while(rl.size() > 0) {
                    r1 = rl.front();
                    rl.pop_front();
                    while(rl.size() > 0) {
                        r2 = rl.front();
                        rl.pop_front();
                        IsoRouteList rl2;
                        if(Merge(rl2, r1, r2, options.InvertedRegions, 0)) {
                            rl.splice(rl.end(), rl2);
                            goto remerge2;
                        } else
                            unmerged2.push_back(r2);
                    }
                    unmerged.push_back(r1);
                remerge2:
                    unmerged.splice(unmerged.end(), unmerged2);
                }
#endif
                goto remerge;
            } else
                unmerged.push_back(r2);
        }
        /* none more in list so nothing left to merge with */
        merged.push_back(r1);

    remerge:
        /* put any unmerged back in list to continue */
        for(IsoRouteList::iterator it = unmerged.begin(); it != unmerged.end(); ++it)
            routelist.push_back(*it);
        unmerged.clear();
    }
    return true;
}

/* enlarge the map by 1 level */
bool RouteMap::Propagate()
{
    Lock();
    bool notready = m_bFinished || m_bNeedsGrib;

    if(notready) {
        Unlock();
        return false;
    }

    if((!m_NewGrib ||
        !m_NewGrib->m_GribRecordPtrArray[Idx_WIND_VX] ||
        !m_NewGrib->m_GribRecordPtrArray[Idx_WIND_VY]) &&
       !m_Options.AllowDataDeficient) {
        m_bGribFailed = true;
        Unlock();
        return false;
    }

    /* copy the grib record set */
    GribRecordSet *grib = new GribRecordSet;
    grib->m_Reference_Time = m_NewGrib->m_Reference_Time;
    for(int i=0; i<Idx_COUNT; i++) {
        if(m_NewGrib->m_GribRecordPtrArray[i])
            grib->m_GribRecordPtrArray[i] = new GribRecord(*m_NewGrib->m_GribRecordPtrArray[i]);
        else
            grib->m_GribRecordPtrArray[i] = NULL;
    }

    wxDateTime GribTime = m_NewGribTime;

    RouteMapOptions options = m_Options;

    IsoRouteList routelist;
    Unlock();

    if(origin.size())
        origin.back()->PropagateIntoList(routelist, *grib, options);
    else
        routelist.push_back(new IsoRoute(new Position(options.StartLat, options.StartLon)));

    Lock();
    m_NewGrib = NULL;
    m_NewGribTime = GribTime + wxTimeSpan(0, 0, options.dt);
    m_bNeedsGrib = true;
    Unlock();

    IsoChron* update;
    if(routelist.Size()) {
        IsoRouteList merged;
        if(!ReduceList(merged, routelist, options)) {
            Unlock();
            return false;
        }

        update = new IsoChron(merged, GribTime, grib);
    } else
        update = NULL;

    Lock();
    if(update) {
        origin.push_back(update);
        if(update->Contains(m_Options.EndLat, m_Options.EndLon)) {
            m_bFinished = true;
            m_bReachedDestination = true;
        }
    } else
        m_bFinished = true;

    Unlock();
    return true;
}

Position *RouteMap::ClosestPosition(double lat, double lon)
{
    Position p(lat, lon), *minpos = NULL;
    double mindist = 0;
    Lock();
    for(IsoChronList::iterator it = origin.begin(); it != origin.end(); ++it) {
        for(IsoRouteList::iterator rit = (*it)->routes.begin(); rit != (*it)->routes.end(); ++rit) {
            Position *pos = (*rit)->ClosestPosition(lat, lon);
            if(pos) {
                double dist = p.Distance(pos);
                if(dist < mindist || mindist == 0) {
                    minpos = pos;
                    mindist = dist;
                }
            }
        }
    }
    Unlock();
    return minpos;
}

void RouteMap::Reset(wxDateTime time)
{
    Lock();
    Clear();

    m_NewGrib = NULL;
    m_NewGribTime = time;
    m_bNeedsGrib = true;

    m_bReachedDestination = false;
    m_bGribFailed = false;
    m_bFinished = false;

    Unlock();
}

void RouteMap::GetStatistics(int &isochrons, int &routes, int &invroutes, int &positions)
{
    Lock();
    isochrons = origin.size();
    routes = invroutes = positions = 0;
    for(IsoChronList::iterator it = origin.begin(); it != origin.end(); ++it)
        for(IsoRouteList::iterator rit = (*it)->routes.begin(); rit != (*it)->routes.end(); ++rit)
            (*rit)->UpdateStatistics(routes, invroutes, positions);
    Unlock();
}

void RouteMap::Clear()
{
    for(IsoChronList::iterator it = origin.begin(); it != origin.end(); ++it)
        delete *it;

    origin.clear();
}
