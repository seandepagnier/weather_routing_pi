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
#include "wx/tokenzr.h"
#include "wx/datetime.h"
#include "wx/sound.h"
#include <wx/wfstream.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/debug.h>
#include <wx/graphics.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "BoatSpeed.h"
#include "BoatDialog.h"
#include "RouteMap.h"
#include "weather_routing_pi.h"

#include "Utilities.h"

WeatherRoutingSettingsDialog::WeatherRoutingSettingsDialog( wxWindow *parent, RouteMap &routemap )
    : WeatherRoutingSettingsDialogBase(parent), m_routemap(routemap)
{
    LoadSettings();
}

WeatherRoutingSettingsDialog::~WeatherRoutingSettingsDialog( )
{
}

void WeatherRoutingSettingsDialog::LoadSettings()
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
    pConf->Read( _T("TackingTime"), &tackingtime, 10);
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
    pConf->Read ( _T ( "SettingsDialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "SettingsDialogY" ), &p.y, p.y);
    SetPosition(p);
}

void WeatherRoutingSettingsDialog::SaveSettings( )
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
    pConf->Write( _T("MaxLatittude"), m_sMaxLatitude->GetValue());
    pConf->Write( _T("TackingTime"), m_sTackingTime->GetValue());
    pConf->Write( _T("SubSteps"), m_sSubSteps->GetValue());

    pConf->Write( _T("DetectLand"), m_cbDetectLand->GetValue());
    pConf->Write( _T("InvertedRegions"), m_cbInvertedRegions->GetValue());
    pConf->Write( _T("Anchoring"), m_cbAnchoring->GetValue());
    pConf->Write( _T("AllowDataDeficient"), m_cbAllowDataDeficient->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "SettingsDialogX" ), p.x);
    pConf->Write ( _T ( "SettingsDialogY" ), p.y);
}

void WeatherRoutingSettingsDialog::ReconfigureRouteMap()
{
    m_routemap.dt = wxTimeSpan(0, 0, m_sTimeStep->GetValue());

    if(m_lDegreeSteps->GetCount() < 4) {
        wxMessageDialog mdlg(this, _("Warning: less than 4 degree steps specified\n"),
                             wxString(_("Weather Routing"), wxOK | wxICON_WARNING));
        mdlg.ShowModal();
    }

    m_routemap.DegreeSteps.clear();
    for(unsigned int i=0; i<m_lDegreeSteps->GetCount(); i++) {
        double step;
        m_lDegreeSteps->GetString(i).ToDouble(&step);
        m_routemap.DegreeSteps.push_back(positive_degrees(step));
    }
    m_routemap.DegreeSteps.sort();

    m_routemap.MaxDivertedCourse = m_sMaxDivertedCourse->GetValue();
    m_routemap.MaxWindKnots= m_sMaxWindKnots->GetValue();
    m_routemap.MaxSwellMeters = m_sMaxSwellMeters->GetValue();
    m_routemap.MaxLatitude = m_sMaxLatitude->GetValue();
    m_routemap.TackingTime = m_sTackingTime->GetValue();
    m_routemap.SubSteps = m_sSubSteps->GetValue();

    m_routemap.DetectLand = m_cbDetectLand->GetValue();
    m_routemap.InvertedRegions = m_cbInvertedRegions->GetValue();
    m_routemap.Anchoring = m_cbAnchoring->GetValue();
    m_routemap.AllowDataDeficient = m_cbAllowDataDeficient->GetValue();
}

void WeatherRoutingSettingsDialog::OnAddDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s == -1)
        s = m_lDegreeSteps->GetCount();

    double step;
    m_tDegreeStep->GetValue().ToDouble(&step);
    m_lDegreeSteps->Insert(wxString::Format(_T("%f"), step), s);
    m_tDegreeStep->Clear();
}

void WeatherRoutingSettingsDialog::OnRemoveDegreeStep( wxCommandEvent& event )
{
    int s = m_lDegreeSteps->GetSelection();
    if(s != -1)
        m_lDegreeSteps->Delete(s);
}

void WeatherRoutingSettingsDialog::OnGenerateDegreeSteps( wxCommandEvent& event )
{
    double value, v;
    m_tDegreeStep->GetValue().ToDouble(&value);
    m_lDegreeSteps->Clear();
    for(v = value; v < 360; v+=value)
        m_lDegreeSteps->Append(wxString::Format(_T("%.1f"), v));
}

void WeatherRoutingSettingsDialog::OnClearDegreeSteps( wxCommandEvent& event )
{
    m_lDegreeSteps->Clear();
}
