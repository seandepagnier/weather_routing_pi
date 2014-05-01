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
#include <wx/imaglist.h>
#include <wx/progdlg.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "tinyxml/tinyxml.h"

#include "Utilities.h"
#include "Boat.h"
#include "BoatDialog.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"
#include "PlotDialog.h"
#include "AboutDialog.h"

/* XPM */
static const char *eye[]={
"20 20 7 1",
". c none",
"# c #000000",
"a c #333333",
"b c #666666",
"c c #999999",
"d c #cccccc",
"e c #ffffff",
"....................",
"....................",
"....................",
"....................",
".......######.......",
".....#aabccb#a#.....",
"....#deeeddeebcb#...",
"..#aeeeec##aceaec#..",
".#bedaeee####dbcec#.",
"#aeedbdabc###bcceea#",
".#bedad######abcec#.",
"..#be#d######dadb#..",
"...#abac####abba#...",
".....##acbaca##.....",
".......######.......",
"....................",
"....................",
"....................",
"....................",
"...................."};

WeatherRoute::WeatherRoute() : routemapoverlay(new RouteMapOverlay) {}
WeatherRoute::~WeatherRoute() { delete routemapoverlay; }

static int sortcol, sortorder = 1;
// sort callback. Sort by body.
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortWeatherRoutes(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortWeatherRoutes(long item1, long item2, long list)
#endif            
{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;

    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(sortcol);
    
    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(sortcol);
    
    lc->GetItem(it1);
    lc->GetItem(it2);
    
    return sortorder * it1.GetText().Cmp(it2.GetText());
}

WeatherRouting::WeatherRouting(wxWindow *parent, weather_routing_pi &plugin)
    : WeatherRoutingBase(parent), m_ConfigurationDialog(this),
      m_ConfigurationBatchDialog(this), m_SettingsDialog(this),
      m_StatisticsDialog(this), m_ReportDialog(this), m_FilterRoutesDialog(this),
      m_bRunning(false), m_bSkipUpdateCurrentItem(false),
      m_bShowConfiguration(false), m_bShowConfigurationBatch(false),
      m_bShowSettings(false), m_bShowStatistics(false), m_bShowReport(false),
      m_bShowFilter(false), m_weather_routing_pi(plugin)
{
    m_SettingsDialog.LoadSettings();

    m_lPositions->InsertColumn(POSITION_NAME, _("Name"));
    m_lPositions->InsertColumn(POSITION_LAT, _("Lat"));
    m_lPositions->InsertColumn(POSITION_LON, _("Lon"));

    wxImageList *imglist = new wxImageList(20, 20, true, 1);
    imglist->Add(wxBitmap(eye));
    m_lWeatherRoutes->AssignImageList(imglist, wxIMAGE_LIST_SMALL);

    m_lWeatherRoutes->InsertColumn(VISIBLE, _T(""));
    m_lWeatherRoutes->SetColumnWidth(0, 28);

    m_lWeatherRoutes->InsertColumn(START, _("Start"));
    m_lWeatherRoutes->InsertColumn(STARTTIME, _("Start Time"));
    m_lWeatherRoutes->InsertColumn(END, _("End"));
    m_lWeatherRoutes->InsertColumn(TIME, _("Time"));
    m_lWeatherRoutes->InsertColumn(DISTANCE, _("Distance"));
    m_lWeatherRoutes->InsertColumn(AVGSPEED, _("Average Speed"));
    m_lWeatherRoutes->InsertColumn(STATE, _("State"));

    m_default_configuration_path = weather_routing_pi::StandardPath()
        + _T("WeatherRoutingConfiguration.xml");

    if(!OpenXML(m_default_configuration_path, false)) {
        /* create directory for plugin files if it doesn't already exist */
        wxFileName fn(m_default_configuration_path);
        wxFileName fn2 = fn.GetPath();
        if(!fn.DirExists()) {
            fn2.Mkdir();
            fn.Mkdir();
        }
    }

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxPoint p = GetPosition();
    pConf->Read ( _T ( "DialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "DialogY" ), &p.y, p.y);
    SetPosition(p);

    wxSize s = GetSize();
    pConf->Read ( _T ( "DialogWidth" ), &s.x, s.x);
    pConf->Read ( _T ( "DialogHeight" ), &s.y, s.y);
    SetSize(s);

    /* periodically check for updates from computation thread */
    m_tCompute.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRouting::OnComputationTimer ), NULL, this);

    m_tHideConfiguration.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRouting::OnHideConfigurationTimer ), NULL, this);

    SetEnableConfigurationMenu();

    /* initialize crossing land routine from main thread as it is
       not re-entrant */
    PlugIn_GSHHS_CrossesLand(0, 0, 0, 0);
}

WeatherRouting::~WeatherRouting( )
{
    m_SettingsDialog.SaveSettings();

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "DialogX" ), p.x);
    pConf->Write ( _T ( "DialogY" ), p.y);

    wxSize s = GetSize();
    pConf->Write ( _T ( "DialogWidth" ), s.x);
    pConf->Write ( _T ( "DialogHeight" ), s.y);

    SaveXML(m_default_configuration_path);

    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++)
        delete *it;
}

void WeatherRouting::Render(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!dc.GetDC()) {
        glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
        glEnable( GL_LINE_SMOOTH );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    }

    wxDateTime time = m_ConfigurationDialog.m_GribTimelineTime;
    if(!time.IsValid())
        time = wxDateTime::Now();

    RouteMapOverlay *routemapoverlay = CurrentRouteMap();
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        if(weatherroute->routemapoverlay->m_bEndRouteVisible)
            weatherroute->routemapoverlay->Render(time, m_SettingsDialog, dc, vp, true);
    }

    if(routemapoverlay)
        routemapoverlay->Render(time, m_SettingsDialog, dc, vp, false);

    m_ConfigurationBatchDialog.Render(dc, vp);

    if(!dc.GetDC())
        glPopAttrib();
}

