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
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "weather_routing_pi.h"
#include "ConfigurationBatchDialog.h"
#include "Utilities.h"
#include "Boat.h"
#include "RouteMap.h"

ConfigurationBatchDialog::ConfigurationBatchDialog(wxWindow *parent, RouteMapConfiguration configuration)
    : ConfigurationBatchDialogBase(parent), m_boatFileName(configuration.boatFileName)
{
    Reset();
}

void ConfigurationBatchDialog::OnRemoveSource( wxCommandEvent& event )
{
    int index = m_lSources->GetSelection();
    if (index < 0) return;

    m_lSources->Delete(index);
}

void ConfigurationBatchDialog::OnAddDestination( wxCommandEvent& event )
{
    wxString name = m_cDestination->GetStringSelection();
    if(name.size() > 0)
        m_lDestinations->Append(name);
}

void ConfigurationBatchDialog::OnRemoveDestination( wxCommandEvent& event )
{
    int index = m_lDestinations->GetSelection();
    if (index < 0) return;

    m_lDestinations->Delete(index);
}

void ConfigurationBatchDialog::OnClearDestinations( wxCommandEvent& event )
{
    m_lDestinations->Clear();
}

void ConfigurationBatchDialog::OnReciprocateDestinations( wxCommandEvent& event )
{
}

void ConfigurationBatchDialog::OnAddBoat( wxCommandEvent& event )
{
    wxFileDialog openDialog
        ( this, _( "Select Polar" ), weather_routing_pi::StandardPath(), wxT ( "" ),
          wxT ( "XML Weather Routing files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_OPEN  );

    if(openDialog.ShowModal() == wxID_OK)
        m_lBoats->Append(openDialog.GetPath());
}

void ConfigurationBatchDialog::OnRemoveBoat( wxCommandEvent& event )
{
    int index = m_lBoats->GetSelection();
    if(index >= 0)
        m_lBoats->Delete(index);
}

void ConfigurationBatchDialog::OnReset( wxCommandEvent& event )
{
    Reset();
}

void ConfigurationBatchDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("Batch mode  iterates over the current route map configuration\n"),
                         _("Weather Routing"), wxOK | wxICON_WARNING);
    mdlg.ShowModal();
}

void ConfigurationBatchDialog::OnCancel( wxCommandEvent& event )
{
    EndModal(wxID_CANCEL);
}

void ConfigurationBatchDialog::OnGenerate( wxCommandEvent& event )
{
    EndModal(wxID_OK);
}

void ConfigurationBatchDialog::Reset()
{
    m_tStartDays->SetValue(_T("0"));
    m_tStartHours->SetValue(_T("0"));
    m_tStartSpacingDays->SetValue(_T("1"));
    m_tStartSpacingHours->SetValue(_T("0"));

    m_lBoats->Clear();
    m_lBoats->Append(m_boatFileName);
}
