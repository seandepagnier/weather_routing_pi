/***************************************************************************
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
 *
 */

#include <wx/wx.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Utilities.h"
#include "Boat.h"
#include "BoatDialog.h"
#include "ConfigurationBatchDialog.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"
#include "PlotDialog.h"
#include "AboutDialog.h"

WeatherRouting::WeatherRouting(wxWindow *parent, weather_routing_pi &plugin)
    : WeatherRoutingBase(parent), m_ConfigurationDialog(this, plugin), m_SettingsDialog(this)
{
    m_ConfigurationDialog.Hide();

    m_SettingsDialog.LoadSettings();
    SetRouteMapOverlaySettings();

    m_SettingsDialog.Hide();

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );


    wxPoint p = GetPosition();
    pConf->Read ( _T ( "DialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "DialogY" ), &p.y, p.y);
    SetPosition(p);

    /* periodically check for updates from computation thread */
    m_tCompute.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRouting::OnComputationTimer ), NULL, this);
}

WeatherRouting::~WeatherRouting( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "DialogX" ), p.x);
    pConf->Write ( _T ( "DialogY" ), p.y);
}

void WeatherRouting::RenderRouteMap(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!dc.GetDC()) {
        glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
        glEnable( GL_LINE_SMOOTH );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    }

    m_RouteMapOverlay.Render(m_ConfigurationDialog.m_GribTimelineTime, dc, vp);

    if(!dc.GetDC())
        glPopAttrib();
}

void WeatherRouting::OnConfiguration()
{
    m_ConfigurationDialog->ReadConfiguration(CurrentRouteMap()->GetConfiguration());
    m_ConfigurationDialog->Show();
}

void WeatherRouting::OnWeatherRouteSelected( wxListEvent& event )
{
    GetParent()->Refresh();
    m_ConfigurationDialog->ReadConfiguration(...);
}

int debugcnt, debuglimit = 1, debugsize = 2;
void WeatherRouting::OnCompute ( wxCommandEvent& event )
{
    if(m_RouteMapOverlay.Running())
        Stop();
    else
        Start();
}

