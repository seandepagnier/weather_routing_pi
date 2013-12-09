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

#include "Utilities.h"
#include "BoatPlan.h"
#include "SwitchPlanDialog.h"

SwitchPlanDialog::SwitchPlanDialog( wxWindow *parent, SwitchPlan &p, std::vector<wxString> plans )
    : SwitchPlanDialogBase(parent), plan(p)
{
    if(!isnan(plan.MaxWindSpeed))
        m_cbMaxWindSpeed->SetValue(true), m_sMaxWindSpeed->SetValue(plan.MaxWindSpeed);

    if(!isnan(plan.MinWindSpeed))
        m_cbMinWindSpeed->SetValue(true), m_sMinWindSpeed->SetValue(plan.MinWindSpeed);

    if(!isnan(plan.MaxWindDirection))
        m_cbMaxWindDirection->SetValue(true), m_sMaxWindDirection->SetValue(plan.MaxWindDirection);

    if(!isnan(plan.MinWindDirection))
        m_cbMinWindDirection->SetValue(true), m_sMinWindDirection->SetValue(plan.MinWindDirection);

    if(!isnan(plan.MaxWaveHeight))
        m_cbMaxWaveHeight->SetValue(true), m_sMaxWaveHeight->SetValue(plan.MaxWaveHeight);

    if(!isnan(plan.MinWaveHeight))
        m_cbMinWaveHeight->SetValue(true), m_sMinWaveHeight->SetValue(plan.MinWaveHeight);

    m_cbDayTime->SetValue(plan.DayTime);
    m_cbNightTime->SetValue(plan.NightTime);

    for(std::vector<wxString>::iterator it = plans.begin(); it != plans.end(); it++)
        m_cPlans->Append(*it);

    int index = m_cPlans->FindString(plan.Name);
    if(index == wxNOT_FOUND) {
        wxMessageDialog md(this, _("Failed to find plan name to switch to in list"),
                           _("Switch Plan"), wxICON_ERROR | wxOK );
        md.ShowModal();
    } else
        m_cPlans->SetSelection(index);
}

SwitchPlanDialog::~SwitchPlanDialog()
{
}


void SwitchPlanDialog::OnDone( wxCommandEvent& event )
{
    if(m_cbMaxWindSpeed->GetValue() && m_cbMinWindSpeed->GetValue()) {
        wxMessageDialog md(this, _("Cannot have min and max wind speed"),
                           _("Switch Plan"), wxICON_INFORMATION | wxOK );
        md.ShowModal();
        return;
    }

    if(m_cbMaxWindDirection->GetValue() && m_cbMinWindDirection->GetValue()) {
        wxMessageDialog md(this, _("Cannot have min and max wind direction"),
                           _("Switch Plan"), wxICON_INFORMATION | wxOK );
        md.ShowModal();
        return;
    }

    if(m_cbMaxWaveHeight->GetValue() && m_cbMinWaveHeight->GetValue()) {
        wxMessageDialog md(this, _("Cannot have min and max wave height"),
                           _("Switch Plan"), wxICON_INFORMATION | wxOK );
        md.ShowModal();
        return;
    }

    if(!m_cbMaxWindSpeed->GetValue() && !m_cbMinWindSpeed->GetValue() &&
       !m_cbMaxWindDirection->GetValue() && !m_cbMinWindDirection->GetValue() &&
       !m_cbMaxWaveHeight->GetValue() && !m_cbMinWaveHeight->GetValue()) {
        wxMessageDialog md(this, _("Cannot make a rule from nothing specified."),
                           _("Switch Plan"), wxICON_INFORMATION | wxOK );
        md.ShowModal();
        return;
    }

    if(!m_cbDayTime->GetValue() || !m_cbNightTime->GetValue()) {
        wxMessageDialog md(this, _("Cannot make a rule if neither daytime nor nighttime is specified."),
                           _("Switch Plan"), wxICON_INFORMATION | wxOK );
        md.ShowModal();
        return;
    }

    plan.MaxWindSpeed = m_cbMaxWindSpeed->GetValue() ? m_sMaxWindSpeed->GetValue() : NAN;
    plan.MinWindSpeed = m_cbMinWindSpeed->GetValue() ? m_sMinWindSpeed->GetValue() : NAN;
    plan.MaxWindDirection = m_cbMaxWindDirection->GetValue() ? m_sMaxWindDirection->GetValue() : NAN;
    plan.MinWindDirection = m_cbMinWindDirection->GetValue() ? m_sMinWindDirection->GetValue() : NAN;
    plan.MaxWaveHeight = m_cbMaxWaveHeight->GetValue() ? m_sMaxWaveHeight->GetValue() : NAN;
    plan.MinWaveHeight = m_cbMinWaveHeight->GetValue() ? m_sMinWaveHeight->GetValue() : NAN;
    plan.DayTime = m_cbDayTime->GetValue();
    plan.NightTime = m_cbNightTime->GetValue();

    plan.Name = m_cPlans->GetString(m_cPlans->GetSelection());

    EndModal(wxID_OK);
}

void SwitchPlanDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog md(this, _("\
Specify conditions where we should change \
to a different plan.\n\
If conditions change dramatically, then the program will look at the \
plan we just changed to and see if it needs to change again, so \
there is only need to specify rules for the next closest plan."),
                       _("Switch Plan"), wxICON_INFORMATION | wxOK );
    md.ShowModal();
}
