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

#include "tinyxml/tinyxml.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "ConfigurationDialog.h"
#include "BoatDialog.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"
#include "icons.h"

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

void ConfigurationDialog::EditBoat()
{
    m_WeatherRouting.m_BoatDialog.LoadPolar(m_tBoat->GetValue());
    m_WeatherRouting.m_BoatDialog.Show();
}

void ConfigurationDialog::OnGribTime( wxCommandEvent& event )
{
    SetStartDateTime(m_GribTimelineTime);
    Update();
}

void ConfigurationDialog::OnCurrentTime( wxCommandEvent& event )
{
    SetStartDateTime(wxDateTime::Now().ToUTC());
    Update();
}

void ConfigurationDialog::OnAvoidCyclones( wxCommandEvent& event )
{
    Update();
}

void ConfigurationDialog::OnBoatFilename( wxCommandEvent& event )
{
    wxFileDialog openDialog
        ( this, _( "Select Boat File" ), wxFileName(m_tBoat->GetValue()).GetPath(), wxT ( "" ),
          wxT ( "xml (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ), wxFD_OPEN  );

    if( openDialog.ShowModal() == wxID_OK )
        SetBoatFilename(openDialog.GetPath());
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
    std::list<RouteMapConfiguration>::iterator it = configurations.begin(); \
    TYPE value = VALUE; \
    for(it++; it != configurations.end(); it++) {                             \
        if(value != VALUE) {                                       \
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

    bool ult = m_WeatherRouting.m_SettingsDialog.m_cbUseLocalTime->GetValue();
#define STARTTIME (ult ? it->StartTime.FromUTC() : it->StartTime)

    SET_CONTROL_VALUE(STARTTIME, m_dpStartDate, SetValue, wxDateTime, wxDateTime());
    SET_CONTROL_VALUE(wxString::Format(_T("%.2f"), STARTTIME.GetHour()
                                       + (double)STARTTIME.GetMinute()/60.0),
                m_tStartHour, SetValue, wxString, _T(""));

    SET_SPIN_VALUE(TimeStepHours, (int)((*it).dt / 3600));
    SET_SPIN_VALUE(TimeStepMinutes, ((int)(*it).dt / 60) % 60);
    SET_SPIN_VALUE(TimeStepSeconds, (int)(*it).dt%60);

    SET_CONTROL(boatFileName, m_tBoat, SetValue, wxString, _T(""));

    SET_CHOICE(End);

    SET_SPIN(FromDegree);
    SET_SPIN(ToDegree);
    SET_CONTROL_VALUE(wxString::Format(_T("%f"), (*it).ByDegrees), m_tByDegrees, SetValue, wxString, _T(""));

    SET_CONTROL_VALUE(((*it).Integrator == RouteMapConfiguration::RUNGE_KUTTA ?
                       _T("Runge Kutta") : _T("Newton")), m_cIntegrator, SetValue, wxString, _T(""));

    SET_SPIN(MaxDivertedCourse);
    SET_SPIN(MaxCourseAngle);
    SET_SPIN(MaxSearchAngle);
    SET_SPIN(MaxTrueWindKnots);
    SET_SPIN(MaxApparentWindKnots);

    SET_SPIN(MaxSwellMeters);
    SET_SPIN(MaxLatitude);
    SET_SPIN(TackingTime);
    SET_SPIN(WindVSCurrent);

    SET_CHECKBOX(AvoidCycloneTracks);
    SET_SPIN(CycloneMonths);
    SET_SPIN(CycloneDays);

    SET_CHECKBOX(DetectLand);
    SET_CHECKBOX(DetectBoundary);
    SET_CHECKBOX(Currents);
    SET_CHECKBOX(InvertedRegions);
    SET_CHECKBOX(Anchoring);

    SET_CHECKBOX(UseGrib);
    SET_CONTROL(ClimatologyType, m_cClimatologyType, SetSelection, int, -1);
    SET_CHECKBOX(AllowDataDeficient);
    SET_SPIN_VALUE(WindStrength, (int)((*it).WindStrength * 100));

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

void ConfigurationDialog::SetBoatFilename(wxString path)
{
    m_tBoat->SetValue(path);
    Update();
}

void ConfigurationDialog::SetStartDateTime(wxDateTime datetime)
{
    if(datetime.IsValid()) {
        if(m_WeatherRouting.m_SettingsDialog.m_cbUseLocalTime->GetValue())
            datetime = datetime.FromUTC();

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

#define GET_CHOICE(FIELD) \
    if(!m_c##FIELD->GetValue().empty()) \
        configuration.FIELD = m_c##FIELD->GetValue();

void ConfigurationDialog::Update()
{
    if(m_bBlockUpdate)
        return;

    bool refresh = false;
    RouteMapConfiguration configuration;
    std::list<RouteMapOverlay*> currentroutemaps = m_WeatherRouting.CurrentRouteMaps();
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++) {

        configuration = (*it)->GetConfiguration();

        GET_CHOICE(Start);
        GET_CHOICE(End);

        if(m_dpStartDate->GetValue().IsValid())
            configuration.StartTime = m_dpStartDate->GetValue();

        if(!m_tStartHour->GetValue().empty()) {
            double hour;
            m_tStartHour->GetValue().ToDouble(&hour);
            configuration.StartTime.SetHour((int)hour);
            configuration.StartTime.SetMinute((int)(60*hour)%60);
        }

        if(m_WeatherRouting.m_SettingsDialog.m_cbUseLocalTime->GetValue())
            configuration.StartTime = configuration.StartTime.ToUTC();

        if(!m_tBoat->GetValue().empty())
            configuration.boatFileName = m_tBoat->GetValue();

        if(m_sTimeStepHours->IsEnabled()) {
            configuration.dt = 60*(60*m_sTimeStepHours->GetValue()
                                   + m_sTimeStepMinutes->GetValue())
                + m_sTimeStepSeconds->GetValue();
        }

        if(m_cIntegrator->GetValue() == _T("Newton"))
            configuration.Integrator = RouteMapConfiguration::NEWTON;
        else if(m_cIntegrator->GetValue() == _T("Runge Kutta"))
            configuration.Integrator = RouteMapConfiguration::RUNGE_KUTTA;

        GET_SPIN(MaxDivertedCourse);
        GET_SPIN(MaxCourseAngle);
        GET_SPIN(MaxSearchAngle);
        GET_SPIN(MaxTrueWindKnots);
        GET_SPIN(MaxApparentWindKnots);

        GET_SPIN(MaxSwellMeters);
        GET_SPIN(MaxLatitude);
        GET_SPIN(TackingTime);
        GET_SPIN(WindVSCurrent);

        GET_CHECKBOX(AvoidCycloneTracks);
        GET_SPIN(CycloneMonths);
        GET_SPIN(CycloneDays);

        GET_CHECKBOX(DetectLand);
        GET_CHECKBOX(DetectBoundary);
        GET_CHECKBOX(Currents);
        GET_CHECKBOX(InvertedRegions);
        GET_CHECKBOX(Anchoring);

        GET_CHECKBOX(UseGrib);
        if(m_cClimatologyType->GetSelection() != -1)
            configuration.ClimatologyType = (RouteMapConfiguration::ClimatologyDataType)
                m_cClimatologyType->GetSelection();
        GET_CHECKBOX(AllowDataDeficient);
        if(m_sWindStrength->IsEnabled())                     \
            configuration.WindStrength = m_sWindStrength->GetValue() / 100.0;

        GET_SPIN(FromDegree);
        GET_SPIN(ToDegree);
        if(!m_tByDegrees->GetValue().empty())
            m_tByDegrees->GetValue().ToDouble(&configuration.ByDegrees);

        (*it)->SetConfiguration(configuration);

        /* if the start position changed, we must reset the route */
        RouteMapConfiguration newc = (*it)->GetConfiguration();
        if(newc.StartLat != configuration.StartLat || newc.StartLon != configuration.StartLon) {
            (*it)->Reset();
            refresh = true;
        } else if(newc.EndLat != configuration.EndLat || newc.EndLon != configuration.EndLon)
            refresh = true; // update drawing
    }

    double by;
    m_tByDegrees->GetValue().ToDouble(&by);
    if(m_sToDegree->GetValue() - m_sFromDegree->GetValue() < 2*by) {
        wxMessageDialog mdlg(this, _("Warning: less than 4 different degree steps specified\n"),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
    }

    m_WeatherRouting.UpdateCurrentConfigurations();

    if(refresh)
        m_WeatherRouting.GetParent()->Refresh();
}
