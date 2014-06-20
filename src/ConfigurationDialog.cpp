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

#include "tinyxml/tinyxml.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "ConfigurationDialog.h"
#include "BoatDialog.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"

ConfigurationDialog::ConfigurationDialog(WeatherRouting &weatherrouting)
    : ConfigurationDialogBase(&weatherrouting), m_WeatherRouting(weatherrouting),
      m_bBlockUpdate(false)

{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxPoint p = GetPosition();
    pConf->Read ( _T ( "ConfigurationX" ), &p.x, p.x);
    pConf->Read ( _T ( "ConfigurationY" ), &p.y, p.y);
    SetPosition(p);
}

ConfigurationDialog::~ConfigurationDialog( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "ConfigurationX" ), p.x);
    pConf->Write ( _T ( "ConfigurationY" ), p.y);
}

void ConfigurationDialog::EditBoat( )
{
    BoatDialog boatdlg(this, m_fpBoat->GetPath());
    int updated = boatdlg.ShowModal();
    m_fpBoat->SetPath(boatdlg.m_boatpath);

    if(updated == wxID_OK) {
        /* update any configurations that use this boat */
        m_WeatherRouting.UpdateBoatFilename(boatdlg.m_boatpath);
        Update();
    }
}

void ConfigurationDialog::OnGribTime( wxCommandEvent& event )
{
    SetStartDateTime(m_GribTimelineTime);
    Update();
}

void ConfigurationDialog::OnCurrentTime( wxCommandEvent& event )
{
    SetStartDateTime(wxDateTime::Now());
    Update();
}

void ConfigurationDialog::OnAvoidCyclones( wxCommandEvent& event )
{
    Update();
}

// ensure mutually exclusive
void ConfigurationDialog::OnUpdateIntegratorNewton( wxCommandEvent& event )
{
    m_cbNewton->Set3StateValue(wxCHK_CHECKED);
    m_cbRungeKutta->Set3StateValue(wxCHK_UNCHECKED);
    Update();
}

void ConfigurationDialog::OnUpdateIntegratorRungeKutta( wxCommandEvent& event )
{
    m_cbRungeKutta->Set3StateValue(wxCHK_CHECKED);
    m_cbNewton->Set3StateValue(wxCHK_UNCHECKED);
    Update();
}

void ConfigurationDialog::OnAddDegreeStep( wxCommandEvent& event )
{
    m_lDegreeSteps->Enable();
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        s = m_lDegreeSteps->GetCount();

    double step;
    m_tDegreeStep->GetValue().ToDouble(&step);
    m_lDegreeSteps->Insert(wxString::Format(_T("%f"), step), s);
    m_tDegreeStep->Clear();
    Update();
}

void ConfigurationDialog::OnRemoveDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        return;

    m_lDegreeSteps->Delete(s);
    m_lDegreeSteps->SetSelection(s);
    Update();
}

void ConfigurationDialog::OnClearDegreeSteps( wxCommandEvent& event )
{
    m_lDegreeSteps->Enable();
    m_lDegreeSteps->Clear();
    Update();
}

void ConfigurationDialog::OnGenerateDegreeSteps( wxCommandEvent& event )
{
    m_lDegreeSteps->Enable();
    m_lDegreeSteps->Clear();

    double from, to, by;
    m_tFromDegrees->GetValue().ToDouble(&from);
    m_tToDegrees->GetValue().ToDouble(&to);
    m_tByDegrees->GetValue().ToDouble(&by);

    if(from < 0 || from >= 180 || to <= 0 || to > 180 || from >= to || by <= 0 || by >= 180) {
        wxMessageDialog mdlg(this, _("Invalid settings, nothing will be done."),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
        return;
    }

    double v;
    for(v = from; v <= to; v+=by)
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), v));

    if(v == to) v -= by;
    for(; v >= from; v-=by)
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), 360-v));

    Update();
}

#define SET_CHECKBOX_FIELD(FIELD, VALUE)              \
    do { \
    bool alltrue = true, allfalse = true; \
    for(std::list<RouteMapConfiguration>::iterator it = configurations.begin(); \
        it != configurations.end(); it++)                          \
        if(VALUE) allfalse = false; else alltrue = false; \
    m_cb##FIELD->Set3StateValue(alltrue ? wxCHK_CHECKED : allfalse ? wxCHK_UNCHECKED : wxCHK_UNDETERMINED); \
    } while (0)

#define SET_CHECKBOX(FIELD)              \
    SET_CHECKBOX_FIELD(FIELD, (*it).FIELD)

