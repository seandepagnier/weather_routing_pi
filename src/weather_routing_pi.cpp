/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier                                 *
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
#include <wx/stdpaths.h>
#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "WeatherRouting.h"
#include "weather_routing_pi.h"

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new weather_routing_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

#include "icons.h"

weather_routing_pi::weather_routing_pi(void *ppimgr)
      :opencpn_plugin_110(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

      m_tCursorLatLon.Connect(wxEVT_TIMER, wxTimerEventHandler
                              ( weather_routing_pi::OnCursorLatLonTimer ), NULL, this);
}

weather_routing_pi::~weather_routing_pi(void)
{
      delete _img_WeatherRouting;
}

int weather_routing_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-weather_routing_pi") );

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      // Get a pointer to the opencpn display canvas, to use as a parent for the WEATHER_ROUTING dialog
      m_parent_window = GetOCPNCanvasWindow();

      m_pWeather_Routing = NULL;

      m_leftclick_tool_id  = InsertPlugInTool
          (_T(""), _img_WeatherRouting, _img_WeatherRouting, wxITEM_CHECK,
           _("Weather_Routing"), _T(""), NULL,
           WEATHER_ROUTING_TOOL_POSITION, 0, this);

      wxMenu dummy_menu;
      m_position_menu_id = AddCanvasContextMenuItem
          (new wxMenuItem(&dummy_menu, -1, _("Weather Route Position")), this );
      SetCanvasContextMenuItemViz(m_position_menu_id, false);

      //    And load the configuration items
      LoadConfig();

      return (WANTS_OVERLAY_CALLBACK |
              WANTS_OPENGL_OVERLAY_CALLBACK |
              WANTS_TOOLBAR_CALLBACK    |
              WANTS_CONFIG              |
              WANTS_CURSOR_LATLON       |
              WANTS_NMEA_EVENTS         |
              WANTS_PLUGIN_MESSAGING
            );
}

bool weather_routing_pi::DeInit(void)
{
    if(m_pWeather_Routing)
        m_pWeather_Routing->Close();
    WeatherRouting *wr = m_pWeather_Routing;
    m_pWeather_Routing = NULL; /* needed first as destructor may call event loop */
    delete wr;

    return true;
}

int weather_routing_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int weather_routing_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int weather_routing_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int weather_routing_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *weather_routing_pi::GetPlugInBitmap()
{
    return new wxBitmap(_img_WeatherRouting->ConvertToImage().Copy());
}

wxString weather_routing_pi::GetCommonName()
{
      return _("WeatherRouting");
}

wxString weather_routing_pi::GetShortDescription()
{
    return _("Compute optimal routes based on weather and constraints.");
}

wxString weather_routing_pi::GetLongDescription()
{
    return _("\
Weather Routing features include:\n\
  optimal routing subject to various constraints based on weather data\n\
  automatic boat polar computation\n\
");
}

void weather_routing_pi::SetDefaults(void)
{
}

int weather_routing_pi::GetToolbarToolCount(void)
{
      return 1;
}

void weather_routing_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_pWeather_Routing && m_pWeather_Routing->CurrentRouteMap() && !m_tCursorLatLon.IsRunning())
        m_tCursorLatLon.Start(50, true);

    m_cursor_lat = lat;
    m_cursor_lon = lon;
}

