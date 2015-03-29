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
//#include "ConfigurationBatchDialog.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"
#include "FilterRoutesDialog.h"

wxString FilterNames[] = {_("Start"), _("Start Time"), _("End"), _("BoatFileName"), _("State")};

FilterRoutesDialog::FilterRoutesDialog(WeatherRouting *weatherrouting)
    : FilterRoutesDialogBase(weatherrouting), m_WeatherRouting(weatherrouting)
{
    for(int i=0; i<NUM_FILTERS; i++)
        m_cCategory->Append(FilterNames[i]);
    m_cCategory->SetSelection(0);
}

void FilterRoutesDialog::OnCategory( wxCommandEvent& event )
{

    m_tFilter->SetValue(m_Filters[m_cCategory->GetSelection()]);
}

void FilterRoutesDialog::OnFilterText( wxCommandEvent& event )
{
    m_Filters[m_cCategory->GetSelection()] = m_tFilter->GetValue();
    ApplyFilters();
}

void FilterRoutesDialog::OnResetAll( wxCommandEvent& event )
{
    for(int i=0; i<NUM_FILTERS; i++)
        m_Filters[i] = _T("");
    ApplyFilters();
}

void FilterRoutesDialog::OnDone( wxCommandEvent& event )
{
    Hide();
}

void FilterRoutesDialog::ApplyFilters()
{
    for(std::list<WeatherRoute*>::iterator it = m_WeatherRouting->m_WeatherRoutes.begin();
        it != m_WeatherRouting->m_WeatherRoutes.end(); it++) {
        (*it)->Filtered = 0;
        for(int f = 0; f < NUM_FILTERS; f++) {
            wxString value;
            switch(f) {
            case START: value = (*it)->Start; break;
            case STARTTIME: value = (*it)->StartTime; break;
            case END: value = (*it)->End; break;
            case BOATFILENAME: value = (*it)->BoatFilename; break;
            case STATE: value = (*it)->State; break;
            }

            if(!value.Matches(_T("*") + m_Filters[f] + _T("*")))
                (*it)->Filtered = 1;
        }
    }

    m_WeatherRouting->RebuildList();
}
