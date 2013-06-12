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

#include "../../../include/tinyxml.h"

#include "Utilities.h"
#include "Boat.h"
#include "RouteMap.h"
#include "ConfigurationDialog.h"
#include "ConfigurationBatchDialog.h"
#include "BoatDialog.h"
#include "weather_routing_pi.h"

ConfigurationDialog::ConfigurationDialog( wxWindow *parent, weather_routing_pi &plugin )
    : ConfigurationDialogBase(parent), Plugin(plugin)
{
}

ConfigurationDialog::~ConfigurationDialog( )
{
}

#if 0
void ConfigurationDialog::Load()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    m_tStartLat->SetValue
        (pConf->Read( _T("StartLat"), wxString::Format(_T("%.5f"), Plugin.m_boat_lat)));
    m_tStartLon->SetValue
        (pConf->Read( _T("StartLon"), wxString::Format(_T("%.5f"), Plugin.m_boat_lon)));
    m_tEndLat->SetValue
        (pConf->Read( _T("EndLat"), wxString::Format(_T("%.5f"), Plugin.m_boat_lat+1)));
    m_tEndLon->SetValue
        (pConf->Read( _T("EndLon"), wxString::Format(_T("%.5f"), Plugin.m_boat_lon+1)));

    wxString degreesteps;
    m_lDegreeSteps->Clear();
    pConf->Read( _T("DegreeSteps"), &degreesteps, _T("\
40;50;60;70;80;90;105;120;135;150;165;195;210;225;240;255;270;280;290;300;310;320;"));
    while(degreesteps.size()) {
        m_lDegreeSteps->Append(degreesteps.BeforeFirst(';'));
        degreesteps = degreesteps.AfterFirst(';');
    }
 
    int maxdivertedcourse;
    pConf->Read( _T("MaxDivertedCourse"), &maxdivertedcourse, 100);
    m_sMaxDivertedCourse->SetValue(maxdivertedcourse);

    int maxwindknots;
    pConf->Read( _T("MaxWindKnots"), &maxwindknots, 100);
    m_sMaxWindKnots->SetValue(maxwindknots);

    int maxswellmeters;
    pConf->Read( _T("MaxSwellMeters"), &maxswellmeters, 20);
    m_sMaxSwellMeters->SetValue(maxswellmeters);

    int maxlatitude;
    pConf->Read( _T("MaxLatitude"), &maxlatitude, 90);
    m_sMaxLatitude->SetValue(maxlatitude);

    int maxtacks;
    pConf->Read( _T("MaxTacks"), &maxtacks, -1);
    m_sMaxLatitude->SetValue(maxtacks);

    int tackingtime;
    pConf->Read( _T("TackingTime"), &tackingtime, 0);
    m_sTackingTime->SetValue(tackingtime);

    bool usegrib;
    pConf->Read( _T("UseGrib"), &usegrib, false);
    m_cbUseGrib->SetValue(usegrib);

    bool useclimatology;
    pConf->Read( _T("UseClimatology"), &useclimatology, false);
    m_cbUseClimatology->SetValue(useclimatology);

    bool allowdatadeficient;
    pConf->Read( _T("AllowDataDeficient"), &allowdatadeficient, false);
    m_cbAllowDataDeficient->SetValue(allowdatadeficient);

    bool detectland;
    pConf->Read( _T("DetectLand"), &detectland, true);
    m_cbDetectLand->SetValue(detectland);

    bool currents;
    pConf->Read( _T("Currents"), &currents, true);
    m_cbCurrents->SetValue(currents);

    bool invertedregions;
    pConf->Read( _T("InvertedRegions"), &invertedregions, false);
    m_cbInvertedRegions->SetValue(invertedregions);

    bool anchoring;
    pConf->Read( _T("Anchoring"), &anchoring, false);
    m_cbAnchoring->SetValue(anchoring);

    int timestephours;
    pConf->Read( _T("TimeStepHours"), &timestephours, 1);
    m_sTimeStepHours->SetValue(timestephours);

    int timestepminutes;
    pConf->Read( _T("TimeStepMinutes"), &timestepminutes, 0);
    m_sTimeStepMinutes->SetValue(timestepminutes);

    int timestepseconds;
    pConf->Read( _T("TimeStepSeconds"), &timestepseconds, 0);
    m_sTimeStepSeconds->SetValue(timestepseconds);

    wxPoint p = GetPosition();
    pConf->Read ( _T ( "ConfigurationX" ), &p.x, p.x);
    pConf->Read ( _T ( "ConfigurationY" ), &p.y, p.y);
    SetPosition(p);
}

