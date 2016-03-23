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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _BOAT_DIALOG_H_
#define _BOAT_DIALOG_H_

#include <wx/fileconf.h>

#include "EditPolarDialog.h"

class weather_routing_pi;

class BoatDialog : public BoatDialogBase
{
public:
    BoatDialog(wxWindow *parent, wxString boatpath);
    ~BoatDialog();

    EditPolarDialog m_EditPolarDialog;

    Boat m_Boat;
    wxString m_boatpath;

private:
    void OnMouseEventsPolarPlot( wxMouseEvent& event );
    
    void PaintPolar(wxPaintDC &dc, long index);
    void PaintSpeed(wxPaintDC &dc, long index);

    void OnPaintPlot( wxPaintEvent& event );
    void OnUpdatePlot( wxSizeEvent& event ) { OnUpdatePlot(); }
    void OnOrientation( wxCommandEvent& event );
    void OnPaintCrossOverChart( wxPaintEvent& event );
    void OnGenerateCrossOverChart( wxCommandEvent& event );
    void OnSizePlot( wxSizeEvent& event ) { RefreshPlots(); }
    void OnUpdatePlot();
    void OnUpdatePlot( wxCommandEvent& event ) { OnUpdatePlot(); }
    void OnUpdatePlot( wxSpinEvent& event ) { OnUpdatePlot(); }
    void OnOpen( wxCommandEvent& event );
    void SaveBoat();
    void OnSaveAsBoat( wxCommandEvent& event );
    void OnSaveBoat( wxCommandEvent& event ) { SaveBoat(); }
    void OnClose( wxCommandEvent& event );
    void LoadFile(bool switched = false);
    void OnSaveFile( wxCommandEvent& event );
    void OnPolarSelected( wxListEvent& event ) { OnPolarSelected(); }
    void OnPolarSelected();

    void OnUpPolar( wxCommandEvent& event );
    void OnDownPolar( wxCommandEvent& event );
    void OnEditPolar( wxCommandEvent& event );
    void OnAddPolar( wxCommandEvent& event );
    void OnRemovePolar( wxCommandEvent& event );

    void RepopulatePolars();

    void RefreshPlots() { m_PlotWindow->Refresh(); m_CrossOverChart->Refresh(); }

    wxString FormatVMG(double W, double VW);
    void UpdateVMG();

    long SelectedPolar() { return m_lPolars->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED); }

    double m_PlotScale;
    int m_MouseW;

    bool m_orientation[2];
};

#endif
