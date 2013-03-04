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

#include "georef.h"

static void Wind(double &winddirground, double &windspeedground,
                 double lat, double lon, wxDateTime time)
{
    windspeedground = 3+100*(lon-174);
    winddirground = remainder(300*(lat+35), 180);//rand()%60;
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
    double Cx = VC * cos(C);
    double Cy = VC * sin(C);
    double Wx = VWG * cos(WG) - Cx;
    double Wy = VWG * sin(WG) - Cy;
    VW = hypot(Wx, Wy);
    WA = atan2(Wy, Wx);
}

/* provisions to compute boat movement over ground

   BG  - boat direction over ground
   BGV - boat speed over ground (gps velocity)
*/

void BoatOverGround(double B, double VB, double C, double VC, double &BG, double &VBG)
{
    double Cx = VC * cos(C);
    double Cy = VC * sin(C);
    double BGx = VB * cos(B) + Cx;
    double BGy = VB * sin(B) + Cy;

    BG  = atan2(BGy, BGx);
    VBG = hypot(BGx, BGy);
}

Position::Position(double latitude, double longitude, Position *p)
    : lat(latitude), lon(longitude), parent(p) { prev = next = this; }

/* create a looped route by propagating from a position by computing
   the location the boat would be in if sailed at every angle
   relative to the true wind. */
Route *Position::PropagateRoutePosition(wxDateTime time, wxTimeSpan dt,
                                        double degree_step, BoatSpeed &boat)
{
    Route *r = new Route(NULL, 1);
    /* through all angles relative to wind */
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

        double BtA = BA - WA;
        if(BtA < 0)
            BtA += 360;

        /* compound boatspeed with current */
        double BG, VBG;
        BoatOverGround(BtA, VB, C, VC, BG, VBG);

        /* distance over ground */
        static double distd = .1;

        distd +=.005;

//        double dist = 1;
    double dist = VBG / 3600000.0 * dt.GetMilliseconds().ToDouble();

        double dlat, dlon;
        ll_gc_ll(lat, lon, /*BG*/W, dist, &dlat, &dlon);
        Position *rp = new Position(dlat, dlon, this);
        if(r->points) {
            rp->prev=r->points->prev;
            rp->next=r->points;
            r->points->prev->next = rp;
            r->points->prev = rp;
        } else
            r->points = rp;
    }
    return r;
}