void ConfigurationDialog::Save( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    pConf->Write( _T("StartLat"), m_tStartLat->GetValue());
    pConf->Write( _T("StartLon"), m_tStartLon->GetValue());
    pConf->Write( _T("EndLat"), m_tEndLat->GetValue());
    pConf->Write( _T("EndLon"), m_tEndLon->GetValue());

    wxString degreesteps;
    for(unsigned int i=0; i<m_lDegreeSteps->GetCount(); i++)
        degreesteps += m_lDegreeSteps->GetString(i) + _(";");
    pConf->Write( _T("DegreeSteps"), degreesteps);

    pConf->Write( _T("MaxDivertedCourse"), m_sMaxDivertedCourse->GetValue());
    pConf->Write( _T("MaxWindKnots"), m_sMaxWindKnots->GetValue());
    pConf->Write( _T("MaxSwellMeters"), m_sMaxSwellMeters->GetValue());
    pConf->Write( _T("MaxLatitude"), m_sMaxLatitude->GetValue());
    pConf->Write( _T("MaxTacks"), m_sMaxTacks->GetValue());
    pConf->Write( _T("TackingTime"), m_sTackingTime->GetValue());

    pConf->Write( _T("UseGrib"), m_cbUseGrib->GetValue());
    pConf->Write( _T("UseClimatology"), m_cbUseClimatology->GetValue());
    pConf->Write( _T("AllowDataDeficient"), m_cbAllowDataDeficient->GetValue());

    pConf->Write( _T("DetectLand"), m_cbDetectLand->GetValue());
    pConf->Write( _T("Currents"), m_cbCurrents->GetValue());
    pConf->Write( _T("InvertedRegions"), m_cbInvertedRegions->GetValue());
    pConf->Write( _T("Anchoring"), m_cbAnchoring->GetValue());

    pConf->Write( _T("TimeStepHours"), m_sTimeStepHours->GetValue());
    pConf->Write( _T("TimeStepMinutes"), m_sTimeStepMinutes->GetValue());
    pConf->Write( _T("TimeStepSeconds"), m_sTimeStepSeconds->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "ConfigurationX" ), p.x);
    pConf->Write ( _T ( "ConfigurationY" ), p.y);
}
#endif

void ConfigurationDialog::OnBoatPosition( wxCommandEvent& event )
{
    m_tStartLat->SetValue(wxString::Format(_T("%.5f"), Plugin.m_boat_lat));
    m_tStartLon->SetValue(wxString::Format(_T("%.5f"), Plugin.m_boat_lon));
}

void ConfigurationDialog::OnGribTime( wxCommandEvent& event )
{
    SetStartDateTime(m_GribTimelineTime);
}

void ConfigurationDialog::OnCurrentTime( wxCommandEvent& event )
{
    SetStartDateTime(wxDateTime::Now());
}

void ConfigurationDialog::OnEditBoat ( wxCommandEvent& event )
{
    BoatDialog boatdlg(this);
    boatdlg.ShowModal();
}

void ConfigurationDialog::OnAddDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        s = m_lDegreeSteps->GetCount();

    double step;
    m_tDegreeStep->GetValue().ToDouble(&step);
    m_lDegreeSteps->Insert(wxString::Format(_T("%f"), step), s);
    m_tDegreeStep->Clear();
}

void ConfigurationDialog::OnRemoveDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        return;

    m_lDegreeSteps->Delete(s);
    m_lDegreeSteps->SetSelection(s);
}

void ConfigurationDialog::OnClearDegreeSteps( wxCommandEvent& event )
{
    m_lDegreeSteps->Clear();
}

void ConfigurationDialog::OnGenerateDegreeSteps( wxCommandEvent& event )
{
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

    m_lDegreeSteps->Clear();
    for(double v = from; v <= to; v+=by) {
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), v));
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), -v));
    }
}

