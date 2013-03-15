/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Weather Routing Plugin
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

#include "Utilities.h"
#include "BoatSpeed.h"
#include "BoatDialog.h"

//---------------------------------------------------------------------------------------
//          Weather Routing Dialog Implementation
//---------------------------------------------------------------------------------------


BoatDialog::BoatDialog( wxWindow *parent, BoatSpeed &b)
    : BoatDialogBase(parent), boat(b), m_PlotScale(0)
{
    m_sEta->SetValue(boat.eta * 1000.0);
    m_sHullDrag->SetValue(boat.hull_drag * 100.0);
    m_sKeelPressure->SetValue(boat.keel_pressure * 100.0);
    m_sKeelLift->SetValue(boat.keel_lift * 100.0);
    m_sLWL->SetValue(boat.lwl_ft);
    m_sDisplacement->SetValue(boat.displacement_lbs);
    m_sPlaningConstant->SetValue(boat.planing_constant);

    UpdateVMG();    
}

BoatDialog::~BoatDialog()
{
}

void BoatDialog::OnMouseEventsPlot( wxMouseEvent& event )
{
    if(event.Leaving()) {
        m_stTrueWindAngle->SetLabel(_("N/A"));
        m_stTrueWindKnots->SetLabel(_("N/A"));
        m_stApparentWindAngle->SetLabel(_("N/A"));
        m_stApparentWindKnots->SetLabel(_("N/A"));
        m_stBoatAngle->SetLabel(_("N/A"));
        m_stBoatKnots->SetLabel(_("N/A"));
    }

    if(!m_PlotScale)
        return;

    wxPoint p = event.GetPosition();
    wxSize s = m_PlotWindow->GetSize();

    /* range + to - */
    double x = (double)p.x - s.x/2;
    double y = (double)p.y - s.y/2;

    /* range +- */
    x /= m_PlotScale;
    y /= m_PlotScale;

    switch(m_cPlotAxis->GetSelection()) {
    case 0: /* Boat */
    {
        double W = rad2posdeg(atan2(x, -y)), VB = hypot(x, y);
        m_stBoatAngle->SetLabel(wxString::Format(_T("%03.0f"), W));
        m_stBoatKnots->SetLabel(wxString::Format(_T("%.1f"), VB));

        double VW = boat.TrueWindSpeed(0, VB, W, 30);

        int newmousew = round(W/DEGREE_STEP)*DEGREE_STEP;
        if(newmousew != m_MouseW) {
            m_MouseW = newmousew;
            m_PlotWindow->Refresh();
        }

        m_stTrueWindAngle->SetLabel(wxString::Format(_T("%03.0f"), W));
        m_stTrueWindKnots->SetLabel(wxString::Format(_T("%.1f"), VW));

        double VA = BoatSpeed::VelocityApparentWind(VB, deg2rad(W), VW);
        double A = rad2posdeg(BoatSpeed::DirectionApparentWind(VA, VB, deg2rad(W), VW));

        m_stApparentWindAngle->SetLabel(wxString::Format(_T("%03.0f"), A));

        m_stApparentWindKnots->SetLabel(wxString::Format(_T("%.1f"), VA));

    } break;
    case 1: /* True Wind */
    {
        x *= m_PlotScale, y *= m_PlotScale;
        double W = rad2posdeg(atan2(x, -y)), VW = hypot(x, y);
        m_stTrueWindAngle->SetLabel(wxString::Format(_T("%03.0f"), W));
        m_stTrueWindKnots->SetLabel(wxString::Format(_T("%.1f"), VW));

        double VB = boat.Speed(0, W, VW);
        double B = -W;

        m_stBoatAngle->SetLabel(wxString::Format(_T("%03.0f"), B));
        m_stBoatKnots->SetLabel(wxString::Format(_T("%.1f"), VB));

        double VA = BoatSpeed::VelocityApparentWind(VB, W, VW);
        double A = BoatSpeed::DirectionApparentWind(VA, VB, W, VW);

        m_stApparentWindAngle->SetLabel(wxString::Format(_T("%03.0f"), A));
        m_stApparentWindKnots->SetLabel(wxString::Format(_T("%.1f"), VA));

    } break;
    case 2: /* Apparent Wind */
    {

    } break;
    }
}

void BoatDialog::OnPaintPlot(wxPaintEvent& event)
{
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window)
        return;

