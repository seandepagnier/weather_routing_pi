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

#include "ReportDialog.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"

ReportDialog::ReportDialog(wxWindow *parent)
    : ReportDialogBase(parent)
{
    SetRouteMapOverlay(NULL);
}

void ReportDialog::SetRouteMapOverlay(RouteMapOverlay *routemapoverlay)
{
    // ...
    GenerateReport();
}

void ReportDialog::OnReportAllRoutes( wxCommandEvent& event )
{
    GenerateReport();
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

void ReportDialog::GenerateReport()
{
}