void ConfigurationDialog::SetConfiguration(RouteMapConfiguration configuration)
{
    m_tName->SetValue(configuration.Name);

    m_tStartLat->SetValue(wxString::Format(_T("%.4f"), &configuration.StartLat));
    m_tStartLon->SetValue(wxString::Format(_T("%.4f"), &configuration.StartLon));

    m_dpStartDate->SetValue(configuration.StartTime);
    m_tStartHour->SetValue(wxString::Format(_T("%.2f"),
                                            configuration.StartTime.GetHour()
                                            + (double)configuration.StartTime.GetMinute()/60.0));

    m_fpBoat->SetPath(configuration.boatFileName);

    int dt = configuration.dt;
    int hours = dt / 3600;
    m_sTimeStepHours->SetValue(wxString::Format(_T("%d"), hours));
    dt -= hours * 3600;
    int minutes = dt / 60;
    m_sTimeStepMinutes->SetValue(wxString::Format(_T("%d"), minutes));
    dt -= minutes * 60;
    m_sTimeStepSeconds->SetValue(wxString::Format(_T("%d"), dt));

    m_tEndLat->SetValue(wxString::Format(_T("%.4f"), &configuration.EndLat));
    m_tEndLon->SetValue(wxString::Format(_T("%.4f"), &configuration.EndLon));

    m_lDegreeSteps->Clear();
    for(std::list<double>::iterator it = configuration.DegreeSteps.begin();
        it != configuration.DegreeSteps.end(); it++)
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), *it));

    m_sMaxDivertedCourse->SetValue(configuration.MaxDivertedCourse);
    m_sMaxWindKnots->SetValue(configuration.MaxWindKnots);
    m_sMaxSwellMeters->SetValue(configuration.MaxSwellMeters);
    m_sMaxLatitude->SetValue(configuration.MaxLatitude);
    m_sMaxTacks->SetValue(configuration.MaxTacks);
    m_sTackingTime->SetValue(configuration.TackingTime);

    m_cbDetectLand->SetValue(configuration.DetectLand);
    m_cbCurrents->SetValue(configuration.Currents);
    m_cbInvertedRegions->SetValue(configuration.InvertedRegions);
    m_cbAnchoring->SetValue(configuration.Anchoring);

    m_cbAllowDataDeficient->SetValue(configuration.AllowDataDeficient);

    m_cbUseGrib->SetValue(configuration.UseGrib);
    m_cbUseClimatology->SetValue(configuration.UseClimatology);
}

RouteMapConfiguration ConfigurationDialog::Configuration()
{
    RouteMapConfiguration configuration;

    configuration.Name = m_tName->GetValue();

    m_tStartLat->GetValue().ToDouble(&configuration.StartLat);
    m_tStartLon->GetValue().ToDouble(&configuration.StartLon);

    configuration.StartTime = m_dpStartDate->GetValue();
    double hour;
    m_tStartHour->GetValue().ToDouble(&hour);
    configuration.StartTime.SetHour((int)hour);
    configuration.StartTime.SetMinute((int)(60*hour)%60);

    configuration.boatFileName = m_fpBoat->GetPath();

    configuration.dt = 60*(60*m_sTimeStepHours->GetValue()
                     + m_sTimeStepMinutes->GetValue())
        + m_sTimeStepSeconds->GetValue();
    if(configuration.dt == 0) {
        wxMessageDialog mdlg(this, _("Zero Time Step invalid"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }

    m_tEndLat->GetValue().ToDouble(&configuration.EndLat);
    m_tEndLon->GetValue().ToDouble(&configuration.EndLon);

    if(m_lDegreeSteps->GetCount() < 4) {
        wxMessageDialog mdlg(this, _("Warning: less than 4 different degree steps specified\n"),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
    }

    configuration.DegreeSteps.clear();
    for(unsigned int i=0; i<m_lDegreeSteps->GetCount(); i++) {
        double step;
        m_lDegreeSteps->GetString(i).ToDouble(&step);
        configuration.DegreeSteps.push_back(positive_degrees(step));
    }
    configuration.DegreeSteps.sort();

    configuration.MaxDivertedCourse = m_sMaxDivertedCourse->GetValue();
    configuration.MaxWindKnots = m_sMaxWindKnots->GetValue();
    configuration.MaxSwellMeters = m_sMaxSwellMeters->GetValue();
    configuration.MaxLatitude = m_sMaxLatitude->GetValue();
    configuration.MaxTacks = m_sMaxTacks->GetValue();
    configuration.TackingTime = m_sTackingTime->GetValue();

    configuration.DetectLand = m_cbDetectLand->GetValue();
    configuration.Currents = m_cbCurrents->GetValue();
    configuration.InvertedRegions = m_cbInvertedRegions->GetValue();
    configuration.Anchoring = m_cbAnchoring->GetValue();

    configuration.AllowDataDeficient = m_cbAllowDataDeficient->GetValue();

    configuration.UseGrib = m_cbUseGrib->GetValue();
    configuration.UseClimatology = m_cbUseClimatology->GetValue();
//    configuration.boat = m_pBoatDialog->m_Boat;

//    m_RouteMapOverlay.SetConfiguration(configuration);
//    m_RouteMapOverlay.Reset(time);
    return configuration;
}

void ConfigurationDialog::SetStartDateTime(wxDateTime datetime)
{
    m_dpStartDate->SetValue(datetime);
    m_tStartHour->SetValue(wxString::Format(_T("%.3f"), datetime.GetHour()
                                            +datetime.GetMinute() / 60.0));
}
