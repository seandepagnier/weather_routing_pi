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
class PlugIn_Route;

class piDC;
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
                        AVGWIND, MAXWIND, MAXWINDGUST, AVGCURRENT, MAXCURRENT, AVGSWELL, MAXSWELL,
                        PERCENTAGE_UPWIND, PORT_STARBOARD, TACKS, COMFORT};

    RouteMapOverlay();
    ~RouteMapOverlay();

    bool SetCursorLatLon(double lat, double lon);
    void Render(wxDateTime time, SettingsDialog &settingsdialog,
                piDC &dc, PlugIn_ViewPort &vp, bool justendroute,
                RoutePoint* positionOnRoute=nullptr);

    static wxColour sailingConditionColor(int level);
    static wxString sailingConditionText(int level);

    void RenderWindBarbs(piDC &dc, PlugIn_ViewPort &vp);

    void RenderCurrent(piDC &dc, PlugIn_ViewPort &vp);

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
    wxDateTime GetLastCursorTime() { return m_cursor_time; }
    
    // CUSTOMIZATION
    Position *getClosestRoutePositionFromCursor(double cursorLat, double cursorLon, PlotData &posData);

    bool m_UpdateOverlay;
    bool m_bEndRouteVisible;
    void RouteAnalysis(PlugIn_Route *proute);

private:
    void RenderAlternateRoute(IsoRoute *r, bool each_parent,
                              piDC &dc, PlugIn_ViewPort &vp);

    void RenderIsoRoute(IsoRoute *r, wxColour &grib_color, wxColour &climatology_color,
                        piDC &dc, PlugIn_ViewPort &vp);
    void RenderPolarChangeMarks(bool cursor_route,  piDC &dc, PlugIn_ViewPort &vp);
    void RenderBoatOnCourse(bool cursor_route,  wxDateTime time, piDC &dc, PlugIn_ViewPort &vp);

    // Customization ComfortDisplay
    void RenderCourse(bool cursor_route, piDC &dc, PlugIn_ViewPort &vp, bool comfortRoute = false);

    // Customization WindBarbsOnRoute
    void RenderWindBarbsOnRoute(piDC &dc, PlugIn_ViewPort &vp, int lineWidth, bool apparentWind);

    int sailingConditionLevel(const PlotData &plot) const;

    virtual bool TestAbort() { return Finished(); }

    RouteMapOverlayThread *m_Thread;
    wxMutex routemutex;

    void SetPointColor(piDC &dc, Position *p);
    void DrawLine(RoutePoint *p1, RoutePoint *p2, piDC &dc, PlugIn_ViewPort &vp);
    void DrawLine(RoutePoint *p1, wxColour &color1, RoutePoint *p2, wxColour &color2,
                  piDC &dc, PlugIn_ViewPort &vp);

    double last_cursor_lat, last_cursor_lon;
    Position *last_cursor_position, *destination_position, *last_destination_position;
    wxDateTime m_cursor_time;
    wxDateTime m_EndTime;
    bool m_bUpdated;

    int m_overlaylist, m_overlaylist_projection;

    bool clear_destination_plotdata; // should be volatile
    std::list<PlotData> last_destination_plotdata;

    std::list<PlotData> last_cursor_plotdata;

    LineBuffer wind_barb_cache;
    double wind_barb_cache_scale;
    size_t wind_barb_cache_origin_size;
    int wind_barb_cache_projection;
    
    // Customization WindBarbsOnRoute
    LineBuffer wind_barb_route_cache;
    
    // Customization Sailing Comfort
    int m_sailingComfort;

    LineBuffer current_cache;
    double current_cache_scale;
    size_t current_cache_origin_size;
    int current_cache_projection;
};
