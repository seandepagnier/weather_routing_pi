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
   orthonormal lines which describe the position of the boat.

   Starting at a given location, propagate outwards in all directions.
   the outward propagation is guarenteed a closed region, except it may
   be shifted significantly.  There are wall regions
   which must be tested to determine intersections.  If the route
   crosses a wall, then the resulting position of the point is reduced
   to the position of intersection, and tagged as dead so it is not
   propagated any further.

   The previous route can be treated as a wall because normally we
   don't sail in one direction then turn around and sail the other way.

   Once the first region is established, repeat this process for
   each outward point merging each region together.  It is best
   to merge small regions, inserting to the end of the list
   so that typically you merge regions with similar point count.

   To merge regions requires the same algorithm for descrambling
   single regions.  Therefore just append them to the list of positive
   regions before descrambling.

   To normalize a region means that no two line segments between border points
   will intersect.  So given a complete route, and a list of
   both positive and negative regions, the normalized route will no longer intersect
   or contain any points from any of the positive or negative regions,
   but may append additional regions to both of these lists.

   For each segment go through and see if it intersects
   with any other line segment.  When it does the old route will follow
   the correct direction of the intersection on the intersected route,

   A positive intersection comes in from the right.

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

   takes a single route which may intersect itself, and return a list of
   routes which contain no intersections but define the region

   normalize(route)
   for each segment a from start to end
   for each segment b from a+1 to end
   if a intersects b
   split into two routes so that we continue on outer route
   recursively normalize new route and push to merge region list
   break from iner loop

   if it is a different sign intersection than route sign then
   use outer list otberwise use inner list

   for each route in normalized list
   try to merge the route each route in inner or outer list
   if success, normalize the result and add to normalized list
   otherwise add the route to the inner or outer list

   continue finding intersections until we make a complete loop

   merge(route1, route2)
   take two normalized routes and merge them into a single route if they intersect,
   if none exists then
   determine if either route is inside the other, if so, eliminate same sign
   nested route, or else subregion it, and return outer route as merged
   if the routes do not intersect or nest, fail to merge

   merge_regions(region1, region2)
   region = empty region
   while region1.notempty
   route1 = region1.pop()
   while region2.notempty
   route2 = region2.pop()
   if(route = merge(route1, route2))
   new_region = normalize(route)
   old_regions = merge_regions(region1, region2)
   return merge_regions(new_region, old_regions)
   region2.push(route2)
   region.push(route1);
   region.splice(region2);
   return region;
*/

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "wx/datetime.h"

#include "BoatSpeed.h"
#include "RouteMap.h"
#include "weather_routing_pi.h"
#include "Utilities.h"

#include "georef.h"

Route *curRoute = NULL;

static void Wind(double &winddirground, double &windspeedground,
                 double lat, double lon, wxDateTime time)
{
    windspeedground = 20;//+100*(lon-174);
    winddirground = 170;//remainder(300*(lat+35), 360);//rand()%60;

    windspeedground = fabs(10*(1+sin((lon-166)/2)));
    winddirground = remainder(800*(lat+16), 360);//rand()%60;
}

