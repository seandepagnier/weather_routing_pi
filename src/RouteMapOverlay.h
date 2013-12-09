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

#include "RouteMap.h"

class PlugIn_ViewPort;

class ocpnDC;
class RouteMapOverlay;
class SettingsDialog;

class RouteMapOverlayThread : public wxThread
{
public:
    RouteMapOverlayThread(RouteMapOverlay &routemapoverlay);
    void *Entry();

private:
    RouteMapOverlay &m_RouteMapOverlay;
};

class RouteMapOverlay : public RouteMap
{
public:
    enum RouteInfoType {DISTANCE, AVGSPEED, PERCENTAGE_UPWIND, PORT_STARBOARD, AVGWIND, AVGWAVE};

    RouteMapOverlay();
    ~RouteMapOverlay();

    bool SetCursorLatLon(double lat, double lon);
    void RenderIsoRoute(IsoRoute *r, wxColour &color, int IsoChronThickness,
                        ocpnDC &dc, PlugIn_ViewPort &vp);
    void Render(wxDateTime time, SettingsDialog &settingsdialog,
                ocpnDC &dc, PlugIn_ViewPort &vp, bool justendroute);
    void RenderCourse(Position *pos, wxDateTime time, bool SquaresAtSailChanges,
                      ocpnDC &dc, PlugIn_ViewPort &vp);
    void RequestGrib(wxDateTime time);
    std::list<PlotData> GetPlotData();
    double RouteInfo(enum RouteInfoType type);
    void RouteInfo(double &distance, double &avgspeed, double &percentage_upwind);

    bool Updated();
    void UpdateDestination();

    virtual void Clear();
    virtual void Lock() { routemutex.Lock(); }
    virtual void Unlock() { routemutex.Unlock(); }
    bool Running() { return m_Thread; }

    void Start();
    void Stop();
    void DeleteThread();

    bool m_UpdateOverlay;
    bool m_bEndRouteVisible;

private:
    void RenderAlternateRoute(IsoRoute *r, bool each_parent, int AlternateRouteThickness,
                              ocpnDC &dc, PlugIn_ViewPort &vp);
    virtual bool TestAbort() { return m_Thread->TestDestroy(); }

    RouteMapOverlayThread *m_Thread;
    wxMutex routemutex;

    void SetPointColor(ocpnDC &dc, Position *p);
    void DrawLine(Position *p1, Position *p2,
                  ocpnDC &dc, PlugIn_ViewPort &vp);

    Position *last_cursor_position, *destination_position, *last_destination_position;
    bool m_bUpdated;

    int m_overlaylist;
    double m_scale, m_clat, m_clon;
    wxPoint m_point;
    bool m_scale_changed;
    double m_lastscale;
    wxPoint m_lastpoint;
};
