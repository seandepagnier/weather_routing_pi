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

#include <wx/wx.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Utilities.h"
#include "Boat.h"
#include "BoatDialog.h"
#include "StatisticsDialog.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRoutingDialog.h"
#include "PlotDialog.h"

WeatherRoutingDialog::WeatherRoutingDialog( wxWindow *parent, double boat_lat, double boat_lon )
    : WeatherRoutingDialogBase(parent), m_ConfigurationDialog(this), m_SettingsDialog(this)
{
    m_ConfigurationDialog.Hide();

    m_SettingsDialog.LoadSettings();
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

    SetRouteMapOverlaySettings();
    ReconfigureRouteMap();

    /* periodically check for updates from computation thread */
    m_tCompute.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRoutingDialog::OnComputationTimer ), NULL, this);
    m_tCompute.Start(100);
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

    m_RouteMapOverlay.Render(dc, vp);

    if(!dc.GetDC())
        glPopAttrib();
}

void WeatherRoutingDialog::OnUpdateEnd( wxCommandEvent& event )
{
    UpdateEnd();
}

int debugcnt, debuglimit = 107, debugsize = 5;
void WeatherRoutingDialog::OnCompute ( wxCommandEvent& event )
{
    if(m_RouteMapOverlay.Running()) {
        Stop();
#if 0
    debugcnt = 0;
    debuglimit++;
    Start();
#endif

    } else
        Start();
}

void WeatherRoutingDialog::OnBoat ( wxCommandEvent& event )
{
    m_pBoatDialog->Show(!m_pBoatDialog->IsShown());
}

void WeatherRoutingDialog::OnPlot ( wxCommandEvent& event )
{
    std::list<PlotData> plotdata = m_RouteMapOverlay.GetPlotData();
    PlotDialog plotdialog(this, plotdata);
    plotdialog.ShowModal();
}

#ifdef __MSVC__
void WeatherRoutingDialog::OnExport ( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("export function not supported for visual studio compiler on windows.\n"),
                         _("Weather Routing"), wxOK | wxICON_WARNING);
    mdlg.ShowModal();
}

#else

#include "../../../include/chart1.h"
#include "../../../include/chcanv.h"
#include "../../../include/chartdb.h"
#include "../../../include/navutil.h"
#include "../../../include/routeprop.h"
#include "../../../include/routemanagerdialog.h"

extern RouteList        *pRouteList;
extern MyConfig         *pConfig;
extern RouteProp        *pRoutePropDialog;
extern RouteManagerDialog *pRouteManagerDialog;

