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

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "StatisticsDialog.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"

StatisticsDialog::StatisticsDialog(wxWindow *parent)
    : StatisticsDialogBase(parent)
{
    SetRouteMapOverlay(NULL);
}

void StatisticsDialog::SetRouteMapOverlay(RouteMapOverlay *routemapoverlay)
{
    if(routemapoverlay) {
        m_stState->SetLabel(routemapoverlay->Running() ? _("Running") : _("Stopped"));
       
        int isochrons, routes, invroutes, skippositions, positions;
        routemapoverlay->GetStatistics(isochrons, routes, invroutes, skippositions, positions);
        m_stIsoChrons->SetLabel(wxString::Format(_T("%d"), isochrons));
        m_stRoutes->SetLabel(wxString::Format(_T("%d"), routes));
        m_stInvRoutes->SetLabel(wxString::Format(_T("%d"), invroutes));
        m_stSkipPositions->SetLabel(wxString::Format(_T("%d"), skippositions));
        m_stPositions->SetLabel(wxString::Format(_T("%d"), positions));
    } else {
        m_stState->SetLabel(_("No Route"));
        m_stIsoChrons->SetLabel(_T(""));
        m_stRoutes->SetLabel(_T(""));
        m_stInvRoutes->SetLabel(_T(""));
        m_stSkipPositions->SetLabel(_T(""));
        m_stPositions->SetLabel(_T(""));
    }

    Fit();
}

void StatisticsDialog::SetRunTime(wxTimeSpan RunTime)
{
    m_stRunTime->SetLabel(RunTime.Format());
}
