/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
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

#include <wx/wx.h>

#include "ocpn_plugin.h"
#include "ocpndc.h"
#include "wxJSON/jsonreader.h"
#include "wxJSON/jsonwriter.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "SettingsDialog.h"

RouteMapOverlayThread::RouteMapOverlayThread(RouteMapOverlay &routemapoverlay)
    : wxThread(wxTHREAD_JOINABLE), m_RouteMapOverlay(routemapoverlay)
{
    Create();
}

void *RouteMapOverlayThread::Entry()
{
    while(!TestDestroy() && !m_RouteMapOverlay.Finished())
        if(!m_RouteMapOverlay.Propagate())
            wxThread::Sleep(50);
        else {
            m_RouteMapOverlay.UpdateCursorPosition();
            m_RouteMapOverlay.UpdateDestination();
            wxThread::Sleep(5);
        }

//    m_RouteMapOverlay.m_Thread = NULL;
    return 0;
}

RouteMapOverlay::RouteMapOverlay()
    : m_UpdateOverlay(true), m_bEndRouteVisible(false), m_Thread(NULL),
      last_cursor_lat(0), last_cursor_lon(0),
      last_cursor_position(NULL), destination_position(NULL), last_destination_position(NULL),
      m_bUpdated(false), m_overlaylist(0)
{
}

RouteMapOverlay::~RouteMapOverlay()
{
    delete destination_position;

    if(m_Thread)
        Stop();
}

bool RouteMapOverlay::Start(wxString &error)
{
    if(m_Thread) {
        error = _("error, thread already created\n");
        return false;
    }

    error = LoadBoat();

    if(error.size())
        return false;

    m_Thread = new RouteMapOverlayThread(*this);
    m_Thread->Run();
    return true;
}

void RouteMapOverlay::DeleteThread()
{
    if(!m_Thread)
        return;

    m_Thread->Delete();
    delete m_Thread;
    m_Thread = NULL;
}

static void SetColor(ocpnDC &dc, wxColour c, bool penifgl = false)
{
    if(!dc.GetDC()) {
        glColor4ub(c.Red(), c.Green(), c.Blue(), c.Alpha());
        if(!penifgl)
            return;
    }
    wxPen pen = dc.GetPen();
    pen.SetColour(c);
    dc.SetPen(pen);
}

static void SetWidth(ocpnDC &dc, int w, bool penifgl = false)
{
    if(!dc.GetDC()) {
        glLineWidth(w);
        if(!penifgl)
            return;
    }
    wxPen pen = dc.GetPen();
    pen.SetWidth(w);
    dc.SetPen(pen);
}

void RouteMapOverlay::DrawLine(Position *p1, Position *p2,
                               ocpnDC &dc, PlugIn_ViewPort &vp)
{
    wxPoint p1p, p2p;
    GetCanvasPixLL(&vp, &p1p, p1->lat, p1->lon);
    GetCanvasPixLL(&vp, &p2p, p2->lat, p2->lon);

    if(dc.GetDC())
        dc.StrokeLine(p1p.x, p1p.y, p2p.x, p2p.y);
    else {
        glVertex2d(p1p.x, p1p.y);
        glVertex2d(p2p.x, p2p.y);
    }
}

void RouteMapOverlay::DrawLine(Position *p1, wxColour &color1, Position *p2, wxColour &color2,
                               ocpnDC &dc, PlugIn_ViewPort &vp)
{
#if 0
    double p1plon, p2plon;
    if(fabs(vp.clon) > 90)
        p1plon = positive_degrees(p1->lon), p2plon = positive_degrees(p2->lon);
    else
        p1plon = heading_resolve(p1->lon), p2plon = heading_resolve(p2->lon);

    if((p1plon+180 < vp.clon && p2plon+180 > vp.clon) ||
       (p1plon+180 > vp.clon && p2plon+180 < vp.clon) ||
       (p1plon-180 < vp.clon && p2plon-180 > vp.clon) ||
       (p1plon-180 > vp.clon && p2plon-180 < vp.clon))
        return;
#endif

    wxPoint p1p, p2p;
    GetCanvasPixLL(&vp, &p1p, p1->lat, p1->lon);
    GetCanvasPixLL(&vp, &p2p, p2->lat, p2->lon);

    SetColor(dc, color1);
    if(dc.GetDC())
        dc.DrawLine(p1p.x, p1p.y, p2p.x, p2p.y);
    else {
        glVertex2d(p1p.x, p1p.y);
        SetColor(dc, color2);
        glVertex2d(p2p.x, p2p.y);
    }
}

