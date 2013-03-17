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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers
#include "wx/wxprec.h"

#include "../../grib_pi/src/GribRecordSet.h"

#include "BoatSpeed.h"
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
   Wx - X component of wind
   Wy - Y component of wind
   VW - velocity of wind over water
   WA  - Angle of wind relative to true north
*/

void WindOverWater(double C, double VC, double WG, double VWG, double &WA, double &VW)
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

/* I don't actually think the speed of this is critical anymore */
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

    /* quick test to avoid calculations if segments are far apart */
#if 0 /* is this faster? */
    double max1x, min1x, max1y, min1y;
    double max2x, min2x, max2y, min2y;
    if(x1 > x2) max1x = x1, min1x = x2; else max1x = x2, min1x = x1;
    if(x3 > x4) max2x = x3, min2x = x4; else max2x = x3, min2x = x4;

    if(max1x < min2x || max2x < min1x)
        return 0;

    if(y1 > y2) max1y = y1, min1y = y2; else max1y = y2, min1y = y1;
    if(y3 > y4) max2y = y3, min2y = y4; else max2y = y3, min2y = y4;

    if(max1y < min2y || max2y < min1y)
        return 0;
#else
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

Position::Position(double latitude, double longitude, Position *p)
    : lat(latitude), lon(longitude), parent(p), propagated(false)
{
    lat -= fmod(lat, EPSILON);
    lon -= fmod(lon, EPSILON);

    prev = next = this;
}

Position::Position(Position *p)
    : lat(p->lat), lon(p->lon), parent(p->parent),
      propagated(p->propagated)
{
}

/* create a looped route by propagating from a position by computing
   the location the boat would be in if sailed at every angle
   relative to the true wind. */