#if 0        
        switch(m_cPlotAxis->GetSelection()) {
        case 0: /* boat */
        {
            points[i] = wxPoint(1000*VB*sin(deg2rad(B)), -1000*VB*cos(deg2rad(B)));
        } break;
        case 1: /* true wind */
        {
            
        } break;
        case 2: /* apparent wind */
        {
        } break;
        }
#endif

    wxPaintDC dc(window);
    dc.SetBackgroundMode(wxTRANSPARENT);

    int w, h;
    m_PlotWindow->GetSize( &w, &h);
    m_PlotScale = (w < h ? w : h)/1.8;
    double maxVB = 0;

    int VW = m_sWindSpeed->GetValue();

    int W, i;
    /* plot scale */
    for(W = 0, i=0; W<DEGREES; W += DEGREE_STEP, i++) {
        SailingSpeed &speed = boat.speed[0][VW][W];
        double VB = speed.VB;
        if(VB > maxVB)
            maxVB = VB;
    }
    
    dc.SetPen(wxPen(wxColor(0, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(wxColour(0, 55, 75));

    if(maxVB <= 0) maxVB = 1; /* avoid lock */
    double Vstep = ceil(maxVB / 5);
    maxVB += Vstep;

    m_PlotScale = m_PlotScale / (maxVB+1);

    /* polar circles */
    for(double V = Vstep; V <= maxVB; V+=Vstep) {
        dc.DrawCircle(w/2, h/2, V*m_PlotScale);
        dc.DrawText(wxString::Format(_T("%.0f"), V), w/2, h/2+(int)V*m_PlotScale);
    }

    /* polar meridians */
    dc.SetTextForeground(wxColour(0, 0, 155));
    for(double B = 0; B < 360; B+=15) {
        double x = maxVB*m_PlotScale*sin(deg2rad(B));
        double y = maxVB*m_PlotScale*cos(deg2rad(B));
        if(B < 180)
            dc.DrawLine(w/2 - x, h/2 + y, w/2 + x, h/2 - y);

        wxString str = wxString::Format(_T("%.0f"), B);
        int sw, sh;
        dc.GetTextExtent(str, &sw, &sh);
        dc.DrawText(str, w/2 + x - sw/2, h/2 - y - sh/2);
    }

    /* vmg */
    int P = 0, px, py;
    dc.SetPen(wxPen(wxColor(255, 0, 255), 2));

    double s = maxVB*m_PlotScale;

    W = boat.VMG[P][VW].PortUpWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    W = boat.VMG[P][VW].StarboardUpWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    dc.SetPen(wxPen(wxColor(255, 255, 0), 2));

    W = boat.VMG[P][VW].PortDownWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    W = boat.VMG[P][VW].StarboardDownWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    /* boat speeds */
    int lx, ly;
    for(W = 0; W<=DEGREES; W += DEGREE_STEP) {
        SailingSpeed speed = boat.speed[0][VW][(W%DEGREES)];
        double VB = speed.VB;

        if(speed.w == 0)
            dc.SetPen(wxPen(wxColor(0, 255, 0), 2));
        else
            dc.SetPen(wxPen(wxColor(255, 0, 0), 2));

        int px =  m_PlotScale*VB*sin(deg2rad(W)) + w/2;
        int py = -m_PlotScale*VB*cos(deg2rad(W)) + h/2;

        if(W > 0)
            dc.DrawLine(lx, ly, px, py);
        lx = px, ly = py;

        if(W == m_MouseW) {
            int B = speed.b;
            SailingSpeed speedB = boat.speed[0][VW][B];
            double BVB = speedB.VB;
            int pxb =  m_PlotScale*speed.w*BVB*sin(deg2rad(B)) + w/2;
            int pyb = -m_PlotScale*speed.w*BVB*cos(deg2rad(B)) + h/2;

            dc.SetPen(wxPen(wxColor(0, 0, 255), 2));
            dc.DrawLine(w/2, h/2, pxb, pyb);

            if(speed.w < 1) {
                dc.SetPen(wxPen(wxColor(0, 255, 0), 2));
                dc.DrawLine(pxb, pyb, px, py);
            }
        }
    }
}

void BoatDialog::OnUpdateWindSpeed( wxSpinEvent& event )
{
    UpdateVMG();
    m_PlotWindow->Refresh();
}

void BoatDialog::OnOpen ( wxCommandEvent& event )
{
    wxFileDialog openDialog( this, _( "Select Polar" ), _("/home/sean/qtVlm/polar"), wxT ( "" ),
                             wxT ( "Boat Polar files (*.obp, *.cvs)|*.OBP;*.obp;*.CSV;*.csv|All files (*.*)|*.*" ), wxFD_OPEN  );
    if( openDialog.ShowModal() == wxID_OK ) {
        wxFileName filename = openDialog.GetPath();
        bool binary = filename.GetExt() == _("obp") || filename.GetExt() == _("OBP");

        bool success;
        if(binary) {
            success = boat.OpenBinary(openDialog.GetPath().ToAscii());
        } else {
            BoatSpeedTable table;
            int wind_speed_step, wind_degree_step;
            if((success = table.Open(openDialog.GetPath().ToAscii(),
                                     wind_speed_step, wind_degree_step))) {
                m_sFileCSVWindSpeedStep->SetValue(wind_speed_step);
                m_sFileCSVWindDegreeStep->SetValue(wind_degree_step);
                boat.SetSpeedsFromTable(table);
            }
        }
             
        if(success) {
            UpdateVMG();
            m_PlotWindow->Refresh();
        } else {
            wxMessageDialog md(this, _("Failed Loading boat polar"), _("OpenCPN Weather Routing Plugin"),
                               wxICON_INFORMATION | wxOK );
            md.ShowModal();
        }
    }
}

void BoatDialog::OnSave ( wxCommandEvent& event )
{
    wxFileDialog saveDialog( this, _( "Select Polar" ), _("/home/sean/qtVlm/polar"), wxT ( "" ),
                             wxT ( "Boat Polar files (*.obs, *.cvs)|*.OBS;*.obs;*.CSV;*.csv|All files (*.*)|*.*" ), wxFD_SAVE  );
    if( saveDialog.ShowModal() == wxID_OK ) {
        wxFileName filename = saveDialog.GetPath();
        bool binary = filename.GetExt() == _("obs") || filename.GetExt() == _("OBS");

        bool success;
        if(binary)
            success = boat.SaveBinary(saveDialog.GetPath().ToAscii());
        else {
            BoatSpeedTable table = boat.CreateTable
                (m_sFileCSVWindSpeedStep->GetValue(),
                 m_sFileCSVWindDegreeStep->GetValue());
            success = table.Save(saveDialog.GetPath().ToAscii());
        }

        if(!success) {
            wxMessageDialog md(this, _("Failed saving boat polar"),
                               _("OpenCPN Weather Routing Plugin"),
                               wxICON_INFORMATION | wxOK );
            md.ShowModal();
        }
    }
}

void BoatDialog::OnOptimizeTacking ( wxCommandEvent& event )
{
    boat.OptimizeTackingSpeed();
    m_PlotWindow->Refresh();
}

void BoatDialog::OnResetOptimalTackingSpeed( wxCommandEvent& event )
{
    boat.ResetOptimalTackingSpeed();
    m_PlotWindow->Refresh();
}

void BoatDialog::Compute()
{
    boat.eta = m_sEta->GetValue() / 1000.0;
    boat.hull_drag = m_sHullDrag->GetValue() / 100.0;
    boat.keel_pressure = m_sKeelPressure->GetValue() / 100.0;
    boat.keel_lift = m_sKeelLift->GetValue() / 100.0;
    boat.lwl_ft = m_sLWL->GetValue();
    boat.displacement_lbs = m_sDisplacement->GetValue();
    boat.planing_constant = m_sPlaningConstant->GetValue();

    boat.ComputeBoatSpeeds();

    UpdateVMG();
    m_PlotWindow->Refresh();
}

void BoatDialog::UpdateVMG()
{
    int P = 0;
    int VW = m_sWindSpeed->GetValue();

    m_stBestCourseUpWindPort->SetLabel
        (wxString::Format(_T("%d"), boat.VMG[P][VW].PortUpWind));
    m_stBestCourseUpWindStarboard->SetLabel
        (wxString::Format(_T("%d"), boat.VMG[P][VW].StarboardUpWind));
    m_stBestCourseDownWindPort->SetLabel
        (wxString::Format(_T("%d"), boat.VMG[P][VW].PortDownWind));
    m_stBestCourseDownWindStarboard->SetLabel
        (wxString::Format(_T("%d"), boat.VMG[P][VW].StarboardDownWind));
}