static inline wxColour &PositionColor(Position *p, wxColour &grib_color, wxColour &climatology_color,
                                      wxColour &grib_deficient_color, wxColour &climatology_deficient_color)
{
    if(p->data_mask & Position::GRIB_WIND) {
        if(p->data_mask & Position::DATA_DEFICIENT_WIND)
           return grib_deficient_color;
        else
            return grib_color;
    }

    if(p->data_mask & Position::CLIMATOLOGY_WIND) {
        if(p->data_mask & Position::DATA_DEFICIENT_WIND)
            return climatology_deficient_color;
        else
            return climatology_color;
    }

    static wxColour black(0,0,0);
    return black;
}

static wxColour TransparentColor(wxColor c)
{
    return wxColor(c.Red(), c.Green(), c.Blue(), c.Alpha()*7/24);
}

void RouteMapOverlay::RenderIsoRoute(IsoRoute *r, wxColour &grib_color, wxColour &climatology_color,
                                     ocpnDC &dc, PlugIn_ViewPort &vp)
{
    SkipPosition *s = r->skippoints;
    if(!s)
        return;

    wxColour grib_deficient_color = TransparentColor(grib_color);
    wxColour climatology_deficient_color = TransparentColor(climatology_color);

    Position *p = s->point;
    wxColour *pcolor = &PositionColor(p, grib_color, climatology_color,
                                      grib_deficient_color, climatology_deficient_color);
    if(!dc.GetDC())
        glBegin(GL_LINES);
    do {
        wxColour &ncolor = PositionColor(p->next, grib_color, climatology_color,
                                         grib_deficient_color, climatology_deficient_color);
        if(!p->copied || !p->next->copied)
            DrawLine(p, *pcolor, p->next, ncolor, dc, vp);
        pcolor = &ncolor;
        p = p->next;
    } while(p != s->point);
    if(!dc.GetDC())
        glEnd();

    /* now render any children */
    wxColour cyan(0, 255, 255), magenta(255, 0, 255);
    for(IsoRouteList::iterator it = r->children.begin(); it != r->children.end(); ++it)
        RenderIsoRoute(*it, cyan, magenta, dc, vp);
}

void RouteMapOverlay::RenderAlternateRoute(IsoRoute *r, bool each_parent,
                                           ocpnDC &dc, PlugIn_ViewPort &vp)
{
    Position *pos = r->skippoints->point;
    wxColor black = wxColour(0, 0, 0, 192), tblack = TransparentColor(black);
    do {
        wxColour *color = pos->data_mask & Position::DATA_DEFICIENT_WIND ? &tblack : &black;
        for(Position *p = pos; p && !p->drawn && p->parent; p = p->parent) {
//            wxColour &color = p->data_mask & Position::DATA_DEFICIENT_WIND ? tblack : black;
            wxColour &pcolor = p->parent->data_mask & Position::DATA_DEFICIENT_WIND ? tblack : black;
            if(!p->copied || each_parent)
                DrawLine(p, *color, p->parent, pcolor, dc, vp);
            p->drawn = true;
            if(!each_parent)
                break;
            color = &pcolor;
        }

        pos = pos->next;
    } while(pos != r->skippoints->point);

    wxColour blue(0, 0, 255);
    SetColor(dc, blue);
    for(IsoRouteList::iterator cit = r->children.begin(); cit != r->children.end(); cit++)
        RenderAlternateRoute(*cit, each_parent, dc, vp);
}

static wxColour Darken(wxColour c)
{
    return wxColour(c.Red()*2/3, c.Green()*2/3, c.Blue()*2/3, c.Alpha());
}