void Position::Propagate(RouteHeap &routeheap, GribRecordSet &grib, RouteMapOptions &options)
{
    /* already propagated from this position, don't need to again */
    if(propagated)
        return;

    propagated = true;

    Position *points = NULL;
    /* through all angles relative to wind */
    int count = 0;

    if(Swell(grib, lat, lon) > options.MaxSwellMeters)
        return;

    if(fabs(lat) > options.MaxLatitude)
        return;

    double WG, VWG;
    Position *p = this;
    while(!Wind(grib, WG, VWG, p->lat, p->lon)) {
        p = p->parent; /* should we use parent's time here ? */
        if(!options.AllowDataDeficient || !p)
            return;
    }

    if(VWG > options.MaxWindKnots)
        return;

    double C, VC;
    if(!Current(grib, VC, C, lat, lon))
        C = VC = 0;

    double W, VW;
    WindOverWater(C, VC, WG, VWG, W, VW);

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
        double P = 0; /* for now using sails only no power */
        double B, VB;

        VB = options.boat->Speed(P, H, VW);
        B = W + H; /* rotated relative to wind */

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
            Position *rp = new Position(dlat, dlon, this);

            /* avoid propagating from positions which go in a direction too much
               diverted from the correct course.  The reason to have this position
               at all (and not move this test to before the position exists), is
               to maintain continuity in the generated map (it looks much better) */
            if(fabs(heading_resolve(BG - bearing)) > options.MaxDivertedCourse)
                rp->propagated = true;

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

    if(count < 3) /* tested in normalize, but save the extra new and delete */
        return;

    RouteList rl = (new Route(points, count))->Normalize(0, options.InvertedRegions);
    for(RouteList::iterator it = rl.begin(); it != rl.end(); it++)
        routeheap.Insert(*it);
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

Position* Position::Copy()
{
    Position *p = this;
    Position *np = new Position(p);
    Position *fp = np;

    while(p->next != this) {
        p = p->next;
        Position *nnp = new Position(p);
        np->next = nnp;
        nnp->prev = np;
        np = nnp;
    }
    fp->prev = np;
    np->next = fp;
    return fp;
}

Route::Route(Position *p, int cnt, int dir)
    : points(p), count(cnt), direction(dir), parent(NULL)
{
}

/* copy constructor */
Route::Route(Route *r, Route *p)
    : points(r->points->Copy()), count(r->count), direction(r->direction), parent(p)
{
    /* copy children */
    for(RouteList::iterator it = r->children.begin(); it != r->children.end(); ++it)
        children.push_back(new Route(*it, this));
}

Route::~Route()
{
    for(RouteList::iterator it = children.begin(); it != children.end(); ++it)
        delete *it;

    if(!points)
        return;

    Position *p = points;
    do {
        Position *dp = p;
        p = p->next;
        delete dp;
    } while(p != points);
}

void Route::Print()
{
    if(!points)
        printf("Empty Route\n");
    else {
        Position *p = points;
        do {
            printf("(%.2f %.2f) ", p->lat, p->lon);
            p = p->next;
        } while(p != points);
        printf("\n");
    }
}

bool Route::Contains(Route *r)
{
    if(count < 3)
        return false; /* I hope never to get here.. but obviously these regions contain nothing */

    Position *pos = r->points;
    Position np(91, 0); /* latitude 91 should be farther north than anywhere */

tryagain:
    // Now go through each of the lines in the polygon and test intersections
    int   wnumintsct = 0 ;
    Position *r1 = points;
    Position *r2 = r1->next;
    do {
        int ret = TestIntersection(r1, r2, pos, &np);
        if(ret == -2)  { // this case should actually never get hit
            printf("cannot deal with failed region for containment test\n");
            exit(1);
        }
        if(ret == 2) { // not conclusive, try another point
            pos = pos->next;
            if(pos == r->points) /* avoid deadlock.. lets hope we dont do this often */
                return true; /* probably good to delete route in this case */

            goto tryagain;
        } else if(ret)
            wnumintsct++;

        r1 = r2;
        r2 = r2->next;
    } while(r1 != points);

    // odd number of intersections?
    if(wnumintsct&1)
        return true;

    return false;
}

/* apply current to given route */
void Route::ApplyCurrent(GribRecordSet &grib, RouteMapOptions &options)
{
    if(!points)
        return;

    Position *p = points;
    double timeseconds = options.dt;
    do {
        double C, VC;
        if(Current(grib, VC, C, p->lat, p->lon)) {
            /* drift distance over ground */
            double dist = VC * timeseconds / 3600.0;
            ll_gc_ll(p->lat, p->lon, C, dist, &p->lat, &p->lon);
        }

        p = p->next;
    } while(p != points);
}

enum { MINLON, MAXLON, MINLAT, MAXLAT };

/* return false if longitude is possibly invalid */
bool Route::FindRouteBounds(double bounds[4])
{
    bounds[MINLAT] = bounds[MAXLAT] = points->lat;
    bounds[MINLON] = bounds[MAXLON] = points->lon;
    Position *maxlat = points;

    Position *p = points->next;
    while(p != points) {
        bounds[MINLAT] = MIN(p->lat, bounds[MINLAT]);
        bounds[MAXLAT] = MAX(p->lat, bounds[MAXLAT]);
        bounds[MINLON] = MIN(p->lon, bounds[MINLON]);
        bounds[MAXLON] = MAX(p->lon, bounds[MAXLON]);
            
        if(p->lat == bounds[MAXLAT])
            maxlat = p;
        p = p->next;
    }
    points = maxlat; /* set to max lat for merging to keep outside */
    
    /* cross date line? */
    if(bounds[MAXLON] - bounds[MINLON] > 180)
        return false; /* possibly invalid for longitude */
    return true;
}

void Route::RemovePosition(Position *p)
{
    if(points == points->next)
        points = NULL;
    else {
        p->next->prev = p->prev;
        p->prev->next = p->next;
        if(p == points)
            points = p->next;
    }
    count--;
    delete p;
}

/* Take a route which may overlaps with itself and convert to a list
   of normalized routes which no longer have overlaps */
RouteList Route::Normalize(int level, bool inverted_regions)
{
    RouteList ret;
reset:
    /* don't do anything with triangles or fewer points */
    if(count < 4) {
        if(count < 3)
            delete this;
        else
            ret.push_back(this);
        return ret;
    }

    Position *p = points;
    Position *q = p->next;
    do {
        Position *r = q->next;
        Position *s = r->next;

        int innercount = 2;
        /* dont test adjacent segments, so in the case p first, we skip the last segment */
        while(!(p == points && s == points) && r != points) {
            int dir = TestIntersection(p, q, r, s);
                /* this can change points, so reset to be safe
                   in the future maybe we can avoid this and not reset */
            if(dir) {
                if(dir == -2) {
                    RemovePosition(q);
                    goto reset;
                }
                if(dir == 2) { /* too close to call, delete s and try next point */
                    RemovePosition(s);
                    goto reset;
                }

                p->next = s;
                s->prev = p;
                r->next = q;
                q->prev = r;

                count -= innercount;
                Route *x = new Route(q, innercount, dir);
                RouteList sub = x->Normalize(level + 1, inverted_regions);

                if(inverted_regions) {
                    if(level == 0) {
                        if(dir == -1) {
                            /* we should not actually get here, slight numerical error */
                            for(RouteList::iterator it = sub.begin(); it != sub.end(); ++it)
                                delete *it; /* discard */
                        } else
                        for(RouteList::iterator it = sub.begin(); it != sub.end(); ++it) {
                            if(direction == (*it)->direction)
                                ret.push_back(*it); /* sibling */
                            else if(parent) /* no grandchildren allowed */
                                delete *it;
                            else if(!Contains(*it)) {
                                printf("error, supposed child wasn't contained\n");
                                delete *it;
                            } else
                                /* better be completely inside us
                                    and not intersect with any existing children */
                            {
                                (*it)->parent = this;
                                children.push_back(*it); 
                            }
                        }
                    } else
                        ret.splice(ret.end(), sub); /* all subregions are siblings */

                } else { /* no inverted regions mode */
                    for(RouteList::iterator it = sub.begin(); it != sub.end(); ++it) {
                        if(direction != (*it)->direction || dir != 1)
                            delete *it; /* discard inversion or outer region */
                        else
                            ret.push_back(*it); /* merge as sibling */
                    }
                }
                goto reset; /* should be possible to do something faster than
                               complete reset here */
            }

            innercount++;
            r = s;
            s = s->next;
        }
        p = q;
        q = q->next;
    } while(q != points);

    ret.push_back(this);
    return ret;
}

/* merge another route into this one */
bool Merge(RouteList &rl, Route *route1, Route *route2, bool inverted_regions)
{
    static int mergecnt;
    mergecnt++;

    /* quick sanity check */
    if(route1->direction != route2->direction) {
        if(!inverted_regions) {
            printf("cannot merge routes in different direction\n");
            exit(1);
        }
    }

    if(route1->direction == -1 && route2->direction == -1) {
        printf("cannot merge inverted routes\n");
        exit(1);
    }

    /* quick test to make sure we could possibly intersect with bounds */
    double bounds1[4], bounds2[4];
    bool r1bv = route1->FindRouteBounds(bounds1);
    bool r2bv = route2->FindRouteBounds(bounds2);
    if(bounds1[MINLAT] > bounds2[MAXLAT] || bounds1[MAXLAT] < bounds2[MINLAT])
        return false;
    if(r1bv && r2bv && (bounds1[MINLON] > bounds2[MAXLON] || bounds1[MAXLON] < bounds2[MINLON]))
        return false;

    /* make sure we start on the outside */
    if(route2->points->lat > route1->points->lat) {
        Route *t = route1;
        route1 = route2;
        route2 = t;
    }

    Position *p = route1->points;
    Position *q = p->next;
    do {
        Position *r = route2->points;
        Position *s = r->next;
        do {
            double dir = TestIntersection(p, q, r, s);
            /* too close to call, delete s and try next point */
            if(dir == -2) {
                route1->RemovePosition(q);
                if(route1->count < 3) { /* nothing left to merge */
                    delete route1;
                    rl.push_back(route2); /* no need to normalize */
                    return true;
                }
                
                q = p->next;
                goto outer_continue;
            }

            if(dir == 2) { 
                route2->RemovePosition(s);
                if(route2->count < 3) { /* nothing left to merge */
                    delete route2;
                    rl.push_back(route1); /* no need to normalize */
                    return true;
                }
                
                s = r->next;
                continue;
            }

            if(dir == 1 || dir == -1) {
                /* debugging sanity check */
                if(dir == -1) {
                    if(route1->direction != 1 || route2->direction != -1) {
                        /* we intersected at the wrong side, skip this intersection
                           and continue to find the intersection we want,  this occurs
                           when a line segment passes completely through a region.
                           could possibly merge here anyway and
                           still get the right result...  */
                        goto skip;
                    }
                } else if(dir == 1) {
                    if(route1->direction == 1 && route2->direction == -1) {
                        goto skip;
                    }
                }

                /* swap intersecting connections */
                p->next = s;
                s->prev = p;
                r->next = q;
                q->prev = r;

                route1->count += route2->count;

                /* combine children, if route1 is top level */
                if(!route1->parent) {
                    for(RouteList::iterator it = route2->children.begin();
                        it != route2->children.end(); it++)
                        (*it)->parent = route1;
                    route1->children.splice(route1->children.end(), route2->children);
                }

                route2->points = NULL; /* all points are now in route1 */
                delete route2;
                rl = route1->Normalize(0, inverted_regions);
                return true;
            }
        skip:

            r = s;
            s = s->next;
        } while(r != route2->points);

        p = q;
        q = q->next;
    outer_continue:;
    } while(p != route1->points);

    /* no intersection found, test if the second route is completely
       inside the first */
    if(route1->Contains(route2)) {
        if(inverted_regions) {
            /* remove all of route2's children, route1 clears them */
            for(RouteList::iterator it2 = route2->children.begin();
                it2 != route2->children.end(); it2++)
                delete *it2;
            route2->children.clear();

            /* now determine if route2 affects any of route1's children */
            for(RouteList::iterator it = route1->children.begin();
                it != route1->children.end(); it++) {
                RouteList childrl; /* see if route2 can merge with this child */
                if(Merge(childrl, route2, *it, inverted_regions)) { 
                    route1->children.erase(it);
                    for(RouteList::iterator cit = childrl.begin(); cit != childrl.end(); cit++)
                        if((*cit)->direction == 1)
                            rl.push_back(*cit); /* could affect other children.. keep it */
                        else {
                            (*cit)->parent = route1;
                            route1->children.push_back(*cit);
                        }
                    goto childmerge;
                }
            }
        }

        delete route2; /* it covers a sub area, can remove */

    childmerge:
        rl.push_back(route1); /* no need to normalize */
        return true;
    }

    /* routes close enough to fail initial rectangle test but no
       actual intersection or overlap occurs so no merge takes places */
    return false;
}

Position *Route::ClosestPosition(double lat, double lon)
{
    Position pos(lat, lon), *minpos = NULL;
    double mindist = 0;
    Position *p = points;
    do {
        double dist = pos.Distance(p);
        if(dist < mindist || mindist == 0) {
            minpos = p;
            mindist = dist;
        }
        p = p->next;
    } while(p != points);

    /* now try children */
    for(RouteList::iterator it = children.begin(); it != children.end();  it++) {
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

void Route::Propagate(RouteHeap &routeheap, GribRecordSet &grib, RouteMapOptions &options)
{
    if(!points)
        return;
        
    Position *p = points;
    do {
        p->Propagate(routeheap, grib, options);
        p = p->next;
    } while(p != points);

    for(RouteList::iterator it = children.begin(); it != children.end(); it++)
        (*it)->Propagate(routeheap, grib, options);
}

RouteHeap::RouteHeap()
  : size(0), maxsize(256)
{
    Heap = (Route **)malloc((sizeof *Heap) * maxsize);
}

RouteHeap::~RouteHeap()
{
    free(Heap);
}

void RouteHeap::expand()
{
    if(++size <= maxsize)
        return;
    maxsize = size;
    Heap = (Route **)realloc(Heap, (sizeof *Heap) * maxsize);
}

void RouteHeap::Insert(Route *r)
{
    int n = size;
    expand();

    for(;;) {
        int o = parent(n);
        if(n == 0 || Heap[o]->count <= r->count) {
            Heap[n] = r;
            break;
        }

        Heap[n] = Heap[o];
        n = o;
    }
}

Route *RouteHeap::Remove()
{
    Route *ret = Heap[0];

    int o = 0; /* remove index 0 */
    int c = child(o);
    int s = --size;
    /* shuffle up filling removed entry */
    while(c < s && MIN(Heap[c]->count, Heap[c+1]->count) < Heap[s]->count) {
        if(Heap[c]->count > Heap[c+1]->count)
            c++;

        Heap[o] = Heap[c];
        o = c;
        c = child(o);
    }

    /* now find place for last entry to satisfy heap condition */
    int p = parent(o);
    while(o > 0 && Heap[p]->count > Heap[s]->count) {
        Heap[o] = Heap[p];
        o = p;
        p = parent(o);
    }

    Heap[o] = Heap[s];

    return ret;
}

RouteIso::RouteIso(Position *p, wxDateTime t)
    : time(t)
{
    routes.push_back(new Route(p));
}

RouteIso::RouteIso(RouteList r, wxDateTime t)
    : routes(r), time(t)
{
}

RouteIso::~RouteIso()
{
    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it)
        delete *it;
}

void RouteIso::PropagateIntoHeap(RouteHeap &routeheap, GribRecordSet &grib, RouteMapOptions &options)
{
    /* build up a list of iso regions for each point
       in the current iso */
    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it)
        (*it)->Propagate(routeheap, grib, options);

    /* if none propagated we aren't going anywhere */
    if(routeheap.Size() == 0)
        return;

    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it) {
        /* also keep previous route to avoid backtracking,
           but update it to drift with the current */
        Route *x = new Route(*it);
        x->ApplyCurrent(grib, options);
        routeheap.Insert(x);
        
        /* Is anchoring possible? Add the old route without currents */
        if(options.Anchoring)
            routeheap.Insert(new Route(*it));
    }
}

