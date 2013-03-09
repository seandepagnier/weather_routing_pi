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

            Demo
                  Loads or Saves all possible settings from a file.  Select
                  demos in the lsit.
                  Can create a demo.  Demos have a sequence so whenever you
                  do something it is recorded as an event in the table.  Certain
                  events have automatic delays when replayed

  */


WeatherRoutingDialog::WeatherRoutingDialog( wxWindow *parent, double boat_lat, double boat_lon )
    : WeatherRoutingDialogBase(parent), routemap(boat),
      m_thCompute(*parent, routemap)
{
    m_pBoatDialog = new BoatDialog(this, boat);

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    m_tStartLat->SetValue(pConf->Read( _T("StartLat"), wxString::Format(_T("%.5f"), boat_lat)));
    m_tStartLon->SetValue(pConf->Read( _T("StartLon"), wxString::Format(_T("%.5f"), boat_lon)));
    m_tEndLat->SetValue(pConf->Read( _T("EndLat"), wxString::Format(_T("%.5f"), boat_lat+1)));
    m_tEndLon->SetValue(pConf->Read( _T("EndLon"), wxString::Format(_T("%.5f"), boat_lon+1)));

    int degreestep;
    pConf->Read( _T("DegreeStep"), &degreestep, 15);
    m_sDegreeStep->SetValue(degreestep);

    int timespan;
    pConf->Read( _T("TimeSpan"), &timespan, 1000);
    m_sTimeStep->SetValue(timespan);

    int maxdivertedcourse;
    pConf->Read( _T("MaxDivertedCourse"), &maxdivertedcourse, 100);
    m_sMaxDivertedCourse->SetValue(maxdivertedcourse);

    int maxwindknots;
    pConf->Read( _T("MaxWindKnots"), &maxwindknots, 100);
    m_sMaxWindKnots->SetValue(maxwindknots);

    int maxswellmeters;
    pConf->Read( _T("MaxSwellMeters"), &maxswellmeters, 20);
    m_sMaxSwellMeters->SetValue(maxswellmeters);

    int maxlatitude;
    pConf->Read( _T("MaxLatitude"), &maxlatitude, 90);
    m_sMaxLatitude->SetValue(maxlatitude);

    int tackingtime;
    pConf->Read( _T("TackingTime"), &tackingtime, 10);
    m_sTackingTime->SetValue(tackingtime);

    int substeps;
    pConf->Read( _T("SubSteps"), &substeps, 90);
    m_sSubSteps->SetValue(substeps);

    bool detectland;
    pConf->Read( _T("DetectLand"), &detectland, true);
    m_cbDetectLand->SetValue(detectland);

    bool invertedregions;
    pConf->Read( _T("InvertedRegions"), &invertedregions, false);
    m_cbInvertedRegions->SetValue(invertedregions);

    bool anchoring;
    pConf->Read( _T("Anchoring"), &anchoring, false);
    m_cbAnchoring->SetValue(anchoring);

    bool allowdatadeficient;
    pConf->Read( _T("AllowDataDeficient"), &allowdatadeficient, false);
    m_cbAllowDataDeficient->SetValue(allowdatadeficient);

    wxPoint p = GetPosition();
    pConf->Read ( _T ( "DialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "DialogY" ), &p.y, p.y);
    SetPosition(p);

    GetGribDialog();
    ReconfigureRouteMap();
}

WeatherRoutingDialog::~WeatherRoutingDialog( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    pConf->Write( _T("StartLat"), m_tStartLat->GetValue());
    pConf->Write( _T("StartLon"), m_tStartLon->GetValue());
    pConf->Write( _T("EndLat"), m_tEndLat->GetValue());
    pConf->Write( _T("EndLon"), m_tEndLon->GetValue());

    pConf->Write( _T("DegreeStep"), m_sDegreeStep->GetValue());
    pConf->Write( _T("TimeSpan"), m_sTimeStep->GetValue());
    pConf->Write( _T("MaxDivertedCourse"), m_sMaxDivertedCourse->GetValue());
    pConf->Write( _T("MaxWindKnots"), m_sMaxWindKnots->GetValue());
    pConf->Write( _T("MaxSwellMeters"), m_sMaxSwellMeters->GetValue());
    pConf->Write( _T("MaxLatittude"), m_sMaxLatitude->GetValue());
    pConf->Write( _T("TackingTime"), m_sTackingTime->GetValue());
    pConf->Write( _T("SubSteps"), m_sSubSteps->GetValue());

    pConf->Write( _T("DetectLand"), m_cbDetectLand->GetValue());
    pConf->Write( _T("InvertedRegions"), m_cbInvertedRegions->GetValue());
    pConf->Write( _T("Anchoring"), m_cbAnchoring->GetValue());
    pConf->Write( _T("AllowDataDeficient"), m_cbAllowDataDeficient->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "DialogX" ), p.x);
    pConf->Write ( _T ( "DialogY" ), p.y);
}

void WeatherRoutingDialog::RenderRouteMap(PlugIn_ViewPort *vp)
{
//    m_thCompute.routemutex.Lock();
    routemap.Render(vp);
//    m_thCompute.routemutex.Unlock();
}

extern GRIBUIDialog *g_pGribDialog;

bool WeatherRoutingDialog::GetGribDialog()
{
#if 0
    return true;
#endif
    if(!g_pGribDialog) {
        SendPluginMessage(wxString(_T("GRIB_DIALOG_REQUEST")), _T(""));

        if(g_pGribDialog)
            Reset();
        else {
            wxMessageDialog mdlg(this, _("Failed to get handle to Grib Dialog, is it loaded?\n"),
                                 wxString(_("OpenCPN Alert"), wxOK | wxICON_ERROR));
            mdlg.ShowModal();
            return false;
        }
    }
    return true;
}

void WeatherRoutingDialog::OnCompute ( wxCommandEvent& event )
{
    if(!GetGribDialog())
        return;

    ReconfigureRouteMap();

    extern int debugstep, debugcount;
#if 0
    debugcount++;
    Reset();
    debugstep = -1;
    routemap.Propagate();
    routemap.Propagate();
    debugstep = 0;
    routemap.Propagate();
#else
    debugstep = -1;
    routemap.Propagate();
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

extern void RouteDebugStep();

void WeatherRoutingDialog::OnStep ( wxCommandEvent& event )
{
    RouteDebugStep();
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

void WeatherRoutingDialog::Reset()
{
    if(!GetGribDialog())
        return;

    double startlat, startlon;
    m_tStartLat->GetValue().ToDouble(&startlat);
    m_tStartLon->GetValue().ToDouble(&startlon);

    wxDateTime time = g_pGribDialog ? g_pGribDialog->TimelineTime() : wxDateTime::Now();
    routemap.Reset(startlat, startlon, time);
}

void WeatherRoutingDialog::ReconfigureRouteMap()
{
      m_tEndLat->GetValue().ToDouble(&routemap.EndLat);
      m_tEndLon->GetValue().ToDouble(&routemap.EndLat);

      routemap.dt = wxTimeSpan(0, 0, m_sTimeStep->GetValue());
      routemap.DegreeStep = m_sDegreeStep->GetValue();
      routemap.MaxDivertedCourse = m_sMaxDivertedCourse->GetValue();
      routemap.MaxWindKnots= m_sMaxWindKnots->GetValue();
      routemap.MaxSwellMeters = m_sMaxSwellMeters->GetValue();
      routemap.MaxLatitude = m_sMaxLatitude->GetValue();
      routemap.TackingTime = m_sTackingTime->GetValue();
      routemap.SubSteps = m_sSubSteps->GetValue();

      routemap.DetectLand = m_cbDetectLand->GetValue();
      routemap.InvertedRegions = m_cbInvertedRegions->GetValue();
      routemap.Anchoring = m_cbAnchoring->GetValue();
      routemap.AllowDataDeficient = m_cbAllowDataDeficient->GetValue();
}

extern wxMutex routemutex;
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
