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
    RouteMapOverlay();
    ~RouteMapOverlay();

    bool SetCursorLatLon(double lat, double lon);
    void RenderIsoRoute(IsoRoute *r, wxColour &color, ocpnDC &dc, PlugIn_ViewPort &vp);
    void Render(ocpnDC &dc, PlugIn_ViewPort &vp);
    void RenderCourse(Position *pos, ocpnDC &dc, PlugIn_ViewPort &vp);
    void RequestGrib(wxDateTime time);
    std::list<PlotData> GetPlotData();

    void SetSettings(wxColor CursorColor, wxColor DestinationColor,
                     int IsoRouteThickness, int IsoChronThickness,
                     int AlternateRouteThickness, bool AlternatesForAll,
                     bool SquaresAtSailChanges);

    bool Updated();
    void UpdateDestination();

    virtual void Clear();
    virtual void Lock() { routemutex.Lock(); }
    virtual void Unlock() { routemutex.Unlock(); }
    bool Running() { return m_Thread; }

    void Start();
    void Stop();

    wxDateTime m_GribTimelineTime;

private:
    void RenderAlternateRoute(IsoRoute *r, bool each_parent, ocpnDC &dc, PlugIn_ViewPort &vp);
    virtual bool TestAbort() { return m_Thread->TestDestroy(); }

    RouteMapOverlayThread *m_Thread;
    wxMutex routemutex;

    void SetPointColor(ocpnDC &dc, Position *p);
    void DrawLine(Position *p1, Position *p2,
                  ocpnDC &dc, PlugIn_ViewPort &vp);

    Position *last_cursor_position, *last_destination_position;
    bool m_bUpdated;

    wxColour m_CursorColor, m_DestinationColor;
    int m_RouteThickness, m_IsoChronThickness, m_AlternateRouteThickness;
    bool m_bAlternatesForAll, m_bSquaresAtSailChanges;
};