void RouteMapOverlay::Render(wxDateTime time, SettingsDialog &settingsdialog,
                             ocpnDC &dc, PlugIn_ViewPort &vp, bool justendroute)
{
    if(!justendroute) {
        RouteMapConfiguration configuration = GetConfiguration();

        wxPoint r;
        GetCanvasPixLL(&vp, &r, configuration.StartLat, configuration.StartLon);
        SetColor(dc, *wxBLUE, true);
        SetWidth(dc, 3, true);
        dc.DrawLine(r.x, r.y-10, r.x+10, r.y+7);
        dc.DrawLine(r.x, r.y-10, r.x-10, r.y+7);
        dc.DrawLine(r.x-10, r.y+7, r.x+10, r.y+7);

        GetCanvasPixLL(&vp, &r, configuration.EndLat, configuration.EndLon);
        SetColor(dc, *wxRED, true);
        SetWidth(dc, 3, true);
        dc.DrawLine(r.x-10, r.y-10, r.x+10, r.y+10);
        dc.DrawLine(r.x-10, r.y+10, r.x+10, r.y-10);

        static const double NORM_FACTOR = 16;
        const bool use_dl = true;
        if(!dc.GetDC() && use_dl) {
            glPushMatrix();

            /* center display list on start lat/lon */

            wxPoint point;
            GetCanvasPixLL(&vp, &point, configuration.StartLat, configuration.StartLon);

            glTranslated(point.x, point.y, 0);
            glScalef(vp.view_scale_ppm / NORM_FACTOR, vp.view_scale_ppm / NORM_FACTOR, 1);
            glRotated(vp.rotation*180/M_PI, 0, 0, 1);
        }

        if(!dc.GetDC() && !m_UpdateOverlay && use_dl) {
            glCallList(m_overlaylist);
            glPopMatrix();

        } else {
            PlugIn_ViewPort nvp = vp;

            if(!dc.GetDC() && use_dl) {
                m_UpdateOverlay = false;

                if(!m_overlaylist)
                    m_overlaylist = glGenLists(1);
            
                glNewList(m_overlaylist, GL_COMPILE);

                nvp.clat = configuration.StartLat, nvp.clon = configuration.StartLon;
                nvp.pix_width = nvp.pix_height = 0;
                nvp.view_scale_ppm = NORM_FACTOR;
                nvp.rotation = nvp.skew = 0;
            }

            /* draw alternate routes first */
            int AlternateRouteThickness = settingsdialog.m_sAlternateRouteThickness->GetValue();
            if(AlternateRouteThickness) {
                Lock();
                IsoChronList::iterator it;

                /* reset drawn flag for all positions
                   this is used to avoid duplicating alternate route segments */
                for(it = origin.begin(); it != origin.end(); ++it)
                    (*it)->ResetDrawnFlag();

                bool AlternatesForAll = settingsdialog.m_cbAlternatesForAll->GetValue();
                if(AlternatesForAll)
                    it = origin.begin();
                else {
                    it = origin.end();
                    it--;
                }

                SetWidth(dc, AlternateRouteThickness);
                if(!dc.GetDC())
                    glBegin(GL_LINES);
                for(; it != origin.end(); ++it)
                    for(IsoRouteList::iterator rit = (*it)->routes.begin();
                        rit != (*it)->routes.end(); ++rit) {
                        RenderAlternateRoute(*rit, !AlternatesForAll, dc, nvp);
                    }

                if(!dc.GetDC())
                    glEnd();
                Unlock();
            }

            unsigned char routecolors[][3] = {
                {  0,   0, 128}, {  0, 192,   0}, {  0, 128, 192}, {  0, 255,   0},
                {  0,   0, 255}, {  0, 128, 128}, {  0, 255,   0}, {  0, 192, 192},
                {  0, 128, 255}, {  0, 255, 128}, {  0,   0, 255}, {  0, 192,   0},
                {  0,   0, 128}, {  0, 255,   0}, {  0, 192, 128}, {  0, 128, 255}, 
                {  0, 192,   0}, {  0, 128,   0}, {  0,   0, 255}, {  0, 192, 192}};
#if 0
                {255, 127,   0}, {255, 127, 127},
                {  0, 255,   0}, {  0, 255, 127},
                {127, 255,   0}, {127, 255, 127},
                {127, 127,   0},                  {127, 127, 255},
                {255,   0,   0}, {255,   0, 127}, {255,   0, 255},
                {127,   0,   0}, {127,   0, 127}, {127,   0, 255},
                {  0, 127,   0}, {  0, 127, 127}, {  0, 127, 255},
                {255, 255,   0},                  };
#endif

            int IsoChronThickness = settingsdialog.m_sIsoChronThickness->GetValue();
            if(IsoChronThickness) {
                SetWidth(dc, IsoChronThickness);
                Lock();
                int c = 0;
                for(IsoChronList::iterator i = origin.begin(); i != origin.end(); ++i) {
                    Unlock();
                    wxColor grib_color(routecolors[c][0], routecolors[c][1], routecolors[c][2], 224);
                    wxColor climatology_color(255-routecolors[c][0], routecolors[c][2],
                                              routecolors[c][1], 224);
                
                    for(IsoRouteList::iterator j = (*i)->routes.begin(); j != (*i)->routes.end(); ++j)
                        RenderIsoRoute(*j, grib_color, climatology_color, dc, nvp);
                
                    if(++c == (sizeof routecolors) / (sizeof *routecolors))
                        c = 0;
                    Lock();
                }
                Unlock();
            }
        
            if(!dc.GetDC() && use_dl) {
                glEndList();
                glCallList(m_overlaylist);
                glPopMatrix();
            }
        }
    }
    
    int RouteThickness = settingsdialog.m_sRouteThickness->GetValue();
    if(RouteThickness) {
        wxColour CursorColor = settingsdialog.m_cpCursorRoute->GetColour(),
            DestinationColor = settingsdialog.m_cpDestinationRoute->GetColour();
        bool MarkAtSailChange = settingsdialog.m_cbMarkAtSailChange->GetValue();

        if(!justendroute) {
            SetColor(dc, CursorColor, true);
            SetWidth(dc, RouteThickness, true);
            RenderCourse(last_cursor_position, time, false, dc, vp);

            if(MarkAtSailChange) {
                SetColor(dc, Darken(CursorColor), true);
                SetWidth(dc, (RouteThickness+1)/2, true);
                RenderCourse(last_cursor_position, time, true, dc, vp);
            }
        }
        SetColor(dc, DestinationColor, true);
        SetWidth(dc, RouteThickness, true);
        RenderCourse(last_destination_position, time, false, dc, vp);
        
        if(MarkAtSailChange) {
            SetColor(dc, Darken(DestinationColor), true);
            SetWidth(dc, (RouteThickness+1)/2, true);
            RenderCourse(last_destination_position, time, true, dc, vp);
        }
    }
}

