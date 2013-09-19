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

#ifndef _WEATHER_ROUTING_CONFIGURATION_DIALOG_H_
#define _WEATHER_ROUTING_CONFIGURATION_DIALOG_H_


#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"

class WeatherRouting;
class weather_routing_pi;

class ConfigurationDialog : public ConfigurationDialogBase
{
public:
    ConfigurationDialog(WeatherRouting *weatherrouting);
    ~ConfigurationDialog();

    void SetConfiguration(RouteMapConfiguration configuration);
    void Update();

    RouteMapConfiguration Configuration();

    void AddSource(wxString name);
    void RemoveSource( wxString name );
    void ClearSources();

    wxDateTime m_GribTimelineTime;

protected:
    void OnUpdate( wxCommandEvent& event ) { Update(); }
    void OnUpdate( wxDateEvent& event ) { Update(); }
    void OnGribTime( wxCommandEvent& event );
    void OnCurrentTime( wxCommandEvent& event );
    void OnUpdate( wxSpinEvent& event ) { Update(); }
    void OnUpdate( wxFileDirPickerEvent& event ) { Update(); }
    void OnEditBoat( wxCommandEvent& event );
    void OnAddDegreeStep( wxCommandEvent& event );
    void OnRemoveDegreeStep( wxCommandEvent& event );
    void OnClearDegreeSteps( wxCommandEvent& event );
    void OnGenerateDegreeSteps( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event ) { Hide(); }

private:
    enum ConfigurationItem {START, END, START_TIME, TIME_STEP};

    void SetStartDateTime(wxDateTime datetime);

    WeatherRouting *m_WeatherRouting;
};

#endif
