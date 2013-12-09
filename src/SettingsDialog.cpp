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
#include "weather_routing_pi.h"

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

    bool ConcurrentThreads = m_sConcurrentThreads->GetValue();
    pConf->Write( _T("ConcurrentThreads"), ConcurrentThreads);

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "SettingsDialogX" ), p.x);
    pConf->Write ( _T ( "SettingsDialogY" ), p.y);
}