bool RouteIso::Contains(double lat, double lon)
{
    Route r(new Position(lat, lon));
    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it)
        if((*it)->Contains(&r))
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

RouteList RouteMap::ReduceHeap(RouteHeap &routeheap, RouteMapOptions &options)
{
    /* once we have multiple worker threads, we can delegate a workers here
       to merge routes. */
               
    RouteList merged, unmerged;
    while(routeheap.Size()) {
        Route *r1 = routeheap.Remove();
        while(routeheap.Size()) {
            Route *r2 = routeheap.Remove();
            RouteList rl;
            if(Merge(rl, r1, r2, options.InvertedRegions)) {
#if 0
                for(RouteList::iterator it = rl.begin(); it != rl.end(); ++it)
                    routeheap.Insert(*it);
#else /* merge new routes with each other right away before hitting the main heap */
                RouteList unmerged2;
                while(rl.size() > 0) {
                    r1 = rl.front();
                    rl.pop_front();
                    while(rl.size() > 0) {
                        r2 = rl.front();
                        rl.pop_front();
                        RouteList rl2;
                        if(Merge(rl2, r1, r2, options.InvertedRegions)) {
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
        /* none more in heap so nothing left to merge with */
        merged.push_back(r1);

    remerge:
        /* put any unmerged back in heap to continue */
        for(RouteList::iterator it = unmerged.begin(); it != unmerged.end(); ++it)
            routeheap.Insert(*it);
        unmerged.clear();
    }
    return merged;
}

/* enlarge the map by 1 level */
bool RouteMap::Propagate()
{
    Lock();
    bool notready = !origin.size() || m_bFinished || m_bNeedsGrib;

    if(notready) {
        Unlock();
        return false;
    }

    if(!m_NewGrib ||
       !m_NewGrib->m_GribRecordPtrArray[Idx_WIND_VX] ||
       !m_NewGrib->m_GribRecordPtrArray[Idx_WIND_VY]) {
        m_bGribFailed = true;
        Unlock();
        return false;
    }

    /* copy the grib record set */
    GribRecordSet grib;
    grib.m_Reference_Time = m_NewGrib->m_Reference_Time;
    for(int i=0; i<Idx_COUNT; i++) {
        if(m_NewGrib->m_GribRecordPtrArray[i])
            grib.m_GribRecordPtrArray[i] = new GribRecord(*m_NewGrib->m_GribRecordPtrArray[i]);
        else
            grib.m_GribRecordPtrArray[i] = NULL;
    }

    wxDateTime GribTime = m_NewGribTime;

    if(m_bUpdateBoat) {
        m_Options.boat = &m_boats[m_currentboat];
        m_currentboat = !m_currentboat;
    }

    RouteMapOptions options = m_Options;
    Unlock();

    RouteHeap routeheap;
    RouteIso *last = origin.back();
    last->PropagateIntoHeap(routeheap, grib, options);

    /* done with grib, so free memory and request next one */
    for(int i=0; i<Idx_COUNT; i++)
        delete grib.m_GribRecordPtrArray[i];

    Lock();
    m_NewGrib = NULL;
    m_NewGribTime = last->time + wxTimeSpan(0, 0, options.dt);
    m_bNeedsGrib = true;
    Unlock();

    RouteIso* update;
    if(routeheap.Size()) {
        RouteList merged = ReduceHeap(routeheap, options);
        update = new RouteIso(merged, GribTime);
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
    for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it) {
        for(RouteList::iterator rit = (*it)->routes.begin(); rit != (*it)->routes.end(); ++rit) {
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

void RouteMap::Reset(double lat, double lon, wxDateTime time)
{
    Clear();
    Lock();
    origin.push_back(new RouteIso(new Position(lat, lon), time));

    m_NewGrib = NULL;
    m_NewGribTime = time;
    m_bNeedsGrib = true;

    m_bReachedDestination = false;
    m_bGribFailed = false;
    m_bFinished = false;
    Unlock();
}

void RouteMap::Clear()
{
    Lock();
    for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it)
        delete *it;

    origin.clear();
    Unlock();
}
