/******************************************************************************
 *
 * Project:  OpenCPN Advanced Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2012 by Sean D'Epagnier                                 *
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

#include "boatspeed.h"
#include "routemap.h"
#include "advanced_routing_pi.h"


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new advanced_routing_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


//---------------------------------------------------------------------------------------------------------
//
//    Advanced_Routing PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

advanced_routing_pi::advanced_routing_pi(void *ppimgr)
      :opencpn_plugin_18(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

}

advanced_routing_pi::~advanced_routing_pi(void)
{
      delete _img_advancedrouting;
}

int advanced_routing_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-advanced_routing_pi") );

      // Set some default private member parameters
      m_dialog_x = 0;
      m_dialog_y = 0;
      m_dialog_sx = 200;
      m_dialog_sy = 200;
      m_pAdvanced_RoutingDialog = NULL;
      m_bShowAdvanced_Routing = false;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      // Get a pointer to the opencpn display canvas, to use as a parent for the ADVANCED_ROUTING dialog
      m_parent_window = GetOCPNCanvasWindow();

      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_advancedrouting, _img_advancedrouting, wxITEM_CHECK,
                                              _("Advanced_Routing"), _T(""), NULL,
                                              ADVANCED_ROUTING_TOOL_POSITION, 0, this);

      m_pAdvanced_RoutingDialog = new AdvancedRoutingDialog();
      m_pAdvanced_RoutingDialog->Create ( m_parent_window, this, -1,
                                          _("Advanced Routing Display Control"),
                                          wxPoint( m_dialog_x,
                                                   m_dialog_y),
                                          wxSize( m_dialog_sx,
                                                  m_dialog_sy));
      m_pAdvanced_RoutingDialog->Hide();

      //    And load the configuration items
      LoadConfig();

      return (WANTS_OVERLAY_CALLBACK |
           WANTS_OPENGL_OVERLAY_CALLBACK |
           WANTS_CURSOR_LATLON       |
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
           WANTS_NMEA_EVENTS         |
           WANTS_PREFERENCES
            );
}

bool advanced_routing_pi::DeInit(void)
{
      if(m_pAdvanced_RoutingDialog)
            m_pAdvanced_RoutingDialog->Close();

      return true;
}

int advanced_routing_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int advanced_routing_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int advanced_routing_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int advanced_routing_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *advanced_routing_pi::GetPlugInBitmap()
{
      return _img_advancedrouting;
}

wxString advanced_routing_pi::GetCommonName()
{
      return _("ADVANCED_ROUTING");
}


wxString advanced_routing_pi::GetShortDescription()
{
      return _("ADVANCED_ROUTING PlugIn for OpenCPN");
}


wxString advanced_routing_pi::GetLongDescription()
{
      return _("Advanced Routing PlugIn for OpenCPN\n\n"
"Provides Advanced routing features include:"
"  automatic routing subject to various constraints:"
"          islands or inverted regions (places we can't go in a boat)"
"          optimal speed based on wind and currents"
"          boat speed calculation"
"          sail/power "
"          "
"          , great circle route, constrained routes, optimal routing. "
"        basic ADVANCED_ROUTING file overlay capabilities for several ADVANCED_ROUTING file types.\n\n"
"Supported ADVANCED_ROUTING file types include:\n"
"- wind direction and speed\n"
"- significant wave height\n"
"- sea surface temperature\n"
"- surface current direction and speed.");

}


void advanced_routing_pi::SetDefaults(void)
{
}


int advanced_routing_pi::GetToolbarToolCount(void)
{
      return 1;
}

void advanced_routing_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
      if(m_pAdvanced_RoutingDialog)
      {
            m_pAdvanced_RoutingDialog->SetBoatLatLon(pfix.Lat, pfix.Lon);
      }
}

void advanced_routing_pi::ShowPreferencesDialog( wxWindow* parent )
{
  wxDialog *dialog = new wxDialog( parent, wxID_ANY, _("Advanced Routing Preferences"),
                                   wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE );
  wxFlexGridSizer* pGrid = new wxFlexGridSizer(2);
  dialog->SetSizer(pGrid);

  wxStaticText *stime_step = new wxStaticText( dialog, -1, _("degree step for apparent wind angles"));
  pGrid->Add(stime_step, 0, wxALIGN_LEFT|wxALL, 1);
  wxSlider* degree_step = new wxSlider(dialog, wxID_ANY,
                                       m_pAdvanced_RoutingDialog->routemap.degree_step, DEGREE_STEP, 60,
                                       wxDefaultPosition, wxDefaultSize,
                                       wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  pGrid->Add(degree_step, 1, wxEXPAND | wxALL, 1);

  wxStaticText *ptime_step = new wxStaticText( dialog, -1, _("time step between iterations"));
  pGrid->Add(ptime_step, 0, wxALIGN_LEFT | wxALL, 1);
  wxTextCtrl *time_step = new wxTextCtrl( dialog, -1 );
  pGrid->Add(time_step, 1, wxALIGN_RIGHT | wxALL, 1);
  wxString s;
  s.Printf(_T("%ld"), m_pAdvanced_RoutingDialog->routemap.dt.GetSeconds().ToLong());
  time_step->SetValue(s);

  wxStdDialogButtonSizer* DialogButtonSizer = dialog->CreateStdDialogButtonSizer(wxOK|wxCANCEL);
  pGrid->Add(DialogButtonSizer, 0, wxALIGN_RIGHT|wxALL, 1);

  if(dialog->ShowModal() == wxID_OK)
  {
    m_pAdvanced_RoutingDialog->routemap.degree_step = degree_step->GetValue();
    double ts;
    if(time_step->GetValue().ToDouble(&ts))
      m_pAdvanced_RoutingDialog->routemap.dt = wxTimeSpan(0, 0, ts, 0);
    SaveConfig();
  }
  dialog->Destroy();
}

void advanced_routing_pi::OnToolbarToolCallback(int id)
{
      // Qualify the ADVANCED_ROUTING dialog position
            bool b_reset_pos = false;

#ifdef __WXMSW__
        //  Support MultiMonitor setups which an allow negative window positions.
        //  If the requested window does not intersect any installed monitor,
        //  then default to simple primary monitor positioning.
            RECT frame_title_rect;
            frame_title_rect.left =   m_dialog_x;
            frame_title_rect.top =    m_dialog_y;
            frame_title_rect.right =  m_dialog_x + m_dialog_sx;
            frame_title_rect.bottom = m_dialog_y + 30;


            if(NULL == MonitorFromRect(&frame_title_rect, MONITOR_DEFAULTTONULL))
                  b_reset_pos = true;
#else
       //    Make sure drag bar (title bar) of window on Client Area of screen, with a little slop...
            wxRect window_title_rect;                    // conservative estimate
            window_title_rect.x = m_dialog_x;
            window_title_rect.y = m_dialog_y;
            window_title_rect.width = m_dialog_sx;
            window_title_rect.height = 30;

            wxRect ClientRect = wxGetClientDisplayRect();
            ClientRect.Deflate(60, 60);      // Prevent the new window from being too close to the edge
            if(!ClientRect.Intersects(window_title_rect))
                  b_reset_pos = true;

#endif

            if(b_reset_pos)
            {
                  m_dialog_x = 20;
                  m_dialog_y = 170;
                  m_dialog_sx = 300;
                  m_dialog_sy = 540;
            }

      //Toggle advanced routing overlay display
      m_bShowAdvanced_Routing = !m_bShowAdvanced_Routing;

      if(m_bShowAdvanced_Routing)
            m_pAdvanced_RoutingDialog->Show();
      else
            m_pAdvanced_RoutingDialog->Hide();


      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowAdvanced_Routing );
}

void advanced_routing_pi::OnAdvanced_RoutingDialogClose()
{
      if(m_pAdvanced_RoutingDialog)
            m_pAdvanced_RoutingDialog->Hide();
      SaveConfig();
}

bool advanced_routing_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
      if(m_bShowAdvanced_Routing)
      {
            return true;
      }
      else
            return false;
}

bool advanced_routing_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
  if(m_bShowAdvanced_Routing) {
    m_pAdvanced_RoutingDialog->RenderRouteMap(vp);
    return true;
  } else
    return false;
}


void advanced_routing_pi::SetCursorLatLon(double lat, double lon)
{
      if(m_pAdvanced_RoutingDialog)
      {
            m_pAdvanced_RoutingDialog->SetCursorLatLon(lat, lon);
      }
}


bool advanced_routing_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/PlugIns/AdvancedRouting" ) );


            m_dialog_sx = pConf->Read ( _T ( "DialogSizeX" ), 300L );
            m_dialog_sy = pConf->Read ( _T ( "DialogSizeY" ), 540L );
            m_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 170L );

            m_pAdvanced_RoutingDialog->routemap.degree_step
              = pConf->Read( _T("DegreeStep"), 60L);
            m_pAdvanced_RoutingDialog->routemap.dt
              = wxTimeSpan(0, 0, pConf->Read( _T("TimeSpan"), 1000L), 1);

            return true;
      }
      else
            return false;
}

bool advanced_routing_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/PlugIns/AdvancedRouting" ) );

            pConf->Write ( _T ( "DialogSizeX" ),  m_dialog_sx );
            pConf->Write ( _T ( "DialogSizeY" ),  m_dialog_sy );
            pConf->Write ( _T ( "DialogPosX" ),   m_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_dialog_y );

            pConf->Write( _T("DegreeStep"), m_pAdvanced_RoutingDialog->routemap.degree_step);
            pConf->Write( _T("TimeSpan"),
                          m_pAdvanced_RoutingDialog->routemap.dt.GetSeconds().ToLong());

            return true;
      }
      else
            return false;
}

void advanced_routing_pi::SetColorScheme(PI_ColorScheme cs)
{
      DimeWindow(m_pAdvanced_RoutingDialog);
}
