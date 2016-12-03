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

#include "RouteMap.h"
#include "LineBufferOverlay.h"

class PlugIn_ViewPort;

class wrDC;
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
    friend class RouteMapOverlayThread;
public:
    enum RouteInfoType {DISTANCE, AVGSPEED, MAXSPEED, AVGSPEEDGROUND, MAXSPEEDGROUND,
                        AVGWIND, MAXWIND, AVGCURRENT, MAXCURRENT, AVGSWELL, MAXSWELL,
                        PERCENTAGE_UPWIND, PORT_STARBOARD, TACKS};

    RouteMapOverlay();
    ~RouteMapOverlay();

    bool SetCursorLatLon(double lat, double lon);
    void RenderIsoRoute(IsoRoute *r, wxColour &grib_color, wxColour &climatology_color,
                        wrDC &dc, PlugIn_ViewPort &vp);
    void Render(wxDateTime time, SettingsDialog &settingsdialog,
                wrDC &dc, PlugIn_ViewPort &vp, bool justendroute);
    void RenderCourse(Position *pos, wxDateTime time, bool MarkAtPolarChange,
                      wrDC &dc, PlugIn_ViewPort &vp);
    void RenderWindBarbs(wrDC &dc, PlugIn_ViewPort &vp);
    void RenderCurrent(wrDC &dc, PlugIn_ViewPort &vp);

    void GetLLBounds(double &latmin, double &latmax, double &lonmin, double &lonmax);
    void RequestGrib(wxDateTime time);

    std::list<PlotData> &GetPlotData(bool cursor_route=false);
    double RouteInfo(enum RouteInfoType type, bool cursor_route=false);
    int Cyclones(int *months);
    Position *GetDestination() { return destination_position; }

    bool Updated();
    void UpdateCursorPosition();
    void UpdateDestination();

    wxDateTime EndTime() { return m_EndTime; }

    virtual void Clear();
    virtual void Lock() { routemutex.Lock(); }
    virtual void Unlock() { routemutex.Unlock(); }
    bool Running() { return m_Thread && m_Thread->IsAlive(); }

    bool Start(wxString &error);
    void DeleteThread(); // like Stop(), but waits until the thread is deleted

    Position *GetLastCursorPosition() { return last_cursor_position; }
    
    bool m_UpdateOverlay;
    bool m_bEndRouteVisible;

private:
    void RenderAlternateRoute(IsoRoute *r, bool each_parent,
                              wrDC &dc, PlugIn_ViewPort &vp);
    virtual bool TestAbort() { return Finished(); }

    RouteMapOverlayThread *m_Thread;
    wxMutex routemutex;

    void SetPointColor(wrDC &dc, Position *p);
    void DrawLine(Position *p1, Position *p2, wrDC &dc, PlugIn_ViewPort &vp);
    void DrawLine(Position *p1, wxColour &color1, Position *p2, wxColour &color2,
                  wrDC &dc, PlugIn_ViewPort &vp);

    double last_cursor_lat, last_cursor_lon;
    Position *last_cursor_position, *destination_position, *last_destination_position;
    wxDateTime m_EndTime;
    bool m_bUpdated;

    int m_overlaylist, m_overlaylist_projection;

    std::list<PlotData> last_destination_plotdata, last_cursor_plotdata;

    LineBuffer wind_barb_cache;
    double wind_barb_cache_scale;
    size_t wind_barb_cache_origin_size;
    int wind_barb_cache_projection;

    LineBuffer current_cache;
    double current_cache_scale;
    size_t current_cache_origin_size;
    int current_cache_projection;
};