void RouteMapOverlay::RenderCourse(Position *pos, wxDateTime time, bool MarkAtSailChange,
                                   ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!pos)
        return;

    Lock();

    /* draw lines to this route */
    Position *p;
    int sailplan = pos->sailplan;
    if(!dc.GetDC())
        glBegin(GL_LINES);

    for(p = pos; p && p->parent; p = p->parent) {
        if(MarkAtSailChange && p->sailplan != sailplan) {
            wxPoint r;
            GetCanvasPixLL(&vp, &r, p->lat, p->lon);
            int s = 6;
            glVertex2i(r.x-s, r.y-s), glVertex2i(r.x+s, r.y-s);
            glVertex2i(r.x+s, r.y-s), glVertex2i(r.x+s, r.y+s);
            glVertex2i(r.x+s, r.y+s), glVertex2i(r.x-s, r.y+s);
            glVertex2i(r.x-s, r.y+s), glVertex2i(r.x-s, r.y-s);
            sailplan = p->sailplan;
        } else
            DrawLine(p, p->parent, dc, vp);
    }
    if(!dc.GetDC())
        glEnd();

    /* render boat on optimal course at time */
    IsoChronList::iterator it = origin.begin();

    /* get route iso for this position */
    for(p=pos->parent; p; p=p->parent)
        if(++it == origin.end())
            return;

    if(it != origin.begin())
        it--;

    for(p = pos; p->parent; p = p->parent) {
        wxDateTime ittime = (*it)->time;
        wxPoint r;

        if(time >= ittime) {
            wxDateTime timestart = (*it)->time;
            it++;
            wxDateTime timeend = (*it)->time;

            wxTimeSpan span = timeend - timestart, cspan = time - timestart;
            double d = cspan.GetSeconds().ToDouble() / span.GetSeconds().ToDouble();

            if(d > 1)
                d = 1;

            GetCanvasPixLL(&vp, &r,
                           p->parent->lat + d*(p->lat - p->parent->lat),
                           p->parent->lon + d*heading_resolve(p->lon - p->parent->lon));

        } else if(it == origin.begin())
            GetCanvasPixLL(&vp, &r, p->parent->lat, p->parent->lon);
        else {
            it--;
            continue;
        }
        
        dc.SetBrush( *wxTRANSPARENT_BRUSH);
        dc.DrawCircle( r.x, r.y, 7 );
        break;
    }
    Unlock();
}

