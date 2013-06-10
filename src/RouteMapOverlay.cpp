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

#include <wx/wx.h>

#include "../../../include/ocpn_plugin.h"
#include "../../../include/ocpndc.h"
#include "../../../include/wx/jsonreader.h"
#include "../../../include/wx/jsonwriter.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"

RouteMapOverlayThread::RouteMapOverlayThread(RouteMapOverlay &routemapoverlay)
    : wxThread(wxTHREAD_JOINABLE), m_RouteMapOverlay(routemapoverlay)
{
    Create();
}

void *RouteMapOverlayThread::Entry()
{
    while(!TestDestroy())
        if(!m_RouteMapOverlay.Propagate())
            wxThread::Sleep(150);
        else {
            m_RouteMapOverlay.UpdateDestination();
            wxThread::Sleep(50);
        }
    return 0;
}

RouteMapOverlay::RouteMapOverlay()
    : m_UpdateOverlay(true), m_Thread(NULL),
      last_cursor_position(NULL), destination_position(NULL), last_destination_position(NULL),
      m_bUpdated(false)
{
}

RouteMapOverlay::~RouteMapOverlay()
{
    delete destination_position;

    if(m_Thread)
        Stop();
}

void RouteMapOverlay::Start()
{
    if(m_Thread) {
        printf("error, thread already created\n");
        return;
    }
    m_Thread = new RouteMapOverlayThread(*this);
    m_Thread->Run();
}

void RouteMapOverlay::Stop()
{
    m_Thread->Delete();
    delete m_Thread;
    m_Thread = NULL;
}

void SetColor(ocpnDC &dc, bool penifgl, wxColour c, int w)
{
    if(!dc.GetDC()) {
        glColor4ub(c.Red(), c.Green(), c.Blue(), 168);
        glLineWidth(w);
        if(!penifgl)
            return;
    }
    dc.SetPen(wxPen(c, w));
}


void RouteMapOverlay::DrawLine(Position *p1, Position *p2,
                               ocpnDC &dc, PlugIn_ViewPort &vp)
{
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

    wxPoint p1p, p2p;
    GetCanvasPixLL(&vp, &p1p, p1->lat, p1->lon);
    GetCanvasPixLL(&vp, &p2p, p2->lat, p2->lon);

    if(dc.GetDC())
        dc.DrawLine(p1p.x, p1p.y, p2p.x, p2p.y);
    else {
//        glBegin(GL_LINES);
        glVertex2i(p1p.x, p1p.y);
        glVertex2i(p2p.x, p2p.y);
//        glEnd();
    }
}

void RouteMapOverlay::RenderIsoRoute(IsoRoute *r, wxColour &color, ocpnDC &dc, PlugIn_ViewPort &vp)
{
    SkipPosition *s = r->skippoints;
    if(!s)
        return;

    Position *p = s->point;
    SetColor(dc, false, color, m_IsoChronThickness);
    if(!dc.GetDC())
        glBegin(GL_LINES);
    do {
        DrawLine(p, p->next, dc, vp);
        p = p->next;
    } while(p != s->point);
    if(!dc.GetDC())
        glEnd();

    /* now render any children */
    wxColour cyan(0, 255, 255);
    for(IsoRouteList::iterator it = r->children.begin(); it != r->children.end(); ++it)
        RenderIsoRoute(*it, cyan, dc, vp);
}

void RouteMapOverlay::RenderAlternateRoute(IsoRoute *r, bool each_parent,
                                           ocpnDC &dc, PlugIn_ViewPort &vp)
{
    Position *pos = r->skippoints->point;
    do {
        for(Position *p = pos; p && p->parent; p = p->parent) {
            DrawLine(p, p->parent, dc, vp);
            if(!each_parent)
                break;
        }

        pos = pos->next;
    } while(pos != r->skippoints->point);

    for(IsoRouteList::iterator cit = r->children.begin(); cit != r->children.end(); cit++) {
        wxColour blue(0, 0, 255);
        SetColor(dc, false, blue, m_AlternateRouteThickness);
        RenderAlternateRoute(*cit, each_parent, dc, vp);
    }
}

