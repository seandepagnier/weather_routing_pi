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
#include <wx/progdlg.h>

#include <map>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "ocpn_plugin.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"

#include "WeatherRouting.h"

ReportDialog::ReportDialog( WeatherRouting &weatherrouting )
#ifndef __WXOSX__
    : ReportDialogBase(&weatherrouting),
#else
    : ReportDialogBase(&weatherrouting, wxID_ANY, _("Weather Route Report"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP),
#endif
      m_WeatherRouting(weatherrouting)
{
    m_bReportStale = true;
    SetRouteMapOverlays(std::list<RouteMapOverlay*>());
#ifdef __OCPN__ANDROID__
    wxSize sz = ::wxGetDisplaySize();
    SetSize(0, 0, sz.x, sz.y-40);
#endif
}

void ReportDialog::SetRouteMapOverlays(std::list<RouteMapOverlay*> routemapoverlays)
{
    GenerateRoutesReport();

    if(routemapoverlays.empty()) {
        m_htmlConfigurationReport->SetPage(_("No Configuration selected."));
        return;
    }

    wxString page;
    for(std::list<RouteMapOverlay *>::iterator it = routemapoverlays.begin();
        it != routemapoverlays.end(); it++) {

        page += _T("<p>");
        if(!(*it)->ReachedDestination() ) {
            m_htmlConfigurationReport->SetPage(_("Destination not yet reached."));
            continue;
        }

        RouteMapConfiguration c = (*it)->GetConfiguration();
        Position *d = (*it)->GetDestination();

        page += _("Boat Filename") + _T(" ") + wxFileName(c.boatFileName).GetName() + _T("<dt>");
        page += _("Route from ") + c.Start + _(" to ") + c.End + _T("<dt>");
        page += _("Leaving ") + FormatTime((*it)->StartTime()) + _T("<dt>");
        if (d) {
            page += _("Arriving ") + FormatTime((*it)->EndTime()) + _T("<dt>");
            page += _("Duration ") + ((*it)->EndTime() - (*it)->StartTime()).Format() + _T("<dt>");
        }
        page += _T("<p>");
        double distance = DistGreatCircle_Plugin(c.StartLat, c.StartLon, c.EndLat, c.EndLon);
        double distance_sailed = (*it)->RouteInfo(RouteMapOverlay::DISTANCE);
        page += _("Distance sailed: ") + wxString::Format
            (_T("%.2f NMi : %.2f NMi or %.2f%% "), distance_sailed,
             distance_sailed - distance, (distance_sailed / distance - 1) * 100.0) +
            _("longer than great circle route") + _T("<br>");

        double avgspeed = (*it)->RouteInfo(RouteMapOverlay::AVGSPEED);
        double avgspeedground = (*it)->RouteInfo(RouteMapOverlay::AVGSPEEDGROUND);
        page += _("Average Speed Over Water (SOW)") + wxString(_T(": ")) + wxString::Format
            (_T(" %.2f"), avgspeed) + _T(" ") + _("knots") + _T("<dt>");
        page += _("Average Speed Over Ground (SOG)") + wxString(_T(": ")) + wxString::Format
            (_T(" %.2f"), avgspeedground) + _T(" ") + _("knots") + _T("<dt>");
        page += _("Average Wind") + wxString(_T(": ")) + wxString::Format
            (_T(" %.2f"), (*it)->RouteInfo(RouteMapOverlay::AVGWIND)) + _T(" ")
	    + _("knots") + _T("<dt>");
        
        // CUSTOMIZATION
        // Add max wind. I think this is more important than the average
        // wind as it gives an indication on how strong will be the sailing
        // conditions, and if the crew has sufficient experience to handle it.
        page += _("Maximum Wind") + wxString(_T(": ")) \
                + wxString::Format(_T(" %.2f"), (*it)->RouteInfo(RouteMapOverlay::MAXWIND)) \
                + _T(" ") + _("knots") + _T("<dt>");;
        
        page += _("Average Swell") + wxString(_T(": ")) + wxString::Format
            (_T(" %.2f"), (*it)->RouteInfo(RouteMapOverlay::AVGSWELL)) + _T(" ")
	    + _("meters") + _T("<dt>");
        page += _("Upwind") + wxString(_T(": ")) + wxString::Format
            (_T(" %.2f%%"), (*it)->RouteInfo(RouteMapOverlay::PERCENTAGE_UPWIND)) + _T("<dt>");
        double port_starboard = (*it)->RouteInfo(RouteMapOverlay::PORT_STARBOARD);
        page += _("Port/Starboard") + wxString(_T(": ")) +
            (wxIsNaN(port_starboard) ? _T("nan") : wxString::Format
             (_T("%d/%d"), (int)port_starboard, 100-(int)port_starboard)) + _T("<dt>");

        if (d) {
            page += _("Number of tacks") + wxString::Format(_T(": %d "), d->tacks) + _T("<dt>");
        }
        
        // CUSTOMIZATION
        // Display sailing comfort in the report
        page += ("Sailing comfort") + wxString(_T(": ")) \
                + (*it)->sailingConditionText((*it)->RouteInfo(RouteMapOverlay::COMFORT)) \
                + _T("<dt>\n");

        /* determine if currents significantly improve this (boat over ground speed average is 10% or
           more faster than boat over water)  then attempt to determine which current based on lat/lon
           eg, gulf stream, japan, current aghulles current etc.. and report it. */
        page += _T("<p>");
        double wspddiff = avgspeedground / avgspeed;
        if(fabs(1-wspddiff) > .03) {
            page += wxString::Format (_T("%.2f%% "), ((wspddiff > 1 ? wspddiff : 1/wspddiff) - 1) * 100.0)
                + _("speed change due to ");
            if(wspddiff > 1)
                page += _("favorable");
            else
                page += _("unfavorable");
            page += _(" currents.");
        }
    }

    m_htmlConfigurationReport->SetPage(page);
}

wxDateTime ReportDialog::DisplayedTime(wxDateTime t)
{
    wxDateTime display_time = t;
    if(m_WeatherRouting.m_SettingsDialog.m_cbUseLocalTime->GetValue())
        display_time = t.FromUTC();
    return display_time;
}

wxString ReportDialog::FormatTime(wxDateTime t)
{
    wxString r = DisplayedTime(t).Format(_T("%x %X"));
#if 0
    // XXX add this?
    if(m_WeatherRouting.m_SettingsDialog.m_cbUseLocalTime->GetValue())
        r += _(" (local)");
    else
        r += _T(" (UTC)");
#endif
    return r;
}

void ReportDialog::GenerateRoutesReport()
{
    if(!m_bReportStale)
        return;
    m_bReportStale = false;

    /* sort configurations interate over each group of configurations
       with the same start and end to determine best and worst times,
       and cyclone crossings to determine cyclone times
    */

    std::map<wxString, std::list<RouteMapOverlay *> > routes;
    for(std::list<WeatherRoute*>::iterator it = m_WeatherRouting.m_WeatherRoutes.begin();
        it != m_WeatherRouting.m_WeatherRoutes.end(); it++) {
        if(!(*it)->routemapoverlay->ReachedDestination())
            continue;
        wxString route_string = (*it)->Start + _T(" - ") + (*it)->End;
        std::list<RouteMapOverlay *> overlays = routes[route_string];
        overlays.push_back((*it)->routemapoverlay);
        routes[route_string] = overlays;
    }

    if(routes.size() == 0) {
        m_htmlRoutesReport->SetPage(_("No routes to report yet."));
        return;
    }

    wxString page;
    for(std::map<wxString, std::list<RouteMapOverlay *> >::iterator it = routes.begin();
                          it != routes.end(); it++)
    {
        std::list<RouteMapOverlay *> overlays = it->second;
        if (overlays.begin() == overlays.end()) {
            // XXX not possible? but shut up compilers warnings
            continue;
        }
        RouteMapOverlay *first = *overlays.begin();

        RouteMapConfiguration c = first->GetConfiguration();
        page += _T("<p>");
        page += c.Start + _T(" ") + _("to") + _T(" ") + c.End + _T(" ") + wxString::Format
            (_T("(%ld ") + wxString(_("configurations")) + _T(")\n"), overlays.size());

        /* determine fastest time */
        wxTimeSpan fastest_time;
        RouteMapOverlay *fastest;

        std::multimap< wxDateTime, RouteMapOverlay * > sort_by_start;
        bool any_bad = false;
        bool any_good = false;

        for(std::list<RouteMapOverlay *>::iterator it2 = overlays.begin(); it2 != overlays.end(); it2++) {
            RouteMapOverlay *r = *it2;
            wxTimeSpan current_time = r->EndTime() - r->StartTime();
            sort_by_start.insert(std::pair< wxDateTime, RouteMapOverlay * >(r->StartTime() , r));
            if(r == first || current_time < fastest_time) {
                fastest_time = current_time;
                fastest = r;
            }
            if (r->RouteInfo(RouteMapOverlay::PERCENTAGE_UPWIND) > 50) {
                any_bad = true;
            } else {
                any_good = true;
            }
        }

        page += _("<dt>Fastest configuration ") + FormatTime(fastest->StartTime());
        page += wxString(_T(" ")) + _("avg speed") + wxString::Format
            (_T(": %.2f "), fastest->RouteInfo(RouteMapOverlay::AVGSPEED))
	    + _("knots");

        /* determine best times if upwind percentage is below 50 */
        page += _T("<dt>");
        page += _("Best Times (mostly downwind)") + wxString(_T(": "));
        if (any_good == false) {
            // no downwind route
            page += _("none");
        }
        else if (any_bad == false) {
            // all routes are downwind
            page += _("any");
        }
        else {
            bool first_print = true;
            std::multimap< wxDateTime, RouteMapOverlay * > reduce_by_start;
            // merge downwind routes in bigger interval
            // assume most routes with same start time are of same kind (downwind or upwind)
            std::multimap< wxDateTime, RouteMapOverlay * >::iterator it = sort_by_start.begin();
            while (it != sort_by_start.end() ) {
                // remove first upwind routes, from any_good test there's at least one downwind route
                for(; it != sort_by_start.end(); it++) {
                    RouteMapOverlay *r = it->second;
                    if (r->RouteInfo(RouteMapOverlay::PERCENTAGE_UPWIND) <= 50) {
                        break;
                     }
                }
                if (it == sort_by_start.end()) 
                    break;

                RouteMapOverlay *r = it->second;
                wxDateTime s = DisplayedTime(r->StartTime());
                wxDateTime e = DisplayedTime(r->EndTime());
                // merge downwind
                for(; it != sort_by_start.end(); it++) {
                    RouteMapOverlay *r = it->second;
                    if (r->RouteInfo(RouteMapOverlay::PERCENTAGE_UPWIND) > 50) {
                        break;
                    }
                    e = r->EndTime();
                }
                if(first_print)
                    first_print = false;
                else
                    page += _(" and ");
                page += s.Format(_T("%d %B ")) + _("to") + e.Format(_T(" %d %B"));
            }
        }
        
        // CUSTOMIZATION
        // Display the best option to travel in order
        // to get the most comfortable sailing
        page += _T("<dt>");
        page += _("Best Sailing Comfort") + wxString(_T(": "));
        wxDateTime best_comfort_date;
        int best_sailing_comfort = 6;
        for(std::multimap< wxDateTime, RouteMapOverlay * >::iterator it3 = sort_by_start.begin(); it3 != sort_by_start.end(); it3++) {
            RouteMapOverlay *r = it3->second;
            if (!best_comfort_date.IsValid() ||
                (best_comfort_date < r->StartTime() && best_sailing_comfort > r->RouteInfo(RouteMapOverlay::COMFORT)))
            {
                best_comfort_date = r->StartTime();
                best_sailing_comfort = r->RouteInfo(RouteMapOverlay::COMFORT);
            }
        }
        page += RouteMapOverlay::sailingConditionText(best_sailing_comfort);
        page += _T(" on ");
        page += FormatTime(best_comfort_date);
        
        page += _T("<dt>");
        page += _("Cyclones") + wxString(_T(": "));

        wxProgressDialog progressdialog(_("Weather Routing"), _("Calculating Cyclone Crossings"),
                                        overlays.size(), this, wxPD_ELAPSED_TIME);
        int pdi = 0;

        int cyclonemonths[12] = {0};
        std::list<RouteMapOverlay *> cyclone_safe_routes;
        bool allsafe = true, nonesafe = true;
        for(std::list<RouteMapOverlay *>::iterator it2 = overlays.begin(); it2 != overlays.end(); it2++) {
            switch((*it2)->Cyclones(cyclonemonths)) {
            case -1:
                page += _("Climatology data unavailable.");
                goto cyclonesfailed;
            case 0:
                cyclone_safe_routes.push_back(*it2);
                nonesafe = false;
            default:
                cyclone_safe_routes.push_back(NULL);
                allsafe = false;
            }

            progressdialog.Update(pdi++);
        }
        
        int i, j;
        for(i=0, j=11; i<12; j=i++)
            /* get first cyclone month */
            if(cyclonemonths[i] && !cyclonemonths[j]) {
                int lm = i;
                page += wxDateTime::GetMonthName((wxDateTime::Month)i);
                for(int k=i+1, l=i; ; l = k++) {
                    if(k==12) k = 0;
                    if(k == i)
                        break;
                    if(cyclonemonths[k] && !cyclonemonths[l]) {
                        page += _(" and ") + wxDateTime::GetMonthName((wxDateTime::Month)k);
                        lm = k;
                    } else if(!cyclonemonths[k] && cyclonemonths[l] && l!=lm)
                        page += _(" to ") + wxDateTime::GetMonthName((wxDateTime::Month)l);
                }
                goto had_some_cyclones;
            }
                    
        if(cyclonemonths[0])
            page += _("all months");
        else
            page += _("none");

    had_some_cyclones:;

        page += _T("<dt>");
        if(allsafe)
            page += _("All routes are safe from cyclones.");
        else if(nonesafe)
            page += _("No routes found to be safe from cyclones.");
        else {
            page += _("Start times for cyclone safe routes: ");
            /* note: does not merge beginning and end of linked list for safe times,
               this sometimes might be nice, but they will be in different years. */
            bool first = true;
            for(std::list<RouteMapOverlay *>::iterator it2 = cyclone_safe_routes.begin(); it2 != cyclone_safe_routes.end();
                        it2++)
            {
                if(!*it2) continue;
                if(!first)
                    page += _(" and ");
                first = false;
                page += DisplayedTime((*it2)->StartTime()).Format(_T("%x"));

                if(++it2 == cyclone_safe_routes.end())
                    break;

                if(!*it2)
                    continue;

                while(*it2 && ++it2 != cyclone_safe_routes.end());

                it2--;
                page += _(" to ") + DisplayedTime((*it2)->StartTime()).Format(_T("%x"));
            }
        }
    cyclonesfailed:;
    }

    m_htmlRoutesReport->SetPage(page);
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
