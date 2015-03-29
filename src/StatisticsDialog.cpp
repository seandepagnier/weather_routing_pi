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

#include <list>

#include "StatisticsDialog.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"

StatisticsDialog::StatisticsDialog(wxWindow *parent)
    : StatisticsDialogBase(parent)
{
    SetRouteMapOverlays(std::list<RouteMapOverlay*>());
}

void StatisticsDialog::SetRouteMapOverlays(std::list<RouteMapOverlay*> routemapoverlays)
{
    bool running = false;
    int tisochrons = 0, troutes = 0, tinvroutes = 0, tskippositions = 0, tpositions = 0;
    for(std::list<RouteMapOverlay *>::iterator it = routemapoverlays.begin();
        it != routemapoverlays.end(); it++) {
        if((*it)->Running())
            running = true;

        int isochrons, routes, invroutes, skippositions, positions;
        (*it)->GetStatistics(isochrons, routes, invroutes, skippositions, positions);
        tisochrons += isochrons, troutes += routes, tinvroutes += invroutes;
        tskippositions+= skippositions, tpositions += positions;
    }

    m_stState->SetLabel(routemapoverlays.empty() ? _("No Route") :
                        running ? _("Running") : _("Stopped"));
    m_stIsoChrons->SetLabel(wxString::Format(_T("%d"), tisochrons));
    m_stRoutes->SetLabel(wxString::Format(_T("%d"), troutes));
    m_stInvRoutes->SetLabel(wxString::Format(_T("%d"), tinvroutes));
    m_stSkipPositions->SetLabel(wxString::Format(_T("%d"), tskippositions));
    m_stPositions->SetLabel(wxString::Format(_T("%d"), tpositions));

    Fit();
}

void StatisticsDialog::SetRunTime(wxTimeSpan RunTime)
{
    m_stRunTime->SetLabel(RunTime.Format());
}