void RouteMapOverlay::Render(wxDateTime time, ocpnDC &dc, PlugIn_ViewPort &vp)
{
    /* render start and end */
    RouteMapConfiguration configuration = GetConfiguration();
    wxPoint r;
    GetCanvasPixLL(&vp, &r, configuration.StartLat, configuration.StartLon);
    SetColor(dc, true, *wxBLUE, 3);
    dc.DrawLine(r.x, r.y-10, r.x+10, r.y+7);
    dc.DrawLine(r.x, r.y-10, r.x-10, r.y+7);
    dc.DrawLine(r.x-10, r.y+7, r.x+10, r.y+7);

    GetCanvasPixLL(&vp, &r, configuration.EndLat, configuration.EndLon);
    SetColor(dc, true, *wxRED, 3);
    dc.DrawLine(r.x-10, r.y-10, r.x+10, r.y+10);
    dc.DrawLine(r.x-10, r.y+10, r.x+10, r.y-10);

    static double s_scale = 1, s_clat, s_clon;
    static wxPoint s_point(0, 0);

    if(dc.GetDC())
        m_UpdateOverlay = true;

    double scale = vp.view_scale_ppm/s_scale;
    
    wxPoint point;
    GetCanvasPixLL(&vp, &point, s_clat, s_clon);

    static bool s_scale_changed = false;
    static double s_lastscale;
    static wxPoint s_lastpoint;
    
    if(scale == s_lastscale) {
        if(point != s_lastpoint)
            if(s_scale_changed) {
                m_UpdateOverlay = true;
                s_scale_changed = false;
            }
    } else
        s_scale_changed = true;
    s_lastscale = scale;
    s_lastpoint = point;
        
    if(!m_UpdateOverlay) {
        glPushMatrix();
        glTranslated(point.x - s_point.x, point.y - s_point.y, 0);
#if 1
        glTranslated(vp.pix_width*(1-scale)/2, vp.pix_height*(1-scale)/2, 0);

        glScaled(scale, scale, 1);
#endif

        glCallList(m_overlaylist);
        glPopMatrix();
    } else {
        m_UpdateOverlay = false;

        if(!dc.GetDC()) {
            s_clat = vp.clat, s_clon = vp.clon;
            GetCanvasPixLL(&vp, &s_point, s_clat, s_clon);
            s_scale = vp.view_scale_ppm;

            
            if(!m_overlaylist)
                m_overlaylist = glGenLists(1);
            
            glNewList(m_overlaylist, GL_COMPILE_AND_EXECUTE);
        }

    /* draw alternate routes first */
    if(m_AlternateRouteThickness) {
        Lock();
        IsoChronList::iterator it;
        if(m_bAlternatesForAll)
            it = origin.begin();
        else {
            it = origin.end();
            it--;
        }

        wxColour black(0, 0, 0);
        SetColor(dc, false, black, m_AlternateRouteThickness);
        if(!dc.GetDC())
            glBegin(GL_LINES);
        for(; it != origin.end(); ++it)
            for(IsoRouteList::iterator rit = (*it)->routes.begin();
                rit != (*it)->routes.end(); ++rit) {
                RenderAlternateRoute(*rit, !m_bAlternatesForAll, dc, vp);
            }
        if(!dc.GetDC())
            glEnd();
        Unlock();
    }

    unsigned char routecolors[][3] = {
                         {  0,   0, 127}, {  0,   0, 255},
        {255, 127,   0}, {255, 127, 127},
        {  0, 255,   0}, {  0, 255, 127},
        {127, 255,   0}, {127, 255, 127},
        {127, 127,   0},                  {127, 127, 255},
        {255,   0,   0}, {255,   0, 127}, {255,   0, 255},
        {127,   0,   0}, {127,   0, 127}, {127,   0, 255},
        {  0, 127,   0}, {  0, 127, 127}, {  0, 127, 255},
        {255, 255,   0},                  };

    if(m_IsoChronThickness) {
        Lock();
        int c = 0;
        for(IsoChronList::iterator i = origin.begin(); i != origin.end(); ++i) {
            Unlock();
            wxColor color(routecolors[c][0], routecolors[c][1], routecolors[c][2]);

            for(IsoRouteList::iterator j = (*i)->routes.begin(); j != (*i)->routes.end(); ++j)
                RenderIsoRoute(*j, color, dc, vp);
            
            if(++c == (sizeof routecolors) / (sizeof *routecolors))
                c = 0;
            Lock();
        }
        Unlock();
    }

        if(!dc.GetDC())
            glEndList();
    }

    if(m_RouteThickness) {
        SetColor(dc, true, m_CursorColor, m_RouteThickness);
        RenderCourse(last_cursor_position, time, dc, vp);
        SetColor(dc, true, m_DestinationColor, m_RouteThickness);
        RenderCourse(last_destination_position, time, dc, vp);
    }
}

