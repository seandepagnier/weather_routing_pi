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

#include "RouteMap.h"

class PlugIn_ViewPort;

class ocpnDC;

class RouteMapOverlay : public RouteMap, public wxThread
{
public:
    RouteMapOverlay();
    void *Entry();
    bool Updated();
    bool Paused() { return m_bPaused; }
    void TogglePaused() { m_bPaused = !m_bPaused; }
    void Pause() { m_bPaused = true; }

    bool SetCursorLatLon(double lat, double lon);
    void RenderRoute(Route *r, ocpnDC &dc, PlugIn_ViewPort &vp);
    void Render(ocpnDC &dc, PlugIn_ViewPort &vp);
    void RenderCourse(Position *pos, ocpnDC &dc, PlugIn_ViewPort &vp);

    virtual void Clear();
    virtual void Lock() { routemutex.Lock(); }
    virtual void Unlock() { routemutex.Unlock(); }

    wxMutex routemutex;

    wxDateTime m_GribTimelineTime;

private:
    Position *last_cursor_position;
    bool m_bPaused, m_bUpdated;
};