#define FAIL(X) do { error = X; goto failed; } while(0)
void WeatherRouting::OnOpen( wxCommandEvent& event )
{
    wxString error;
    wxFileDialog openDialog
        ( this, _( "Select Configuration" ), _("~"), wxT ( "" ),
          wxT ( "XML files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_OPEN  );

    if( openDialog.ShowModal() != wxID_OK )
        return;

    wxFileName filename = openDialog.GetPath();
    TiXmlDocument doc;
    if(!doc.LoadFile(filename.GetPath().mb_str()))
        FAIL(_("Failed to load file."));
    else {
        TiXmlHandle root( doc.RootElement() );
        if(strcmp(doc.RootElement()->Value(), "OCPNWeatherRoutingConfiguration"))
            FAIL(_("Invalid xml file"));

        for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement()) {
            if(!strcmp(e->Value(), "Configuration")) {
                RouteMapConfiguration configuration;
                configuration.Name = wxString::FromUTF8(e->Attribute("Name"));
                configuration.StartLat = AttributeDouble(e, "StartLat", 0);
                configuration.StartLon = AttributeDouble(e, "StartLon", 0);
                configuration.EndLat = AttributeDouble(e, "EndLat", 0);
                configuration.EndLon = AttributeDouble(e, "EndLon", 0);
                configuration.dt = AttributeDouble(e, "dt", 0);
                
                configuration.boatFileName = wxString::FromUTF8(e->Attribute("Boat"));
                
                configuration.MaxDivertedCourse = AttributeDouble(e, "MaxDivertedCourse", 180);
                configuration.MaxWindKnots = AttributeDouble(e, "MaxWindKnots", 100);
                configuration.MaxSwellMeters = AttributeDouble(e, "MaxSwellMeters", 20);
                configuration.MaxLatitude = AttributeDouble(e, "MaxLatitude", 90);
                configuration.MaxTacks = AttributeDouble(e, "MaxTacks", -1);
                configuration.TackingTime = AttributeDouble(e, "TackingTime", 0);
                
                configuration.UseGrib = AttributeBool(e, "UseGrib", true);
                configuration.UseClimatology = AttributeBool(e, "UseClimatology", true);
                configuration.AllowDataDeficient = AttributeBool(e, "AllowDataDeficient", false);
                configuration.DetectLand = AttributeBool(e, "DetectLand", true);
                configuration.Currents = AttributeBool(e, "Currents", true);
                configuration.InvertedRegions = AttributeBool(e, "InvertedRegions", false);
                configuration.Anchoring = AttributeBool(e, "Anchoring", false);
                
                AddConfiguration(configuration);
            } else
                FAIL(_("Unrecognized xml node"));
        }
    }

    return;
failed:

    wxMessageDialog mdlg(this, error, _("Weather Routing"), wxOK | wxICON_ERROR);
    mdlg.ShowModal();
}

void WeatherRouting::OnSave( wxCommandEvent& event )
{
    wxString error;
    wxFileDialog saveDialog
        ( this, _( "Select Configuration" ), _("~"), wxT ( "" ),
          wxT ( "XML files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_SAVE  );

    if( saveDialog.ShowModal() != wxID_OK )
        return;

    wxString filename = saveDialog.GetPath();

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * root = new TiXmlElement( "OCPNWeatherRoutingConfiguration" );
    doc.LinkEndChild( root );

    char version[24];
    sprintf(version, "%d.%d", PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR);
    root->SetAttribute("version", version);
    root->SetAttribute("creator", "Opencpn Weather Routing plugin");

    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        TiXmlElement *c = new TiXmlElement( "configuration" );

        RouteMapConfiguration configuration =
            reinterpret_cast<RouteMap*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)))
            ->GetConfiguration();

        c->SetAttribute("Name", configuration.Name.mb_str());
        c->SetAttribute("StartLat", configuration.StartLat);
        c->SetAttribute("StartLon", configuration.StartLon);
        c->SetAttribute("EndLat", configuration.EndLat);
        c->SetAttribute("EndLon", configuration.EndLon);
        c->SetAttribute("dt", configuration.dt);

        c->SetAttribute("Boat", configuration.boatFileName.ToUTF8());

        c->SetAttribute("MaxDivertedCourse", configuration.MaxDivertedCourse);
        c->SetAttribute("MaxWindKnots", configuration.MaxWindKnots);
        c->SetAttribute("MaxSwellMeters", configuration.MaxSwellMeters);
        c->SetAttribute("MaxLatitude", configuration.MaxLatitude);
        c->SetAttribute("MaxTacks", configuration.MaxTacks);
        c->SetAttribute("TackingTime", configuration.TackingTime);

        c->SetAttribute("UseGrib", configuration.UseGrib);
        c->SetAttribute("UseClimatology", configuration.UseClimatology);
        c->SetAttribute("AllowDataDeficient", configuration.AllowDataDeficient);
        c->SetAttribute("DetectLand", configuration.DetectLand);
        c->SetAttribute("Currents", configuration.Currents);
        c->SetAttribute("InvertedRegions", configuration.InvertedRegions);
        c->SetAttribute("Anchoring", configuration.Anchoring);
        
        root->LinkEndChild(c);
    }

    if(!doc.SaveFile(filename.mb_str())) {
        wxMessageDialog mdlg(this, _("Failed to save xml file: ") + filename,
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void WeatherRouting::OnClose( wxCommandEvent& event )
{
    Hide();
}

void WeatherRouting::OnNew( wxCommandEvent& event )
{
    RouteMapConfiguration configuration = ReadConfiguration();
    AddConfiguration(configuration);
}

void WeatherRouting::OnBatch( wxCommandEvent& event )
{
    ConfigurationBatchDialog dlg(this, CurrentRouteMap()->GetConfiguration());
    dlg.ShowModal();
}

void WeatherRouting::OnExport ( wxCommandEvent& event )
{
    Export(CurrentRouteMap());
}

void WeatherRouting::OnDelete( wxCommandEvent& event )
{
    long index = m_lWeatherRoutes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index < 0) return;

    RouteMap *routemap = m_lWeatherRoutes->ItemData(index);
    delete routemap;
    m_lWeatherRoutes->DeleteItem(index);
}

void WeatherRouting::OnFilter( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("Unimplemented"),
                         _("OpenCPN Alert"), wxOK);
    mdlg.ShowModal();
}

