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
#include "SettingsDialog.h"
#include "StatisticsDialog.h"
#include "FilterRoutesDialog.h"

class weather_routing_pi;

class WeatherRoute
{
public:
    WeatherRoute();
    ~WeatherRoute();
    void Update(bool stateonly=false);

    bool Filtered;
    wxString Name, BoatFilename, Start, StartTime, End, Time, Distance, AvgSpeed, State;
    RouteMapOverlay *routemapoverlay;
};

class WeatherRouting : public WeatherRoutingBase
{
public:
    WeatherRouting(wxWindow *parent, weather_routing_pi &plugin, int batchposition_menu_id);
    ~WeatherRouting();

    void Reset();

    void Render(ocpnDC &dc, PlugIn_ViewPort &vp);
    ConfigurationDialog m_ConfigurationDialog;
    ConfigurationBatchDialog m_ConfigurationBatchDialog;

    void UpdateCurrentItem(RouteMapConfiguration configuration);
    void UpdateStates();
    RouteMapOverlay *CurrentRouteMap(bool messagedialog = false), *m_RouteMapOverlayNeedingGrib;

    void RebuildList();
    std::list<RouteMapOverlay*> m_RunningRouteMaps, m_WaitingRouteMaps;
    std::list<WeatherRoute*> m_WeatherRoutes;

    void GenerateBatch();
    bool Show(bool show);

private:
    void OnClose( wxCloseEvent& event );
    void OnIdle( wxIdleEvent& event );
    void OnConfiguration();
    void OnConfiguration( wxMouseEvent& event ) { OnConfiguration(); }
    void OnWeatherRouteSort( wxListEvent& event );
    void OnWeatherRouteSelected( wxListEvent& event );
    void OnWeatherRoutesListLeftDown(wxMouseEvent &event);
    void OnCompute( wxCommandEvent& event );
    void OnOpen( wxCommandEvent& event );
    void OnSave( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event );
    void OnNew( wxCommandEvent& event );
    void OnBatch( wxCommandEvent& event );
    void OnConfiguration( wxCommandEvent& event ) { OnConfiguration(); }
    void OnExport( wxCommandEvent& event );
    void OnDelete( wxCommandEvent& event );
    void OnFilter( wxCommandEvent& event );
    void OnReset( wxCommandEvent& event );
    void OnExportAll( wxCommandEvent& event );
    void OnDeleteAll( wxCommandEvent& event );
    void OnSettings( wxCommandEvent& event );
    void OnStatistics( wxCommandEvent& event );
    void OnPlot( wxCommandEvent& event );
    void OnInformation( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );

    void OnComputationTimer( wxTimerEvent & );

    bool OpenXML(wxString filename, bool reportfailure = true);
    void SaveXML(wxString filename);

    void AddConfiguration(RouteMapConfiguration configuration);
    void UpdateRouteMap(RouteMapOverlay *routemapoverlay);
    void UpdateItem(long index);

    RouteMap *SelectedRouteMap();
    void Export(RouteMapOverlay &routemapoverlay);

    void Start();
    void Stop();

    void DeleteRouteMap(RouteMapOverlay *routemapoverlay);
    RouteMapConfiguration DefaultConfiguration();

    SettingsDialog m_SettingsDialog;
    StatisticsDialog m_StatisticsDialog;
    FilterRoutesDialog m_FilterRoutesDialog;

    bool m_bComputing;

    wxTimer m_tCompute;

    bool m_bRunning;
    wxTimeSpan m_RunTime;
    wxDateTime m_StartTime;

    wxString m_default_configuration_path;

    int m_RoutesToRun;
    bool m_bSkipUpdateCurrentItem;

    int m_batchposition_menu_id;

    bool m_bShowConfiguration, m_bShowConfigurationBatch;
    bool m_bShowSettings, m_bShowStatistics, m_bShowFilter;

    weather_routing_pi &m_weather_routing_pi;
};

#endif
