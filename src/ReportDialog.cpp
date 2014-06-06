/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier                                 *
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

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "ocpn_plugin.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"

#include "WeatherRouting.h"

ReportDialog::ReportDialog( WeatherRouting &weatherrouting )
    : ReportDialogBase(&weatherrouting), m_WeatherRouting(weatherrouting)
{
    SetRouteMapOverlay(NULL);
}

void ReportDialog::SetRouteMapOverlay(RouteMapOverlay *routemapoverlay)
{
    GenerateRoutesReport();

    if(routemapoverlay == NULL) {
        m_htmlConfigurationReport->SetPage(_("No Configuration selected."));
        return;
    }

    if(!routemapoverlay->ReachedDestination()) {
        m_htmlConfigurationReport->SetPage(_("Destination not yet reached."));
        return;
    }

    wxString page;
    RouteMapConfiguration c = routemapoverlay->GetConfiguration();
    std::list<PlotData> p = routemapoverlay->GetPlotData();

    page += _("Route from ") + c.Start + _(" to ") + c.End + _(" Leaving ") + c.StartTime.Format();
    page += _T("<p>");
    double distance = DistGreatCircle_Plugin(c.StartLat, c.StartLon, c.EndLat, c.EndLon);
    double distance_sailed = routemapoverlay->RouteInfo(RouteMapOverlay::DISTANCE);
    page += _("Distance sailed: ") + wxString::Format
        (_T("%.3f NMi : %.3f (%.2f%%) "), distance_sailed,
         distance_sailed - distance, distance_sailed / distance - 1) +
        _("longer than great circle route") + _T("\n");
    page += _("Averages: ") +
        wxString::Format(_T(" %.2f "), routemapoverlay->RouteInfo(RouteMapOverlay::AVGSPEED)) + _("speed") +
        wxString::Format(_T(" %.2f "), routemapoverlay->RouteInfo(RouteMapOverlay::AVGWIND)) + _("wind") +
        wxString::Format(_T(" %.2f "), routemapoverlay->RouteInfo(RouteMapOverlay::AVGWAVE)) + _("wave") + _T("\n");

    m_htmlConfigurationReport->SetPage(page);
}

void ReportDialog::GenerateRoutesReport()
{
    /* sort configurations interate over each group of configurations
       with the same start and end to determine best and worst times,
       and cyclone crossings to determine cyclone times

       determine if currents significantly improve this (boat over ground speed average is 10% or
       more faster than boat over water)  then attempt to determine which current based on lat/lon
       eg, gulf stream, japan, current aghulles current etc.. and report it.

    */

    m_htmlRoutesReport->SetPage(_("No report yet."));
}

void ReportDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Weather Routing Reports gives an overview of a given route based on multiple configurations.\n\n\
For example using the configuration batch dialog, it is possible to easily generate multiple \
otherwise identical configurations which have different starting times. \
Once all of these configurations are computed, they become available to the report generator. \
An overview can be given of the best times, expected speed, and weather conditions. \
If climatology is available, cyclone risk and additional weather conditions may be described."),
                         _("Weather Routing Report"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}
