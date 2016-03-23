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
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "WeatherRouting.h"
#include "wx28compat.h"

//---------------------------------------------------------------------------------------
//          Weather Routing Dialog Implementation
//---------------------------------------------------------------------------------------


PlotDialog::PlotDialog( WeatherRouting &weatherrouting )
    : PlotDialogBase(&weatherrouting), m_WeatherRouting(weatherrouting)
{
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
        double value = (1.0 - (double)p.y / h) * (m_maxvalue[i] - m_minvalue[i]) + m_minvalue[i];
        stMousePosition[i]->SetLabel(wxString::Format(_T(" %.3f"), value));
    }
}

double PlotDialog::GetValue(PlotData &data, int var)
{
    switch(var) {
    case SPEED_OVER_GROUND:            return data.VBG;
    case COURSE_OVER_GROUND:           return data.BG;
    case SPEED_OVER_WATER:             return data.VB;
    case COURSE_OVER_WATER:            return data.B;
    case WIND_VELOCITY:                return data.VW;
    case WIND_DIRECTION:               return heading_resolve(data.B - data.W);
    case WIND_COURSE:                  return data.W;
    case WIND_VELOCITY_GROUND:         return data.VWG;
    case WIND_DIRECTION_GROUND:        return heading_resolve(data.BG - data.WG);
    case WIND_COURSE_GROUND:           return data.WG;
    case APPARENT_WIND_SPEED:
        return Polar::VelocityApparentWind(data.VB, GetValue(data, WIND_DIRECTION), data.VW);
    case APPARENT_WIND_ANGLE: {
        return rad2deg(Polar::DirectionApparentWind
                       (GetValue(data, APPARENT_WIND_SPEED), data.VB,
                        GetValue(data, WIND_DIRECTION), data.VW));
    }
    case CURRENT_VELOCITY: return data.VC;
    case CURRENT_DIRECTION: return data.C;
    case SIG_WAVE_HEIGHT: return data.WVHT;
    case TACKS: return data.tacks;
    }
    return NAN;
}

enum Type {SPEED, COURSE, WIND_SPEED, WIND_DIRECTION, CURRENT_SPEED, CURRENT_DIRECTION, WAVE_HEIGHT, TACKS, INVALID};
int PlotDialog::GetType(int var)
{
    switch(var) {
    case SPEED_OVER_GROUND:
    case SPEED_OVER_WATER:
        return SPEED;
    case COURSE_OVER_GROUND:
    case COURSE_OVER_WATER:
        return COURSE;
    case WIND_VELOCITY:
    case WIND_VELOCITY_GROUND:
    case APPARENT_WIND_SPEED:
        return WIND_SPEED;
    case WIND_DIRECTION:
    case WIND_DIRECTION_GROUND:
    case APPARENT_WIND_ANGLE:
        return WIND_DIRECTION;
    case WIND_COURSE:
    case WIND_COURSE_GROUND:
    case CURRENT_VELOCITY:
        return CURRENT_SPEED;
    case CURRENT_DIRECTION:
        return CURRENT_DIRECTION;
    case SIG_WAVE_HEIGHT:
        return WAVE_HEIGHT;
    case TACKS:
        return TACKS;
    }
    return INVALID;
}

void PlotDialog::GetScale()
{
    wxChoice *cVariable[3] = {m_cVariable1, m_cVariable2, m_cVariable3};
    for(int i=0; i<3; i++) {
        bool first = true;
        for(std::list<PlotData>::iterator it = m_PlotData.begin(); it != m_PlotData.end(); it++) {
            double value = GetValue(*it, cVariable[i]->GetSelection());
            if(first) {
                m_StartTime = (*it).time;
                m_mintime = m_maxtime = 0.;
                m_minvalue[i] = m_maxvalue[i] = value;
                first = false;
            } else {
                double time = ((*it).time - m_StartTime).GetSeconds().ToDouble();
                m_mintime = MIN(time, m_mintime);
                m_maxtime = MAX(time, m_maxtime);
                m_minvalue[i] = MIN(value, m_minvalue[i]);
                m_maxvalue[i] = MAX(value, m_maxvalue[i]);
            }
        }
    }

    // force same scales for comparible datatypes
    for(int i=0; i<2; i++)
        for(int j=i+1; j<3; j++)
            if(GetType(cVariable[i]->GetSelection()) == GetType(cVariable[j]->GetSelection())) {
                m_minvalue[i] = m_minvalue[j] = wxMin(m_minvalue[i], m_minvalue[j]);
                m_maxvalue[i] = m_maxvalue[j] = wxMax(m_maxvalue[i], m_maxvalue[j]);
            }
}

