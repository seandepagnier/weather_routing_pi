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
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Utilities.h"
#include "BoatSpeed.h"
#include "BoatDialog.h"

//---------------------------------------------------------------------------------------
//          Weather Routing Dialog Implementation
//---------------------------------------------------------------------------------------


BoatDialog::BoatDialog( wxWindow *parent )
    : BoatDialogBase(parent), m_PlotScale(0)
{
    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
#ifdef __WXMSW__
    wxString stdPath  = std_path.GetConfigDir();
#endif
#ifdef __WXGTK__
    wxString stdPath  = std_path.GetUserDataDir();
#endif
#ifdef __WXOSX__
    wxString stdPath  = std_path.GetUserConfigDir();   // should be ~/Library/Preferences	
#endif

    m_default_boat_path = stdPath + wxFileName::GetPathSeparator() + _T("boat.obs");
    m_Boat.OpenBinary(m_default_boat_path.ToAscii());

    m_sEta->SetValue(m_Boat.eta * 1000.0);
    m_sHullDrag->SetValue(m_Boat.hull_drag * 1000.0);
    m_sKeelPressure->SetValue(m_Boat.keel_pressure * 100.0);
    m_sKeelLift->SetValue(m_Boat.keel_lift * 100.0);
    m_sLWL->SetValue(m_Boat.lwl_ft);
    m_sDisplacement->SetValue(m_Boat.displacement_lbs);
    m_sPlaningConstant->SetValue(m_Boat.planing_constant);

    UpdateVMG();    
}

BoatDialog::~BoatDialog()
{
    m_Boat.SaveBinary(m_default_boat_path.ToAscii());
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

#if 0
    switch(m_cPlotAxis->GetSelection()) {
    case 0: /* Boat */
    {
#endif

        double B = rad2posdeg(atan2(x, -y));
        double W = -B, VW = m_sWindSpeed->GetValue();
        double VB = m_Boat.Speed(0, B, VW);

        m_stBoatAngle->SetLabel(wxString::Format(_T("%03.0f"), B));
        m_stBoatKnots->SetLabel(wxString::Format(_T("%.1f"), VB));

        int newmousew = round(B);
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

#if 0
    } break;
    case 1: /* True Wind */
    {
        x *= m_PlotScale, y *= m_PlotScale;
        double W = rad2posdeg(atan2(x, -y)), VW = hypot(x, y);
        m_stTrueWindAngle->SetLabel(wxString::Format(_T("%03.0f"), W));
        m_stTrueWindKnots->SetLabel(wxString::Format(_T("%.1f"), VW));

        double VB = m_Boat.Speed(0, W, VW);
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
#endif
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
        SailingSpeed &speed = m_Boat.speed[0][VW][W];
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

    W = m_Boat.VMG[P][VW].PortTackUpWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    W = m_Boat.VMG[P][VW].StarboardTackUpWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    dc.SetPen(wxPen(wxColor(255, 255, 0), 2));

    W = m_Boat.VMG[P][VW].PortTackDownWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    W = m_Boat.VMG[P][VW].StarboardTackDownWind;
    px = s*sin(deg2rad(W));
    py = s*cos(deg2rad(W));
    dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);

    /* boat speeds */
    int lx, ly;
    for(W = 0; W<=DEGREES; W += DEGREE_STEP) {
        SailingSpeed speed = m_Boat.speed[0][VW][(W%DEGREES)];
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
            SailingSpeed speedB = m_Boat.speed[0][VW][B];
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
            success = m_Boat.OpenBinary(openDialog.GetPath().ToAscii());
        } else {
            BoatSpeedTable table;
            int wind_speed_step, wind_degree_step;
            if((success = table.Open(openDialog.GetPath().ToAscii(),
                                     wind_speed_step, wind_degree_step))) {
                m_sFileCSVWindSpeedStep->SetValue(wind_speed_step);
                m_sFileCSVWindDegreeStep->SetValue(wind_degree_step);
                m_Boat.SetSpeedsFromTable(table);
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
            success = m_Boat.SaveBinary(saveDialog.GetPath().ToAscii());
        else {
            BoatSpeedTable table = m_Boat.CreateTable
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
    m_Boat.OptimizeTackingSpeed();
    m_PlotWindow->Refresh();
}

void BoatDialog::OnResetOptimalTackingSpeed( wxCommandEvent& event )
{
    m_Boat.ResetOptimalTackingSpeed();
    m_PlotWindow->Refresh();
}

void BoatDialog::Compute()
{
    m_Boat.eta = m_sEta->GetValue() / 1000.0;
    m_Boat.hull_drag = m_sHullDrag->GetValue() / 1000.0;
    m_Boat.keel_pressure = m_sKeelPressure->GetValue() / 100.0;
    m_Boat.keel_lift = m_sKeelLift->GetValue() / 100.0;
    m_Boat.lwl_ft = m_sLWL->GetValue();
    m_Boat.displacement_lbs = m_sDisplacement->GetValue();
    m_Boat.planing_constant = m_sPlaningConstant->GetValue();

    m_Boat.ComputeBoatSpeeds();

    UpdateVMG();
    m_PlotWindow->Refresh();
}

void BoatDialog::UpdateVMG()
{
    int P = 0;
    int VW = m_sWindSpeed->GetValue();

    m_stBestCourseUpWindPortTack->SetLabel
        (wxString::Format(_T("%d"), m_Boat.VMG[P][VW].PortTackUpWind));
    m_stBestCourseUpWindStarboardTack->SetLabel
        (wxString::Format(_T("%d"), m_Boat.VMG[P][VW].StarboardTackUpWind));
    m_stBestCourseDownWindPortTack->SetLabel
        (wxString::Format(_T("%d"), m_Boat.VMG[P][VW].PortTackDownWind));
    m_stBestCourseDownWindStarboardTack->SetLabel
        (wxString::Format(_T("%d"), m_Boat.VMG[P][VW].StarboardTackDownWind));
}
