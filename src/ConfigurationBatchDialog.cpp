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
    : ConfigurationBatchDialogBase(parent), m_WeatherRouting(*parent)
{
    Reset();
}

void ConfigurationBatchDialog::Render(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!IsShown() ||
       m_notebookConfigurations->GetCurrentPage() != m_pRoutes)
        return;

    wxFont mfont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
    dc.SetFont( mfont );

    dc.SetTextForeground(*wxRED);
//    dc.SetTextBackground(*wxTRANSPARENT);
//    dc.SetTextBackground(wxColour(0, 0, 0,  wxALPHA_TRANSPARENT));

    dc.SetPen(wxPen(*wxRED, 3));

    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end(); it++) {
        wxPoint p1, p2;
        double lat, lon;
        RouteMap::PositionLatLon((*it)->Name, lat, lon);
        GetCanvasPixLL(&vp, &p1, lat, lon);

        dc.DrawText((*it)->Name, p1.x, p1.y);
        dc.DrawCircle(p1.x, p1.y, 5);

        for(std::list<BatchSource*>::iterator it2 = (*it)->destinations.begin();
            it2 != (*it)->destinations.end(); it2++) {

            RouteMap::PositionLatLon((*it2)->Name, lat, lon);
            GetCanvasPixLL(&vp, &p2, lat, lon);

            dc.DrawLine(p1.x, p1.y, p2.x, p2.y);

            wxPoint p3((2*p1.x+3*p2.x)/5, (2*p1.y+3*p2.y)/5);
            wxPoint p4((p1.x+p2.x)/2, (p1.y+p2.y)/2);
            wxPoint p5((p2.y-p1.y)/8, (p1.x-p2.x)/8);
            dc.DrawLine(p3.x, p3.y, p4.x+p5.x, p4.y+p5.y);
            dc.DrawLine(p3.x, p3.y, p4.x-p5.x, p4.y-p5.y);
        }
    }
}

void ConfigurationBatchDialog::AddSource(wxString name)
{
    sources.push_back(new BatchSource(name));
    m_lSources->Append(name);
    m_lDestinations->Append(name);
}
    
void ConfigurationBatchDialog::RemoveSource( wxString name )
{
    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end();) {
        for(std::list<BatchSource*>::iterator it2 = (*it)->destinations.begin();
            it2 != (*it)->destinations.end();)
            if((*it2)->Name == name)
                it2 = (*it)->destinations.erase(it2);
            else
                it2++;

        if((*it)->Name == name) {
            delete *it;
            it = sources.erase(it);
        } else
            it++;
    }
}

void ConfigurationBatchDialog::ClearSources()
{
    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end();it++)
        delete *it;
    sources.clear();
}

void ConfigurationBatchDialog::OnSources( wxCommandEvent& event )
{
    m_lDestinations->DeselectAll();

    int index = m_lSources->GetSelection();
    if(index < 0)
        return;

    for(unsigned int i = 0; i<m_lDestinations->GetCount(); i++)
        for(std::list<BatchSource*>::iterator it = sources[index]->destinations.begin();
                it != sources[index]->destinations.end(); it++)
            if((*it)->Name == m_lDestinations->GetString(i))
                m_lDestinations->SetSelection(i);
}

void ConfigurationBatchDialog::OnDestinations( wxCommandEvent& event )
{
    int index = m_lSources->GetSelection();
    if(index < 0)
        return;

    sources[index]->destinations.clear();
    for(int i = 0; i<(int)m_lDestinations->GetCount(); i++)
        if(m_lDestinations->IsSelected(i)) {
            if(i == index)
                m_lDestinations->Deselect(i);
            else
                sources[index]->destinations.push_back(sources[i]);
        }
}

void ConfigurationBatchDialog::OnRemoveSource( wxCommandEvent& event )
{
    int index = m_lSources->GetSelection();
    if (index < 0) return;

    RemoveSource(m_lSources->GetString(index));

    m_lSources->Delete(index);
    m_lDestinations->Delete(index);
}

void ConfigurationBatchDialog::OnClearSources( wxCommandEvent& event )
{
    ClearSources();
    m_lSources->Clear();
    m_lDestinations->Clear();
}

void ConfigurationBatchDialog::OnConnect( wxCommandEvent& event )
{
    double nm;
    m_tMiles->GetValue().ToDouble(&nm);

    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end(); it++) {
        (*it)->destinations.clear();

        for(std::vector<BatchSource*>::iterator it2 = sources.begin();
            it2 != sources.end(); it2++)
            if(*it != *it2) {
                double distance;

                double lat1, lon1, lat2, lon2;
                RouteMap::PositionLatLon((*it)->Name, lat1, lon1);
                RouteMap::PositionLatLon((*it2)->Name, lat2, lon2);

                DistanceBearingMercator_Plugin(lat1, lon1, lat2, lon2, 0, &distance);
                if(distance <= nm)
                    (*it)->destinations.push_back(*it2);
            }
    }

    m_lSources->SetSelection(-1);
}

void ConfigurationBatchDialog::OnDisconnectAll( wxCommandEvent& event )
{
    for(std::vector<BatchSource*>::iterator it = sources.begin();
        it != sources.end(); it++)
        (*it)->destinations.clear();
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
    wxMessageDialog mdlg(this, _("Batch mode generates multiple configurations based on \
the selected configuration."),
                         _("Weather Routing"), wxOK);
    mdlg.ShowModal();
}

void ConfigurationBatchDialog::OnClose( wxCommandEvent& event )
{
    Hide();
}

void ConfigurationBatchDialog::OnGenerate( wxCommandEvent& event )
{
    m_WeatherRouting.GenerateBatch();
}

void ConfigurationBatchDialog::Reset()
{
    m_tStartDays->SetValue(_T("0"));
    m_tStartHours->SetValue(_T("0"));
    m_tStartSpacingDays->SetValue(_T("1"));
    m_tStartSpacingHours->SetValue(_T("0"));

    RouteMapOverlay *routemapoverlay = m_WeatherRouting.CurrentRouteMap();
    if(routemapoverlay) {
        RouteMapConfiguration configuration = routemapoverlay->GetConfiguration();

        for(std::vector<BatchSource*>::iterator it = sources.begin();
            it != sources.end(); it++) {
            (*it)->destinations.clear();

            if((*it)->Name == configuration.Start)
                for(std::vector<BatchSource*>::iterator it2 = sources.begin();
                    it2 != sources.end(); it2++)
                    if((*it2)->Name == configuration.End)
                        (*it)->destinations.push_back(*it2);
        }

        m_lBoats->Append(configuration.boatFileName);
    }
}
