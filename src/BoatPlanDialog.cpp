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
#include "BoatPlanDialog.h"
#include "SwitchPlanDialog.h"

BoatPlanDialog::BoatPlanDialog( wxWindow *parent, BoatPlan &BoatPlan, std::vector<wxString> &PlanNames)
    : BoatPlanDialogBase(parent), m_BoatPlan(BoatPlan), m_PlanNames(PlanNames)
{
    m_tBoatPlanName->SetValue(BoatPlan.Name);
    PopulatePlans();
}

BoatPlanDialog::~BoatPlanDialog()
{
}

void BoatPlanDialog::OnMouseEventsPlot( wxMouseEvent& event )
{
}

void BoatPlanDialog::OnPaintPlot(wxPaintEvent& event)
{
}

void BoatPlanDialog::OnNewSwitchPlanRule( wxCommandEvent& event )
{
    SwitchPlan plan;
    plan.Name = m_PlanNames[0];
    m_BoatPlan.SwitchPlans.push_back(plan);

    int index = m_lSwitchPlans->Append(wxString());
    m_lSwitchPlans->SetSelection(index, true);

    m_bEditSwitchBoatPlan->Enable();
    m_bDeleteSwitchBoatPlan->Enable();

    OnEditSwitchPlanRule(event);
}

void BoatPlanDialog::OnEditSwitchPlanRule( wxCommandEvent& event )
{
    int index = m_lSwitchPlans->GetSelection();
    if(index < 0)
        return;

    SwitchPlan plan = m_BoatPlan.SwitchPlans[index];

    if(m_PlanNames.size() < 1) {
        wxMessageDialog md(this, _("Cannot edit switch plan since there is no other plan to switch to."),
                           _("Sail Plans"),
                           wxICON_ERROR | wxOK );
        md.ShowModal();
        return;
    }
        
    SwitchPlanDialog dialog(this, plan, m_PlanNames);
    if(dialog.ShowModal() == wxID_OK)
        m_BoatPlan.SwitchPlans[index] = plan;
    else
        m_BoatPlan.SwitchPlans.erase(m_BoatPlan.SwitchPlans.begin() + index);

    PopulatePlans();
}

void BoatPlanDialog::OnDeleteSwitchPlanRule( wxCommandEvent& event )
{
    int index = m_lSwitchPlans->GetSelection();
    if(index < 0)
        return;

    SwitchPlan plan = m_BoatPlan.SwitchPlans[index];
    m_BoatPlan.SwitchPlans.erase(m_BoatPlan.SwitchPlans.begin() + index);
    PopulatePlans();
}

void BoatPlanDialog::OnDone( wxCommandEvent& event )
{
    m_BoatPlan.Name = m_tBoatPlanName->GetValue();
    for(std::vector<wxString>::iterator it = m_PlanNames.begin(); it != m_PlanNames.end(); it++)
        if(*it == m_BoatPlan.Name) {
            wxMessageDialog md(this, _("Cannot add plan with same name as existing plan."),
                               _("Sail Plan"), wxICON_INFORMATION | wxOK );
            md.ShowModal();
            return;
        }

    EndModal(wxID_OK);
}

void BoatPlanDialog::PopulatePlans()
{
    if(m_BoatPlan.SwitchPlans.size() == 0) {
        m_bEditSwitchBoatPlan->Disable();
        m_bDeleteSwitchBoatPlan->Disable();
    }

    m_lSwitchPlans->Clear();

    for(unsigned int i=0; i<m_BoatPlan.SwitchPlans.size(); i++) {
        SwitchPlan plan = m_BoatPlan.SwitchPlans[i];
    
        wxString des, a, andstr = _(" and ");
        if(!isnan(plan.MaxWindSpeed))
            des += a + _("Wind Speed > ") + wxString::Format(_("%.0f"), plan.MaxWindSpeed), a = andstr;
        
        if(!isnan(plan.MinWindSpeed))
            des += a + _("Wind Speed < ") + wxString::Format(_("%.0f"), plan.MinWindSpeed), a = andstr;

        if(!isnan(plan.MaxWindDirection))
            des += a + _("Wind Direction > ") + wxString::Format(_("%.0f"), plan.MaxWindDirection), a = andstr;
        
        if(!isnan(plan.MinWindDirection))
            des += a + _("Wind Direction < ") + wxString::Format(_("%.0f"), plan.MinWindDirection), a = andstr;
        
        if(!isnan(plan.MaxWaveHeight))
            des += a + _("Wave Height > ") + wxString::Format(_("%.0f"), plan.MaxWaveHeight), a = andstr;

        if(!isnan(plan.MinWaveHeight))
            des += a + _("Wave Height < ") + wxString::Format(_("%.0f"), plan.MinWaveHeight), a = andstr;

        if(!plan.DayTime) {
            des += a + _("Night Time");
        } else if(!plan.NightTime) {
            des += a + _("Day Time");
        }
        
        des += _(" switch to ");
        des += plan.Name;
        
        m_lSwitchPlans->Append(des);
    }
}
    
