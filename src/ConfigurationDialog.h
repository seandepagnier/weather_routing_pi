/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
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
    ConfigurationDialog(WeatherRouting &weatherrouting);
    ~ConfigurationDialog();

    void EditBoat();
    void SetConfigurations(std::list<RouteMapConfiguration>configuration);
    void Update();

    void AddSource(wxString name);
    void RemoveSource( wxString name );
    void ClearSources();
    void SetBoatFilename(wxString path);

    wxDateTime m_GribTimelineTime;

protected:
    void OnUpdate( wxCommandEvent& event ) { Update(); }
    void OnUpdateDate( wxDateEvent& event ) { Update(); }
    void OnGribTime( wxCommandEvent& event );
    void OnCurrentTime( wxCommandEvent& event );
    void OnUpdateSpin( wxSpinEvent& event ) { Update(); }
    void OnBoatFilename( wxCommandEvent& event );
    void OnEditBoat( wxCommandEvent& event ) { EditBoat(); }
    void OnUpdateIntegratorNewton( wxCommandEvent& event );
    void OnUpdateIntegratorRungeKutta( wxCommandEvent& event );

    void EnableSpin( wxMouseEvent& event ) {
        wxDynamicCast(event.GetEventObject(), wxSpinCtrl)->Enable();
        event.Skip();
    }

    void OnAvoidCyclones( wxCommandEvent& event );
    void OnAddDegreeStep( wxCommandEvent& event );
    void OnRemoveDegreeStep( wxCommandEvent& event );
    void OnClearDegreeSteps( wxCommandEvent& event );
    void OnGenerateDegreeSteps( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event ) { Hide(); }    

private:
    void UpdateCycloneControls();

    enum ConfigurationItem {START, END, START_TIME, TIME_STEP};

    void SetStartDateTime(wxDateTime datetime);

    WeatherRouting &m_WeatherRouting;
    bool m_bBlockUpdate;
};

#endif