#define SET_CONTROL_VALUE(VALUE, CONTROL, SETTER, TYPE, NULLVALUE)     \
    do { \
    bool allsame = true;                            \
    TYPE value = NULLVALUE; \
    for(std::list<RouteMapConfiguration>::iterator it = configurations.begin(); \
        it != configurations.end(); it++) {                             \
        if(it == configurations.begin()) \
            value = VALUE;                \
        else if(value != VALUE) {         \
            allsame = false; \
            break; \
        } \
    } \
    CONTROL->SETTER(allsame ? value : NULLVALUE); \
    } while (0)

#define SET_CONTROL(FIELD, CONTROL, SETTER, TYPE, NULLVALUE) \
    SET_CONTROL_VALUE((*it).FIELD, CONTROL, SETTER, TYPE, NULLVALUE)

#define SET_CHOICE(FIELD) SET_CONTROL(FIELD, m_c##FIELD, SetValue, wxString, _T(""))

#define SET_SPIN_VALUE(FIELD, VALUE)                                          \
    m_s##FIELD->Enable(); \
    SET_CONTROL_VALUE(VALUE, m_s##FIELD, SetValue, int, (m_s##FIELD->Disable(), value))

#define SET_SPIN(FIELD) \
    SET_SPIN_VALUE(FIELD, (*it).FIELD)

void ConfigurationDialog::SetConfigurations(std::list<RouteMapConfiguration> configurations)
{
    m_bBlockUpdate = true;

    SET_CHOICE(Start);
    SET_CONTROL(StartTime, m_dpStartDate, SetValue, wxDateTime, wxDateTime());
    SET_CONTROL_VALUE(wxString::Format(_T("%.2f"), (*it).StartTime.GetHour()
                                            + (double)(*it).StartTime.GetMinute()/60.0),
                m_tStartHour, SetValue, wxString, _T(""));

    SET_SPIN_VALUE(TimeStepHours, (*it).dt / 3600);
    SET_SPIN_VALUE(TimeStepMinutes, ((int)(*it).dt / 60) % 60);
    SET_SPIN_VALUE(TimeStepSeconds, (int)(*it).dt%60);

    SET_CONTROL(boatFileName, m_fpBoat, SetPath, wxString, _T(""));

    SET_CHOICE(End);

    m_lDegreeSteps->Clear();
    m_lDegreeSteps->Enable();
    std::list<RouteMapConfiguration>::iterator it = configurations.begin();
    std::list<double> firstDegreeSteps = it->DegreeSteps;
    for(it++; it != configurations.end(); it++)
        if(firstDegreeSteps != it->DegreeSteps)
            m_lDegreeSteps->Disable();

    for(std::list<double>::iterator it = firstDegreeSteps.begin();
        it != firstDegreeSteps.end(); it++)
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), *it));

    SET_CHECKBOX_FIELD(Newton, (*it).Integrator == RouteMapConfiguration::NEWTON);
    SET_CHECKBOX_FIELD(RungeKutta, (*it).Integrator == RouteMapConfiguration::RUNGE_KUTTA);

    SET_SPIN(MaxDivertedCourse);
    SET_SPIN(MaxSearchAngle);
    SET_SPIN(MaxWindKnots);
    SET_SPIN(MaxSwellMeters);

    SET_SPIN(MaxLatitude);
    SET_SPIN(MaxTacks);
    SET_SPIN(TackingTime);
    SET_SPIN(WindVSCurrent);

    SET_CHECKBOX(AvoidCycloneTracks);
    SET_SPIN(CycloneMonths);
    SET_SPIN(CycloneDays);

    SET_CHECKBOX(DetectLand);
    SET_CHECKBOX(Currents);
    SET_CHECKBOX(InvertedRegions);
    SET_CHECKBOX(Anchoring);

    SET_CHECKBOX(AllowDataDeficient);
    SET_CHECKBOX(UseGrib);

    SET_CONTROL(ClimatologyType, m_cClimatologyType, SetSelection, int, -1);

    m_bBlockUpdate = false;
}

void ConfigurationDialog::AddSource(wxString name)
{
    m_cStart->Append(name);
    m_cEnd->Append(name);
}

void ConfigurationDialog::RemoveSource( wxString name )
{
    int i = m_cStart->FindString(name, true);
    if(i >= 0) {
        m_cStart->Delete(i);
        m_cEnd->Delete(i);
    }
}

