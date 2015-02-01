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

#ifndef _WEATHER_ROUTING_SETTINGS_H_
#define _WEATHER_ROUTING_SETTINGS_H_


#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"

class SettingsDialog : public SettingsDialogBase
{
public:
    SettingsDialog( wxWindow *parent );

    void LoadSettings();
    void SaveSettings();

    void OnUpdateColor( wxColourPickerEvent& event ) { OnUpdate(); }
    void OnUpdateSpin( wxSpinEvent& event ) { OnUpdate(); }
    void OnUpdate( wxCommandEvent& event ) { OnUpdate(); }
    void OnUpdate( );
    void OnUpdateColumns( wxCommandEvent& event );
    void OnHelp( wxCommandEvent& event );
};

#endif
