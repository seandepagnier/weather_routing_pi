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

#include "SettingsDialog.h"
#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"

#include "Utilities.h"

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

    bool SquaresAtSailChanges = m_cbSquaresAtSailChanges->GetValue();
    pConf->Read( _T("SquaresAtSailChanges"), &SquaresAtSailChanges, SquaresAtSailChanges);
    m_cbSquaresAtSailChanges->SetValue(SquaresAtSailChanges);

    bool FilterbyClimatology = m_cbFilterbyClimatology->GetValue();
    pConf->Read( _T("FilterbyClimatology"), &FilterbyClimatology, FilterbyClimatology);
    m_cbFilterbyClimatology->SetValue(FilterbyClimatology);

    int ConcurrentThreads = m_sConcurrentThreads->GetValue();
    pConf->Read( _T("ConcurrentThreads"), &ConcurrentThreads, ConcurrentThreads);
    m_sConcurrentThreads->SetValue(ConcurrentThreads);
    
    wxPoint p = GetPosition();
    pConf->Read ( _T ( "SettingsDialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "SettingsDialogY" ), &p.y, p.y);
    SetPosition(p);
}

void SettingsDialog::SaveSettings( )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxString CursorColorStr = m_cpCursorRoute->GetColour().GetAsString();
    pConf->Write( _T("CursorColor"), CursorColorStr);

    wxString DestinationColorStr = m_cpDestinationRoute->GetColour().GetAsString();
    pConf->Write( _T("DestinationColor"), DestinationColorStr);

    int RouteThickness = m_sRouteThickness->GetValue();
    pConf->Write( _T("RouteThickness"), RouteThickness);

    int IsoChronThickness = m_sIsoChronThickness->GetValue();
    pConf->Write( _T("IsoChronThickness"), IsoChronThickness);

    int AlternateRouteThickness = m_sAlternateRouteThickness->GetValue();
    pConf->Write( _T("AlternateRouteThickness"), AlternateRouteThickness);

    bool AlternatesForAll = m_cbAlternatesForAll->GetValue();
    pConf->Write( _T("AlternatesForAll"), AlternatesForAll);

    bool SquaresAtSailChanges = m_cbSquaresAtSailChanges->GetValue();
    pConf->Write( _T("SquaresAtSailChanges"), SquaresAtSailChanges);

    bool FilterbyClimatology = m_cbFilterbyClimatology->GetValue();
    pConf->Write( _T("FilterbyClimatology"), FilterbyClimatology);

    bool ConcurrentThreads = m_sConcurrentThreads->GetValue();
    pConf->Write( _T("ConcurrentThreads"), ConcurrentThreads);

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