void WeatherRouting::OnReset ( wxCommandEvent& event )
{
    Reset();
}

void WeatherRouting::OnExportAll( wxCommandEvent& event )
{
    for(int i=0; i<m_lWeatherRoutes->GetCount(); i++)
        Export(reinterpret_cast<RouteMap*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i))));
}

void WeatherRouting::OnDeleteAll( wxCommandEvent& event )
{
    for(int i=0; i<m_lWeatherRoutes->GetCount(); i++) {
        RouteMap *routemap =
            reinterpret_cast<RouteMap*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        delete routemap;
    }
    m_lWeatherRoutes->DeleteAllItems();
}

void WeatherRouting::OnSettings( wxCommandEvent& event )
{
    m_SettingsDialog.LoadSettings();

    if(m_SettingsDialog.ShowModal() == wxID_OK) {
        m_SettingsDialog.SaveSettings();
        SetRouteMapOverlaySettings();
        m_RouteMapOverlay.m_UpdateOverlay = true;
        GetParent()->Refresh();
    }
}

void WeatherRouting::OnStatistics( wxCommandEvent& event )
{
    m_StatisticsDialog->Show();
}

void WeatherRouting::OnPlot ( wxCommandEvent& event )
{
    std::list<PlotData> plotdata = m_RouteMapOverlay.GetPlotData();
    PlotDialog plotdialog(this, plotdata);
    plotdialog.ShowModal();
}

void WeatherRouting::OnInformation ( wxCommandEvent& event )
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

void WeatherRouting::OnAbout ( wxCommandEvent& event )
{
    AboutDialog dlg(this);
    dlg.ShowModal();
}

void WeatherRouting::OnComputationTimer( wxTimerEvent & )
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

    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++) {
        RouteMapOverlay *routemapoverlay = *it;
        if(!routemapoverlay.Running()) {
            m_RunningRouteMaps.erase(it);
            continue;
        }
        

        /* get a new grib for the route map if needed */
        if(routemapoverlay.NeedsGrib()) {
            routemapoverlayNeedingGrib = routemapoverlay;
            routemapoverlay.RequestGrib(routemapoverlay.NewTime());
            routemapoverlayNeedingGrib = NULL;
#if 0
            if(!routemapoverlay.HasGrib() && !routemapoverlay.HasClimatology()) {
                wxMessageDialog mdlg(this, _("Failed to obtain grib for timestep,\
and no climatology data to fall back on\n"),
                                     _("Weather Routing"), wxOK);
                mdlg.ShowModal();
                Stop();
                return;
            }
#endif
        }

        if( routemapoverlay.ReachedDestination()) {
#if 0
            wxMessageDialog mdlg(this, _("Computation completed, destination reached\n"),
                                 _("Weather Routing"), wxOK);
            mdlg.ShowModal();
#endif
#if 0
        wxDateTime enddate = routemapoverlay.EndDate();
        if(enddate.IsValid()) {
            m_stEndDate->SetLabel(enddate.FormatDate());
            m_stEndHour->SetLabel(enddate.Format(_T("%H")));

            wxTimeSpan span = enddate - routemapoverlay.StartTime();
            int days = span.GetDays();
            span -= wxTimeSpan::Days(days);
            int hours = span.GetHours();
            span -= wxTimeSpan::Hours(hours);
            double minutes = (double)span.GetSeconds().ToLong()/60.0;

            double distance, avgspeed, percentage_upwind;
            routemapoverlay.RouteInfo(distance, avgspeed, percentage_upwind);
            m_stEndStats->SetLabel(wxString::Format(_T("%dd %02d:%05.2f "),
                                                    days, hours, minutes) +
                                   wxString::Format(_T("%.0f nm, %.1f avg knts, %.0f%% upwind"),
                                                    distance, avgspeed, percentage_upwind));
        }
#endif
    } else {
        wxString addmsg;
        if(routemapoverlay.GribFailed())
            addmsg += _("Grib Data Failed to contain required information\n");

        RouteMapConfiguration configuration = routemapoverlay.GetConfiguration();
        if(!configuration.UseGrib && !configuration.UseClimatology)
            addmsg += _("No Data source configured!\n");

#if 0
        wxMessageDialog mdlg(this, _("Computation completed, destination not reached.\n")
                             + addmsg, _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
#endif
    }
    }

    if(m_RunningRouteMaps.size() < m_SettingsDialog->m_sConcurrentThreads->GetValue()
       && m_WaitingRouteMaps.size()) {
        RouteMapOverlay *routemapoverlay = m_WaitingRouteMaps.pop_front();
        routemapoverlay->Start();
        m_RunningRouteMaps.push_back(routemapoverlay);
    }
        
    static int cycles; /* don't refresh all the time */
    if(++cycles > 25 && routemapoverlay.Updated()) {
        cycles = 0;
        
        m_RunTime += wxDateTime::Now() - m_StartTime;
        m_StartTime = wxDateTime::Now();
            
        GetParent()->Refresh();
    }

    if(m_RunningRouteMaps.size()) {
        m_tCompute.Start(100, true);
        return;
    }

    Stop();
}

