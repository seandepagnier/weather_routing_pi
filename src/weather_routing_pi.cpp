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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers


#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "BoatSpeed.h"
#include "RouteMap.h"
#include "weather_routing_pi.h"


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new weather_routing_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


//---------------------------------------------------------------------------------------------------------
//
//    Weather_Routing PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

weather_routing_pi::weather_routing_pi(void *ppimgr)
      :opencpn_plugin_18(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

      m_pWeather_RoutingDialog = NULL;

      m_boat_lat = -35;
      m_boat_lon = 174;
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

      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_WeatherRouting, _img_WeatherRouting, wxITEM_CHECK,
                                              _("Weather_Routing"), _T(""), NULL,
                                              WEATHER_ROUTING_TOOL_POSITION, 0, this);

      //    And load the configuration items
      LoadConfig();

      return (WANTS_OVERLAY_CALLBACK |
           WANTS_OPENGL_OVERLAY_CALLBACK |
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
           WANTS_CURSOR_LATLON       |
           WANTS_NMEA_EVENTS         |
           WANTS_PREFERENCES
            );
}

bool weather_routing_pi::DeInit(void)
{
      if(m_pWeather_RoutingDialog)
            m_pWeather_RoutingDialog->Close();

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
      return _img_WeatherRouting;
}

wxString weather_routing_pi::GetCommonName()
{
      return _("WeatherRouting");
}


wxString weather_routing_pi::GetShortDescription()
{
      return _("Weather Routing PlugIn for OpenCPN");
}


wxString weather_routing_pi::GetLongDescription()
{
      return _("Weather Routing PlugIn for OpenCPN\n\n\
Provides Weather routing features include:\n\
  automatic routing subject to various constraints:\n\
          islands or inverted regions (places we can't go in a boat)\n\
          optimal speed based on wind and currents\n\
          boat speed calculation\n\
          sail/power \n\
          \n\
          , great circle route, constrained routes, optimal routing. \n\
p");

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
    if(m_pWeather_RoutingDialog)
        if(m_pWeather_RoutingDialog->routemap.SetCursorLatLon(lat, lon))
            RequestRefresh(m_parent_window);
}

void weather_routing_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    m_boat_lat = pfix.Lat;
    m_boat_lon = pfix.Lon;
}

void weather_routing_pi::ShowPreferencesDialog( wxWindow* parent )
{
  wxDialog *dialog = new wxDialog( parent, wxID_ANY, _("Weather Routing Preferences"),
                                   wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE );
  wxFlexGridSizer* pGrid = new wxFlexGridSizer(2);
  dialog->SetSizer(pGrid);

  wxStaticText *stime_step = new wxStaticText( dialog, -1, _("degree step for apparent wind angles"));
  pGrid->Add(stime_step, 0, wxALIGN_LEFT|wxALL, 1);
  wxSlider* degree_step = new wxSlider(dialog, wxID_ANY,
                                       m_pWeather_RoutingDialog->routemap.degree_step, DEGREE_STEP, 60,
                                       wxDefaultPosition, wxDefaultSize,
                                       wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  pGrid->Add(degree_step, 1, wxEXPAND | wxALL, 1);

  wxStaticText *ptime_step = new wxStaticText( dialog, -1, _("time step between iterations"));
  pGrid->Add(ptime_step, 0, wxALIGN_LEFT | wxALL, 1);
  wxTextCtrl *time_step = new wxTextCtrl( dialog, -1 );
  pGrid->Add(time_step, 1, wxALIGN_RIGHT | wxALL, 1);
  wxString s;
  s.Printf(_T("%ld"), m_pWeather_RoutingDialog->routemap.dt.GetSeconds().ToLong());
  time_step->SetValue(s);

  wxStdDialogButtonSizer* DialogButtonSizer = dialog->CreateStdDialogButtonSizer(wxOK|wxCANCEL);
  pGrid->Add(DialogButtonSizer, 0, wxALIGN_RIGHT|wxALL, 1);

  if(dialog->ShowModal() == wxID_OK)
  {
    m_pWeather_RoutingDialog->routemap.degree_step = degree_step->GetValue();
    double ts;
    if(time_step->GetValue().ToDouble(&ts))
      m_pWeather_RoutingDialog->routemap.dt = wxTimeSpan(0, 0, ts, 0);
    SaveConfig();
  }
  dialog->Destroy();
}

void weather_routing_pi::OnToolbarToolCallback(int id)
{
    if(!m_pWeather_RoutingDialog)
        m_pWeather_RoutingDialog = new WeatherRoutingDialog(m_parent_window, m_boat_lat, m_boat_lon);
    m_pWeather_RoutingDialog->Show(!m_pWeather_RoutingDialog->IsShown());
}

bool weather_routing_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    return false;
}

bool weather_routing_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(m_pWeather_RoutingDialog && m_pWeather_RoutingDialog->IsShown()) {
        m_pWeather_RoutingDialog->RenderRouteMap(vp);
        return true;
    } else
        return false;
}

bool weather_routing_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

 #if 0
           m_dialog_sx = pConf->Read ( _T ( "DialogSizeX" ), 300L );
            m_dialog_sy = pConf->Read ( _T ( "DialogSizeY" ), 540L );
            m_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 170L );
#endif

            return true;
      }
      else
            return false;
}

bool weather_routing_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/PlugIns/WeatherRouting" ) );

#if 0
            pConf->Write ( _T ( "DialogSizeX" ),  m_dialog_sx );
            pConf->Write ( _T ( "DialogSizeY" ),  m_dialog_sy );
            pConf->Write ( _T ( "DialogPosX" ),   m_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_dialog_y );
#endif
            return true;
      }
      else
            return false;
}

void weather_routing_pi::SetColorScheme(PI_ColorScheme cs)
{
      DimeWindow(m_pWeather_RoutingDialog);
}
