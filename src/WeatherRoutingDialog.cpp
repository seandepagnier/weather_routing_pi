/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Weather Routing Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier   *
 *   sean@depagnier.com   *
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
 *
 */

#include "wx/wx.h"
#include "wx/tokenzr.h"
#include "wx/datetime.h"
#include "wx/sound.h"
#include <wx/wfstream.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/debug.h>
#include <wx/graphics.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "BoatSpeed.h"
#include "BoatDialog.h"
#include "RouteMap.h"
#include "weather_routing_pi.h"

  /*  User interface:
      Tabs Control
             Routing
                 Button to start computing routing changes to stop, continue and restart
                 Buttons load/save current routing
                 Timeline showing each iso region with data, can render each one and go back
                             rewind, clear before etc..
                             min and max lat/lon, area covered,
                             average speed
                             selectable to display
                                    route with min/max boat-speed,wind-speed,wave-height,
                                                       current-speed,temperature,cloud-cover etc..
                                    

             Configuration
                 Save Load buttons for configuration
                 Timespan
                      In seconds
                         Fixed
                         Variable (is based on the curvature of the isoregion/walls etc..
                 Allow inversions (current overpowering for a time to reverse later (maybe)
                 Rendering Colors/Enabled for
                         Positive/Negative Routes
                         
             Boat
                 List of boats with current boat selected
                 Buttons - New Edit Save Load Clone Simulate
                   New/Edit Dialog Tabs
                     Performance with
                       Polar graph showing boat performance with and without tacking optimizations
                       with each sail plan different color selectable wind speed and thrust level
                       Graph showing boat speed over wind speed and over thrust level with selectable
                       angle of attack.
                       Graph showing apparent wind vs true wind 
                       Setup hull characteristics drag etc...
                       Can precompute these from a few values for actual measured performance
                     Sailing
                       Can add/remove sail configurations from table (reefs different sails etc..)
                       Has rules for when to switch to another configuration from each configuration
                       Always have "none" when sails are down
                     Electric Power
                       Timeline over current route graph
                       Renewable Storage
                         Battery capacity, efficiency leakage etc type etc..
                                  initial charge allow negative value?
                                        with type have coefficents for temp charge discharge tetc..
                         hydrogen tanks capacity efficiency etc..
                         pressurized air tanks  efficiency and capacity
                       
                       Photo Voltaic coefficients for power vs light
                       Hydro Generator coefficients for efficiency, use if
                                               wind speed is > x, boat speed is > x, battery charge
                                               will slow boat down
                       Wind Generator coefficients for efficiency wind-loading etc..
                                               can slow beating and help running
                       Fuel Generator coefficients for efficiency and capacity (will cut on in fuel shortage)
                       Human Generator coefficients for efficiency, human and willingness
                   Other propulsion...

                 Overall boat has 
                   Simulate mode renders the boats sailing various selectable patterns
                   with various winds
                 Interactive mode allows you to sail in realtime
             Goal
                  Save Load goal
                  Start Point1 ... End
                      Lat Lon Time  Reaching a point effectively kills the last iso and restarts from there

             Route Blockers
                  Configure areas to avoid
                  Can draw them, import coastline data etc, have a list of areas
                  Can select how to render them or not
                  Collisions with other vessels ais etc..
                  Rules to prevent certain boat conditions from occuring

                      If (VB VW VA A E B Fuel etc...) (< > = != etc..) 
                         (VB VW VA A E B Fuel etc...) (AND OR 

                         So if the swell is > 7 meters we may choose that 50 < VA < 65 || VA > 120
             Weather/Sea
                  Can import from grib file
                  Selecting either Wind or Current
                     Also allow user to specifyd speed and direction for a given:
                      timespan, and distance from cursor
                      Can render on screen speed and direction at a given time

                  Allow generated weather systems to simulate dynamic weather
                  Can use high and low pressure positions with value and weight
                  to create isobar map and from this generate wind map

                  Can import current data

                  Coefficients to reduce boat speed given swell data
  */


