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

static void drawTransformedLine( ocpnDC &dc, double si, double co, int di, int dj,
                                 int i, int j, int k, int l )
{
    double fi, fj, fk, fl; // For Hi Def Graphics.

    fi = ( i * co - j * si + 0.5 ) + di;
    fj = ( i * si + j * co + 0.5 ) + dj;
    fk = ( k * co - l * si + 0.5 ) + di;
    fl = ( k * si + l * co + 0.5 ) + dj;

    dc.StrokeLine( fi, fj, fk, fl );
}

static void drawPetiteBarbule( ocpnDC &dc, double si, double co, int di, int dj, int b )
{
    drawTransformedLine( dc, si, co, di, dj, b, 0, b + 2, 5 );
}

static void drawGrandeBarbule( ocpnDC &dc, double si, double co, int di, int dj, int b )
{
    drawTransformedLine( dc, si, co, di, dj, b, 0, b + 4, 10 );
}

static void drawTriangle( ocpnDC &dc, double si, double co, int di, int dj, int b )
{
    drawTransformedLine( dc, si, co, di, dj, b, 0, b + 4, 10 );
    drawTransformedLine( dc, si, co, di, dj, b + 8, 0, b + 4, 10 );
}

static void drawWindArrowWithBarbs(ocpnDC &dc, int x, int y, double vkn, double ang, double rotate_angle)
{
    ang += rotate_angle;
    double si = -cos( ang ), co = sin( ang );

    if( vkn < 1 ) {
        dc.DrawCircle( x, y, 5 ); // wind is very light, draw a circle
        return;
    }

    // Arrange for arrows to be centered on origin
    int windBarbuleSize = 26;
    int dec = -windBarbuleSize / 2;
    drawTransformedLine( dc, si, co, x, y, dec, 0, dec + windBarbuleSize, 0 );   // hampe
    drawTransformedLine( dc, si, co, x, y, dec, 0, dec + 5, 2 );    // flèche
    drawTransformedLine( dc, si, co, x, y, dec, 0, dec + 5, -2 );   // flèche
    
    int b1 = dec + windBarbuleSize - 4;  // position de la 1ère barbule
    if( vkn >= 7.5 && vkn < 45 ) {
        b1 = dec + windBarbuleSize;  // position de la 1ère barbule si >= 10 noeuds
    }

    if( vkn < 7.5 ) {  // 5 ktn
        drawPetiteBarbule( dc, si, co, x, y, b1 );
    } else if( vkn < 12.5 ) { // 10 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
    } else if( vkn < 17.5 ) { // 15 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
        drawPetiteBarbule( dc, si, co, x, y, b1 - 4 );
    } else if( vkn < 22.5 ) { // 20 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 4 );
    } else if( vkn < 27.5 ) { // 25 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 4 );
        drawPetiteBarbule( dc, si, co, x, y, b1 - 8 );
    } else if( vkn < 32.5 ) { // 30 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 4 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 8 );
    } else if( vkn < 37.5 ) { // 35 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 4 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 8 );
        drawPetiteBarbule( dc, si, co, x, y, b1 - 12 );
    } else if( vkn < 45 ) { // 40 ktn
        drawGrandeBarbule( dc, si, co, x, y, b1 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 4 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 8 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 12 );
    } else if( vkn < 55 ) { // 50 ktn
        drawTriangle( dc, si, co, x, y, b1 - 4 );
    } else if( vkn < 65 ) { // 60 ktn
        drawTriangle( dc, si, co, x, y, b1 - 4 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 8 );
    } else if( vkn < 75 ) { // 70 ktn
        drawTriangle( dc, si, co, x, y, b1 - 4 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 8 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 12 );
    } else if( vkn < 85 ) { // 80 ktn
        drawTriangle( dc, si, co, x, y, b1 - 4 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 8 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 12 );
        drawGrandeBarbule( dc, si, co, x, y, b1 - 16 );
    } else { // > 90 ktn
        drawTriangle( dc, si, co, x, y, b1 - 4 );
        drawTriangle( dc, si, co, x, y, b1 - 12 );
    }
}

void RouteMapOverlay::RenderWindBarbs(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    RouteMapConfiguration configuration = GetConfiguration();

    if(origin.size() < 2) // no map to work with
        return;

    wxColour colour(180, 140, 14);

    wxPen pen( colour, 2 );

    dc.SetPen( pen );
    dc.SetBrush( *wxTRANSPARENT_BRUSH);

    Lock();
    
    double step = 36;
    for(double x = vp.rv_rect.x; x<vp.rv_rect.width; x+=step)
        for(double y = vp.rv_rect.y; y<vp.rv_rect.height; y+=step) {
            double lat, lon;
            GetCanvasLLPix( &vp, wxPoint(x, y), &lat, &lon );

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
            p1->GetWindData(configuration, W1, VW1, data_mask1);

            it++;
            Position *p2 = (*it)->ClosestPosition(lat, lon);
            configuration.grib = (*it)->m_Grib;
            p2->GetWindData(configuration, W2, VW2, data_mask2);

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

            drawWindArrowWithBarbs( dc, x, y, VW, deg2rad(W), vp.rotation );
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

std::list<PlotData> &RouteMapOverlay::GetPlotData(bool cursor_route)
{
    std::list<PlotData> &plotdata = cursor_route ? last_cursor_plotdata : last_destination_plotdata;
    if(plotdata.empty()) {
        Position *pos = cursor_route ? last_cursor_position : last_destination_position;
        if(!pos)
            return plotdata;

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
                return plotdata;

        itn = it;

        l = pos;
        for(p = pos; p; p = p->parent) {
            configuration.grib = (*it)->m_Grib;

            PlotData data;
            /* this omits the starting position */
            double dt = 0;
            if(p != destination_position) {
                wxDateTime endtime = itn==it ? EndTime() : (*itn)->time;
                wxDateTime starttime = (*it)->time;
                if(starttime.IsValid() && endtime.IsValid())
                    dt = (endtime - starttime).GetSeconds().ToDouble();
            }
            data.time = (*it)->time;
            data.lat = p->lat, data.lon = p->lon;
            if(l->GetPlotData(dt, configuration, data))
                plotdata.push_front(data);

            l = p;
            itn = it;

            if(it == origin.begin())
                break;
            if(p != destination_position)
                it--;
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
