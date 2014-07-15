/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier                                 *
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
    wxString m_boatpath;

private:
    void OnMouseEventsPlot( wxMouseEvent& event );
    void PlotVMG(wxPaintDC &dc, double W, double s);

    void PaintPolar(wxPaintDC &dc);
    void PaintSpeed(wxPaintDC &dc);
    void PaintVMG(wxPaintDC &dc);

    void OnPaintPlot( wxPaintEvent& event );
    void OnSizePlot( wxSizeEvent& event ) { m_PlotWindow->Refresh(); }
    void OnUpdatePlot();
    void OnUpdatePlot( wxListbookEvent& event ) { OnUpdatePlot(); }
    void OnUpdatePlot( wxCommandEvent& event ) { OnUpdatePlot(); }
    void OnUpdatePlot( wxSpinEvent& event ) { OnUpdatePlot(); }
    void OnOpen( wxCommandEvent& event );
    void Save();
    void OnSaveAs( wxCommandEvent& event );
    void OnSave( wxCommandEvent& event ) { Save(); }
    void OnClose( wxCommandEvent& event );
    void LoadCSV(bool switched = false);
    void OnSaveCSV( wxCommandEvent& event );
    void OnPolarCSVFile( wxFileDirPickerEvent& event );
    void OnUpdateVMG( wxSpinEvent& event ) { UpdateVMG(); }
    void OnUpdateVMG( wxCommandEvent& event ) { UpdateVMG(); }
    void OnRecompute( );
    void OnRecomputeSpin( wxSpinEvent& event ) { OnRecompute(); }
    void OnRecompute( wxCommandEvent& event ) { OnRecompute(); }
    void OnOptimizeTacking( wxCommandEvent& event );
    void OnUpdateStatistics( wxCommandEvent& event ) { StoreBoatParameters(); UpdateStats(); }
    void OnUpdateStatisticsSpin( wxSpinEvent& event ) { StoreBoatParameters(); UpdateStats(); }
    void OnRecomputeDrag( wxCommandEvent& event );
    void OnDragInfo( wxCommandEvent& event );
    void OnSailPlanSelected( wxListEvent& event );
    void OnPolarMethod( wxCommandEvent& event );
    void OnEtaSlider( wxScrollEvent& event );
    void OnEta( wxCommandEvent& event );
    void OnNewBoatPlan( wxCommandEvent& event );
    void OnDeleteBoatPlan( wxCommandEvent& event );

    void StoreBoatParameters();
    void RepopulatePlans();
    void Compute();
    void UpdateTrackingControls();
    void SetEta(double eta) { m_tEta->SetValue(wxString::Format(_T("%.5g"), eta)); }

    void OnNewSwitchPlanRule( wxCommandEvent& event );
    void OnEditSwitchPlanRule( wxCommandEvent& event );
    void OnDeleteSwitchPlanRule( wxCommandEvent& event );
    void PopulatePlans();

    wxString FormatVMG(double W, double VW);
    void UpdateVMG();
    void UpdateStats();

    double m_PlotScale;
    int m_MouseW;

    int m_SelectedSailPlan;
};

#endif