void WeatherRouting::UpdateDisplaySettings()
{
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        weatherroute->routemapoverlay->m_UpdateOverlay = true;
    }

    GetParent()->Refresh();
}

void WeatherRouting::AddPosition(double lat, double lon)
{
    wxTextEntryDialog pd( this, _("Enter Name"), _("New Position") );
    if(pd.ShowModal() == wxID_OK) {
        wxString name = pd.GetValue();
        for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
            it != RouteMap::Positions.end(); it++) {
            if((*it).Name == name) {
                wxMessageDialog mdlg(this, _("This name already exists, replace?\n"),
                                     _("Weather Routing"), wxYES | wxNO | wxICON_ERROR);
                if(mdlg.ShowModal() == wxID_YES) {
                    long index = m_lPositions->FindItem(0, name);
                    (*it).lat = lat;
                    (*it).lon = lon;
                    m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
                    m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
                }

                UpdateConfigurations();
                return;
            }
        }
        AddPosition(lat, lon, name);
    }
}

void WeatherRouting::AddPosition(double lat, double lon, wxString name)
{
    RouteMap::Positions.push_back(RouteMapPosition(name, lat, lon));
    UpdateConfigurations();

    wxListItem item;
    long index = m_lPositions->InsertItem(m_lPositions->GetItemCount(), item);
    m_lPositions->SetItem(index, POSITION_NAME, name);
    m_lPositions->SetColumnWidth(POSITION_NAME, wxLIST_AUTOSIZE);
    
    m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
    m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
    
    m_ConfigurationDialog.AddSource(name);
    m_ConfigurationBatchDialog.AddSource(name);
}

void WeatherRouting::OnNewPosition( wxCommandEvent& event )
{
    wxTextEntryDialog pd( this, _("Enter Latitude"), _("New Position") );
    if(pd.ShowModal() == wxID_OK) {
        wxString latitude = pd.GetValue();

        wxTextEntryDialog pd( this, _("Enter Longitude"), _("New Position") );
        if(pd.ShowModal() == wxID_OK) {
            wxString longitude = pd.GetValue();

            double lat, lon;
            latitude.ToDouble(&lat);
            longitude.ToDouble(&lon);
            AddPosition(lat, lon);
        }
    }
}

void WeatherRouting::OnUpdateBoat( wxCommandEvent& event )
{
    double lat = m_weather_routing_pi.m_boat_lat;
    double lon = m_weather_routing_pi.m_boat_lon;

    long index = 0;
    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++, index++)
        if((*it).Name == _("Boat")) {
            m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
            m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));

            (*it).lat = lat, (*it).lon = lon;
            UpdateConfigurations();
            return;
        }

    AddPosition(lat, lon, _("Boat"));
}

#if 0 /* wx widgets is shit, can only allow users
         to edit the first column, so this doesn't work */
void WeatherRouting::OnListLabelEdit( wxListEvent& event )
{
    long index = event.GetIndex();
    int col = event.GetColumn();
    
    long i = 0;
    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++, i++)
        if(i == index) {
            if(col == POSITION_NAME) {
                (*it).Name = event.GetText(); 
            } else {
                double value;
                event.GetText().ToDouble(&value);
                if(col == POSITION_LAT)
                    (*it).lat = value;
                else if(col == POSITION_LON)
                    (*it).lon = value;

                m_lPositions->SetItem(index, col, wxString::Format(_T("%.5f"), value));
                UpdateConfigurations();
            }
        }
}
#endif

void WeatherRouting::OnRemovePosition( wxCommandEvent& event )
{
    long index = m_lPositions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if(index < 0)
        return;

    wxListItem it;
    it.SetId(index);
    it.SetColumn(0);
    m_lPositions->GetItem(it);
    wxString name = it.GetText();

    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++)
        if((*it).Name == name) {
            RouteMap::Positions.erase(it);
            break;
        }

    m_ConfigurationDialog.RemoveSource(name);
    m_ConfigurationBatchDialog.RemoveSource(name);
    m_lPositions->DeleteItem(index);

    UpdateConfigurations();
}

void WeatherRouting::OnClearPositions( wxCommandEvent& event )
{
    RouteMap::Positions.clear();
    m_ConfigurationDialog.ClearSources();
    m_ConfigurationBatchDialog.ClearSources();
    m_lPositions->DeleteAllItems();
}

void WeatherRouting::OnEditConfiguration()
{
    RouteMapOverlay *routemapoverlay = CurrentRouteMap(true);
    if(routemapoverlay) {
        m_ConfigurationDialog.Show();
        /* if boat filename doesn't exist open boat dialog immediately */
        if(!wxFileName::FileExists(routemapoverlay->GetConfiguration().boatFileName))
            m_ConfigurationDialog.EditBoat();
    }
}

void WeatherRouting::OnGoTo( wxCommandEvent& event )
{
    RouteMapOverlay *rmo = CurrentRouteMap(true);
    if(rmo) {
        RouteMapConfiguration config = rmo->GetConfiguration();

        double distance;
        DistanceBearingMercator_Plugin(config.StartLat, config.StartLon,
                                       config.EndLat, config.EndLon,
                                       NULL, &distance);

        JumpToPosition(config.StartLat, config.StartLon, .25/distance);
    }
}

void WeatherRouting::OnDelete( wxCommandEvent& event )
{
    long index = m_lWeatherRoutes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index < 0) return;

    WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
        (wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)));
    DeleteRouteMap(weatherroute->routemapoverlay);

    int cnt = m_lWeatherRoutes->GetItemCount();
    m_lWeatherRoutes->SetItemState(index == cnt ? index - 1 : index,
                                   wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    GetParent()->Refresh();
}

