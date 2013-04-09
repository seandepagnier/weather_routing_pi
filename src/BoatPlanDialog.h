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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"

class weather_routing_pi;

class BoatPlanDialog : public BoatPlanDialogBase
{
public:

    BoatPlanDialog( wxWindow *parent, BoatPlan &BoatPlan, std::vector<wxString> &PlanNames);
    ~BoatPlanDialog();

    void OnMouseEventsPlot( wxMouseEvent& event );
    void OnPaintPlot(wxPaintEvent& event);

    void OnNewSwitchPlanRule( wxCommandEvent& event );
    void OnEditSwitchPlanRule( wxCommandEvent& event );
    void OnDeleteSwitchPlanRule( wxCommandEvent& event );
    void OnDone( wxCommandEvent& event );

private:
    void PopulatePlans();

    BoatPlan &m_BoatPlan;
    std::vector<wxString> &m_PlanNames;
};
