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

#ifndef _EDITPOLAR_DIALOG_H_
#define _EDITPOLAR_DIALOG_H_

#include "WeatherRoutingUI.h"

class BoatDialog;

class Polar;
class EditPolarDialog : public EditPolarDialogBase
{
public:
    EditPolarDialog(wxWindow *parent);

    void SetPolarIndex(int i);

private:
    void OnPolarGridChanged( wxGridEvent& event );
    void OnAddTrueWindAngle( wxCommandEvent& event );
    void OnRemoveTrueWindAngle( wxCommandEvent& event );
    void OnAddTrueWindSpeed( wxCommandEvent& event );
    void OnRemoveTrueWindSpeed( wxCommandEvent& event );
    void OnAddMeasurement( wxCommandEvent& event );
    void OnRemoveMeasurement( wxCommandEvent& event );
    void OnRemoveAllMeasurements( wxCommandEvent& event );
    void OnGeneratePolar( wxCommandEvent& event );
    void OnSave( wxCommandEvent& event ) { EndModal(wxID_SAVE); }

    void RebuildTrueWindAngles();
    void RebuildTrueWindSpeeds();
    void RebuildGrid();
    Polar *GetPolar();

    int polarindex;
    BoatDialog *m_BoatDialog;
};

#endif