void WeatherRouting::OnDeleteAll( wxCommandEvent& event )
{
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        DeleteRouteMap(weatherroute->routemapoverlay);
    }

    GetParent()->Refresh();
}

void WeatherRouting::OnWeatherRouteSort( wxListEvent& event )
{
    sortcol = event.GetColumn();
    sortorder = -sortorder;
    m_lWeatherRoutes->SortItems(SortWeatherRoutes, (long)m_lWeatherRoutes);
}

void WeatherRouting::OnWeatherRouteSelected( wxListEvent& event )
{
    GetParent()->Refresh();

    RouteMapOverlay *routemapoverlay = CurrentRouteMap();

    if(routemapoverlay) {
        m_tHideConfiguration.Stop();
        m_bSkipUpdateCurrentItem = true;
        m_ConfigurationDialog.SetConfiguration(routemapoverlay->GetConfiguration());
        m_bSkipUpdateCurrentItem = false;
    } else
        m_tHideConfiguration.Start(25, true);

    if(m_StatisticsDialog.IsShown())
        m_StatisticsDialog.SetRouteMapOverlay(routemapoverlay);

    if(m_ReportDialog.IsShown())
        m_ReportDialog.SetRouteMapOverlay(routemapoverlay);

    if(m_ConfigurationBatchDialog.IsShown())
        m_ConfigurationBatchDialog.Reset();

    SetEnableConfigurationMenu();
}

void WeatherRouting::OnWeatherRoutesListLeftDown(wxMouseEvent &event)
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long index = m_lWeatherRoutes->HitTest(pos, flags);
    
    //    Clicking Visibility column?
    if (index > -1 && event.GetX() < m_lWeatherRoutes->GetColumnWidth(0))
    {
        // Process the clicked item
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)));
        weatherroute->routemapoverlay->m_bEndRouteVisible =
            !weatherroute->routemapoverlay->m_bEndRouteVisible;
        UpdateItem(index);
        RequestRefresh( GetParent() );
    }

    // Allow wx to process...
    event.Skip();
}

void WeatherRouting::UpdateComputeState()
{
    m_RoutesToRun = m_WaitingRouteMaps.size();
    m_gProgress->SetRange(m_RoutesToRun);
    m_gProgress->SetValue(0);

    m_bRunning = true;
    m_mCompute->SetText(_( "&Stop" ));
    m_mCompute->Enable();
    m_mComputeAll->Enable(false);
    m_StartTime = wxDateTime::Now();
    m_tCompute.Start(0, true);
}

int debugcnt, debuglimit = 1, debugsize = 2;
void WeatherRouting::OnCompute( wxCommandEvent& event )
{
    if(m_bRunning)
        Stop();
    else {
        Start(CurrentRouteMap());
        UpdateComputeState();
    }
}

void WeatherRouting::OnComputeAll ( wxCommandEvent& event )
{
    /* todo: make this work when already running to start routes not computed,
       and not waiting */
    if(!m_bRunning) {
        StartAll();
        UpdateComputeState();
    }
}

