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
    : m_GribTimelineTime(wxDateTime::Now()),
      m_Thread(NULL),
      last_cursor_position(NULL), last_destination_position(NULL),
      m_bUpdated(false)
{
}

RouteMapOverlay::~RouteMapOverlay()
{
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
        glBegin(GL_LINES);
        glVertex2i(p1p.x, p1p.y);
        glVertex2i(p2p.x, p2p.y);
        glEnd();
    }
}

void RouteMapOverlay::RenderIsoRoute(IsoRoute *r, wxColour &color, ocpnDC &dc, PlugIn_ViewPort &vp)
{
    SkipPosition *s = r->skippoints;
    if(!s)
        return;

    Position *p = s->point;
    do {
        SetColor(dc, false, color, m_IsoChronThickness);
        DrawLine(p, p->next, dc, vp);
        p = p->next;
    } while(p != s->point);

#if 0
    do {
        SetColor(dc, false, wxColour(255, 0, 0), m_IsoChronThickness);
        DrawLine(s->point, s->next->point, dc, vp);
        s = s->next;
    } while(s != r->skippoints);
#endif
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

void RouteMapOverlay::Render(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    /* render start and end */
    RouteMapOptions options = GetOptions();
    wxPoint r;
    GetCanvasPixLL(&vp, &r, options.StartLat, options.StartLon);
    SetColor(dc, true, *wxBLUE, 3);
    dc.DrawLine(r.x, r.y-10, r.x+10, r.y+7);
    dc.DrawLine(r.x, r.y-10, r.x-10, r.y+7);
    dc.DrawLine(r.x-10, r.y+7, r.x+10, r.y+7);

    GetCanvasPixLL(&vp, &r, options.EndLat, options.EndLon);
    SetColor(dc, true, *wxRED, 3);
    dc.DrawLine(r.x-10, r.y-10, r.x+10, r.y+10);
    dc.DrawLine(r.x-10, r.y+10, r.x+10, r.y-10);

    if(!origin.size())
       return;

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

        for(; it != origin.end(); ++it)
            for(IsoRouteList::iterator rit = (*it)->routes.begin();
                rit != (*it)->routes.end(); ++rit) {
                wxColour black(0, 0, 0);
                SetColor(dc, false, black, m_AlternateRouteThickness);
                RenderAlternateRoute(*rit, !m_bAlternatesForAll, dc, vp);
            }
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
#if 0
            if(*i != origin.back())
                continue;
#endif
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

    if(m_RouteThickness) {
        SetColor(dc, true, m_CursorColor, m_RouteThickness);
        RenderCourse(last_cursor_position, dc, vp);
        SetColor(dc, true, m_DestinationColor, m_RouteThickness);
        RenderCourse(last_destination_position, dc, vp);
    }
}

void RouteMapOverlay::RenderCourse(Position *pos, ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!pos)
        return;

    Lock();

    /* draw lines to this route */
    Position *p;
    int sailplan = pos->sailplan;
    for(p = pos; p && p->parent; p = p->parent) {
        DrawLine(p, p->parent, dc, vp);
        if(m_bSquaresAtSailChanges && p->sailplan != sailplan) {
            wxPoint r;
            GetCanvasPixLL(&vp, &r, p->lat, p->lon);
            dc.DrawRectangle(r.x-5, r.y-5, 10, 10);
            sailplan = p->sailplan;
        }
    }

    /* render boat on optimal course at current grib time */
    wxDateTime time;
    IsoChronList::iterator it = origin.begin();

    /* get route iso for this position */
    for(p=pos->parent; p; p=p->parent)
        if(++it == origin.end())
            return;

    if(it != origin.begin())
        it--;

    time = m_GribTimelineTime;
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
}

std::list<PlotData> RouteMapOverlay::GetPlotData()
{
    Position *pos = last_destination_position;
    std::list<PlotData> plotdatalist;
    if(!pos)
        return plotdatalist;

    RouteMapOptions options = GetOptions();
    Lock();
    IsoChronList::iterator it = origin.begin(), itp;

    /* get route iso for this position */
    Position *p;
    for(p=pos->parent; p; p=p->parent)
        if(++it == origin.end())
            return plotdatalist;

    if(it != origin.begin())
        it--;

    for(p = pos; p->parent && it != origin.begin(); p = p->parent) {
        GribRecordSet *grib = (*it)->m_Grib;

        PlotData data;
        itp = it, itp--;
        /* this omits the starting position */
        double dt = ((*it)->time - (*itp)->time).GetSeconds().ToDouble();
        data.time = (*it)->time;
        data.lat = p->lat, data.lon = p->lon;
        if(p->GetPlotData(grib, (*it)->time, dt, options, data))
            plotdatalist.push_front(data);
        it--;
    }

    m_NewGrib = NULL;
    m_bNeedsGrib = true;

    Unlock();
    return plotdatalist;
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
    RouteMapOptions options = GetOptions();
    last_destination_position = ClosestPosition(options.EndLat, options.EndLon);
    m_bUpdated = true;
}