double Position::Distance(Position *p)
{
    return DistGreatCircle(lat, lon, p->lat, p->lon);
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

typedef struct  {
    float y;
    float x;
} MyFlPoint;

bool G_FloatPtInPolygon(MyFlPoint *rgpts, int wnumpts, float x, float y) ;

bool Route::Contains(Position *pos)
{
    MyFlPoint rgpts[size()];
    int n = 0;
    Position *p = points;
    do {
        rgpts[n].x = p->lat;
        rgpts[n].y = p->lon;
        n++;
        p=p->next;
    } while(p != points);

    if(n < 3)
        return false;

    /* TODO: replace this function with one using spherical geometry */
    return G_FloatPtInPolygon(rgpts, n, pos->lat, pos->lon);
}

void Route::Render(PlugIn_ViewPort *vp)
{
    glBegin(GL_LINE_LOOP);
    Position *p = points;
    do {
        wxPoint point;
        GetCanvasPixLL(vp, &point, p->lat, p->lon);
        glVertex2i(point.x, point.y);
        p = p->next;
    } while(p != points);
    glEnd();

    glColor3f(1, 0, 0);
    glPointSize(3);
    glBegin(GL_POINTS);
    p = points;
    do {
        wxPoint point;
        GetCanvasPixLL(vp, &point, p->lat, p->lon);
        glVertex2i(point.x, point.y);
        p = p->next;
    } while(p != points);
    glEnd();
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
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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

/* find intersection of two line segments
   if no intersection return 0, otherwise, 1 if the
   second line crosses from right to left, or -1 for left to right

   Truth equations to calculate intersection (x, y)
   (y-y1) * (x2-x1) = (y2-y1) * (x-x1)
   (y-y3) * (x4-x3) = (y4-y3) * (x-x3)
*/

double TestIntersection(Position *p1, Position *p2, Position *p3, Position *p4)
{
    double x1 = p1->lon, x2 = p2->lon, x3 = p3->lon, x4 = p4->lon;
    double y1 = p1->lat, y2 = p2->lat, y3 = p3->lat, y4 = p4->lat;

/*
  if((x1 == x3 && y1 == y3) ||
  (x1 == x3 && y1 == y4) ||
  (x2 == x3 && y2 == y3) ||
  (x2 == x3 && y2 == y4)) {
  exit(43);
  }
*/

    /* optimization to avoid calculations */
#if 1
    if((x3 > x1 && x3 > x2 && x4 > x1 && x4 > x2) ||
       (x3 < x1 && x3 < x2 && x4 < x1 && x4 < x2) ||
       (y3 > y1 && y3 > y2 && y4 > y1 && y4 > y2) ||
       (y3 < y1 && y3 < y2 && y4 < y1 && y4 < y2))
        return 0;
#endif

    double dem = (x1*(y4-y3)+x2*(y3-y4)+(x4-x3)*y2+(x3-x4)*y1);

    if(dem == 0) /* parallel */
        return 0;

    double x = (x1*(x3*y4-x4*y3+y2*(x4-x3)) + x2*(x4*y3-x3*y4+y1*(x3-x4)))/dem;
    double y = (y1*(x3*y4-x4*y3+x2*(y3-y4)) + y2*(x4*y3-x3*y4+x1*(y4-y3)))/dem;

    if((x > x1 && x > x2) || (x > x3 && x > x4) ||
       (x < x1 && x < x2) || (x < x3 && x < x4) ||
       (y > y1 && y > y2) || (y > y3 && y > y4) ||
       (y < y1 && y < y2) || (y < y3 && y < y4))
        return 0;
    else
        return dem < 0 ? -1 : 1;
}

int debugstf, debug_quit;

/* Take a route which may overlaps with itself and convert to a list
   of normalized routes which no longer have overlaps */
RouteList Route::Normalize()
{
    RouteList ret;

    RepositionForStarting();

    /* cannot have anything to do with triangles or fewer points */
    Position *p = points->next, *q;
    for(int l = 1; l<3; l++, p = p->next)
        if(p == points) {
            ret.push_back(this);
            return ret;
        }

again:
    p = points;
    q = p->next;
    do {
        Position *r = q->next;
        Position *s = r->next;
        /* dont test adjacent segments, so in the case p first, we skip the last segment */
        while(!(p == points && s == points) && r != points) {
            int dir = TestIntersection(p, q, r, s);
            if(dir) {
                p->next = s;
                s->prev = p;
                r->next = q;
                q->prev = r;

                Route *x = new Route(r, dir * direction);

#if 0
                RouteList inner = x->Normalize();
                for(RouteList::iterator it = inner.begin(); it != inner.end(); ++it) {        
                    if(direction != (*it)->direction) {
                        if((*it)->children.size()) {
                            printf("inner route has children, should not be physically possible");
                            exit(45);
                        }
                        children.push_back(*it);
                    } else {
                        /* if it has same dir then it is contained, remove it */
                        delete x;
                    }
                }
#endif

                q = p->next;
                goto again;
            }

            r = s;
            s = s->next; 
        }
        p = q;
        q = q->next;
//    again:;
    } while(q != points);

    ret.push_back(this);
    return ret;
}

/* merge another route into this one */
Route *Merge(Route *route1, Route *route2)
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
        return NULL;

    route1->RepositionForStarting();
    route2->RepositionForStarting();

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
            if(dir) {
                p->next = s;
                s->prev = p;
                r->next = q;
                q->prev = r;

                if(route1->children.size() || route2->children.size()) {
                    printf("cannot merge routes with children yet\n");
                    exit(3);
                }

                delete route2;
                return route1;
            }

            r = s;
            s = s->next;
        } while(r != route2->points);

        p = q;
        q = q->next;
    } while(p != route1->points);


    /* no intersection found, if this route is
       physically inside the other we should remove the inner one */