#define FAIL(X) do { error = X; goto failed; } while(0)
void WeatherRouting::OnOpen( wxCommandEvent& event )
{
    wxString error;
    wxFileDialog openDialog
        ( this, _( "Select Configuration" ), _("~"), wxT ( "" ),
          wxT ( "XML files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_OPEN  );

    if( openDialog.ShowModal() == wxID_OK )
        OpenXML(openDialog.GetPath());
}

void WeatherRouting::OnSave( wxCommandEvent& event )
{
    wxString error;
    wxFileDialog saveDialog
        ( this, _( "Select Configuration" ), _("~"), wxT ( "" ),
          wxT ( "XML files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_SAVE  );

    if( saveDialog.ShowModal() == wxID_OK )
        SaveXML(saveDialog.GetPath());
}

void WeatherRouting::OnClose( wxCommandEvent& event )
{
    Hide();
}

void WeatherRouting::OnNew( wxCommandEvent& event )
{
    RouteMapConfiguration configuration;
    if(CurrentRouteMap())
        configuration = CurrentRouteMap()->GetConfiguration();
    else
        configuration = DefaultConfiguration();

#if 0 /* to have a unique name */
    for(int idx = 0;;idx++) {
        wxString name = configuration.Name;
        wxString n1 = name.BeforeLast('-'), n2 = name.AfterLast('-');
        long l;
        if(n1.size() && n2.ToLong(&l))
            name = n1;

        if(idx > 0)
            name += wxString::Format(_T("-%d"), idx);
        for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
            WeatherRoute *weatherroute =
                reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
            RouteMapConfiguration c = weatherroute->routemapoverlay->GetConfiguration();
            if(c.Name == name)
                goto outer_continue;
        }
        configuration.Name = name;
        break;
    outer_continue:;
    }
#endif

    AddConfiguration(configuration);

    m_lWeatherRoutes->SetItemState(m_lWeatherRoutes->GetItemCount() - 1,
                                   wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

    OnEditConfiguration();
}

void WeatherRouting::OnBatch( wxCommandEvent& event )
{
    m_ConfigurationBatchDialog.Reset();
    m_ConfigurationBatchDialog.Show();
}

void WeatherRouting::GenerateBatch()
{
    RouteMapOverlay *routemapoverlay = CurrentRouteMap(true);
    if(!routemapoverlay) {
        wxMessageDialog mdlg(this, _("Must select a configuration to generate from"),
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
        return;
    }

    RouteMapConfiguration configuration = routemapoverlay->GetConfiguration();
    ConfigurationBatchDialog &dlg = m_ConfigurationBatchDialog;

    wxTimeSpan StartSpan, StartSpacingSpan;
    double days, hours;

    dlg.m_tStartDays->GetValue().ToDouble(&days);
    StartSpan = wxTimeSpan::Days(days);

    dlg.m_tStartHours->GetValue().ToDouble(&hours);
    StartSpan += wxTimeSpan::Hours(hours);
        
    dlg.m_tStartSpacingDays->GetValue().ToDouble(&days);
    StartSpacingSpan = wxTimeSpan::Days(days);
        
    dlg.m_tStartSpacingHours->GetValue().ToDouble(&hours);
    StartSpacingSpan += wxTimeSpan::Hours(hours);
        
    wxDateTime EndTime = configuration.StartTime+StartSpan;
    for(; configuration.StartTime <= EndTime; configuration.StartTime += StartSpacingSpan) {
        for(std::vector<BatchSource*>::iterator it = dlg.sources.begin();
            it != dlg.sources.end(); it++) {
            configuration.Start = (*it)->Name;

            for(std::list<BatchDestination*>::iterator it2 = (*it)->destinations.begin();
                it2 != (*it)->destinations.end(); it2++) {
                configuration.End = (*it2)->Name;

                for(unsigned int boatindex = 0; boatindex < dlg.m_lBoats->GetCount(); boatindex++) {
                    configuration.boatFileName = dlg.m_lBoats->GetString(boatindex);
                    AddConfiguration(configuration);
                }
            }
        }
    }
    DeleteRouteMap(routemapoverlay);
}

bool WeatherRouting::Show(bool show)
{
    m_weather_routing_pi.ShowMenuItems(show);

    if(show) {
        m_ConfigurationDialog.Show(m_bShowConfiguration);
        m_ConfigurationBatchDialog.Show(m_bShowConfigurationBatch);
        m_SettingsDialog.Show(m_bShowSettings);
        if(m_bShowStatistics)
            m_StatisticsDialog.SetRouteMapOverlay(CurrentRouteMap());
        m_StatisticsDialog.Show(m_bShowStatistics);
        if(m_bShowReport)
            m_ReportDialog.SetRouteMapOverlay(CurrentRouteMap());
        m_StatisticsDialog.Show(m_bShowStatistics);
        m_FilterRoutesDialog.Show(m_bShowFilter);
    } else {
        m_bShowConfiguration = m_ConfigurationDialog.IsShown();
        m_bShowConfigurationBatch = m_ConfigurationBatchDialog.IsShown();
        m_bShowSettings = m_SettingsDialog.IsShown();
        m_bShowStatistics = m_StatisticsDialog.IsShown();
        m_bShowReport = m_ReportDialog.IsShown();
        m_bShowFilter = m_FilterRoutesDialog.IsShown();
    }

    return WeatherRoutingBase::Show(show);
}

void WeatherRouting::OnFilter( wxCommandEvent& event )
{
    m_FilterRoutesDialog.Show();
}

void WeatherRouting::OnResetAll( wxCommandEvent& event )
{
    Reset();
    UpdateStates();
}

void WeatherRouting::OnExport ( wxCommandEvent& event )
{
    if(CurrentRouteMap(true))
        Export(*CurrentRouteMap());
}

void WeatherRouting::OnExportAll( wxCommandEvent& event )
{
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++)
        Export(*reinterpret_cast<WeatherRoute*>
               (wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)))->routemapoverlay);
}

void WeatherRouting::OnSettings( wxCommandEvent& event )
{
    m_SettingsDialog.Show();
}

void WeatherRouting::OnStatistics( wxCommandEvent& event )
{
    m_StatisticsDialog.SetRouteMapOverlay(CurrentRouteMap());
    m_StatisticsDialog.Show();
}

void WeatherRouting::OnReport( wxCommandEvent& event )
{
    m_ReportDialog.SetRouteMapOverlay(CurrentRouteMap());
    m_ReportDialog.Show();
}

void WeatherRouting::OnPlot ( wxCommandEvent& event )
{
    if(CurrentRouteMap(true)) {
        std::list<PlotData> plotdata = CurrentRouteMap()->GetPlotData();
        PlotDialog plotdialog(this, plotdata);
        plotdialog.ShowModal();
    }
}

void WeatherRouting::OnInformation ( wxCommandEvent& event )
{
    wxString infolocation = *GetpSharedDataLocation()
        + _("plugins/weather_routing_pi/data/WeatherRoutingInformation.html");
    wxLaunchDefaultBrowser(_T("file://") + infolocation);
}

void WeatherRouting::OnAbout ( wxCommandEvent& event )
{
    AboutDialog dlg(GetParent());
    dlg.ShowModal();
}

void WeatherRouting::OnComputationTimer( wxTimerEvent & )
{
    bool update = false;
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); ) {
        RouteMapOverlay *routemapoverlay = *it;
        if(!routemapoverlay->Running()) {
            UpdateRouteMap(routemapoverlay);
            it = m_RunningRouteMaps.erase(it);

            m_gProgress->SetValue(m_RoutesToRun - m_WaitingRouteMaps.size() - m_RunningRouteMaps.size());
            update = true;

            /* update report if needed */
            if(m_ReportDialog.IsShown()) {
                if(routemapoverlay == CurrentRouteMap())
                    m_ReportDialog.SetRouteMapOverlay(routemapoverlay);
            }

            continue;
        } else
            it++;

        /* get a new grib for the route map if needed */
        if(routemapoverlay->NeedsGrib()) {
            m_RouteMapOverlayNeedingGrib = routemapoverlay;
            routemapoverlay->RequestGrib(routemapoverlay->NewTime());
            m_RouteMapOverlayNeedingGrib = NULL;
        }
    }

    if((int)m_RunningRouteMaps.size() < m_SettingsDialog.m_sConcurrentThreads->GetValue()
       && m_WaitingRouteMaps.size()) {
        RouteMapOverlay *routemapoverlay = m_WaitingRouteMaps.front();
        m_WaitingRouteMaps.pop_front();
        routemapoverlay->Start();
        UpdateRouteMap(routemapoverlay);
        m_RunningRouteMaps.push_back(routemapoverlay);
        update = true;
    }
        
    static int cycles; /* don't refresh all the time */
    if(++cycles > 25 && CurrentRouteMap() && CurrentRouteMap()->Updated()) {
        cycles = 0;
        m_StatisticsDialog.SetRunTime(m_RunTime += wxDateTime::Now() - m_StartTime);
        if(m_StatisticsDialog.IsShown())
            m_StatisticsDialog.SetRouteMapOverlay(CurrentRouteMap());

        m_StartTime = wxDateTime::Now();
        GetParent()->Refresh();
    }

    if(update)
        UpdateStates();

    if(m_RunningRouteMaps.size()) {
        /* todo, instead of respawning the funky timer here,
           maybe we can do it from the thread instead to eliminate the delay */
        m_tCompute.Start(25, true);
        return;
    }

    Stop();
}