static wxString ReadableTime(int seconds)
{
    if(seconds < 60)
        return wxString::Format(_T("%02ds"), seconds);

    if(seconds < 3600)
        return wxString::Format(_T("%02d.%02d"), seconds / 60, seconds % 60);

    if(seconds < 86400)
        return wxString::Format(_T("%02d:%02d"), seconds / 3600, (seconds / 60) % 60);

    return wxString::Format(_T("%dd %02dh"), seconds / 86400, seconds / 3600);
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
    wxColour colors[3] = {wxColour(200, 0, 0), wxColour(0, 200, 0), wxColour(0, 0, 200)};
    for(int i=0; i<3; i++) {
        dc.SetPen(wxPen(colors[i], 3));

        int lx, ly;
        bool first = true;
        for(std::list<PlotData>::iterator it = m_PlotData.begin(); it != m_PlotData.end(); it++) {
            double time = ((*it).time - m_StartTime).GetSeconds().ToDouble();
            double value = GetValue(*it, cVariable[i]->GetSelection());

            int x = w * (scale * ((time - m_mintime) / (m_maxtime - m_mintime) - position) + position);
            int y = h * (1 - (value - m_minvalue[i]) / (m_maxvalue[i] - m_minvalue[i]));
            if(first)
                first = false;
            else
                dc.DrawLine(lx, ly, x, y);
            lx = x, ly = y;
        }
    }

    dc.SetTextForeground(*wxBLACK);
    dc.SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_DOT));

    const double steps = 10;
    bool grid = true;
    for(double i=1/steps; i<1-1/steps; i+=1/steps) {
        int x = i*w, y = i*h;
        if(grid) {
            dc.DrawLine(x, 0, x, h);
            dc.DrawLine(0, y, w, y);
        }

        wxString time = ReadableTime(((i - position)/scale + position) * (m_maxtime - m_mintime) + m_mintime);
        wxSize s = dc.GetTextExtent(time);
        dc.DrawText(time, x-s.x/2, 0);
    }

    int x = 0;
    for(int ci=0; ci<3; ci++) {
        wxColour c = colors[ci];
        dc.SetTextForeground(wxColour(c.Red()*3/4, c.Green()*3/4, c.Blue()*3/4));
        int maxx = 0;
        for(double i=1/steps; i<1-1/steps; i+=1/steps) {
            wxString value = wxString::Format(_T("%.1f"), (1-i)*(m_maxvalue[ci] - m_minvalue[ci]) + m_minvalue[ci]);
            wxSize s = dc.GetTextExtent(value);
            int y = i*h; 
            dc.DrawText(value, x, y - s.y/2);

            if(s.x > maxx)
                maxx = s.x;
        }

        x += maxx + 5;
    }
}

void PlotDialog::OnUpdateRoute( wxCommandEvent& event )
{
    SetRouteMapOverlay(m_WeatherRouting.FirstCurrentRouteMap());
}

void PlotDialog::SetRouteMapOverlay(RouteMapOverlay *routemapoverlay)
{
    if(!routemapoverlay)
        m_PlotData.clear();
    else
        m_PlotData = routemapoverlay->GetPlotData(m_rbCursorRoute->GetValue());
    GetScale();
    m_PlotWindow->Refresh();
}