static void Current(double &currentspeed, double &currentdir,
                    double lat, double lon, wxDateTime time)
{
    currentspeed = 0;
    currentdir = 0;
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
   WA  - Angle of wind relative to compass north
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


wxMutex routemutex;
//int debugstep=1000000, debugcurstep;

void RouteDebugStep()
{
}

int debugcnt = 0;
bool RouteDebugWait()
{

#if 0
    debugcurstep++;
    if(debugcurstep > debugmaxstep) {
        throw debugcurstep;
        return true;
    }
#endif
    return false;
#if 0
    routemutex.Unlock();
    while(debugcurstep == debugstep)
        wxThread::Sleep(100);
    debugcurstep++;
    routemutex.Lock();
#endif
}

/* find intersection of two line segments
   if no intersection return 0, otherwise, 1 if the
   second line crosses from right to left, or -1 for left to right

   Truth equations to calculate intersection (x, y)
   (y-y1) * (x2-x1) = (y2-y1) * (x-x1)
   (y-y3) * (x4-x3) = (y4-y3) * (x-x3)
*/

#define EPS 2e-10

double TestIntersection(Position *p1, Position *p2, Position *p3, Position *p4)
{
    double x1 = p1->lon, x2 = p2->lon, x3 = p3->lon, x4 = p4->lon;
    double y1 = p1->lat, y2 = p2->lat, y3 = p3->lat, y4 = p4->lat;

#if 0

    double ax = x2 - x1, ay = y2 - y1;
    double bx = x3 - x4, by = y3 - y4;
    double cx = x1 - x3, cy = y1 - y3;
    double denom = ay * bx - ax * by;
    if(fabs(denom) < 2e-10) /* parallel or really close to parallel */
        return 2;

    double recip = 1 / denom;
    double na = (by * cx - bx * cy) * recip;
    double nb = (ax * cy - ay * cx) * recip;
    if(na < 0 - EPS || na > 1 + EPS) return 0;
    if(nb < 0 - EPS || nb > 1 + EPS) return 0;

    if((na > EPS && na < 1 - EPS) &&
       (nb > EPS && nb < 1 - EPS))
        return denom < 0 ? 1 : -1;

    return 2;
#endif

    /* optimization to avoid calculations if segments are far apart */
#if 1
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;
#endif

    if((x1 == x2 && y1 == y2) || /* really should not have any zero segments */
       (x3 == x4 && y3 == y4))
        return 2;

    if((x1 == x3 && y1 == y3) ||
       (x1 == x4 && y1 == y4) ||
       (x2 == x3 && y2 == y3) ||
       (x2 == x4 && y2 == y4))
        return 2;

    double dem = (x1*(y4-y3)+x2*(y3-y4)+(x4-x3)*y2+(x3-x4)*y1);

    if(fabs(dem) < 2e-14) /* parallel or really close to parallel */
        return 2;

    /* find intersection point */
    double x = (x1*(x3*y4-x4*y3+y2*(x4-x3)) + x2*(x4*y3-x3*y4+y1*(x3-x4)))/dem;
    double y = (y1*(x3*y4-x4*y3+x2*(y3-y4)) + y2*(x4*y3-x3*y4+x1*(y4-y3)))/dem;

#if 1
    double xa, ya, xb, yb, xc, yc, da, db, dc;
    xa = x1 - x2, ya = y1 - y2, da = xa*xa + ya*ya;
    xb = x1 - x , yb = y1 - y , db = xb*xb + yb*yb;
    xc = x2 - x , yc = y2 - y , dc = xc*xc + yc*yc;

    if(db > da || dc > da)
        return 0;

    xa = x3 - x4, ya = y3 - y4, da = xa*xa + ya*ya;
    xb = x3 - x , yb = y3 - y , db = xb*xb + yb*yb;
    xc = x4 - x , yc = y4 - y , dc = xc*xc + yc*yc;

    if(db > da || dc > da)
        return 0;
#else
    /* is this point outside either segment */
    if((x > x1 && x > x2) || (x > x3 && x > x4) ||
       (x < x1 && x < x2) || (x < x3 && x < x4) ||
       (y > y1 && y > y2) || (y > y3 && y > y4) ||
       (y < y1 && y < y2) || (y < y3 && y < y4))
        return 0;
    else
#endif
        return dem < 0 ? 1 : -1;

}

#define EPSILON (2e-8)
Position::Position(double latitude, double longitude, Position *p, bool hl)
    : lat(latitude), lon(longitude), parent(p), hitland(hl), propagated(false)
{
    lat -= fmod(lat, EPSILON);
    lon -= fmod(lon, EPSILON);

    prev = next = this;
}

Position::Position(Position *p)
    : lat(p->lat), lon(p->lon), parent(p->parent),
      hitland(p->hitland), propagated(p->propagated)
{
}

double gdist;
/* create a looped route by propagating from a position by computing
   the location the boat would be in if sailed at every angle
   relative to the true wind. */
RouteList Position::PropagateRoutePosition(wxDateTime time, wxTimeSpan dt,
                                        double degree_step, BoatSpeed &boat)
{
    /* already propagated from this position, don't need to again */
    if(propagated)
        return RouteList();

    propagated = true;

    Position *points = NULL;
    /* through all angles relative to wind */
    bool hitlandprev = false;
    int count = 0;
    for(double W = 0; W < DEGREES; W += degree_step) {
        double VWG, WG;
        Wind(WG, VWG, lat, lon, time);

        double C, VC;
        Current(VC, C, lat, lon, time);

        double WA, VW;
        WindOverWater(C, VC, WG, VWG, WA, VW);

        double VB, BA;
        double P = 0; /* for now using sails only no power */
        boat.Speed(P, W-WA, VW, BA, VB);

        /* failed to determine speed.. I guess we can always drift? */
        if(isnan(BA) || isnan(VB)) {
            BA = VB = 0;
        }

        double BtA = BA - WA;
        if(BtA < 0)
            BtA += 360;

        /* compound boatspeed with current */
        double BG, VBG;
        BoatOverGround(BtA, VB, C, VC, BG, VBG);

        /* distance over ground */
//        double dist = 1;
    double dist = VBG / 3600000.0 * dt.GetMilliseconds().ToDouble();

//    dist = gdist;
        double dlat, dlon;
        ll_gc_ll(lat, lon, /*BG*/W, dist, &dlat, &dlon);

        bool hitland = CrossesLand(dlat, dlon);
        if(!hitland && dist) {
            Position *rp = new Position(dlat, dlon, this, hitlandprev);
            if(points) {
                rp->prev=points->prev;
                rp->next=points;
                points->prev->next = rp;
                points->prev = rp;
            } else
                points = rp;
            count++;
        }
        hitlandprev = hitland;
    }

    if(points)
        return (new Route(points, count, 1))->Normalize();
    return RouteList();
}

double Position::Distance(Position *p)
{
    return DistGreatCircle(lat, lon, p->lat, p->lon);
}

#include "../../../include/chcanv.h"
#include "../../../include/gshhs.h"
bool Position::CrossesLand(double dlat, double dlon)
{
//    extern ChartCanvas      *cc1;
//    GSHHSChart* gshhschart = cc1->GetWorldBackgroundChart();

//    GSHHSChart* gshhspolyreader = cc1->GetWorldBackgroundChart();

    static GshhsReader *reader;
    static Projection proj;
    if(reader == NULL)
        reader = new GshhsReader(&proj);

    QLineF trajectWorld(lon, lat, dlon, dlat);
    QLineF traject(0, 0, dlon-lon, dlat-lat);

    proj.SetScreenSize(1, 1);
    proj.SetCenterInMap(traject.p1().x, traject.p1().y);


    return reader->crossing2(traject, trajectWorld);

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
    RepositionForStarting();
}

/* copy constructor */
Route::Route(Route *r)
    : points(r->points->Copy()), count(r->count), direction(r->direction), parent(NULL)
{
}

/* make the first position in the route the one with maximum latitude */
void Route::RepositionForStarting()
{
    /* start with maximum latitude */
    double maxlat = -91;
    Position *max;

    Position *p = points;
    do {
        if(p->lat > maxlat) {
            maxlat = p->lat;
            max = p;
        }
        p = p->next;
    } while(p != points);
    points = max;
}

bool Route::Contains(Route *r)
{
    if(count < 3)
        return false; /* I hope never to get here */

    Position *pos = r->points;
    Position np(91, 9);

tryagain:
    // Now go through each of the lines in the polygon and test intersections
    int   wnumintsct = 0 ;
    Position *r1 = points;
    Position *r2 = r1->next;
    do {
        int ret = TestIntersection(r1, r2, pos, &np);
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

void Route::Render(PlugIn_ViewPort *vp)
{
    glBegin(GL_LINE_LOOP);
    Position *p = points;
    do {

//    if(curRoute) {
        if(0) {
            static double crcr, crcg, crcb;
            glColor3f(crcr, crcg, crcb);
            crcr = !crcr; 
            crcg += .06; if(crcg > 1) crcg = 0;
            crcb += .08; if(crcb > 1) crcb = 0;
        } else {
            
            if(parent && parent->parent)
                glColor3f(0, 1, 0);
            else
            if(parent)
                glColor3f(0, 1, 1);
            else
            if(direction == -1)
                    glColor3f(0, 0, 1);
            else
                if(p->hitland || p->next->hitland)
                    glColor3f(.5, .5, 0);
                else
                    glColor3f(1, 0, 0);
        }
        
        wxPoint point;
        GetCanvasPixLL(vp, &point, p->lat, p->lon);
        glVertex2i(point.x, point.y);
        p = p->next;
    } while(p != points);
    glEnd();

    glPointSize(3);
    glBegin(GL_POINTS);
    p = points;
    do {
        wxPoint point;
        if(p->propagated)
            glColor3f(0, 1, 1);
        else
            glColor3f(1, 1, 1);

        GetCanvasPixLL(vp, &point, p->lat, p->lon);
        glVertex2i(point.x, point.y);
        p = p->next;
    } while(p != points);
    glEnd();

    /* now draw the children */
    for(RouteList::iterator it = children.begin(); it != children.end(); ++it)
        (*it)->Render(vp);

}

int Route::size()
{
    int count = 0;
    Position *p = points;
    do {
        count ++;
        p = p->next;
    } while(p != points);
    return count;
}

enum { MINLAT, MAXLAT, MINLON, MAXLON };
//#define MIN(a, b) ((a) < (b) ? (a) : (b))
//#define MAX(a, b) ((a) > (b) ? (a) : (b))

void Route::FindRouteBounds(double bounds[4])
{
    Position *p = points;
    bounds[MINLAT] = bounds[MAXLAT] = p->lat;
    bounds[MINLON] = bounds[MAXLON] = p->lon;
    do {
        bounds[MINLAT] = MIN(p->lat, bounds[MINLAT]);
        bounds[MAXLAT] = MAX(p->lat, bounds[MAXLAT]);
        bounds[MINLON] = MIN(p->lon, bounds[MINLON]);
        bounds[MAXLON] = MAX(p->lon, bounds[MAXLON]);
        p = p->next;
    } while(p != points);
}

void Route::RemovePosition(Position *p)
{
    p->next->prev = p->prev;
    p->prev->next = p->next;
    count--;
    if(p == points) {
        points = p->next;
        RepositionForStarting();
    }
    delete p;
}

/* Take a route which may overlaps with itself and convert to a list
   of normalized routes which no longer have overlaps */
RouteList Route::Normalize()
{
    debugcnt++;
    RouteList ret;

reset:
    /* don't do anything with triangles or fewer points */
    if(count < 4) {
        // delete points
        return ret;
    }

    Position *p = points;
    Position *q = p->next;
    do {
        if(p->lat == q->lat && p->lon == q->lon) {
            int a;
            a+=5;
        }

        Position *r = q->next;
        Position *s = r->next;

        int innercount = 2;
        /* dont test adjacent segments, so in the case p first, we skip the last segment */
        while(!(p == points && s == points) && r != points) {
            int dir = TestIntersection(p, q, r, s);
            if(dir == 2) { /* too close to call, delete s and try next point */
                RemovePosition(s);
                /* this can change points, so reset to be safe
                   in the future maybe we can avoid this*/
                goto reset;
            }

            if(dir) {
                p->next = s;
                s->prev = p;
                r->next = q;
                q->prev = r;

                count -= innercount;
                Route *x = new Route(r, innercount, dir * direction);

                RouteList inner = x->Normalize();
                for(RouteList::iterator it = inner.begin(); it != inner.end(); ++it) {        
                    if(direction != (*it)->direction) {
#if 1 /* handle inverted regions */
                        if((*it)->children.size()) {
                            /* don't care for inner route having children right now */
//                            (*it)->children.clear();
                        }
                        (*it)->parent = this;

                        children.push_back(*it);
#endif
                    } else {
                        /* if it has same dir, we need to merge with it later */
                        ret.push_back(*it);
                    }
                }
                goto reset; /* posssible to not need a complete reset? */
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
bool Merge(RouteList &rl, Route *route1, Route *route2)
{
    if(route1->direction != route2->direction) {
        printf("cannot merge routes in different direction\n");
        exit(1);
    }

    /* quick test to make sure we could possibly intersect with bounds */
    double bounds1[4], bounds2[4];
    route1->FindRouteBounds(bounds1);
    route2->FindRouteBounds(bounds2);
    if((bounds1[MINLAT] > bounds2[MAXLAT] || bounds1[MAXLAT] < bounds2[MINLAT]) &&
       (bounds1[MINLON] > bounds2[MAXLON] || bounds1[MAXLON] < bounds2[MINLON]))
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
            if(dir == 2) { /* too close to call, delete s and try next point */
                route2->RemovePosition(s);
                if(route2->count < 3) { /* nothing left to merge */
                    delete route2;
                    rl.push_back(route1); /* no need to normalize */
                    return true;
                }

                s = r->next;
                continue;
            }

            if(dir) {
                p->next = s;
                s->prev = p;
                r->next = q;
                q->prev = r;

                if(route1->children.size() || route2->children.size()) {
                    for(RouteList::iterator it = route2->children.begin();
                        it != route2->children.end(); ++it)
                        route1->children.push_back(*it);
                }

                route1->count += route2->count;

                delete route2;
                rl = route1->Normalize();
                return true;
            }

            r = s;
            s = s->next;
        } while(r != route2->points);

        p = q;
        q = q->next;
    } while(p != route1->points);

    /* no intersection found, if this route is
       physically inside the other we should remove the inner one */
    if(route1->Contains(route2)) {
//        delete route2;
        rl.push_back(route1); /* no need to normalize */
        return true;
    }

    /* routes close enough to fail initial rectangle test but no
       actual intersection or overlap occurs so no merge takes places */
    return false;
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
    routes.push_back(new Route(p, 1, 1));
}

RouteIso::RouteIso(RouteList r, wxDateTime t)
    : routes(r), time(t)
{
}

RouteIso::~RouteIso()
{
}

int debugstep, debugcount;
RouteIso *RouteIso::Propagate(wxTimeSpan dt, double degree_step, BoatSpeed &boat)
{
    /* build up a list of iso regions for each point
       in the current iso */
    RouteHeap routeheap;
    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it) {
        Position *p = (*it)->points;
        do {
            RouteList route = p->PropagateRoutePosition(time + dt, dt, degree_step, boat);
            for(RouteList::iterator it2 = route.begin(); it2 != route.end(); ++it2)
                routeheap.Insert(*it2);

            p = p->next;
        } while(p != (*it)->points);

        /* also need this route to avoid backtracking */
        routeheap.Insert(new Route(*it));
    }

    RouteList merged, unmerged;
    while(routeheap.Size()) {
        Route *r1 = routeheap.Remove();

        while(routeheap.Size()) {
            Route *r2 = routeheap.Remove();

//            if(++debugstep < debugcount)
            RouteList rl;
            if(Merge(rl, r1, r2)) {
                for(RouteList::iterator it = rl.begin(); it != rl.end(); ++it)
                    routeheap.Insert(*it);

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

    return new RouteIso(merged, time + dt);
}


void RouteIso::Render(PlugIn_ViewPort *vp)
{
    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it)
        (*it)->Render(vp);
}

RouteMap::RouteMap()
    : last_cursor_position(NULL)
{
}

RouteMap::~RouteMap()
{
    Clear();
}

/* enlarge the map by 1 level */
void RouteMap::Propagate(BoatSpeed &boat)
{
    if(origin.size() == 0)
        return;

    RouteIso* update = origin.back()->Propagate(dt, degree_step, boat);

    origin.push_back(update);
}

Position *RouteMap::ClosestPosition(double lat, double lon)
{
    Position p(lat, lon), *minpos = NULL;
    double mindist = 0;
    for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it) {
        for(RouteList::iterator rit = (*it)->routes.begin(); rit != (*it)->routes.end(); ++rit) {
            Position *pos = (*rit)->points;
            do {
                double dist = p.Distance(pos);
                if(dist < mindist || mindist == 0) {
                    minpos = pos;
                    mindist = dist;
                }
                pos = pos->next;
            } while(pos != (*rit)->points);
        }
    }
    return minpos;
}

void RouteMap::Render(PlugIn_ViewPort *vp)
{
    routemutex.Lock();

    glLineWidth(2);
    if(curRoute) {
        curRoute->Render(vp);
    } else {

        int s = origin.size();
        double c = 0;
        int i = 0;
        for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it) {
            c += 1/(double)s;
#if 1
            if(i == s-1)
#endif
                (*it)->Render(vp);
            i++;
        }
    }
    
    glColor3f(1, 0, 1);
    glBegin(GL_LINE_STRIP);
    glLineWidth(4);
    for(Position *p = last_cursor_position; p; p = p->parent) {
        wxPoint r;
        GetCanvasPixLL(vp, &r, p->lat, p->lon);
        glVertex2i(r.x, r.y);
    }
    glEnd();

    routemutex.Unlock();
}

void RouteMap::Clear()
{
    for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it)
        delete *it;

    origin.clear();

    last_cursor_position = NULL;
}

void RouteMap::Reset(double lat, double lon, wxDateTime t)
{
    Clear();
    time = t;
    origin.push_back(new RouteIso(new Position(lat, lon), time));
}

bool RouteMap::SetCursorLatLon(double lat, double lon)
{
    Position *p = ClosestPosition(lat, lon);
    if(p == last_cursor_position)
        return false;
    last_cursor_position = p;
    return true;
}