void WeatherRouting::OnHideConfigurationTimer( wxTimerEvent & )
{
    m_ConfigurationDialog.Hide();
}

bool WeatherRouting::OpenXML(wxString filename, bool reportfailure)
{
    TiXmlDocument doc;
    wxString error;

    wxFileName fn(filename);

    SetTitle(_("Weather Routing") + wxString(_T(" - ")) + fn.GetFullName());

    wxProgressDialog *progressdialog = NULL;
    wxDateTime start = wxDateTime::Now();

    if(!doc.LoadFile(filename.mb_str()))
        FAIL(_("Failed to load file."));
    else {
        TiXmlHandle root(doc.RootElement());

        if(strcmp(root.Element()->Value(), "OpenCPNWeatherRoutingConfiguration"))
            FAIL(_("Invalid xml file"));

        RouteMap::Positions.clear();

        int count = 0;
        for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement())
            count++;
    
        int i=0;
        for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement(), i++) {
            if(progressdialog) {
                if(!progressdialog->Update(i))
                    return true;
            } else {
                wxDateTime now = wxDateTime::Now();
                /* if it's going to take more than a half second, show a progress dialog */
                if((now-start).GetMilliseconds() > 250 && i < count/2) {
                    progressdialog = new wxProgressDialog(
                        _("Load"), _("Weather Routing"), count, this,
                        wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
                }
            }
        
            if(!strcmp(e->Value(), "Position")) {
                wxString name = wxString::FromUTF8(e->Attribute("Name"));
                double lat = AttributeDouble(e, "Latitude", NAN);
                double lon = AttributeDouble(e, "Longitude", NAN);
            
                for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
                    it != RouteMap::Positions.end(); it++) {
                    if((*it).Name == name) {
                        static bool warnonce = true;
                        if(warnonce) {
                            warnonce = false;
                            wxMessageDialog mdlg(this, _("File contains duplicate position name, discaring\n"),
                                                 _("Weather Routing"), wxOK | wxICON_WARNING);
                            mdlg.ShowModal();
                        }
                    
                        goto skipadd;
                    }
                }
                AddPosition(lat, lon, name);
            
            skipadd:;
            } else
                if(!strcmp(e->Value(), "Configuration")) {
                    RouteMapConfiguration configuration;
                    configuration.Start = wxString::FromUTF8(e->Attribute("Start"));
                    wxDateTime date;
                    date.ParseDate(wxString::FromUTF8(e->Attribute("StartDate")));
                    wxDateTime time;
                    time.ParseTime(wxString::FromUTF8(e->Attribute("StartTime")));
                    if(date.IsValid()) {
                        if(time.IsValid()) {
                            date.SetHour(time.GetHour());
                            date.SetMinute(time.GetMinute());
                            date.SetSecond(time.GetSecond());
                        }
                        configuration.StartTime = date;
                    } else
                        configuration.StartTime = wxDateTime::Now();
            
                    configuration.End = wxString::FromUTF8(e->Attribute("End"));
                    configuration.dt = AttributeDouble(e, "dt", 0);
            
                    configuration.boatFileName = wxString::FromUTF8(e->Attribute("Boat"));
            
                    configuration.MaxDivertedCourse = AttributeDouble(e, "MaxDivertedCourse", 180);
                    configuration.MaxSearchAngle = AttributeDouble(e, "MaxSearchAngle", 180);
                    configuration.MaxWindKnots = AttributeDouble(e, "MaxWindKnots", 100);
                    configuration.MaxSwellMeters = AttributeDouble(e, "MaxSwellMeters", 20);

                    configuration.MaxLatitude = AttributeDouble(e, "MaxLatitude", 90);
                    configuration.MaxTacks = AttributeDouble(e, "MaxTacks", -1);
                    configuration.TackingTime = AttributeDouble(e, "TackingTime", 0);
                    configuration.MaxUpwindPercentage = AttributeDouble(e, "MaxUpwindPercentage", 100);

                    configuration.AvoidCycloneTracks = AttributeBool(e, "AvoidCycloneTracks", false);
                    configuration.CycloneMonths = AttributeInt(e, "CycloneMonths", 3);
                    configuration.CycloneDays = AttributeInt(e, "CycloneDays", 0);
                    configuration.CycloneWindSpeed = AttributeInt(e, "CycloneWindSpeed", 50);
                    configuration.CycloneClimatologyStartYear = AttributeInt
                        (e, "CycloneClimatologyStartYear", 1985);
                    
                    configuration.UseGrib = AttributeBool(e, "UseGrib", true);
                    configuration.UseClimatology = AttributeBool(e, "UseClimatology", true);
                    configuration.AllowDataDeficient = AttributeBool(e, "AllowDataDeficient", false);
                    configuration.DetectLand = AttributeBool(e, "DetectLand", true);
                    configuration.Currents = AttributeBool(e, "Currents", true);
                    configuration.InvertedRegions = AttributeBool(e, "InvertedRegions", false);
                    configuration.Anchoring = AttributeBool(e, "Anchoring", false);
            
                    wxString degreesteps = wxString::FromUTF8(e->Attribute("DegreeSteps"));
                    while(degreesteps.size()) {
                        double step;
                        if(degreesteps.BeforeFirst(';').ToDouble(&step))
                            configuration.DegreeSteps.push_back(step);
                        degreesteps = degreesteps.AfterFirst(';');
                    }
            
                    AddConfiguration(configuration);
                } else
                    FAIL(_("Unrecognized xml node"));
        }
    }
    delete progressdialog;
    return true;
