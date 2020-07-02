/******************************************************************************
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
#ifdef DEBUG_BUILD
#  define DEBUGSL(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " : " << x << std::endl; } while (0)

#  define DEBUGST(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " : " << x; } while (0)

#  define DEBUGCONT(x) do { \
std::cout << x ; } while (0)

#  define DEBUGEND(x) do { \
std::cout << x << std::endl; } while (0)
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif

#ifndef _WEATHER_ROUTINGPI_H_
#define _WEATHER_ROUTINGPI_H_

#ifndef __OCPN__ANDROID__
#define GetDateCtrlValue GetValue
#define GetTimeCtrlValue GetValue
#endif

#include "version.h"

#define ABOUT_AUTHOR_URL "http://seandepagnier.users.sourceforge.net"

#include "ocpn_plugin.h"
#include "plugingl/pidc.h"
#include "plugingl/qtstylesheet.h"

/* make some warnings go away */
#ifdef MIN
#undef MIN
#endif

#ifdef MAX
#undef MAX
#endif

#include "json/json.h"

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define WEATHER_ROUTING_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class WeatherRouting;

class weather_routing_pi : public wxEvtHandler, public opencpn_plugin_116
{
public:
      weather_routing_pi(void *ppimgr);
      ~weather_routing_pi();

      int Init();
      bool DeInit();

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();
      bool InBoundary(double lat, double lon);

      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);

      void SetDefaults();

      int GetToolbarToolCount();

      void SetCursorLatLon(double lat, double lon);

      void SetPluginMessage(wxString &message_id, wxString &message_body);
      void SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix);
      void ShowPreferencesDialog( wxWindow* parent );

      void OnToolbarToolCallback(int id);
      void OnContextMenuItemCallback(int id);

      void SetColorScheme(PI_ColorScheme cs);
      static wxString StandardPath();
      void ShowMenuItems(bool show);

      double m_boat_lat, m_boat_lon;
      double m_cursor_lat, m_cursor_lon;

private:
      void OnCursorLatLonTimer( wxTimerEvent & );
      void RequestOcpnDrawSetting();
      void NewWR();

      bool LoadConfig();
      bool SaveConfig();

      bool	       b_in_boundary_reply;

      wxFileConfig     *m_pconfig;
      wxWindow         *m_parent_window;

      WeatherRouting     *m_pWeather_Routing;
      wxDateTime m_GribTime;

      int              m_display_width, m_display_height;
      int              m_leftclick_tool_id;
      int              m_position_menu_id;
      int              m_waypoint_menu_id;
      int              m_route_menu_id;

      wxTimer m_tCursorLatLon;
};

#endif