RouteMapOverlay *WeatherRouting::CurrentRouteMap()
{
    long index = m_lWeatherRoutes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index < 0) return;

    return reinterpret_cast<RouteMapOverlay*>(wxUIntToPtr(m_lWeatherRoutes->ItemData(index)));
}

void WeatherRouting::Export(const RouteMap &routemap)
{
    std::list<PlotData> plotdata = routemap.GetPlotData();

    if(plotdata.size() == 0) {
        wxMessageDialog mdlg(this, _("Empty Route, nothing to export\n"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
        return;
    }

    PlugIn_Track* newTrack = new PlugIn_Track;
    newTrack->m_NameString = _("Weather Route");

//    RouteMapConfiguration configuration = routemap.GetConfiguration();
//    PlugIn_Waypoint* newPoint = new PlugIn_Waypoint
//        (configuration.StartLat, configuration.StartLon, _T("circle"), _("Weather Route Start"));
//    newPoint->m_CreateTime = routemap.StartTime();
//    newTrack->pWaypointList->Append(newPoint);

    for(std::list<PlotData>::iterator it = plotdata.begin(); it != plotdata.end(); it++) {
        PlugIn_Waypoint*  newPoint = new PlugIn_Waypoint
            ((*it).lat, (*it).lon, _T("circle"), _("Weather Route Point"));

        newPoint->m_CreateTime = (*it).time;
        newTrack->pWaypointList->Append(newPoint);
    }

    AddPlugInTrack(newTrack);

    GetParent()->Refresh();
}

void WeatherRouting::Start()
{
    Reset();

    for(int i=0; i<m_lWeatherRoutes->GetCount(); i++) {
        RouteMapOverlay *routemapoverlay =
            reinterpret_cast<RouteMapOverlay*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        m_WaitingRouteMaps.push_back(routemapoverlay);
    }

    m_bCompute->SetLabel(_( "&Stop" ));
    m_StartTime = wxDateTime::Now();
    m_tCompute.Start(100, true);
}

void WeatherRouting::Stop()
{
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++)
        (*it)->Stop();

    m_RunningRouteMaps.Clear();
    m_WaitingRouteMaps.Clear();

    m_bCompute->SetLabel(_( "&Start" ));
    m_RunTime += wxDateTime::Now() - m_StartTime;
}

void WeatherRouting::Reset()
{
    if(m_RouteMapOverlay.Running())
        return;

    m_RunTime = wxTimeSpan(0);
    GetParent()->Refresh();
}

void WeatherRouting::SetRouteMapOverlaySettings()
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