void RouteMapOverlay::RenderWindBarbs(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(origin.size() < 2) // no map to work with
        return;

    RouteMapConfiguration configuration = GetConfiguration();

    // if zoomed way in, don't cache the arrows for panning, instead we just
    // render what's onscreen
    double latmin, latmax, lonmin, lonmax;
    GetLLBounds(latmin, latmax, lonmin, lonmax);

    PlugIn_ViewPort nvp = vp;
    nvp.clat = configuration.StartLat, nvp.clon = configuration.StartLon;
    nvp.pix_width = nvp.pix_height = 0;
    nvp.rotation = nvp.skew = 0;

    wxPoint p1, p2, p3, p4;
    GetCanvasPixLL( &nvp, &p1, latmin, lonmin );
    GetCanvasPixLL( &nvp, &p2, latmin, lonmax );
    GetCanvasPixLL( &nvp, &p3, latmax, lonmin );
    GetCanvasPixLL( &nvp, &p4, latmax, lonmax );

    wxRect r;
    r.x = wxMin(wxMin(p1.x, p2.x), wxMin(p3.x, p4.x));
    r.y = wxMin(wxMin(p1.y, p2.y), wxMin(p3.y, p4.y));
    r.width = wxMax(wxMax(p1.x, p2.x), wxMax(p3.x, p4.x)) - r.x;
    r.height = wxMax(wxMax(p1.y, p2.y), wxMax(p3.y, p4.y)) - r.y;

    // we could somehow "append" to the cache as passing occurs when zoomed really far
    // in rather than making a complete cache... but how complex does it need to be?
    bool nocache = r.width*r.height > vp.rv_rect.width*vp.rv_rect.height*9;

    if(origin.size() != wind_barb_cache_origin_size ||
       vp.view_scale_ppm != wind_barb_cache_scale ||
        nocache) {

        wind_barb_cache_origin_size = origin.size();
        wind_barb_cache_scale = vp.view_scale_ppm;

        if(nocache) {
            r = vp.rv_rect;
            nvp = vp;
        }

        Lock();

        int step = 36;

        wxPoint p;
        GetCanvasPixLL( &nvp, &p, configuration.StartLat, configuration.StartLon );
        int xoff = p.x%step, yoff = p.y%step;
    
        for(double x = r.x + xoff; x<r.x+r.width; x+=step)
            for(double y = r.y + yoff; y<r.y+r.height; y+=step) {
                double lat, lon;
                GetCanvasLLPix( &nvp, wxPoint(x, y), &lat, &lon );

                Position p(lat, configuration.positive_longitudes ? positive_degrees(lon) : lon);

                IsoChronList::iterator it = origin.end();
                it--;
                if(!(*it)->Contains(p)) // don't plot outside map
                    continue;

                for(it--; it != origin.begin(); it--)
                    if(!(*it)->Contains(p))
                        break;

                double W1, VW1, W2, VW2;
                int data_mask1, data_mask2; // can be used to colorize barbs based on data type

                // now it is the isochron before p, so we find the two closest postions
                Position *p1 = (*it)->ClosestPosition(lat, lon);
                configuration.grib = (*it)->m_Grib;
                configuration.time = (*it)->time;
                p.GetWindData(configuration, W1, VW1, data_mask1);

                it++;
                Position *p2 = (*it)->ClosestPosition(lat, lon);
                configuration.grib = (*it)->m_Grib;
                configuration.time = (*it)->time;
                p.GetWindData(configuration, W2, VW2, data_mask2);

                // now polar interpolation of the two wind positions
                double d1 = p.Distance(p1), d2 = p.Distance(p2);
                double d = d1 / (d1+d2);
#if 0
                double W1r = deg2rad(W1), W2r = deg2rad(W2);
                double W1x = VW1*cos(W1r), W1y = VW1*sin(W1r);
                double W2x = VW2*cos(W2r), W2y = VW2*sin(W2r);
                double Wx = d*W1x + (1-d)*W2x, Wy = d*W1y + (1-d)*W2y;
                double W = rad2deg(atan2(Wy, Wx));
#else
                while(W1 - W2 > 180) W1 -= 360;
                while(W2 - W1 > 180) W2 -= 360;
                double W = d*W1 + (1-d)*W2;
#endif
                double VW = d*VW1 + (1-d)*VW2;

                g_LineBufferOverlay.pushWindArrowWithBarbs(wind_barb_cache, x, y, VW, deg2rad(W), lat < 0 );
            }
        Unlock();

        wind_barb_cache.Finalize();
    }

    wxColour colour(180, 140, 14);

    wxPoint point;
    GetCanvasPixLL(&vp, &point, configuration.StartLat, configuration.StartLon);

    if(dc.GetDC()) {
        dc.SetPen( wxPen( colour, 2 ) );
        dc.SetBrush( *wxTRANSPARENT_BRUSH);
    } 
#ifdef ocpnUSE_GL
    else {
        if(!nocache) {
            glPushMatrix();
            glTranslated(point.x, point.y, 0);
            glRotated(vp.rotation*180/M_PI, 0, 0, 1);
        }

        glColor3ub(colour.Red(), colour.Green(), colour.Blue());
        //      Enable anti-aliased lines, at best quality
        glEnable( GL_LINE_SMOOTH );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        glLineWidth( 2 );
        glEnableClientState(GL_VERTEX_ARRAY);
    }
#endif

    if(dc.GetDC()) {
        if(nocache)
            wind_barb_cache.draw(dc.GetDC());
        else {
            LineBuffer tb;
            tb.pushTransformedBuffer(wind_barb_cache, point.x, dc.GetDC()->GetSize().y-point.y, vp.rotation);
            tb.Finalize();
            tb.draw(dc.GetDC());
        }
    } else
        wind_barb_cache.draw(NULL);

#ifdef ocpnUSE_GL
    if( !dc.GetDC() ) {
        glDisableClientState(GL_VERTEX_ARRAY);

        if(!nocache)
            glPopMatrix();
    }
#endif
}