failed:

    if(reportfailure) {
        wxMessageDialog mdlg(this, error, _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
    return false;
}

void WeatherRouting::SaveXML(wxString filename)
{
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * root = new TiXmlElement( "OpenCPNWeatherRoutingConfiguration" );
    doc.LinkEndChild( root );

    char version[24];
    sprintf(version, "%d.%d", PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR);
    root->SetAttribute("version", version);
    root->SetAttribute("creator", "Opencpn Weather Routing plugin");

    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++) {
        TiXmlElement *c = new TiXmlElement( "Position" );

        c->SetAttribute("Name", (*it).Name.mb_str());
        c->SetAttribute("Latitude", wxString::Format(_T("%.5f"), (*it).lat).mb_str());
        c->SetAttribute("Longitude", wxString::Format(_T("%.5f"), (*it).lon).mb_str());

        root->LinkEndChild(c);
    }

    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++) {
        TiXmlElement *c = new TiXmlElement( "Configuration" );

        RouteMapConfiguration configuration =
            (*it)->routemapoverlay->GetConfiguration();

        c->SetAttribute("Start", configuration.Start.mb_str());
        c->SetAttribute("StartDate", configuration.StartTime.FormatDate().mb_str());
        c->SetAttribute("StartTime", configuration.StartTime.FormatTime().mb_str());
        c->SetAttribute("End", configuration.End.mb_str());
        c->SetAttribute("dt", configuration.dt);

        c->SetAttribute("Boat", configuration.boatFileName.ToUTF8());

        c->SetAttribute("MaxDivertedCourse", configuration.MaxDivertedCourse);
        c->SetAttribute("MaxSearchAngle", configuration.MaxSearchAngle);
        c->SetAttribute("MaxWindKnots", configuration.MaxWindKnots);
        c->SetAttribute("MaxSwellMeters", configuration.MaxSwellMeters);

        c->SetAttribute("MaxLatitude", configuration.MaxLatitude);
        c->SetAttribute("MaxTacks", configuration.MaxTacks);
        c->SetAttribute("TackingTime", configuration.TackingTime);
        c->SetAttribute("MaxUpwindPercentage", configuration.MaxUpwindPercentage);

        c->SetAttribute("AvoidCycloneTracks", configuration.AvoidCycloneTracks);
        c->SetAttribute("CycloneMonths", configuration.CycloneMonths);
        c->SetAttribute("CycloneDays", configuration.CycloneDays);
        c->SetAttribute("CycloneWindSpeed", configuration.CycloneWindSpeed);
        c->SetAttribute("CycloneClimatologyStartYear", configuration.CycloneClimatologyStartYear);

        c->SetAttribute("UseGrib", configuration.UseGrib);
        c->SetAttribute("UseClimatology", configuration.UseClimatology);
        c->SetAttribute("AllowDataDeficient", configuration.AllowDataDeficient);
        c->SetAttribute("DetectLand", configuration.DetectLand);
        c->SetAttribute("Currents", configuration.Currents);
        c->SetAttribute("InvertedRegions", configuration.InvertedRegions);
        c->SetAttribute("Anchoring", configuration.Anchoring);

        wxString degreesteps;
        for(std::list<double>::iterator it = configuration.DegreeSteps.begin();
            it != configuration.DegreeSteps.end(); it++)
            degreesteps += wxString::Format(_T("%.1f;"), *it);
        c->SetAttribute("DegreeSteps", degreesteps.mb_str());
        
        root->LinkEndChild(c);
    }

    if(!doc.SaveFile(filename.mb_str())) {
        wxMessageDialog mdlg(this, _("Failed to save xml file: ") + filename,
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void WeatherRouting::SetEnableConfigurationMenu()
{
    bool current = CurrentRouteMap();
    m_mBatch->Enable(current);
    m_mEdit->Enable(current);
    m_mGoTo->Enable(current);
    m_mDelete->Enable(current);
    m_mCompute->Enable(current);
    m_mExport->Enable(current);

    bool cnt = m_lWeatherRoutes->GetItemCount();
    m_mDeleteAll->Enable(cnt);
    m_mComputeAll->Enable(cnt);
    m_mExportAll->Enable(cnt);
}

void WeatherRouting::UpdateConfigurations()
{
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));

        /* get and set configuration to update start/end positions */
        RouteMapConfiguration c = weatherroute->routemapoverlay->GetConfiguration();
        weatherroute->routemapoverlay->SetConfiguration(c);

        weatherroute->Update(true);
        UpdateItem(i);
    }
}

void WeatherRouting::AddConfiguration(RouteMapConfiguration configuration)
{
    WeatherRoute *weatherroute = new WeatherRoute;
    RouteMapOverlay *routemapoverlay = weatherroute->routemapoverlay;
    routemapoverlay->SetConfiguration(configuration);
    routemapoverlay->Reset();
    weatherroute->Update();

    m_WeatherRoutes.push_back(weatherroute);

    wxListItem item;
    item.SetData(weatherroute);
    UpdateItem(m_lWeatherRoutes->InsertItem(item));

    m_mDeleteAll->Enable();
    m_mComputeAll->Enable();
    m_mExportAll->Enable();
}

