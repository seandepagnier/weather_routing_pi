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

#ifndef _WEATHER_ROUTINGPI_H_
#define _WEATHER_ROUTINGPI_H_

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    9

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    10

#include "../../../include/ocpn_plugin.h"
#include "../../../include/ocpndc.h"

/* make some warnings go away */
#ifdef MIN
#undef MIN
#endif

#ifdef MAX
#undef MAX
#endif

#include "../../../include/wx/jsonreader.h"
#include "../../../include/wx/jsonwriter.h"

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define WEATHER_ROUTING_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class WeatherRouting;

class weather_routing_pi : public opencpn_plugin_110
{
public:
      weather_routing_pi(void *ppimgr);
      ~weather_routing_pi(void);

      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);

      void SetDefaults(void);

      int GetToolbarToolCount(void);

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

private:
      bool LoadConfig(void);
      bool SaveConfig(void);

      wxFileConfig     *m_pconfig;
      wxWindow         *m_parent_window;

      WeatherRouting     *m_pWeather_Routing;
      wxDateTime m_GribTime;

      int              m_display_width, m_display_height;
      int              m_leftclick_tool_id;
      int              m_position_menu_id;

      double m_cursor_lat, m_cursor_lon;
};

#endif