#if 1
    if(route1->Contains(route2->points)) {
//        delete route2;
        return route1;
    }
#endif

    /* routes close enough to fail initial rectangle test but no
       actual intersection or overlap occurs so no merge takes places */
  
    return NULL;
}

RouteIso::RouteIso(Position *p, wxDateTime t)
    : time(t)
{
    routes.push_back(new Route(p, 1));
}

RouteIso::RouteIso(RouteList r, wxDateTime t)
    : routes(r), time(t)
{
}

RouteIso::~RouteIso()
{
}

RouteIso *RouteIso::Propagate(wxTimeSpan dt, double degree_step, BoatSpeed &boat)
{
    /* build up a list of iso regions for each point
       in the current iso */
    RouteList routelist;
    for(RouteList::iterator it = routes.begin(); it != routes.end(); ++it) {
        Position *p = (*it)->points;
        do {
            Route *r = p->PropagateRoutePosition(time + dt, dt, degree_step, boat);
            r->parent = *it;
            routelist.push_back(r);
            p = p->next;
        } while(p != (*it)->points);
    }

    /* normalize the propagated routes before merging */
    RouteList nroutelist;
    for(RouteList::iterator it = routelist.begin(); it != routelist.end(); ++it) {
        RouteList nl = (*it)->Normalize();
        nroutelist.splice(nroutelist.end(), nl);
    }

    RouteList merged;
#if 1
    while(nroutelist.size() > 1) {
        Route *r1 = nroutelist.front();
        nroutelist.pop_front();

        RouteList unmerged;
        while(nroutelist.size()) {
            Route *r2 = nroutelist.front();
            nroutelist.pop_front();

#if 0
            if(debugstf >=0 && debugstf++ >= debug_quit) {
                merged.push_back(r1);
                merged.push_back(r2);
                goto quit;
            }
#endif

            Route *r = Merge(r1, r2);
            if(r) {
                r1 = r;
#if 1
                static int static_norm;
//                if(static_norm < 1)
                {
                RouteList nr = r->Normalize();
                r1 = nr.front();
                nr.pop_front();
                nroutelist.splice(nroutelist.end(), nr);

                static_norm++;
                }
#endif
            } else {
                unmerged.push_back(r2);
            }
        }
        nroutelist.splice(nroutelist.end(), unmerged);
        merged.push_back(r1);
    }
#endif

quit:
    merged.splice(merged.end(), nroutelist);

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
    Position p(lat, lon, NULL), *minpos = NULL;
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
    int s = origin.size();
    double c = 0;
    glLineWidth(2);
    int i = 0;
    for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it) {
        glColor3f(c, 0, (1-c));
        c += 1/(double)s;
#if 0
        if(i == s-1)
#endif
        (*it)->Render(vp);
        i++;
    }

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glLineWidth(4);
    for(Position *p = last_cursor_position; p; p = p->parent) {
        wxPoint r;
        GetCanvasPixLL(vp, &r, p->lat, p->lon);
        glVertex2i(r.x, r.y);
    }
    glEnd();
}

void RouteMap::Clear()
{
    for(RouteIsoList::iterator it = origin.begin(); it != origin.end(); ++it)
        delete *it;

    origin.clear();
}

void RouteMap::Reset(double lat, double lon, wxDateTime t)
{
    Clear();
    time = t;
    origin.push_back(new RouteIso(new Position(lat, lon, NULL), time));
}

bool RouteMap::SetCursorLatLon(double lat, double lon)
{
    Position *p = ClosestPosition(lat, lon);
    if(p == last_cursor_position)
        return false;
    last_cursor_position = p;
    return true;
}
