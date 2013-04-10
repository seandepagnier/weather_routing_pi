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
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Utilities.h"
#include "Boat.h"
#include "RouteMap.h"
#include "PlotDialog.h"

//---------------------------------------------------------------------------------------
//          Weather Routing Dialog Implementation
//---------------------------------------------------------------------------------------


PlotDialog::PlotDialog( wxWindow *parent, std::list<PlotData> &PlotData )
    : PlotDialogBase(parent), m_PlotData(PlotData)
{
    GetScale();
}

PlotDialog::~PlotDialog()
{
}

void PlotDialog::OnMouseEventsPlot( wxMouseEvent& event )
{
    wxStaticText *stMousePosition[3] = {m_stMousePosition1, m_stMousePosition2, m_stMousePosition3};
    if(event.Leaving()) {
        for(int i=0; i<3; i++)
            stMousePosition[i]->SetLabel(_("N/A"));
        return;
    }

    int w, h;
    m_PlotWindow->GetSize( &w, &h);

    wxPoint p = event.GetPosition();

#if 0
    double position = m_sPosition->GetValue() / 100.0;
    double scale = 100.0 / m_sScale->GetValue();

    double time = (((double)p.x/w - position) / scale + position) * (m_maxtime - m_mintime) + m_mintime;
    wxDateTime datetime = m_StartTime + wxTimeSpan(0, 0, time);
#endif

    for(int i=0; i<3; i++) {
        double value = (1.0 - (double)p.y / h) * (m_maxvalue - m_minvalue) + m_minvalue;
        stMousePosition[i]->SetLabel(wxString::Format(_T(" %.3f"), value));
    }
}

double PlotDialog::GetValue(PlotData &data, int var)
{
    switch(var) {
    case BOAT_VELOCITY_GROUND:         return data.VBG;
    case BOAT_COURSE_GROUND:           return data.BG;
    case BOAT_VELOCITY_WATER:          return data.VB;
    case BOAT_COURSE_WATER:            return data.B;
    case WIND_VELOCITY:                return data.VW;
    case WIND_DIRECTION:               return heading_resolve(data.B - data.W);
    case WIND_COURSE:                  return data.W;
    case WIND_VELOCITY_GROUND:         return data.VWG;
    case WIND_DIRECTION_GROUND:        return heading_resolve(data.BG - data.WG);
    case WIND_COURSE_GROUND:           return data.WG;
    case APPARENT_WIND_VELOCITY:
        return BoatPlan::VelocityApparentWind(data.VB, deg2rad(GetValue(data, WIND_DIRECTION)), data.VW);
    case APPARENT_WIND_DIRECTION: {
        return rad2deg(BoatPlan::DirectionApparentWind
                       (GetValue(data, APPARENT_WIND_VELOCITY), data.VB,
                        deg2rad(GetValue(data, WIND_DIRECTION)), data.VW));
    }
    case CURRENT_VELOCITY: return data.VC;
    case CURRENT_DIRECTION: return data.C;
    case SIG_WAVE_HEIGHT: return data.WVHT;
    }
    return NAN;
}

void PlotDialog::GetScale()
{
    wxChoice *cVariable[3] = {m_cVariable1, m_cVariable2, m_cVariable3};
    bool first = true;
    for(int i=0; i<3; i++) {
        for(std::list<PlotData>::iterator it = m_PlotData.begin(); it != m_PlotData.end(); it++) {
            double value = GetValue(*it, cVariable[i]->GetSelection());
            if(first) {
                m_StartTime = (*it).time;
                m_mintime = m_maxtime = 0;
                m_minvalue = m_maxvalue = value;
                first = false;
            } else {
                double time = ((*it).time - m_StartTime).GetSeconds().ToDouble();
                m_mintime = MIN(time, m_mintime);
                m_maxtime = MAX(time, m_maxtime);
                m_minvalue = MIN(value, m_minvalue);
                m_maxvalue = MAX(value, m_maxvalue);
            }
        }
    }
}

void PlotDialog::OnPaintPlot(wxPaintEvent& event)
{
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window)
        return;

    double position = m_sPosition->GetValue() / 100.0;
    double scale = 100.0 / m_sScale->GetValue();

    wxPaintDC dc(window);
    dc.SetBackgroundMode(wxTRANSPARENT);

    int w, h;
    m_PlotWindow->GetSize( &w, &h);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxChoice *cVariable[3] = {m_cVariable1, m_cVariable2, m_cVariable3};
    wxColour colors[3] = {wxColour(255, 0, 0), wxColour(0, 255, 0), wxColour(0, 0, 255)};
    for(int i=0; i<3; i++) {
        dc.SetPen(wxPen(colors[i], 3));

        int lx, ly;
        bool first = true;
        for(std::list<PlotData>::iterator it = m_PlotData.begin(); it != m_PlotData.end(); it++) {
            double time = ((*it).time - m_StartTime).GetSeconds().ToDouble();
            double value = GetValue(*it, cVariable[i]->GetSelection());

            int x = w * (scale * ((time - m_mintime) / (m_maxtime - m_mintime) - position) + position);
            int y = h * (1 - (value - m_minvalue) / (m_maxvalue - m_minvalue));
            if(first)
                first = false;
            else
                dc.DrawLine(lx, ly, x, y);
            lx = x, ly = y;
        }
    }

    dc.SetTextForeground(wxColour(0, 0, 0));
    const double steps = 10;
    for(double x=1/steps; x<1-1/steps; x+=1/steps) {
        wxString time = wxString::Format
            (_T("%.0f"), ((x - position)/scale + position) * (m_maxtime - m_mintime) + m_mintime);
        wxSize s = dc.GetTextExtent(time);
        dc.DrawText(time, x*w-s.x/2, 0);

        wxString value = wxString::Format(_T("%.1f"), (1-x)*(m_maxvalue - m_minvalue) + m_minvalue);
        s = dc.GetTextExtent(value);
        dc.DrawText(value, 0, x*h - s.y/2);
    }
}
