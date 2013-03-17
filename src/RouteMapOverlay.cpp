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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "../../../include/ocpn_plugin.h"
#include "../../../include/ocpndc.h"

#include "BoatSpeed.h"
#include "RouteMapOverlay.h"
#include "Utilities.h"

RouteMapOverlay::RouteMapOverlay()
    : wxThread(wxTHREAD_JOINABLE), last_cursor_position(NULL), m_bPaused(true), m_bUpdated(false)
{
    Create();
}

void SetColor(ocpnDC &dc, bool penifgl, unsigned char c[3], int w)
{
    if(!dc.GetDC()) {
        glColor4ub(c[0], c[1], c[2], 192);
        glLineWidth(w);
        if(!penifgl)
            return;
    }
    dc.SetPen(wxPen(wxColour(c[0], c[1], c[2]), w));
}

static unsigned char overcolor[3];
void SetPointColor(ocpnDC &dc, Position *p)
{
    unsigned char cp[3] = {60, 220, 0}, cnp[3] = {240, 40, 0};
    if(p->propagated) {
        for(int i=0; i<3; i++)
            cp[i] += overcolor[i];
        SetColor(dc, false, cp, 2);
    } else
        SetColor(dc, false, cnp, 2);
}

void DrawLine(Position *p1, Position *p2, unsigned char *color,
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

    if(color == overcolor) SetPointColor(dc, p1);
    else if(color) SetColor(dc, false, color, 2);

    if(dc.GetDC()) {
        dc.DrawLine(p1p.x, p1p.y, p2p.x, p2p.y);
    } else {
        glBegin(GL_LINES);
        glVertex2i(p1p.x, p1p.y);
        if(color == overcolor) SetPointColor(dc, p2);
        glVertex2i(p2p.x, p2p.y);
        glEnd();
    }
}

void RouteMapOverlay::RenderRoute(Route *r, ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!r->points)
        return;

    Position *p = r->points;
    do {
        unsigned char cyan[3] = {0, 255, 255};
        unsigned char blue[3] = {0, 0, 255};
        unsigned char *c;
        if(r->parent)
            c = cyan;
        else if(r->direction == -1)
            c = blue;
        else
            c = overcolor;

        DrawLine(p, p->next, c, dc, vp);

        p = p->next;
    } while(p != r->points);

    /* now render any children */
    for(RouteList::iterator it = r->children.begin(); it != r->children.end(); ++it)
        RenderRoute(*it, dc, vp);
}

void RouteMapOverlay::Render(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    unsigned char routecolors[][3] = {{255, 128, 255}, {128, 128, 255}, {0, 230, 152},
                                      {152, 152, 0}, {0, 220, 200}, {202, 0, 180},
                                      {34, 69, 198}, {244, 30, 38}, {100, 240, 20}};
    int c = 0;
    Lock();
    for(RouteIsoList::iterator i = origin.begin(); i != origin.end(); ++i) {
        Unlock();
        for(int d=0; d<3; d++)
            overcolor[d] = routecolors[c][d];

        for(RouteList::iterator j = (*i)->routes.begin(); j != (*i)->routes.end(); ++j)
            RenderRoute(*j, dc, vp);

        if(++c == (sizeof routecolors) / (sizeof *routecolors))
            c = 0;
        Lock();
    }
    Unlock();

    unsigned char lcc[3] = {220, 220, 80}, erc[3] = {80, 40, 200};
    SetColor(dc, true, lcc, 4);
    RenderCourse(last_cursor_position, dc, vp);

    RouteMapOptions options = GetOptions();
    SetColor(dc, true, erc, 4);
    RenderCourse(ClosestPosition(options.EndLat, options.EndLon), dc, vp);
}

void RouteMapOverlay::RenderCourse(Position *pos, ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!pos)
        return;

    /* draw lines to this route */
    Position *p;
    for(p = pos; p && p->parent; p = p->parent)
        DrawLine(p, p->parent, NULL, dc, vp);

    /* render boat on optimal course at current grib time */
    wxDateTime time;
    Lock();
    RouteIsoList::iterator it = origin.begin();

    /* get route iso for this position */
    for(p=pos->parent; p; p=p->parent)
        if(++it == origin.end())
            return;

    if(it != origin.begin())
        it--;
    Unlock();

    time = m_GribTimelineTime;
    for(p = pos; p; p = p->parent) {
        wxDateTime ittime = (*it)->time;
        if(time >= ittime && p->parent) {
            wxDateTime timestart = (*it)->time;
            it++;
            wxDateTime timeend = (*it)->time;

            wxTimeSpan span = timeend - timestart, cspan = time - timestart;
            double d = cspan.GetSeconds().ToDouble() / span.GetSeconds().ToDouble();

            if(d > 1)
                d = 1;

            wxPoint r;
            GetCanvasPixLL(&vp, &r,
                           p->parent->lat + d*(p->lat - p->parent->lat),
                           p->parent->lon + d*heading_resolve(p->lon - p->parent->lon));

            dc.DrawCircle( r.x, r.y, 7 );
            break;
        }
        it--;
    }    
}

void RouteMapOverlay::Clear()
{
    RouteMap::Clear();
    last_cursor_position = NULL;
}

bool RouteMapOverlay::SetCursorLatLon(double lat, double lon)
{
    Position *p = ClosestPosition(lat, lon);
    if(p == last_cursor_position)
        return false;
    
    last_cursor_position = p;
    return true;
}

void *RouteMapOverlay::Entry()
{
    while(!TestDestroy())
        if(m_bPaused || !Propagate())
            wxThread::Sleep(250);
        else
            m_bUpdated = true;
    return 0;
}

bool RouteMapOverlay::Updated()
{
    bool updated = m_bUpdated;
    m_bUpdated = false;
    return updated;
}
