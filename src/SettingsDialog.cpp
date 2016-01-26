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

#include "SettingsDialog.h"
#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"

SettingsDialog::SettingsDialog( wxWindow *parent )
    : SettingsDialogBase(parent)
{
}

void SettingsDialog::LoadSettings()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxString CursorColorStr = m_cpCursorRoute->GetColour().GetAsString();
    pConf->Read( _T("CursorColor"), &CursorColorStr, CursorColorStr);
    m_cpCursorRoute->SetColour(wxColour(CursorColorStr));

    wxString DestinationColorStr = m_cpDestinationRoute->GetColour().GetAsString();
    pConf->Read( _T("DestinationColor"), &DestinationColorStr, DestinationColorStr);
    m_cpDestinationRoute->SetColour(wxColour(DestinationColorStr));

    int RouteThickness = m_sRouteThickness->GetValue();
    pConf->Read( _T("RouteThickness"), &RouteThickness, RouteThickness);
    m_sRouteThickness->SetValue(RouteThickness);

    int IsoChronThickness = m_sIsoChronThickness->GetValue();
    pConf->Read( _T("IsoChronThickness"), &IsoChronThickness, IsoChronThickness);
    m_sIsoChronThickness->SetValue(IsoChronThickness);

    int AlternateRouteThickness = m_sAlternateRouteThickness->GetValue();
    pConf->Read( _T("AlternateRouteThickness"), &AlternateRouteThickness, AlternateRouteThickness);
    m_sAlternateRouteThickness->SetValue(AlternateRouteThickness);

    bool AlternatesForAll = m_cbAlternatesForAll->GetValue();
    pConf->Read( _T("AlternatesForAll"), &AlternatesForAll, AlternatesForAll);
    m_cbAlternatesForAll->SetValue(AlternatesForAll);

    bool MarkAtPolarChange = m_cbMarkAtPolarChange->GetValue();
    pConf->Read( _T("MarkAtPolarChange"), &MarkAtPolarChange, MarkAtPolarChange);
    m_cbMarkAtPolarChange->SetValue(MarkAtPolarChange);
    
    bool DisplayWindBarbs = m_cbDisplayWindBarbs->GetValue();
    pConf->Read( _T("DisplayWindBarbs"), &DisplayWindBarbs, DisplayWindBarbs);
    m_cbDisplayWindBarbs->SetValue(DisplayWindBarbs);

    bool DisplayCurrent = m_cbDisplayCurrent->GetValue();
    pConf->Read( _T("DisplayCurrent"), &DisplayCurrent, DisplayCurrent);
    m_cbDisplayCurrent->SetValue(DisplayCurrent);

    int ConcurrentThreads = wxThread::GetCPUCount();
    pConf->Read( _T("ConcurrentThreads"), &ConcurrentThreads, ConcurrentThreads);
    m_sConcurrentThreads->SetValue(ConcurrentThreads);

    // set defaults
    bool columns[WeatherRouting::NUM_COLS];
    for(int i=0; i<WeatherRouting::NUM_COLS; i++)
        columns[i] = i <= WeatherRouting::DISTANCE || i == WeatherRouting::STATE;

    for(int i=0; i<WeatherRouting::NUM_COLS; i++) {
        if(i == 0)
            m_cblFields->Append(_("Visible"));
        else
            m_cblFields->Append(column_names[i]);
        pConf->Read( wxString::Format(_T("Column%d"), i), &columns[i], columns[i]);
        m_cblFields->Check(i, columns[i]);
    }

    m_cbUseLocalTime->SetValue((bool)pConf->Read( _T("UseLocalTime"), 0L));

    Fit();
    
    wxPoint p = GetPosition();
    pConf->Read ( _T ( "SettingsDialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "SettingsDialogY" ), &p.y, p.y);
    SetPosition(p);
}

void SettingsDialog::SaveSettings( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    pConf->Write( _T("CursorColor"), m_cpCursorRoute->GetColour().GetAsString());
    pConf->Write( _T("DestinationColor"), m_cpDestinationRoute->GetColour().GetAsString());
    pConf->Write( _T("RouteThickness"), m_sRouteThickness->GetValue());
    pConf->Write( _T("IsoChronThickness"), m_sIsoChronThickness->GetValue());
    pConf->Write( _T("AlternateRouteThickness"), m_sAlternateRouteThickness->GetValue());
    pConf->Write( _T("AlternatesForAll"), m_cbAlternatesForAll->GetValue());
    pConf->Write( _T("MarkAtPolarChange"), m_cbMarkAtPolarChange->GetValue());
    pConf->Write( _T("DisplayWindBarbs"), m_cbDisplayWindBarbs->GetValue());
    pConf->Write( _T("DisplayCurrent"), m_cbDisplayCurrent->GetValue());

    pConf->Write( _T("ConcurrentThreads"), m_sConcurrentThreads->GetValue());

    for(int i=0; i<WeatherRouting::NUM_COLS; i++)
        pConf->Write( wxString::Format(_T("Column%d"), i), m_cblFields->IsChecked(i));

    pConf->Write( _T("UseLocalTime"), m_cbUseLocalTime->GetValue());

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "SettingsDialogX" ), p.x);
    pConf->Write ( _T ( "SettingsDialogY" ), p.y);
}

void SettingsDialog::OnUpdate( )
{
    WeatherRouting *weather_routing = dynamic_cast<WeatherRouting*>(GetParent());
    if(weather_routing)
        weather_routing->UpdateDisplaySettings();
}

void SettingsDialog::OnUpdateColumns( wxCommandEvent& event )
{
    WeatherRouting *weather_routing = dynamic_cast<WeatherRouting*>(GetParent());
    if(weather_routing)
        weather_routing->UpdateColumns();
}

void SettingsDialog::OnHelp( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Cursor Route -- optimal route closest to the cursor\n\
Destination Route -- optimal route to the desired destination\n\
Route Thickness -- thickness to draw Cursor and Destination Routes\n\
Iso Chron Thickness -- thickness for isochrons on map\n\
Alternate Routes Thickness -- thickness for alternate routes\n\
Note: All thicknesses can be set to 0 to disable their display\n\
Alternates for all Isochrons -- display all alternate routes not only the ones \
which reach the last isochron\n\
Squares At Sail Changes -- render squares along Routes whenever a sail change is made\n\
Filter Routes by Climatology -- This currently does nothing, but I intended to \
make weather route maps which derive data from grib and climatology clearly render which \
data was used where \n\
\n\
Number of Concurrent Threads -- if there are multiple configurations, they can \
be computed in separate threads which allows a speedup if there are multiple processors\n"),
                         _("Weather Routing"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}
