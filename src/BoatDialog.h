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

#ifndef _BOAT_DIALOG_H_
#define _BOAT_DIALOG_H_

#include <wx/fileconf.h>

#include "WeatherRoutingUI.h"

class weather_routing_pi;

class BoatDialog : public BoatDialogBase
{
public:

    BoatDialog(wxWindow *parent, wxString boatpath);
    ~BoatDialog();

    Boat m_Boat;

private:
    void OnMouseEventsPlot( wxMouseEvent& event );
    void OnPaintPlot( wxPaintEvent& event );
    void OnSizePlot( wxSizeEvent& event ) { m_PlotWindow->Refresh(); }
    void OnUpdatePlot( wxCommandEvent& event ) { m_PlotWindow->Refresh(); }
    void OnUpdatePlot( wxSpinEvent& event ) { m_PlotWindow->Refresh(); }
    void OnUpdateWindSpeed( wxSpinEvent& event );
    void OnOpen( wxCommandEvent& event );
    void Save();
    void OnSaveAs( wxCommandEvent& event );
    void OnSave( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event );
    void OnOpenCSV( wxCommandEvent& event );
    void OnSaveCSV( wxCommandEvent& event );
    void OnRecompute( wxSpinEvent& event ) { StoreBoatParameters(); Compute(); UpdateStats(); }
    void OnOptimizeTacking( wxCommandEvent& event );
    void OnResetOptimalTackingSpeed( wxCommandEvent& event );
    void OnUpdateStatistics( wxCommandEvent& event ) { StoreBoatParameters(); UpdateStats(); }
    void OnUpdateStatistics( wxSpinEvent& event ) { StoreBoatParameters(); UpdateStats(); }
    void OnRecomputeDrag( wxCommandEvent& event );
    void OnDragInfo( wxCommandEvent& event );
    void OnSailPlanSelected( wxListEvent& event );
    void OnEta( wxScrollEvent& event );
    void OnNewBoatPlan( wxCommandEvent& event );
    void OnEditBoatPlan( wxCommandEvent& event );
    void OnDeleteBoatPlan( wxCommandEvent& event );

    void StoreBoatParameters();
    void RepopulatePlans();
    void Compute();
    void UpdateTrackingControls();
    void UpdateVMG();
    void UpdateStats();

    wxString m_boatpath;    
    double m_PlotScale;
    int m_MouseW;

    int m_SelectedSailPlan;
};

#endif