void WeatherRouting::UpdateRouteMap(RouteMapOverlay *routemapoverlay)
{
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        if(weatherroute->routemapoverlay == routemapoverlay) {
            weatherroute->Update();
            UpdateItem(i);
            return;
        }
    }
}

void WeatherRoute::Update(bool stateonly)
{
    if(!stateonly) {
        RouteMapConfiguration configuration = routemapoverlay->GetConfiguration();

        BoatFilename = configuration.boatFileName;
        Start = configuration.Start;
        StartTime = configuration.StartTime.Format(_T("%x %H:%M"));
        End = configuration.End;
    }
        
    wxDateTime enddate = routemapoverlay->EndDate();
    if(enddate.IsValid()) {
        wxTimeSpan span = enddate - routemapoverlay->StartTime();
        int days = span.GetDays();
        span -= wxTimeSpan::Days(days);
        int hours = span.GetHours();
        span -= wxTimeSpan::Hours(hours);
        int minutes = (double)span.GetSeconds().ToLong()/60.0;
        
        Time = (days ? wxString::Format(_T("%dd "), days) : _T("")) +
            wxString::Format(_T("%02d:%02d"), hours, minutes);
    } else
        Time = _("N/A");

    RouteMapConfiguration c = routemapoverlay->GetConfiguration();

    Distance =  wxString::Format
        (_T("%.0f/%.0f"), routemapoverlay->RouteInfo(RouteMapOverlay::DISTANCE),
         DistGreatCircle_Plugin(c.StartLat, c.StartLon, c.EndLat, c.EndLon));
    
    AvgSpeed = wxString::Format
        (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::AVGSPEED));
    
    if(!routemapoverlay->Valid())
        State = _("Invalid Start/End");
    else
    if(routemapoverlay->Running())
        State = _("Computing...");
    else {
        if(routemapoverlay->Finished()) {
            if(routemapoverlay->ReachedDestination())
                State = _("Complete");
            else {
                State = _T("");
                if(routemapoverlay->GribFailed()) {
                    State = _("Grib");
                    State += _T(" ");
                }
                if(routemapoverlay->ClimatologyFailed()) {
                    State = _("Climatology");
                    State += _T(" ");
                }
                if(routemapoverlay->NoData()) {
                    State = _("No Data");
                    State += _T(" ");
                }
                State += _("Failed");
            }
        } else {
            State = _("Not Computed");
        }
    }
}

void WeatherRouting::UpdateItem(long index)
{
    WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
        (wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)));

    m_lWeatherRoutes->SetItemImage(index, weatherroute->routemapoverlay->m_bEndRouteVisible ? 0 : -1);

    m_lWeatherRoutes->SetItem(index, START, weatherroute->Start);
    m_lWeatherRoutes->SetColumnWidth(START, wxLIST_AUTOSIZE);

    m_lWeatherRoutes->SetItem(index, STARTTIME, weatherroute->StartTime);
    m_lWeatherRoutes->SetColumnWidth(STARTTIME, wxLIST_AUTOSIZE);

    m_lWeatherRoutes->SetItem(index, END, weatherroute->End);
    m_lWeatherRoutes->SetColumnWidth(END, wxLIST_AUTOSIZE);

    m_lWeatherRoutes->SetItem(index, TIME, weatherroute->Time);
    m_lWeatherRoutes->SetColumnWidth(TIME, wxLIST_AUTOSIZE);

    m_lWeatherRoutes->SetItem(index, DISTANCE, weatherroute->Distance);
    m_lWeatherRoutes->SetItem(index, AVGSPEED, weatherroute->AvgSpeed);
    m_lWeatherRoutes->SetItem(index, STATE, weatherroute->State);

    m_lWeatherRoutes->SetColumnWidth(STATE, wxLIST_AUTOSIZE);
}

void WeatherRouting::SetConfigurationRoute(RouteMapConfiguration configuration,
                                           WeatherRoute *weatherroute)
{
    if(m_bSkipUpdateCurrentItem)
        return;

    RouteMapOverlay *rmo = weatherroute->routemapoverlay;
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++)
        if(*it == rmo)
            (*it)->Stop();

    rmo->SetConfiguration(configuration);

    weatherroute->Update();

    for(long index = 0; index<m_lWeatherRoutes->GetItemCount(); index++) {
        WeatherRoute *wr = reinterpret_cast<WeatherRoute*>
            (wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)));
        if(weatherroute == wr) {
            UpdateItem(index);
            break;
        }
    }
}

void WeatherRouting::SetConfigurationCurrentRoute(RouteMapConfiguration configuration)
{
    long index = m_lWeatherRoutes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index < 0) return;

    WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
        (wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)));

    SetConfigurationRoute(configuration, weatherroute);
}

void WeatherRouting::UpdateBoatFilename(RouteMapConfiguration configuration)
{
    for(long index = 0; index<m_lWeatherRoutes->GetItemCount(); index++) {
        WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
            (wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)));

        RouteMapConfiguration c = weatherroute->routemapoverlay->GetConfiguration();
        if(c.boatFileName == configuration.boatFileName)
            SetConfigurationRoute(c, weatherroute);
    }
}

void WeatherRouting::UpdateStates()
{
    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++)
        (*it)->Update(true);
    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++)
        UpdateItem(i);
}

