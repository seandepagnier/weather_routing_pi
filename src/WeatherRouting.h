/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
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

#ifndef _WEATHER_ROUTING_H_
#define _WEATHER_ROUTING_H_


#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"
#include "ConfigurationDialog.h"
#include "ConfigurationBatchDialog.h"
#include "BoatDialog.h"
#include "SettingsDialog.h"
#include "StatisticsDialog.h"
#include "ReportDialog.h"
#include "PlotDialog.h"
#include "FilterRoutesDialog.h"

class weather_routing_pi;
class WeatherRouting;

class WeatherRoute
{
public:
    WeatherRoute();
    ~WeatherRoute();
    void Update(WeatherRouting *wr, bool stateonly=false);

    bool Filtered;
    wxString BoatFilename, Start, StartTime, End, EndTime, Time, Distance,
        AvgSpeed, MaxSpeed, AvgSpeedGround, MaxSpeedGround, AvgWind, MaxWind,
        AvgCurrent, MaxCurrent, AvgSwell, MaxSwell, UpwindPercentage, PortStarboard,
        Tacks, State;
    RouteMapOverlay *routemapoverlay;
};

static const wxString column_names[] = {_T(""), _("Start"), _("Start Time"),
                                        _("End"), _("End Time"), _("Time"), _("Distance"),
                                        _("Avg Speed"), _("Max Speed"),
                                        _("Avg Speed Ground"), _("Max Speed Ground"),
                                        _("Avg Wind"), _("Max Wind"),
                                        _("Avg Current"), _("Max Current"),
                                        _("Avg Swell"), _("Max Swell"),
                                        _("Upwind Percentage"),
                                        _("Port Starboard"), _("Tacks"), _("State")};

class WeatherRouting : public WeatherRoutingBase
{
public:
    enum {POSITION_NAME=0, POSITION_LAT, POSITION_LON};

    enum {VISIBLE=0, START, STARTTIME, END, ENDTIME, TIME, DISTANCE, AVGSPEED, MAXSPEED,
          AVGSPEEDGROUND, MAXSPEEDGROUND,
          AVGWIND, MAXWIND, AVGCURRENT, MAXCURRENT, AVGSWELL, MAXSWELL,
          UPWIND_PERCENTAGE, PORT_STARBOARD, TACKS, STATE, NUM_COLS};
    long columns[NUM_COLS];

    WeatherRouting(wxWindow *parent, weather_routing_pi &plugin);
    ~WeatherRouting();

    void Reset();

    void Render(wrDC &dc, PlugIn_ViewPort &vp);
    ConfigurationDialog m_ConfigurationDialog;
    ConfigurationBatchDialog m_ConfigurationBatchDialog;
    CursorPositionDialog m_CursorPositionDialog;
    BoatDialog m_BoatDialog;

    void SetConfigurationRoute(WeatherRoute *weatherroute);
    void UpdateBoatFilename(wxString boatFileName);

    void UpdateCurrentConfigurations();
    void UpdateStates();
    std::list<RouteMapOverlay*> CurrentRouteMaps(bool messagedialog = false);
    RouteMapOverlay *FirstCurrentRouteMap();
    RouteMapOverlay *m_RouteMapOverlayNeedingGrib;

    void RebuildList();
    std::list<RouteMapOverlay*> m_RunningRouteMaps, m_WaitingRouteMaps;
    std::list<WeatherRoute*> m_WeatherRoutes;

    void GenerateBatch();
    bool Show(bool show);

    void UpdateDisplaySettings();

    void AddPosition(double lat, double lon);
    void AddPosition(double lat, double lon, wxString name);

    void CursorRouteChanged();
    void UpdateColumns();

    void UpdateCursorPositionDialog();

    SettingsDialog m_SettingsDialog;

private:

    void OnNewPosition( wxCommandEvent& event );
    void OnUpdateBoat( wxCommandEvent& event );
    void OnDeletePosition( wxCommandEvent& event );
    void OnDeleteAllPositions( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event ) { Hide(); }
    void OnPositionKeyDown( wxListEvent& event );
    void OnEditConfiguration();
    void OnOpen( wxCommandEvent& event );
    void OnSave( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event );
    void OnNew( wxCommandEvent& event );
    void OnEditConfigurationClick( wxMouseEvent& event ) { OnEditConfiguration(); }
    void OnWeatherRouteSort( wxListEvent& event );
    void OnWeatherRouteSelected();
    void OnWeatherRouteSelected( wxListEvent& event ) { OnWeatherRouteSelected(); }
    void OnWeatherRouteKeyDown( wxListEvent& event );
    void OnWeatherRoutesListLeftDown(wxMouseEvent &event);
    void UpdateComputeState();
    void OnCompute( wxCommandEvent& event );
    void OnComputeAll( wxCommandEvent& event );
    void OnStop( wxCommandEvent& event );
    void OnResetAll( wxCommandEvent& event );
    void OnPositions( wxCommandEvent& event );
    void OnBatch( wxCommandEvent& event );
    void OnEditConfiguration( wxCommandEvent& event ) { OnEditConfiguration(); }
    void OnGoTo( wxCommandEvent& event );
    void OnDelete( wxCommandEvent& event );
    void OnDeleteAll( wxCommandEvent& event );
    void OnFilter( wxCommandEvent& event );
    void OnExport( wxCommandEvent& event );
    void OnExportAll( wxCommandEvent& event );
    void OnSettings( wxCommandEvent& event );
    void OnStatistics( wxCommandEvent& event );
    void OnReport( wxCommandEvent& event );
    void OnPlot( wxCommandEvent& event );
    void OnCursorPosition( wxCommandEvent& event );
    void OnManual( wxCommandEvent& event );
    void OnInformation( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );

    void OnComputationTimer( wxTimerEvent & );
    void OnHideConfigurationTimer( wxTimerEvent & );

    bool OpenXML(wxString filename, bool reportfailure = true);
    void SaveXML(wxString filename);

    void SetEnableConfigurationMenu();

    void UpdateConfigurations();
    void UpdateDialogs();

    void AddConfiguration(RouteMapConfiguration configuration);
    void UpdateRouteMap(RouteMapOverlay *routemapoverlay);
    void UpdateItem(long index, bool stateonly=false);

    RouteMap *SelectedRouteMap();
    void Export(RouteMapOverlay &routemapoverlay);

    void Start(RouteMapOverlay *routemapoverlay);
    void StartAll();
    void Stop();

    void DeleteRouteMaps(std::list<RouteMapOverlay *>routemapoverlays);
    RouteMapConfiguration DefaultConfiguration();

    StatisticsDialog m_StatisticsDialog;
    ReportDialog m_ReportDialog;
    PlotDialog m_PlotDialog;
    FilterRoutesDialog m_FilterRoutesDialog;

    wxTimer m_tCompute, m_tHideConfiguration;

    bool m_bRunning;
    wxTimeSpan m_RunTime;
    wxDateTime m_StartTime;

    wxString m_default_configuration_path;

    int m_RoutesToRun;
    bool m_bSkipUpdateCurrentItems;

    bool m_bShowConfiguration, m_bShowConfigurationBatch;
    bool m_bShowSettings, m_bShowStatistics, m_bShowReport, m_bShowPlot, m_bShowFilter;

    weather_routing_pi &m_weather_routing_pi;
};

#endif