void weather_routing_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    if(message_id == _T("GRIB_TIMELINE"))
    {
        wxJSONReader r;
        wxJSONValue v;
        r.Parse(message_body, &v);

        wxDateTime time;
        time.Set
            (v[_T("Day")].AsInt(), (wxDateTime::Month)v[_T("Month")].AsInt(), v[_T("Year")].AsInt(),
             v[_T("Hour")].AsInt(), v[_T("Minute")].AsInt(), v[_T("Second")].AsInt());

        if(m_pWeather_Routing) {
            m_pWeather_Routing->m_ConfigurationDialog.m_GribTimelineTime = time;
            m_pWeather_Routing->m_ConfigurationDialog.m_cbUseGrib->Enable();
            RequestRefresh(m_parent_window);
        }
    }
    if(message_id == _T("GRIB_TIMELINE_RECORD"))
    {
        wxJSONReader r;
        wxJSONValue v;
        r.Parse(message_body, &v);

        wxString sptr = v[_T("TimelineSetPtr")].AsString();
        wxCharBuffer bptr = sptr.To8BitData();
        const char* ptr = bptr.data();

        GribRecordSet *gptr;
        sscanf(ptr, "%p", &gptr);

        if(m_pWeather_Routing) {
            RouteMapOverlay *routemapoverlay = m_pWeather_Routing->m_RouteMapOverlayNeedingGrib;
            if(routemapoverlay)
                routemapoverlay->SetNewGrib(gptr);
        }
    }
    if(message_id == _T("CLIMATOLOGY"))
    {
        if(!m_pWeather_Routing)
            return; /* not ready */

        wxJSONReader r;
        wxJSONValue v;
        r.Parse(message_body, &v);

        int major = v[_T("ClimatologyVersionMajor")].AsInt();
//        int minor = v[_T("ClimatologyVersionMinor")].AsInt();
        if(major > 0) {
            wxMessageDialog mdlg(m_parent_window,
                                 _("Climatology plugin version not supported, no climatology data\n"),
                                 _("Weather Routing"), wxOK | wxICON_WARNING);
            mdlg.ShowModal();
            return;
        }

        wxString sptr = v[_T("ClimatologyDataPtr")].AsString();
        sscanf(sptr.To8BitData().data(), "%p", &RouteMap::ClimatologyData);

        sptr = v[_T("ClimatologyCycloneTrackCrossingsPtr")].AsString();
        sscanf(sptr.To8BitData().data(), "%p", &RouteMap::ClimatologyCycloneTrackCrossings);

        if(m_pWeather_Routing) {
            m_pWeather_Routing->m_ConfigurationDialog.m_cbUseClimatology->Enable(RouteMap::ClimatologyData);
            m_pWeather_Routing->m_ConfigurationDialog.m_pCyclones->Enable
                (RouteMap::ClimatologyCycloneTrackCrossings);
        }
    }
}

void weather_routing_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    m_boat_lat = pfix.Lat;
    m_boat_lon = pfix.Lon;
}

void weather_routing_pi::ShowPreferencesDialog( wxWindow* parent )
{
}

void weather_routing_pi::OnToolbarToolCallback(int id)
{
    if(!m_pWeather_Routing) {
        m_pWeather_Routing = new WeatherRouting(m_parent_window, *this);
        wxPoint p = m_pWeather_Routing->GetPosition();
        m_pWeather_Routing->Move(0,0);        // workaround for gtk autocentre dialog behavior
        m_pWeather_Routing->Move(p);

        SendPluginMessage(wxString(_T("GRIB_TIMELINE_REQUEST")), _T(""));
        SendPluginMessage(wxString(_T("CLIMATOLOGY_REQUEST")), _T(""));
        m_pWeather_Routing->Reset();
    }

    m_pWeather_Routing->Show(!m_pWeather_Routing->IsShown());
}

void weather_routing_pi::OnContextMenuItemCallback(int id)
{
    if(!m_pWeather_Routing)
        return;

    if(id == m_position_menu_id)
        m_pWeather_Routing->AddPosition(m_cursor_lat, m_cursor_lon);

    m_pWeather_Routing->Reset();
}

bool weather_routing_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(m_pWeather_Routing && m_pWeather_Routing->IsShown()) {
        ocpnDC odc(dc);
        m_pWeather_Routing->Render(odc, *vp);
        return true;
    }
    return false;
}

bool weather_routing_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(m_pWeather_Routing && m_pWeather_Routing->IsShown()) {
        ocpnDC odc;
        m_pWeather_Routing->Render(odc, *vp);
        return true;
    }
    return false;
}

void weather_routing_pi::OnCursorLatLonTimer( wxTimerEvent & )
{
    RouteMapOverlay *crm = m_pWeather_Routing->CurrentRouteMap();
    if(crm && crm->SetCursorLatLon(m_cursor_lat, m_cursor_lon))
        RequestRefresh(m_parent_window);
}

bool weather_routing_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(!pConf)
          return false;

      pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );
      return true;
}

bool weather_routing_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(!pConf)
          return false;

      pConf->SetPath ( _T ( "/PlugIns/WeatherRouting" ) );
      return true;
}

void weather_routing_pi::SetColorScheme(PI_ColorScheme cs)
{
      DimeWindow(m_pWeather_Routing);
}

wxString weather_routing_pi::StandardPath()
{
    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
#ifdef __WXMSW__
    wxString stdPath  = std_path.GetConfigDir();
#endif
#ifdef __WXGTK__
    wxString stdPath  = std_path.GetUserDataDir();
#endif
#ifdef __WXOSX__
    wxString stdPath  = std_path.GetUserConfigDir();   // should be ~/Library/Preferences	
#endif

    return stdPath + wxFileName::GetPathSeparator() +
        _T("plugins") + wxFileName::GetPathSeparator() +
        _T("weather_routing") +  wxFileName::GetPathSeparator();
}

void weather_routing_pi::ShowMenuItems(bool show)
{
    SetToolbarItemState( m_leftclick_tool_id, show );
    SetCanvasContextMenuItemViz(m_position_menu_id, show);
}