RouteMapOverlay *WeatherRouting::CurrentRouteMap(bool messagedialog)
{
    long index = m_lWeatherRoutes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    RouteMapOverlay *routemapoverlay = NULL;
    if (index >= 0)
        routemapoverlay = reinterpret_cast<WeatherRoute*>
            (wxUIntToPtr(m_lWeatherRoutes->GetItemData(index)))->routemapoverlay;

    if(messagedialog && !routemapoverlay) {
        wxMessageDialog mdlg(this, _("No Weather Route selected"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }

    return routemapoverlay;
}

void WeatherRouting::RebuildList()
{
    m_lWeatherRoutes->DeleteAllItems();
    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++) {
        if(!(*it)->Filtered) {
            wxListItem item;
            item.SetData(*it);
            UpdateItem(m_lWeatherRoutes->InsertItem(item));
        }
    }
}

void WeatherRouting::Export(RouteMapOverlay &routemapoverlay)
{
    std::list<PlotData> plotdata = routemapoverlay.GetPlotData();

    if(plotdata.size() == 0) {
        wxMessageDialog mdlg(this, _("Empty Route, nothing to export\n"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
        return;
    }

    PlugIn_Track* newTrack = new PlugIn_Track;
    newTrack->m_NameString = _("Weather Route");

    for(std::list<PlotData>::iterator it = plotdata.begin(); it != plotdata.end(); it++) {
        PlugIn_Waypoint*  newPoint = new PlugIn_Waypoint
            ((*it).lat, (*it).lon, _T("circle"), _("Weather Route Point"));

        newPoint->m_CreateTime = (*it).time;
        newTrack->pWaypointList->Append(newPoint);
    }

    AddPlugInTrack(newTrack);

    GetParent()->Refresh();
}

void WeatherRouting::Start(RouteMapOverlay *routemapoverlay)
{
    if(!routemapoverlay ||
       (routemapoverlay->Finished() &&
        !routemapoverlay->GribFailed() &&
        !routemapoverlay->ClimatologyFailed()))
        return;

    wxString error = routemapoverlay->Reset();
    if(error.empty())
        m_WaitingRouteMaps.push_back(routemapoverlay);
    else {
        wxMessageDialog mdlg(this, error,
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}


void WeatherRouting::StartAll()
{
    m_StatisticsDialog.SetRunTime(m_RunTime = wxTimeSpan(0));

    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        RouteMapOverlay *routemapoverlay =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)))->routemapoverlay;
        Start(routemapoverlay);
    }
}

void WeatherRouting::Stop()
{
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++) {
        (*it)->Stop();
        UpdateRouteMap(*it);
    }

    m_RunningRouteMaps.clear();
    m_WaitingRouteMaps.clear();

    m_gProgress->SetValue(0);
    m_bRunning = false;
    m_mCompute->SetText(wxString(_( "&Compute" )) + wxT('\t') + wxT("Ctrl+C"));
    SetEnableConfigurationMenu();
    m_StatisticsDialog.SetRunTime(m_RunTime += wxDateTime::Now() - m_StartTime);
}

void WeatherRouting::Reset()
{
    if(m_bRunning)
        Stop();

    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        weatherroute->routemapoverlay->Reset();
    }

    GetParent()->Refresh();
}

void WeatherRouting::DeleteRouteMap(RouteMapOverlay *routemapoverlay)
{
    for(std::list<RouteMapOverlay*>::iterator it = m_WaitingRouteMaps.begin();
        it != m_WaitingRouteMaps.end(); it++)
        if(*it == routemapoverlay) {
            m_WaitingRouteMaps.erase(it);
            break;
        }

    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++)
        if(*it == routemapoverlay) {
            m_RunningRouteMaps.erase(it);
            break;
        }

    for(int i=0; i<m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_lWeatherRoutes->GetItemData(i)));
        if(weatherroute->routemapoverlay == routemapoverlay) {
            m_lWeatherRoutes->DeleteItem(i);
            break;
        }
    }

    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++)
        if((*it)->routemapoverlay == routemapoverlay) {
            delete *it;
            m_WeatherRoutes.erase(it);
            break;
        }

    SetEnableConfigurationMenu();
}

RouteMapConfiguration WeatherRouting::DefaultConfiguration()
{
    RouteMapConfiguration configuration;

    if(RouteMap::Positions.size() >= 1) {
        RouteMapPosition &p = *RouteMap::Positions.begin();
        configuration.Start = p.Name;
        configuration.StartLat = p.lat, configuration.StartLon = p.lon;
    } else
        configuration.StartLat = 0, configuration.StartLon = 0;

    configuration.StartTime = wxDateTime::Now();
    configuration.dt = 3600;

    if(RouteMap::Positions.size() >= 2) {
        RouteMapPosition &p = *(++RouteMap::Positions.begin());
        configuration.End = p.Name;
        configuration.EndLat = p.lat, configuration.EndLon = p.lon;
    } else
        configuration.EndLat = 0, configuration.EndLon = 0;
    
    configuration.boatFileName = weather_routing_pi::StandardPath() + _T("Boat.xml");
    
    configuration.MaxDivertedCourse = 180;
    configuration.MaxSearchAngle = 180;
    configuration.MaxWindKnots = 100;
    configuration.MaxSwellMeters = 20;

    configuration.MaxLatitude = 90;
    configuration.MaxTacks = -1;
    configuration.TackingTime = 0;
    configuration.MaxUpwindPercentage = 100;
    
    configuration.AvoidCycloneTracks = false;
    configuration.CycloneMonths = 3;
    configuration.CycloneDays = 0;
    configuration.CycloneWindSpeed = 50;
    configuration.CycloneClimatologyStartYear = 1985;

    configuration.UseGrib = configuration.UseClimatology = true;
    configuration.AllowDataDeficient = false;
    configuration.DetectLand = true;
    configuration.Currents = false;
    configuration.InvertedRegions = false;
    configuration.Anchoring = false;
        
    for(double d=45; d<=170; d+=5) {
        configuration.DegreeSteps.push_back(d);
        configuration.DegreeSteps.push_back(-d);
    }

    return configuration;
}
