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
#include "wx/stdpaths.h"

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
      m_thCompute(m_routemap)
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

    m_default_boat_path = stdPath + wxFileName::GetPathSeparator() + _T("boat.obs");
    boat.OpenBinary(m_default_boat_path.ToAscii());

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

    /* periodically check for updates from computation thread */
    m_tCompute.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRoutingDialog::OnComputationTimer ), NULL, this);
    m_tCompute.Start(100);

    m_thCompute.computemutex.Lock();
    m_thCompute.gribmutex.Lock();
    m_thCompute.Run();
}

WeatherRoutingDialog::~WeatherRoutingDialog( )
{
    boat.SaveBinary(m_default_boat_path.ToAscii());

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    pConf->Write( _T("StartLat"), m_tStartLat->GetValue());
    pConf->Write( _T("StartLon"), m_tStartLon->GetValue());
    pConf->Write( _T("EndLat"), m_tEndLat->GetValue());
    pConf->Write( _T("EndLon"), m_tEndLon->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "DialogX" ), p.x);
    pConf->Write ( _T ( "DialogY" ), p.y);

    if(!m_thCompute.computing)
        m_thCompute.computemutex.Unlock();
    m_thCompute.gribmutex.Unlock();
    m_thCompute.Delete();
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

    m_thCompute.routemutex.Lock();
    m_routemap.Render(dc, vp);
    m_thCompute.routemutex.Unlock();

    if(!dc.GetDC())
        glPopAttrib();
}

void WeatherRoutingDialog::OnUpdateEnd( wxCommandEvent& event )
{
    UpdateEnd();
}

void WeatherRoutingDialog::OnCompute ( wxCommandEvent& event )
{
    if(m_thCompute.computing) {
        m_bCompute->SetLabel(_( "&Start" ));
        m_thCompute.computemutex.Lock();
        m_thCompute.computing = false;
    } else {
        m_bCompute->SetLabel(_( "&Stop" ));
        if(m_routemap.origin.size() == 0)
            Reset();
        m_thCompute.computing = true;
        m_thCompute.computemutex.Unlock();
    }

    GetParent()->Refresh();
}

void WeatherRoutingDialog::OnBoat ( wxCommandEvent& event )
{
    m_pBoatDialog->Show(!m_pBoatDialog->IsShown());
//    m_thCompute.routemutex.Lock();
//    m_pBoatDialog->ShowModal();
//    m_thCompute.routemutex.Unlock();
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
                             _("OpenCPN Alert"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void WeatherRoutingDialog::OnSettings( wxCommandEvent& event )
{
    m_SettingsDialog.LoadSettings();
    if(m_SettingsDialog.ShowModal() == wxID_OK) {
        ReconfigureRouteMap();
        m_SettingsDialog.SaveSettings();
    }
}

void WeatherRoutingDialog::OnClose( wxCommandEvent& event )
{
    Hide();
}

void WeatherRoutingDialog::OnComputationTimer( wxTimerEvent & )
{
    if(m_thCompute.needgrib) {
        if(m_routemap.origin.size() == 0) {
            wxMessageDialog mdlg(this, _("routemap should not be empty here"),
                             _("Weather Routing"), wxOK);
            mdlg.ShowModal();
            return;
        }

        g_GribRecordTime = m_routemap.origin.back()->time + m_routemap.dt;
        g_GribRecord = NULL; /* invalidate */
        {
            wxJSONValue v;
            v[_T("Day")] = g_GribRecordTime.GetDay();
            v[_T("Month")] = g_GribRecordTime.GetMonth();
            v[_T("Year")] = g_GribRecordTime.GetYear();
            v[_T("Hour")] = g_GribRecordTime.GetHour();
            v[_T("Minute")] = g_GribRecordTime.GetMinute();
            v[_T("Second")] = g_GribRecordTime.GetSecond();
    
            wxJSONWriter w;
            wxString out;
            w.Write(v, out);
            SendPluginMessage(wxString(_T("GRIB_TIMELINE_RECORD_REQUEST")), out);
        }
        m_thCompute.gribmutex.Unlock();
        wxThread::Sleep(10);
        m_thCompute.gribmutex.Lock();
    }

    if(m_thCompute.Updated())
        GetParent()->Refresh();

    if(!m_thCompute.computing)
        return;

    if(!m_routemap.m_bFinished)
        return;

    m_thCompute.computemutex.Lock();
    m_thCompute.computing = false;

    m_bCompute->SetLabel(_( "&Start" ));

    if(m_routemap.m_bReachedDestination) {
        wxMessageDialog mdlg(this, _("Computation completed, destination reached\n"),
                             _("Weather Routing"), wxOK);
        mdlg.ShowModal();
    } else {
        wxString gribfailmsg = _("Failed to get grib data at this time step\n");
        wxMessageDialog mdlg(this, _("Computation completed, destination not reached.\n")
                             + (m_routemap.m_bGribFailed ? gribfailmsg : _T("")),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }
}

void WeatherRoutingDialog::Reset()
{
    double startlat, startlon;
    m_tStartLat->GetValue().ToDouble(&startlat);
    m_tStartLon->GetValue().ToDouble(&startlon);

    wxDateTime time = g_GribTimelineTime;

    m_stStartDate->SetLabel(time.FormatISODate());
    m_stStartTime->SetLabel(time.FormatISOTime());
    
    if(m_thCompute.computing) {
        m_thCompute.computemutex.Lock();
        m_thCompute.computing = false;
        m_bCompute->SetLabel(_( "&Start" ));
    }

    m_thCompute.routemutex.Lock();
    m_routemap.Reset(startlat, startlon, time);
    m_thCompute.routemutex.Unlock();
    GetParent()->Refresh();
}

void WeatherRoutingDialog::UpdateEnd()
{
    m_thCompute.routemutex.Lock();
    m_tEndLat->GetValue().ToDouble(&m_routemap.EndLat);
    m_tEndLon->GetValue().ToDouble(&m_routemap.EndLon);
    m_thCompute.routemutex.Unlock();
}

void WeatherRoutingDialog::ReconfigureRouteMap()
{
    UpdateEnd();
    m_thCompute.routemutex.Lock();
    m_SettingsDialog.ReconfigureRouteMap();
    m_thCompute.routemutex.Unlock();
}

void *WeatherRoutingThread::Entry()
{
    while(!TestDestroy()) {

        computemutex.Lock();

        needgrib = true;
        gribmutex.Lock();
        gribmutex.Unlock();
        needgrib = false;

        routemutex.Lock();
        routemap.Propagate();
        updated = true;
        routemutex.Unlock();

        computemutex.Unlock();

        wxThread::Sleep(100);
    }
    return 0;
}

bool WeatherRoutingThread::Updated()
{
    bool ret = updated;
    updated = false;
    return ret;
}