void RouteMapOverlay::RenderCourse(Position *pos, wxDateTime time,
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
        DrawLine(p, p->parent, dc, vp);
        if(m_bSquaresAtSailChanges && p->sailplan != sailplan) {
            wxPoint r;
            GetCanvasPixLL(&vp, &r, p->lat, p->lon);
            dc.DrawRectangle(r.x-5, r.y-5, 10, 10);
            sailplan = p->sailplan;
        }
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

std::list<PlotData> RouteMapOverlay::GetPlotData()
{
    Position *pos = last_destination_position;
    std::list<PlotData> plotdatalist;
    if(!pos)
        return plotdatalist;

    RouteMapConfiguration configuration = GetConfiguration();
    Lock();
    IsoChronList::iterator it = origin.begin(), itn;

    /* get route iso for this position */
    Position *p, *l;
    p=pos->parent;
    if(last_destination_position == destination_position)
        p=p->parent;
    for(; p; p=p->parent)
        if(++it == origin.end())
            return plotdatalist;

    itn = it;

    l = pos;
    for(p = pos; p; p = p->parent) {
        GribRecordSet *grib = (*it)->m_Grib;

        PlotData data;
        /* this omits the starting position */
        double dt;
        if(p == destination_position)
            dt = 0;
        else
            dt = ((itn==it?EndDate():(*itn)->time) - (*it)->time).GetSeconds().ToDouble();
        data.time = (*it)->time;
        data.lat = p->lat, data.lon = p->lon;
        if(l->GetPlotData(grib, dt, configuration, data))
            plotdatalist.push_front(data);

        l = p;
        itn = it;

        if(it == origin.begin())
            break;
        if(p != destination_position)
            it--;
    }

    m_NewGrib = NULL;
    m_bNeedsGrib = true;

    Unlock();
    return plotdatalist;
}

void RouteMapOverlay::RouteInfo(double &distance, double &avgspeed, double &percentage_upwind)
{
    std::list<PlotData> plotdata = GetPlotData();
    distance = 0;
    double avgspeedtotal = 0, totalW = 0, count = 0;
    double lat0, lon0;
    for(std::list<PlotData>::iterator it=plotdata.begin(); it!=plotdata.end(); it++)
    {
        if(it != plotdata.begin()) {
            double dist;
            DistanceBearingMercator_Plugin(lat0, lon0, it->lat, it->lon, 0, &dist);
            distance += dist;
        }

        lat0 = it->lat;
        lon0 = it->lon;

        avgspeedtotal += it->VBG;
        if(fabs(it->B - it->W) < 90)
            totalW++;
        count++;
    }
    avgspeed = avgspeedtotal / count;
    percentage_upwind = 100.0 * totalW / count;
}

void RouteMapOverlay::SetSettings(wxColor CursorColor, wxColor DestinationColor,
                                  int RouteThickness, int IsoChronThickness,
                                  int AlternateRouteThickness, bool AlternatesForAll,
                                  bool SquaresAtSailChanges)
                                  
{
    m_CursorColor = CursorColor;
    m_DestinationColor = DestinationColor;
    m_RouteThickness = RouteThickness;
    m_IsoChronThickness = IsoChronThickness;
    m_AlternateRouteThickness = AlternateRouteThickness;
    m_bAlternatesForAll = AlternatesForAll;
    m_bSquaresAtSailChanges = SquaresAtSailChanges;
}

void RouteMapOverlay::Clear()
{
    RouteMap::Clear();
    last_cursor_position = NULL;
    last_destination_position = NULL;
    m_UpdateOverlay = true;
}

bool RouteMapOverlay::SetCursorLatLon(double lat, double lon)
{
    Position *p = ClosestPosition(lat, lon);
    if(p == last_cursor_position)
        return false;
    
    last_cursor_position = p;
    return true;
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

    bool done = ReachedDestination();
    Position *endp = ClosestPosition(configuration.EndLat, configuration.EndLon, 0, done);
    if(endp) {
        if(done) {
            delete destination_position;
            destination_position = new Position(configuration.EndLat,
                                                configuration.EndLon,
                                                endp->sailplan, endp->tacks,
                                                endp);
            last_destination_position = destination_position;
        } else
            last_destination_position = endp;
    } else
        last_destination_position = NULL;

    m_bUpdated = true;
    m_UpdateOverlay = true;
}