void WeatherRoutingDialog::OnExport ( wxCommandEvent& event )
{
    std::list<PlotData> plotdata = m_RouteMapOverlay.GetPlotData();

    if(plotdata.size() == 0) {
        wxMessageDialog mdlg(this, _("Empty Route, nothing to export\n"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
        return;
    }

    Track* newTrack = new Track;
    newTrack->m_RouteNameString = _("Weather Route");

    for(std::list<PlotData>::iterator it = plotdata.begin(); it != plotdata.end(); it++) {
        RoutePoint* newPoint = new RoutePoint
            ((*it).lat, (*it).lon, _T("circle"), _("Weather Route Point"));
        newPoint->m_CreateTime = (*it).time;
        newPoint->m_bIsolatedMark = false;
        newPoint->m_bIsVisible = true;
        newPoint->m_bShowName = false;
        newPoint->m_bKeepXRoute = false;
        newPoint->m_bIsInRoute = false;
        newPoint->m_bIsInTrack = true;

        newTrack->AddPoint(newPoint);
    }

    pRouteList->Append( newTrack );
    pConfig->AddNewRoute( newTrack );
    newTrack->RebuildGUIDList(); // ensure the GUID list is intact and good

    if( pRoutePropDialog && ( pRoutePropDialog->IsShown() ) ) {
        pRoutePropDialog->SetRouteAndUpdate( newTrack );
        pRoutePropDialog->UpdateProperties();
    }

    if( pRouteManagerDialog && pRouteManagerDialog->IsShown() )
        pRouteManagerDialog->UpdateRouteListCtrl();
}
#endif

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

void WeatherRoutingDialog::OnStatistics( wxCommandEvent& event )
{
    StatisticsDialog dlg(this, m_RouteMapOverlay, m_RunTime);
    dlg.ShowModal();
}

void WeatherRoutingDialog::OnConfiguration( wxCommandEvent& event )
{
    m_ConfigurationDialog.Load();
    if(m_ConfigurationDialog.ShowModal() == wxID_OK) {
        ReconfigureRouteMap();
        m_ConfigurationDialog.Save();
    }
}

void WeatherRoutingDialog::OnSettings( wxCommandEvent& event )
{
    m_SettingsDialog.LoadSettings();

    if(m_SettingsDialog.ShowModal() == wxID_OK) {
        m_SettingsDialog.SaveSettings();
        SetRouteMapOverlaySettings();
        GetParent()->Refresh();
    }
}

void WeatherRoutingDialog::OnComputationTimer( wxTimerEvent & )
{
#if 1
    /* complete hack to make window size right.. not sure why we can't call fit earlier,
       but it doesn't work */
    static int fitstartup = 2;
    if(IsShown() && fitstartup) {
        Fit();
        m_SettingsDialog.Fit();
        fitstartup--;
    }
#endif

    if(!m_RouteMapOverlay.Running())
        return;

    /* get a new grib for the route map if needed */
    if(m_RouteMapOverlay.NeedsGrib()) {
        m_RouteMapOverlay.RequestGrib(m_RouteMapOverlay.NewGribTime());

        if(!m_RouteMapOverlay.HasGrib()) {
            wxMessageDialog mdlg(this, _("Failed to obtain grib for timestep\n"),
                                 _("Weather Routing"), wxOK);
            mdlg.ShowModal();
            Stop();
            return;
        }
    }

    static int cycles; /* don't refresh all the time */
    if(++cycles > 10 && m_RouteMapOverlay.Updated()) {
        cycles = 0;

        m_RunTime += wxDateTime::Now() - m_StartTime;
        m_StartTime = wxDateTime::Now();

        GetParent()->Refresh();
    }

    if(!m_RouteMapOverlay.Finished())
        return;

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

    Stop();
}

void WeatherRoutingDialog::Start()
{
    Reset();
    m_RouteMapOverlay.Start();
        
    m_bCompute->SetLabel(_( "&Stop" ));
    m_bReset->Disable();
    m_StartTime = wxDateTime::Now();
}

void WeatherRoutingDialog::Stop()
{
    m_RouteMapOverlay.Stop();

    m_bCompute->SetLabel(_( "&Start" ));
    m_bReset->Enable();
    m_RunTime += wxDateTime::Now() - m_StartTime;
}

void WeatherRoutingDialog::Reset()
{
    if(m_RouteMapOverlay.Running())
        return;

    RouteMapOptions options = m_RouteMapOverlay.GetOptions();
    m_tStartLat->GetValue().ToDouble(&options.StartLat);
    m_tStartLon->GetValue().ToDouble(&options.StartLon);
    options.boat = m_pBoatDialog->m_Boat;
    m_RouteMapOverlay.SetOptions(options);

    wxDateTime time = m_RouteMapOverlay.m_GribTimelineTime;

    m_stStartDate->SetLabel(time.FormatISODate());
    m_stStartTime->SetLabel(time.FormatISOTime());
    
    m_RouteMapOverlay.Reset(time);

    m_RunTime = wxTimeSpan(0);
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
    m_ConfigurationDialog.UpdateOptions(options);
    m_RouteMapOverlay.SetOptions(options);
}

void WeatherRoutingDialog::SetRouteMapOverlaySettings()
{
    m_RouteMapOverlay.SetSettings(
        m_SettingsDialog.m_cpCursorRoute->GetColour(),
        m_SettingsDialog.m_cpDestinationRoute->GetColour(),
        m_SettingsDialog.m_sRouteThickness->GetValue(),
        m_SettingsDialog.m_sIsoChronThickness->GetValue(),
        m_SettingsDialog.m_sAlternateRouteThickness->GetValue(),
        m_SettingsDialog.m_cbAlternatesForAll->GetValue(),
        m_SettingsDialog.m_cbSquaresAtSailChanges->GetValue());
}
