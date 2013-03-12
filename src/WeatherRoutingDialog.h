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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _WEATHER_ROUTING_H_
#define _WEATHER_ROUTING_H_


#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"
#include "WeatherRoutingSettingsDialog.h"

#include "../../grib_pi/src/GribUIDialog.h"

class weather_routing_pi;
class BoatDialog;
class GribUIDialog;

class WeatherRoutingThread : public wxThread
{
public:
WeatherRoutingThread(wxWindow &p,RouteMap &r)
    : wxThread(wxTHREAD_JOINABLE), parent(p), routemap(r), stop(false) { Create(); }
    void End() { stop = true; }
    void *Entry();
    wxWindow &parent;
    RouteMap &routemap;
//    wxMutex routemutex;
    bool stop;
};

class WeatherRoutingDialog : public WeatherRoutingDialogBase
{
public:
    WeatherRoutingDialog( wxWindow *parent, double boat_lat, double boat_lon );
    ~WeatherRoutingDialog( );

    void RenderRouteMap(ocpnDC &dc, PlugIn_ViewPort &vp);
    RouteMap m_routemap;

private:
    void OnUpdateEnd( wxCommandEvent& event );
    void OnCompute( wxCommandEvent& event );
    void OnBoat( wxCommandEvent& event );
    void OnReset( wxCommandEvent& event );
    void OnInformation( wxCommandEvent& event );
    void OnSettings( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event );

    void Reset();
    void UpdateEnd();
    void ReconfigureRouteMap();

    //    Data
    weather_routing_pi   *pPlugIn;

    BoatSpeed boat;

    bool m_bComputing;

    bool m_bShowBoatDialog;
    BoatDialog *m_pBoatDialog;

    WeatherRoutingSettingsDialog m_SettingsDialog;

    WeatherRoutingThread m_thCompute;
};

#endif
