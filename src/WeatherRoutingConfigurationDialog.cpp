/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Weather Routing Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier   *
 *   sean@depagnier.com   *
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

#include "wx/wx.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "BoatSpeed.h"
#include "RouteMap.h"
#include "WeatherRoutingConfigurationDialog.h"
#include "weather_routing_pi.h"

#include "Utilities.h"

WeatherRoutingConfigurationDialog::WeatherRoutingConfigurationDialog( wxWindow *parent )
    : WeatherRoutingConfigurationDialogBase(parent)
{
    LoadConfiguration();
}

WeatherRoutingConfigurationDialog::~WeatherRoutingConfigurationDialog( )
{
}

void WeatherRoutingConfigurationDialog::LoadConfiguration()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxString degreesteps;
    m_lDegreeSteps->Clear();
    pConf->Read( _T("DegreeSteps"), &degreesteps, _T("40;60;90;120;150;210;240;270;300;320;"));
    while(degreesteps.size()) {
        m_lDegreeSteps->Append(degreesteps.BeforeFirst(';'));
        degreesteps = degreesteps.AfterFirst(';');
    }
 
    int timespan;
    pConf->Read( _T("TimeSpan"), &timespan, 1000);
    m_sTimeStep->SetValue(timespan);

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

    int tackingtime;
    pConf->Read( _T("TackingTime"), &tackingtime, 0);
    m_sTackingTime->SetValue(tackingtime);

    int substeps;
    pConf->Read( _T("SubSteps"), &substeps, 90);
    m_sSubSteps->SetValue(substeps);

    bool detectland;
    pConf->Read( _T("DetectLand"), &detectland, true);
    m_cbDetectLand->SetValue(detectland);

    bool invertedregions;
    pConf->Read( _T("InvertedRegions"), &invertedregions, false);
    m_cbInvertedRegions->SetValue(invertedregions);

    bool anchoring;
    pConf->Read( _T("Anchoring"), &anchoring, false);
    m_cbAnchoring->SetValue(anchoring);

    bool allowdatadeficient;
    pConf->Read( _T("AllowDataDeficient"), &allowdatadeficient, false);
    m_cbAllowDataDeficient->SetValue(allowdatadeficient);

    wxPoint p = GetPosition();
    pConf->Read ( _T ( "ConfigurationDialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "ConfigurationDialogY" ), &p.y, p.y);
    SetPosition(p);
}

void WeatherRoutingConfigurationDialog::SaveConfiguration( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxString degreesteps;
    for(unsigned int i=0; i<m_lDegreeSteps->GetCount(); i++)
        degreesteps += m_lDegreeSteps->GetString(i) + _(";");
    pConf->Write( _T("DegreeSteps"), degreesteps);

    pConf->Write( _T("TimeSpan"), m_sTimeStep->GetValue());
    pConf->Write( _T("MaxDivertedCourse"), m_sMaxDivertedCourse->GetValue());
    pConf->Write( _T("MaxWindKnots"), m_sMaxWindKnots->GetValue());
    pConf->Write( _T("MaxSwellMeters"), m_sMaxSwellMeters->GetValue());
    pConf->Write( _T("MaxLatitude"), m_sMaxLatitude->GetValue());
    pConf->Write( _T("TackingTime"), m_sTackingTime->GetValue());
    pConf->Write( _T("SubSteps"), m_sSubSteps->GetValue());

    pConf->Write( _T("DetectLand"), m_cbDetectLand->GetValue());
    pConf->Write( _T("InvertedRegions"), m_cbInvertedRegions->GetValue());
    pConf->Write( _T("Anchoring"), m_cbAnchoring->GetValue());
    pConf->Write( _T("AllowDataDeficient"), m_cbAllowDataDeficient->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "ConfigurationDialogX" ), p.x);
    pConf->Write ( _T ( "ConfigurationDialogY" ), p.y);
}

void WeatherRoutingConfigurationDialog::UpdateOptions(RouteMapOptions &options)
{
    options.dt = m_sTimeStep->GetValue();

    if(m_lDegreeSteps->GetCount() < 4) {
        wxMessageDialog mdlg(this, _("Warning: less than 4 degree steps specified\n"),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
    }

    options.DegreeSteps.clear();
    for(unsigned int i=0; i<m_lDegreeSteps->GetCount(); i++) {
        double step;
        m_lDegreeSteps->GetString(i).ToDouble(&step);
        options.DegreeSteps.push_back(positive_degrees(step));
    }
    options.DegreeSteps.sort();

    options.MaxDivertedCourse = m_sMaxDivertedCourse->GetValue();
    options.MaxWindKnots= m_sMaxWindKnots->GetValue();
    options.MaxSwellMeters = m_sMaxSwellMeters->GetValue();
    options.MaxLatitude = m_sMaxLatitude->GetValue();
    options.TackingTime = m_sTackingTime->GetValue();
    options.SubSteps = m_sSubSteps->GetValue();

    options.DetectLand = m_cbDetectLand->GetValue();
    options.InvertedRegions = m_cbInvertedRegions->GetValue();
    options.Anchoring = m_cbAnchoring->GetValue();
    options.AllowDataDeficient = m_cbAllowDataDeficient->GetValue();
}

void WeatherRoutingConfigurationDialog::OnAddDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        s = m_lDegreeSteps->GetCount();

    double step;
    m_tDegreeStep->GetValue().ToDouble(&step);
    m_lDegreeSteps->Insert(wxString::Format(_T("%f"), step), s);
    m_tDegreeStep->Clear();
}

void WeatherRoutingConfigurationDialog::OnRemoveDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        return;

    m_lDegreeSteps->Delete(s);
    m_lDegreeSteps->SetSelection(s);
}

void WeatherRoutingConfigurationDialog::OnGenerateDegreeSteps( wxCommandEvent& event )
{
    double value, v;
    m_tDegreeStep->GetValue().ToDouble(&value);

    if(value <= 0 || value > 180) {
        wxMessageDialog mdlg(this, _("Invalid step size, nothing will be done."),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
        return;
    }

    m_lDegreeSteps->Clear();
    for(v = value; v < 360; v+=value)
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), v));
}

void WeatherRoutingConfigurationDialog::OnClearDegreeSteps( wxCommandEvent& event )
{
    m_lDegreeSteps->Clear();
}
