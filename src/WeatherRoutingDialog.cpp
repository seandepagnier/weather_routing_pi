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
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRoutingDialog.h"

WeatherRoutingDialog::WeatherRoutingDialog( wxWindow *parent, double boat_lat, double boat_lon )
    : WeatherRoutingDialogBase(parent), m_SettingsDialog(this)
{
    m_SettingsDialog.Hide();
    m_pBoatDialog = new BoatDialog(this);

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

    m_RouteMapOverlay.Run();
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

    m_RouteMapOverlay.Delete();
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

    m_RouteMapOverlay.Render(dc, vp);

    if(!dc.GetDC())
        glPopAttrib();
}

void WeatherRoutingDialog::OnUpdateEnd( wxCommandEvent& event )
{
    UpdateEnd();
}

void WeatherRoutingDialog::OnCompute ( wxCommandEvent& event )
{
    if(m_RouteMapOverlay.Paused()) {
        m_bCompute->SetLabel(_( "&Stop" ));
        if(m_RouteMapOverlay.Empty())
            Reset();
    } else
        m_bCompute->SetLabel(_( "&Start" ));

    m_RouteMapOverlay.TogglePaused();
}

void WeatherRoutingDialog::OnBoat ( wxCommandEvent& event )
{
    m_pBoatDialog->Show(!m_pBoatDialog->IsShown());
//    if(m_pBoatDialog->ShowModal() == wxID_OK) {
//    }
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
    if(m_RouteMapOverlay.Paused())
        return;

    /* get a new grib for the route map if needed */
    if(m_RouteMapOverlay.NeedsGrib()) {
        wxDateTime time = m_RouteMapOverlay.NewGribTime();
        wxJSONValue v;
        v[_T("Day")] = time.GetDay();
        v[_T("Month")] = time.GetMonth();
        v[_T("Year")] = time.GetYear();
        v[_T("Hour")] = time.GetHour();
        v[_T("Minute")] = time.GetMinute();
        v[_T("Second")] = time.GetSecond();
    
        wxJSONWriter w;
        wxString out;
        w.Write(v, out);
        SendPluginMessage(wxString(_T("GRIB_TIMELINE_RECORD_REQUEST")), out);

        m_RouteMapOverlay.Lock();
        if(!m_RouteMapOverlay.HasGrib()) {
            wxMessageDialog mdlg(this, _("Failed to obtain grib for timestep\n"),
                                 _("Weather Routing"), wxOK);
            mdlg.ShowModal();
        }
        m_RouteMapOverlay.Unlock();
    }

    static int cycles; /* don't refresh all the time */
    if(++cycles > 20 && m_RouteMapOverlay.Updated()) {
        cycles = 0;
        GetParent()->Refresh();
    }

    if(!m_RouteMapOverlay.Finished())
        return;

    m_bCompute->SetLabel(_( "&Start" ));

    m_RouteMapOverlay.Pause();

    if( m_RouteMapOverlay.ReachedDestination()) {
        wxMessageDialog mdlg(this, _("Computation completed, destination reached\n"),
                             _("Weather Routing"), wxOK);
        mdlg.ShowModal();
    } else {
        wxString gribfailmsg = _("Grib Data Failed to contain required information\n");
        wxMessageDialog mdlg(this, _("Computation completed, destination not reached.\n")
                             + (m_RouteMapOverlay.GribFailed() ? gribfailmsg : _T("")),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }
}

void WeatherRoutingDialog::Reset()
{
    double startlat, startlon;
    m_tStartLat->GetValue().ToDouble(&startlat);
    m_tStartLon->GetValue().ToDouble(&startlon);

    wxDateTime time = m_RouteMapOverlay.m_GribTimelineTime;

    m_stStartDate->SetLabel(time.FormatISODate());
    m_stStartTime->SetLabel(time.FormatISOTime());

    m_RouteMapOverlay.SetBoat(m_pBoatDialog->m_Boat);
    
    m_RouteMapOverlay.Reset(startlat, startlon, time);
    GetParent()->Refresh();
}

void WeatherRoutingDialog::UpdateEnd()
{
    RouteMapOptions options = m_RouteMapOverlay.GetOptions();
    m_tEndLat->GetValue().ToDouble(&options.EndLat);
    m_tEndLon->GetValue().ToDouble(&options.EndLon);
    m_RouteMapOverlay.SetOptions(options);
}

void WeatherRoutingDialog::ReconfigureRouteMap()
{
    UpdateEnd();
    RouteMapOptions options = m_RouteMapOverlay.GetOptions();
    m_SettingsDialog.UpdateOptions(options);
    m_RouteMapOverlay.SetOptions(options);
}