void RouteMapOverlay::GetLLBounds(double &latmin, double &latmax, double &lonmin, double &lonmax)
{
    latmin = INFINITY, lonmin = INFINITY;
    latmax = -INFINITY, lonmax = -INFINITY;

    IsoChron *last = origin.back();    
    for(IsoRouteList::iterator it = last->routes.begin(); it != last->routes.end(); ++it) {
        Position *pos = (*it)->skippoints->point;
        do {
            latmin = wxMin(latmin, pos->lat);
            latmax = wxMax(latmax, pos->lat);
            lonmin = wxMin(lonmin, pos->lon);
            lonmax = wxMax(lonmax, pos->lon);
            pos = pos->next;
        } while(pos != (*it)->skippoints->point);
    }
}

void RouteMapOverlay::RequestGrib(wxDateTime time)
{
    wxJSONValue v;
    v[_T("Day")] = time.GetDay();
    v[_T("Month")] = time.GetMonth();
    v[_T("Year")] = time.GetYear();
    v[_T("Hour")] = time.GetHour();
    v[_T("Minute")] = time.GetMinute();
    v[_T("Second")] = time.GetSecond();
    
    wxJSONWriter w;
    wxString out;
    w.Write(v, out);
    SendPluginMessage(wxString(_T("GRIB_TIMELINE_RECORD_REQUEST")), out);
    m_bNeedsGrib = false;
}