void ConfigurationDialog::ClearSources()
{
    m_cStart->Clear();
    m_cEnd->Clear();
}

void ConfigurationDialog::SetStartDateTime(wxDateTime datetime)
{
    if(datetime.IsValid()) {
        m_dpStartDate->SetValue(datetime);
        m_tStartHour->SetValue(wxString::Format(_T("%.3f"), datetime.GetHour()
                                                +datetime.GetMinute() / 60.0));
    } else {
        wxMessageDialog mdlg(this, _("Invalid Date Time."),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
    }
}

#define GET_CHECKBOX(FIELD) \
           do { \
           if(m_cb##FIELD->Get3StateValue() == wxCHK_UNCHECKED) \
               configuration.FIELD = false; \
           else if(m_cb##FIELD->Get3StateValue() == wxCHK_CHECKED) \
               configuration.FIELD = true; \
           } while(0)

#define GET_SPIN(FIELD) \
    if(m_s##FIELD->IsEnabled())                                      \
        configuration.FIELD = m_s##FIELD->GetValue()

void ConfigurationDialog::Update()
{
    if(m_bBlockUpdate)
        return;

    std::list<RouteMapOverlay*> currentroutemaps = m_WeatherRouting.CurrentRouteMaps();
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++) {

        RouteMapConfiguration configuration = (*it)->GetConfiguration();

        if(!m_cStart->GetValue().empty())
            configuration.Start = m_cStart->GetValue();

        if(m_dpStartDate->GetValue().IsValid())
            configuration.StartTime = m_dpStartDate->GetValue();

        if(!m_tStartHour->GetValue().empty()) {
            double hour;
            m_tStartHour->GetValue().ToDouble(&hour);
            configuration.StartTime.SetHour((int)hour);
            configuration.StartTime.SetMinute((int)(60*hour)%60);
        }

        if(!m_fpBoat->GetPath().empty())
            configuration.boatFileName = m_fpBoat->GetPath();

        if(m_sTimeStepHours->IsEnabled()) {
            configuration.dt = 60*(60*m_sTimeStepHours->GetValue()
                                   + m_sTimeStepMinutes->GetValue())
                + m_sTimeStepSeconds->GetValue();
        }

        if(!m_cEnd->GetValue().empty())
            configuration.End = m_cEnd->GetValue();

        if(m_cbNewton->Get3StateValue() == wxCHK_CHECKED)
            configuration.Integrator = RouteMapConfiguration::NEWTON;
        else if(m_cbRungeKutta->Get3StateValue() == wxCHK_CHECKED)
            configuration.Integrator = RouteMapConfiguration::RUNGE_KUTTA;

        GET_SPIN(MaxDivertedCourse);
        GET_SPIN(MaxSearchAngle);
        GET_SPIN(MaxWindKnots);
        GET_SPIN(MaxSwellMeters);

        GET_SPIN(MaxLatitude);
        GET_SPIN(MaxTacks);
        GET_SPIN(TackingTime);
        GET_SPIN(WindVSCurrent);

        GET_CHECKBOX(AvoidCycloneTracks);
        GET_SPIN(CycloneMonths);
        GET_SPIN(CycloneDays);

        GET_CHECKBOX(DetectLand);
        GET_CHECKBOX(Currents);
        GET_CHECKBOX(InvertedRegions);
        GET_CHECKBOX(Anchoring);

        GET_CHECKBOX(AllowDataDeficient);

        GET_CHECKBOX(UseGrib);

        if(m_cClimatologyType->GetSelection() != -1)
            configuration.ClimatologyType = (RouteMapConfiguration::ClimatologyDataType)
                m_cClimatologyType->GetSelection();

        if(m_lDegreeSteps->IsEnabled()) {
            configuration.DegreeSteps.clear();
            for(unsigned int i=0; i<m_lDegreeSteps->GetCount(); i++) {
                double step;
                m_lDegreeSteps->GetString(i).ToDouble(&step);
                configuration.DegreeSteps.push_back(positive_degrees(step));
            }
            configuration.DegreeSteps.sort();
        }

        (*it)->SetConfiguration(configuration);
    }

#if 0    
    if(configuration.dt == 0) {
        wxMessageDialog mdlg(this, _("Zero Time Step invalid"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }
#endif

    if(m_lDegreeSteps->GetCount() < 4) {
        wxMessageDialog mdlg(this, _("Warning: less than 4 different degree steps specified\n"),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
    }

    m_WeatherRouting.UpdateCurrentConfigurations();
}