WeatherRoutingDialog::WeatherRoutingDialog( wxWindow *parent, double boat_lat, double boat_lon )
    : WeatherRoutingDialogBase(parent), m_routemap(boat), m_SettingsDialog(this, m_routemap),
      m_thCompute(*parent, m_routemap)
{
    m_SettingsDialog.Hide();

    m_pBoatDialog = new BoatDialog(this, boat);

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    m_tStartLat->SetValue(pConf->Read( _T("StartLat"), wxString::Format(_T("%.5f"), boat_lat)));
    m_tStartLon->SetValue(pConf->Read( _T("StartLon"), wxString::Format(_T("%.5f"), boat_lon)));
    m_tEndLat->SetValue(pConf->Read( _T("EndLat"), wxString::Format(_T("%.5f"), boat_lat+1)));
    m_tEndLon->SetValue(pConf->Read( _T("EndLon"), wxString::Format(_T("%.5f"), boat_lon+1)));

     wxPoint p = GetPosition();
    pConf->Read ( _T ( "DialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "DialogY" ), &p.y, p.y);
    SetPosition(p);

    ReconfigureRouteMap();

    SendPluginMessage(wxString(_T("GRIB_TIMELINE_REQUEST")), _T(""));
    Reset();
}

WeatherRoutingDialog::~WeatherRoutingDialog( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    pConf->Write( _T("StartLat"), m_tStartLat->GetValue());
    pConf->Write( _T("StartLon"), m_tStartLon->GetValue());
    pConf->Write( _T("EndLat"), m_tEndLat->GetValue());
    pConf->Write( _T("EndLon"), m_tEndLon->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "DialogX" ), p.x);
    pConf->Write ( _T ( "DialogY" ), p.y);
}

void WeatherRoutingDialog::RenderRouteMap(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!dc.GetDC()) {
        glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
        glEnable( GL_LINE_SMOOTH );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    }

//    m_thCompute.routemutex.Lock();
    m_routemap.Render(dc, vp);
//    m_thCompute.routemutex.Unlock();

    if(!dc.GetDC())
        glPopAttrib();
}

void WeatherRoutingDialog::OnUpdateEnd( wxCommandEvent& event )
{
    UpdateEnd();
}

void WeatherRoutingDialog::OnCompute ( wxCommandEvent& event )
{
    extern int debugstep;
#if 0
    extern int debugcount;
    debugcount++;
    Reset();
    debugstep = -1;
    for(int i=0; i<13; i++)
        m_routemap.Propagate();
    debugstep = 0;
    m_routemap.Propagate();
#else
    if(m_routemap.origin.size() == 0)
        Reset();

    debugstep = -1;
    for(int i=0; i<1; i++)
        m_routemap.Propagate();
#endif

#if 0
    if(m_thCompute.IsRunning()) {
        m_bCompute->SetLabel(_( "&Compute" ));
        m_thCompute.End();
        m_thCompute.Wait();
    } else {
        m_bCompute->SetLabel(_( "&Stop" ));
        m_thCompute.Run();
    }
#endif

    GetParent()->Refresh();
}

void WeatherRoutingDialog::OnBoat ( wxCommandEvent& event )
{
    m_pBoatDialog->Show(!m_pBoatDialog->IsShown());
}

void WeatherRoutingDialog::OnReset ( wxCommandEvent& event )
{
    Reset();
}

void WeatherRoutingDialog::OnInformation ( wxCommandEvent& event )
{
    InformationDialog dlg(this);
    wxString infolocation = *GetpSharedDataLocation()
        + _("plugins/weather_routing/data/WeatherRoutingInformation.html");
    if(dlg.m_htmlInformation->LoadFile(infolocation))
        dlg.ShowModal();
    else {
        wxMessageDialog mdlg(this, _("Failed to load file:\n") + infolocation,
                             wxString(_("OpenCPN Alert"), wxOK | wxICON_ERROR));
        mdlg.ShowModal();
    }
}

void WeatherRoutingDialog::OnSettings( wxCommandEvent& event )
{
    m_SettingsDialog.LoadSettings();
    if(m_SettingsDialog.ShowModal() == wxID_OK) {
        m_SettingsDialog.ReconfigureRouteMap();
        m_SettingsDialog.SaveSettings();
    }
}

void WeatherRoutingDialog::OnClose( wxCommandEvent& event )
{
    Hide();
}

void WeatherRoutingDialog::Reset()
{
    double startlat, startlon;
    m_tStartLat->GetValue().ToDouble(&startlat);
    m_tStartLon->GetValue().ToDouble(&startlon);

    wxDateTime time = g_GribTimelineTime;

    m_stStartDate->SetLabel(time.FormatISODate());
    m_stStartTime->SetLabel(time.FormatISOTime());

    m_routemap.Reset(startlat, startlon, time);
}

void WeatherRoutingDialog::UpdateEnd()
{
    m_tEndLat->GetValue().ToDouble(&m_routemap.EndLat);
    m_tEndLon->GetValue().ToDouble(&m_routemap.EndLon);
}

void WeatherRoutingDialog::ReconfigureRouteMap()
{
    UpdateEnd();
    m_SettingsDialog.ReconfigureRouteMap();
}

wxMutex routemutex;
void *WeatherRoutingThread::Entry()
{
    int cnt = 0;
    while(!stop) {
        if(cnt < 12) {
            routemutex.Lock();
//            routemap.Propagate(boat);
            routemutex.Unlock();
//            parent.Refresh();
            cnt++;
        }
        wxThread::Yield();
        wxThread::Sleep(2000);
    }
    return 0;
}