std::list<PlotData> &RouteMapOverlay::GetPlotData(bool cursor_route)
{
    std::list<PlotData> &plotdata = cursor_route ? last_cursor_plotdata : last_destination_plotdata;
    if(plotdata.empty()) {
        Position *pos = cursor_route ? last_cursor_position : last_destination_position;

        if(!pos)
            return plotdata;

        // omit destination position.  This is because we don't have grib
        // data for the exact endtime to use and the constraints aren't applied
        // for the end position anyway, so statistics for max wind may exceed the constraints
        if(pos == destination_position)
            pos = pos->parent;

        RouteMapConfiguration configuration = GetConfiguration();
        Lock();
        IsoChronList::iterator it = origin.begin(), itp;

        // get route iso for this position
        Position *p = pos->parent;
        for(; p; p=p->parent)
            if(++it == origin.end())
                return plotdata;

        for(p = pos; p; p = p->parent) {
            itp = it;
            itp--;

            configuration.grib = (*it)->m_Grib;

            PlotData data;

            double dt = configuration.dt;
            if(p == destination_position) {
                wxDateTime starttime = (*itp)->time, endtime = EndTime();
                if(starttime.IsValid() && endtime.IsValid())
                    dt = (endtime - starttime).GetSeconds().ToDouble();
                data.time = endtime;
            } else
                data.time = (*it)->time;

            data.lat = p->lat, data.lon = p->lon;
            if(p->GetPlotData(dt, configuration, data))
                plotdata.push_front(data);

            it = itp;
        }

        Unlock();
    }
    return plotdata;
}

double RouteMapOverlay::RouteInfo(enum RouteInfoType type, bool cursor_route)
{
    std::list<PlotData> &plotdata = GetPlotData(cursor_route);

    double total = 0, count = 0, lat0 = 0, lon0 = 0;
    for(std::list<PlotData>::iterator it=plotdata.begin(); it!=plotdata.end(); it++)
    {
        switch(type) {
        case DISTANCE:
        {
            if(it != plotdata.begin()) {
                double dist;
                DistanceBearingMercator_Plugin(lat0, lon0, it->lat, it->lon, 0, &dist);
                total += dist;
            }
            lat0 = it->lat;
            lon0 = it->lon;
        } break;
        case AVGSPEED:
            total += it->VB;
            break;
        case MAXSPEED:
            if(total < it->VB)
                total = it->VB;
            break;
        case AVGSPEEDGROUND:
            total += it->VBG;
            break;
        case MAXSPEEDGROUND:
            if(total < it->VBG)
               total = it->VBG;
            break;
        case AVGWIND:
            total += it->VW;
            break;
        case MAXWIND:
            if(total < it->VW)
                total = it->VW;
            break;
        case AVGCURRENT:
            total += it->VC;
            break;
        case MAXCURRENT:
            if(total < it->VC)
                total = it->VC;
            break;
        case AVGSWELL:
            total += it->WVHT;
            break;
        case MAXSWELL:
            if(total < it->WVHT)
                total = it->WVHT;
            break;
        case PERCENTAGE_UPWIND:
            if(fabs(heading_resolve(it->B - it->W)) < 90)
                total++;
            break;
        case PORT_STARBOARD:
            if(heading_resolve(it->B - it->W) > 0)
                total++;
            break;
        default:
            break;
        }
        count++;
    }

    /* fixup data */
    switch(type) {
    case TACKS:
        return plotdata.size() ? plotdata.back().tacks : 0;
    case DISTANCE:
        if(total == 0)
            total = NAN;
        return total;
    case PERCENTAGE_UPWIND:
    case PORT_STARBOARD:
        total *= 100.0;
    case AVGSPEED:
    case AVGSPEEDGROUND:
    case AVGWIND:
    case AVGCURRENT:
    case AVGSWELL:
        total /= count;
    default:
        break;
    }
    return total;
}

