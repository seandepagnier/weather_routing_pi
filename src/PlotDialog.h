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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _PLOT_DIALOG_H_
#define _PLOT_DIALOG_H_

#include <list>

#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"

class weather_routing_pi;

class PlotDialog : public PlotDialogBase
{
public:

    PlotDialog( wxWindow *parent, std::list<PlotData> &PlotData);
    ~PlotDialog();

private:
    enum Variable { BOAT_VELOCITY_GROUND, BOAT_COURSE_GROUND, BOAT_VELOCITY_WATER, BOAT_COURSE_WATER,
                    WIND_VELOCITY, WIND_DIRECTION, WIND_COURSE,
                    WIND_VELOCITY_GROUND, WIND_DIRECTION_GROUND, WIND_COURSE_GROUND,
                    APPARENT_WIND_VELOCITY, APPARENT_WIND_DIRECTION,
                    CURRENT_VELOCITY, CURRENT_DIRECTION,
                    SIG_WAVE_HEIGHT };

    void OnMouseEventsPlot( wxMouseEvent& event );
    void OnPaintPlot( wxPaintEvent& event );
    void OnSizePlot( wxSizeEvent& event ) { m_PlotWindow->Refresh(); }
    void OnUpdatePlot( wxScrollEvent& event ) { m_PlotWindow->Refresh(); }
    void OnUpdatePlotVariable( wxCommandEvent& event ) { GetScale(); m_PlotWindow->Refresh(); }

private:
    double GetValue(PlotData &data, int var);
    void GetScale();

    wxDateTime m_StartTime;

    double m_mintime, m_maxtime;
    double m_minvalue, m_maxvalue;

    std::list<PlotData> &m_PlotData;
};

#endif
