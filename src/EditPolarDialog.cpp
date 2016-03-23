/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
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
 */

#include <wx/wx.h>

#include "weather_routing_pi.h"
#include "Boat.h"
#include "BoatDialog.h"
#include "EditPolarDialog.h"

enum {spWIND_SPEED, spWIND_DIRECTION, spBOAT_SPEED};
EditPolarDialog::EditPolarDialog(wxWindow *parent)
    : EditPolarDialogBase(parent),
      m_BoatDialog(static_cast<BoatDialog*>(parent))
{

    m_lMeasurements->InsertColumn(spWIND_SPEED, _("Wind Speed"));
    m_lMeasurements->InsertColumn(spWIND_DIRECTION, _("Wind Direction"));
    m_lMeasurements->InsertColumn(spBOAT_SPEED, _("Boat Speed"));
}

void EditPolarDialog::SetPolarIndex(int i)
{
    polarindex = i;
    RebuildTrueWindAngles();
    RebuildTrueWindSpeeds();
    RebuildGrid();
    m_lMeasurements->DeleteAllItems();
}

void EditPolarDialog::OnPolarGridChange( wxGridEvent& event )
{
    double VB;
    m_gPolar->GetCellValue(event.GetRow(), event.GetCol()).ToDouble(&VB);
    GetPolar()->wind_speeds[event.GetCol()].speeds[event.GetRow()] = VB;
    m_BoatDialog->Refresh();
}

void EditPolarDialog::OnAddTrueWindAngle( wxCommandEvent& event )
{
    double twa;
    m_tTrueWindAngle->GetValue().ToDouble(&twa);
    m_tTrueWindAngle->Clear();
    GetPolar()->AddDegreeStep(twa);
    RebuildTrueWindAngles();
    RebuildGrid();
}

void EditPolarDialog::OnRemoveTrueWindAngle( wxCommandEvent& event )
{
    GetPolar()->RemoveDegreeStep(m_lTrueWindAngles->GetSelection());
    RebuildTrueWindAngles();
    RebuildGrid();
}

void EditPolarDialog::OnAddTrueWindSpeed( wxCommandEvent& event )
{
    double tws;
    m_tTrueWindSpeed->GetValue().ToDouble(&tws);
    m_tTrueWindSpeed->Clear();
    GetPolar()->AddWindSpeed(tws);
    RebuildTrueWindSpeeds();
    RebuildGrid();
}

void EditPolarDialog::OnRemoveTrueWindSpeed( wxCommandEvent& event )
{
    GetPolar()->RemoveWindSpeed(m_lTrueWindSpeeds->GetSelection());
    RebuildTrueWindSpeeds();
    RebuildGrid();
}

void EditPolarDialog::OnAddMeasurement( wxCommandEvent& event )
{
    wxListItem info;
    info.SetId(m_lMeasurements->GetItemCount());
//        info.SetData(i);
    long idx = m_lMeasurements->InsertItem(info);
    m_lMeasurements->SetItem(idx, spWIND_SPEED, m_tWindSpeed->GetValue());
    m_lMeasurements->SetItem(idx, spWIND_DIRECTION, m_tWindDirection->GetValue());
    m_lMeasurements->SetItem(idx, spBOAT_SPEED, m_tBoatSpeed->GetValue());
}

void EditPolarDialog::OnRemoveMeasurement( wxCommandEvent& event )
{
    long index = m_lMeasurements->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if(index == -1)
        return;

    m_lMeasurements->DeleteItem(index);
}

void EditPolarDialog::OnRemoveAllMeasurements( wxCommandEvent& event )
{
    m_lMeasurements->DeleteAllItems();
}

void EditPolarDialog::OnGeneratePolar( wxCommandEvent& event )
{
    std::list<PolarMeasurement> measurements;
    for(int i = 0; i<m_lMeasurements->GetItemCount(); i++) {
        double v[3];
        for(int j=0; j<3; j++)
            m_lMeasurements->GetItemText(i, j).ToDouble(v+j);
        PolarMeasurement m(v[0], v[1], v[2]);
        measurements.push_back(m);
    }
    GetPolar()->Generate(measurements);
    RebuildGrid();
}

void EditPolarDialog::RebuildTrueWindAngles()
{
    m_lTrueWindAngles->Clear();

    if(!GetPolar())
        return;

    for(unsigned int i = 0; i<GetPolar()->degree_steps.size(); i++)
        m_lTrueWindAngles->Append
            (wxString::Format(_T("%4.1f"), GetPolar()->degree_steps[i]));
}

void EditPolarDialog::RebuildTrueWindSpeeds()
{
    m_lTrueWindSpeeds->Clear();

    if(!GetPolar())
        return;

    for(unsigned int i = 0; i<GetPolar()->wind_speeds.size(); i++)
        m_lTrueWindSpeeds->Append
            (wxString::Format(_T("%4.1f"), GetPolar()->wind_speeds[i].VW));
}

void EditPolarDialog::RebuildGrid()
{
    if(m_gPolar->GetNumberRows())
        m_gPolar->DeleteRows(0, m_gPolar->GetNumberRows());
    if(m_gPolar->GetNumberCols())
        m_gPolar->DeleteCols(0, m_gPolar->GetNumberCols());

    if(!GetPolar())
        return;

    m_gPolar->InsertRows(0, GetPolar()->degree_steps.size());
    for(unsigned int i = 0; i<GetPolar()->degree_steps.size(); i++)
        m_gPolar->SetRowLabelValue
            ( i, wxString::Format(_T("%4.1f"), GetPolar()->degree_steps[i]));
        
    m_gPolar->InsertCols(0, GetPolar()->wind_speeds.size());
    for(unsigned int i = 0; i<GetPolar()->wind_speeds.size(); i++) {
        m_gPolar->SetColLabelValue
            ( i, wxString::Format(_T("%4.1f"), GetPolar()->wind_speeds[i].VW));

        for(unsigned int j = 0; j<GetPolar()->degree_steps.size(); j++)
            m_gPolar->SetCellValue
                (j, i, wxString::Format(_T("%4.1f"), GetPolar()->wind_speeds[i].speeds[j]));
    }

    m_BoatDialog->Refresh();
}

Polar *EditPolarDialog::GetPolar()
{
    return polarindex == -1 ? NULL : &m_BoatDialog->m_Boat.Polars[polarindex];
}