/* how many cyclone tracks did we cross? which month? */
int RouteMapOverlay::Cyclones(int *months)
{
    if(!RouteMap::ClimatologyCycloneTrackCrossings)
        return -1;

    int days = 30; // search for 30 day range
    int cyclones = 0;

    Lock();
    wxDateTime ptime = m_EndTime;
    IsoChronList::iterator it = origin.end();

    for(Position *p = destination_position; p && p->parent; p = p->parent) {
        if(RouteMap::ClimatologyCycloneTrackCrossings(p->parent->lat, p->parent->lon,
                                                      p->lat, p->lon, ptime, days)) {
            if(months)
                months[ptime.GetMonth()]++;
            cyclones++;
        }

        it--;
        ptime = (*it)->time;
    }

    Unlock();
    return cyclones;
}

void RouteMapOverlay::Clear()
{
    RouteMap::Clear();
    last_cursor_position = NULL;
    last_destination_position = NULL;
    last_cursor_plotdata.clear();
    last_destination_plotdata.clear();
    m_UpdateOverlay = true;
}

void RouteMapOverlay::UpdateCursorPosition()
{
    Position *last_last_cursor_position = last_cursor_position;
    last_cursor_position = ClosestPosition(last_cursor_lat, last_cursor_lon);
    if(last_last_cursor_position != last_cursor_position)
        last_cursor_plotdata.clear();
}

bool RouteMapOverlay::SetCursorLatLon(double lat, double lon)
{
    Position *p = last_cursor_position;
    last_cursor_lat = lat;
    last_cursor_lon = lon;

    UpdateCursorPosition();
    return p != last_cursor_position;
}

bool RouteMapOverlay::Updated()
{
    bool updated = m_bUpdated;
    m_bUpdated = false;
    return updated;
}

void RouteMapOverlay::UpdateDestination()
{
    RouteMapConfiguration configuration = GetConfiguration();
    Position *last_last_destination_position = last_destination_position;
    bool done = ReachedDestination();
    if(done) {
        delete destination_position;

        Lock();
        /* this doesn't happen often, so can be slow.. for each position in the last
           isochron, we try to propagate to the destination */
        IsoChronList::iterator iit = origin.end();
        iit--; iit--; /* second from last isochron */
        IsoChron *isochron = *iit;
        double mindt = INFINITY;
        Position *endp;
        double minH;
        bool mintacked;
        int mindata_mask;

        for(IsoRouteList::iterator it = isochron->routes.begin(); it != isochron->routes.end(); ++it) {
            configuration.grib = isochron->m_Grib;
            configuration.grib_is_data_deficient = isochron->m_Grib_is_data_deficient;
            
            configuration.time = isochron->time;
            (*it)->PropagateToEnd(configuration, mindt, endp, minH,
                                  mintacked, mindata_mask);
        }
        Unlock();

        if(isinf(mindt))
            goto not_able_to_propagate;

        destination_position = new Position(configuration.EndLat, configuration.EndLon,
                                            endp, minH, NAN, endp->sailplan, endp->tacks + mintacked,
                                            mindata_mask);

        m_EndTime = isochron->time + wxTimeSpan::Milliseconds(1000*mindt);

        last_destination_position = destination_position;
    } else {
    not_able_to_propagate:
        last_destination_position = ClosestPosition(configuration.EndLat, configuration.EndLon);

        m_EndTime = wxDateTime(); // invalid
    }

    if(last_last_destination_position != last_destination_position)
        last_destination_plotdata.clear();

    m_bUpdated = true;
    m_UpdateOverlay = true;
}
