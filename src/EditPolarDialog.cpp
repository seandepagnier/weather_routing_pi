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
#include "Utilities.h"

enum {spTRUE_WIND_SPEED, spTRUE_WIND_DIRECTION, spAPPARENT_WIND_SPEED, spAPPARENT_WIND_DIRECTION, spBOAT_SPEED, spETA};

EditPolarDialog::EditPolarDialog(wxWindow *parent)
#ifndef __WXOSX__
    : EditPolarDialogBase(parent),
#else
    : EditPolarDialogBase(parent, wxID_ANY, _("Edit Polar"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP),
#endif
      m_BoatDialog(static_cast<BoatDialog*>(parent))
{
//	m_gPolar->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( EditPolarDialogBase::OnPolarGridChanged ), NULL, this );
    
    m_lMeasurements->InsertColumn(spTRUE_WIND_SPEED, _("True Wind Speed"));
    m_lMeasurements->InsertColumn(spTRUE_WIND_DIRECTION, _("True Wind Direction"));
    m_lMeasurements->InsertColumn(spAPPARENT_WIND_SPEED, _("Apparent Wind Speed"));
    m_lMeasurements->InsertColumn(spAPPARENT_WIND_DIRECTION, _("Apparent Wind Direction"));
    m_lMeasurements->InsertColumn(spBOAT_SPEED, _("Boat Speed"));
    m_lMeasurements->InsertColumn(spETA, _("Sailboat Transform ETA"));
}

void EditPolarDialog::SetPolarIndex(int i)
{
    polarindex = i;
    RebuildTrueWindAngles();
    RebuildTrueWindSpeeds();
    RebuildGrid();
    m_lMeasurements->DeleteAllItems();

#ifdef __OCPN__ANDROID__
    wxSize sz = ::wxGetDisplaySize();
    SetSize(0, 0, sz.x, sz.y-40);
#endif
}

void EditPolarDialog::OnPolarGridChanged( wxGridEvent& event )
{
    wxString str = m_gPolar->GetCellValue(event.GetRow(), event.GetCol());
    if(str == "0")
        str = "";
    double VB;
    if(!str.ToDouble(&VB))
        VB = NAN;
    GetPolar()->wind_speeds[event.GetCol()].orig_speeds[event.GetRow()] = VB;
    GetPolar()->UpdateSpeeds();
    m_BoatDialog->Refresh();
}

void EditPolarDialog::OnAddTrueWindAngle( wxCommandEvent& event )
{
    double twa;
    if(!m_tTrueWindAngle->GetValue().ToDouble(&twa) || twa < 0 || twa > 180)
        return
    m_tTrueWindAngle->Clear();
    GetPolar()->AddDegreeStep(twa);
    RebuildTrueWindAngles();
    RebuildGrid();
}

void EditPolarDialog::OnRemoveTrueWindAngle( wxCommandEvent& event )
{
    int sel = m_lTrueWindAngles->GetSelection();
    if (sel == -1)
        return;
    GetPolar()->RemoveDegreeStep(sel);
    RebuildTrueWindAngles();
    RebuildGrid();
}

void EditPolarDialog::OnAddTrueWindSpeed( wxCommandEvent& event )
{
    double tws;
    if(!m_tTrueWindSpeed->GetValue().ToDouble(&tws) || tws < 0 || tws > 180)
        return;
    m_tTrueWindSpeed->Clear();
    GetPolar()->AddWindSpeed(tws);
    RebuildTrueWindSpeeds();
    RebuildGrid();
}

void EditPolarDialog::OnRemoveTrueWindSpeed( wxCommandEvent& event )
{
    int sel = m_lTrueWindSpeeds->GetSelection();
    if (sel == -1)
        return;
    GetPolar()->RemoveWindSpeed(sel);
    RebuildTrueWindSpeeds();
    RebuildGrid();
}

static wxString dtos(double d)
{
    return wxString::Format(_T("%f"), d);
}

void EditPolarDialog::OnAddMeasurement( wxCommandEvent& event )
{
    wxListItem info;
    info.SetId(m_lMeasurements->GetItemCount());
//        info.SetData(i);
    long idx = m_lMeasurements->InsertItem(info);

    double windspeed, winddirection, VB;

    m_tWindSpeed->GetValue().ToDouble(&windspeed);
    m_tWindDirection->GetValue().ToDouble(&winddirection);

    m_tBoatSpeed->GetValue().ToDouble(&VB);
    PolarMeasurement m(windspeed, winddirection, VB, m_rbApparentWind->GetValue());

    m_lMeasurements->SetItem(idx, spTRUE_WIND_SPEED, dtos(m.VW()));
    m_lMeasurements->SetItem(idx, spTRUE_WIND_DIRECTION, dtos(m.W()));
    m_lMeasurements->SetItem(idx, spAPPARENT_WIND_SPEED, dtos(m.VA));
    m_lMeasurements->SetItem(idx, spAPPARENT_WIND_DIRECTION, dtos(m.A));
    m_lMeasurements->SetItem(idx, spBOAT_SPEED, dtos(m.VB));
    m_lMeasurements->SetItem(idx, spETA, dtos(m.eta));
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
        const int ind[3] = {spAPPARENT_WIND_SPEED, spAPPARENT_WIND_DIRECTION, spBOAT_SPEED};
        for(int j=0; j<3; j++)
            m_lMeasurements->GetItemText(i, ind[j]).ToDouble(v+j);
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

        for(unsigned int j = 0; j<GetPolar()->degree_steps.size(); j++) {
            double v = GetPolar()->wind_speeds[i].orig_speeds[j];
            wxString str = std::isnan(v) ? "" : v==0 ? "0.0" : wxString::Format(_T("%4.1f"), v);
            m_gPolar->SetCellValue(j, i, str);
        }
    }

    m_BoatDialog->Refresh();
}

Polar *EditPolarDialog::GetPolar()
{
    return polarindex == -1 ? NULL : &m_BoatDialog->m_Boat.Polars[polarindex];
}
