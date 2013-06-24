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

#include "Utilities.h"
#include "Boat.h"
#include "BoatDialog.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"
#include "ConfigurationBatchDialog.h"

ConfigurationBatchDialog::ConfigurationBatchDialog(WeatherRouting *parent)
    : ConfigurationBatchDialogBase(this), m_WeatherRouting(*parent)
{
    Reset();
}

void ConfigurationBatchDialog::Render(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!IsShown() ||
       m_notebookConfigurations->GetCurrentPage() != m_pRoutes)
        return;

    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end(); it++) {
        for(std::list<BatchSource*>::iterator it2 = (*it)->destinations.begin();
            it2 != (*it)->destinations.end(); it2++) {

            dc.SetPen(wxPen(*wxRED, 3));

            wxPoint p1p, p2p;
            GetCanvasPixLL(&vp, &p1p, (*it)->lat, (*it)->lon);
            GetCanvasPixLL(&vp, &p2p, (*it2)->lat, (*it2)->lon);
            dc.DrawLine(p1p.x, p1p.y, p2p.x, p2p.y);
        }
    }
}

void ConfigurationBatchDialog::AddSource(double lat, double lon)
{
    wxTextEntryDialog pd( this, _("Enter Name"), _("New Source") );
    if(pd.ShowModal() == wxID_OK) {
        m_lSources->Append(pd.GetValue());
    }
}

void ConfigurationBatchDialog::RemoveSource( wxString name )
{
    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end();) {
        for(std::list<BatchSource*>::iterator it2 = (*it)->destinations.begin();
            it2 != (*it)->destinations.end();)
            if((*it2)->name == name)
                it2 = (*it)->destinations.erase(it2);
            else
                it2++;

        if((*it)->name == name)
            it = sources.erase(it);
        else
            it++;
    }
}

void ConfigurationBatchDialog::OnSources( wxCommandEvent& event )
{
    m_lDestinations->DeselectAll();

    int index = m_lSources->GetSelection();
    if(index < 0)
        return;

    for(int i = 0; i<m_lDestinations->GetCount(); i++)
        for(std::list<BatchSource*>::iterator it = sources[index]->destinations.begin();
                it != sources[index]->destinations.end(); it++)
            if((*it)->name == m_lDestinations->GetString(i))
                m_lDestinations->SetSelection(i);
}

void ConfigurationBatchDialog::OnDestinations( wxCommandEvent& event )
{
    int index = m_lSources->GetSelection();
    if(index < 0)
        return;

    sources[index]->destinations.clear();
    for(int i = 0; i<m_lDestinations->GetCount(); i++)
        if(m_lDestinations->IsSelected(i))
            sources[index]->destinations.push_back(sources[i]);
}

void ConfigurationBatchDialog::OnRemoveSource( wxCommandEvent& event )
{
    int index = m_lSources->GetSelection();
    if (index < 0) return;

    RemoveSource(m_lSources->GetString(index));

    m_lSources->Delete(index);
    m_lDestinations->Delete(index);
}

void ConfigurationBatchDialog::OnConnect( wxCommandEvent& event )
{
    double nm;
    m_tMiles.GetValue().ToDouble(&nm);
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
    wxMessageDialog mdlg(this, _("Batch mode iterates over the selected route map configuration\
 to generate many configurations.\n\n\
To add a new Position, use the right-click context menu on the main map when the batch dialog is\
visible"),
                         _("Weather Routing"), wxOK | wxICON_WARNING);
    mdlg.ShowModal();
}

void ConfigurationBatchDialog::OnCancel( wxCommandEvent& event )
{
    Hide();
}

void ConfigurationBatchDialog::OnGenerate( wxCommandEvent& event )
{
    m_WeatherRouting.GenerateBatch();
    Hide();
}

void ConfigurationBatchDialog::Reset()
{
    m_tStartDays->SetValue(_T("0"));
    m_tStartHours->SetValue(_T("0"));
    m_tStartSpacingDays->SetValue(_T("1"));
    m_tStartSpacingHours->SetValue(_T("0"));

    RouteMapOverlay *routemapoverlay = m_WeatherRouting.CurrentRouteMap(true);
    if(routemapoverlay) {
        RouteMapConfiguration configuration = routemapoverlay->GetConfiguration();

        AddSource(configuration.Start, configuration.End);

        m_lBoats->Append(configuration.boatFileName);
    }
}
