/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Advanced Routing Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2012 by Sean D'Epagnier   *
 *   sean@depagnier.com   *
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

#include "utilities.h"
#include "boatspeed.h"
#include "boatdialog.h"

//---------------------------------------------------------------------------------------
//          Advanced Routing Dialog Implementation
//---------------------------------------------------------------------------------------
IMPLEMENT_CLASS ( BoatDialog, wxDialog )

BEGIN_EVENT_TABLE ( BoatDialog, wxDialog )

            EVT_CLOSE ( BoatDialog::OnClose )
            EVT_BUTTON ( ID_COMPUTE, BoatDialog::OnIdComputeClick )
            EVT_BUTTON ( ID_CLOSE, BoatDialog::OnIdCloseClick )
            EVT_MOVE ( BoatDialog::OnMove )
            EVT_SIZE ( BoatDialog::OnSize )

END_EVENT_TABLE()

static BoatSpeed *boat;
static wxSlider *power_slider, *eta_slider, *keel_pressure_slider, *keel_lift_slider;

BoatDialog::BoatDialog( BoatSpeed &b)
{
  boat = &b;
}

BoatDialog::~BoatDialog()
{
}

bool BoatDialog::Create ( wxWindow *parent, wxWindowID id,
                              const wxString& caption,
                              const wxPoint& pos, const wxSize& size, long style )
{
      pParent = parent;

      long wstyle = wxDEFAULT_FRAME_STYLE;

      wxSize size_min = size;
      if ( !wxDialog::Create ( parent, id, caption, pos, size_min, wstyle ) )
            return false;

      CreateControls();

      Fit();
      SetMinSize(GetBestSize());

      return true;
}

void BoatDialog::CreateControls()
{
    wxBoxSizer* itemBoxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer);

      // Create the main frame window
      BoatCanvas *myCanvas = new BoatCanvas(this);
      myCanvas->SetSize(500, 350);
      myCanvas->SetBackgroundColour(*wxWHITE);

      itemBoxSizer->Add(myCanvas, 0, wxGROW | wxALL, 1);

      wxFlexGridSizer* pGrid = new wxFlexGridSizer(2);
      itemBoxSizer->Add(pGrid, 0, wxGROW | wxALL, 1);

  wxStaticText *spower = new wxStaticText( this, -1, _("Motor Power"));
  pGrid->Add(spower, 0, wxALIGN_LEFT|wxALL, 1);
  power_slider = new wxSlider(this, wxID_ANY, 0, 0, 10,
                              wxDefaultPosition, wxDefaultSize,
                              wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  pGrid->Add(power_slider, 1, wxEXPAND | wxALL, 1);

  wxStaticText *seta = new wxStaticText( this, -1, _("Eta"));
  pGrid->Add(seta, 0, wxALIGN_LEFT|wxALL, 1);
  eta_slider = new wxSlider(this, wxID_ANY, 500, 1, 1000,
                              wxDefaultPosition, wxDefaultSize,
                              wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  pGrid->Add(eta_slider, 1, wxEXPAND | wxALL, 1);

  wxStaticText *skeelp = new wxStaticText( this, -1, _("Keel pressure"));
  pGrid->Add(skeelp, 0, wxALIGN_LEFT|wxALL, 1);
  keel_pressure_slider = new wxSlider(this, wxID_ANY, 10, 1, 100,
                              wxDefaultPosition, wxDefaultSize,
                              wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  pGrid->Add(keel_pressure_slider, 1, wxEXPAND | wxALL, 1);

  wxStaticText *skeell = new wxStaticText( this, -1, _("Keel lift"));
  pGrid->Add(skeell, 0, wxALIGN_LEFT|wxALL, 1);
  keel_lift_slider = new wxSlider(this, wxID_ANY, 1, 1, 100,
                              wxDefaultPosition, wxDefaultSize,
                              wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  pGrid->Add(keel_lift_slider, 1, wxEXPAND | wxALL, 1);

      wxButton* bCompute = new wxButton ( this, ID_COMPUTE, _( "&Compute" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      pGrid->Add ( bCompute, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

// The close button
      wxButton* bClose = new wxButton ( this, ID_CLOSE, _( "&Close" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      pGrid->Add ( bClose, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
}

void BoatDialog::OnClose ( wxCloseEvent& event )
{
  Hide();
}

void Compute()
{
   boat->ComputeBoatSpeeds(eta_slider->GetValue() / 1000.0,
                           keel_pressure_slider->GetValue(),
                           keel_lift_slider->GetValue() / 100.0);
 //        boat.OptimizeTackingSpeed();
 }

 void BoatDialog::OnIdComputeClick ( wxCommandEvent& event )
 {
   Compute();
   Refresh();
 }

 void BoatDialog::OnIdCloseClick ( wxCommandEvent& event )
 {
       Close();
 }

 void BoatDialog::OnMove ( wxMoveEvent& event )
{
      event.Skip();
}

void BoatDialog::OnSize ( wxSizeEvent& event )
{
      event.Skip();
}

BEGIN_EVENT_TABLE(BoatCanvas, wxWindow)
    EVT_PAINT(BoatCanvas::OnPaint)
END_EVENT_TABLE()

void BoatCanvas::OnPaint(wxPaintEvent& event)
{
  if(!boat->computed)
    Compute();
  
  wxPaintDC dc(this);
  dc.SetTextForeground(wxColour(1, 0, 0));
  dc.SetBackgroundMode(wxTRANSPARENT);

  int w, h;
  GetSize( &w, &h);
  double scale = (w < h ? w : h);
  double maxVB = 0;

  for(int VW = 15; VW > 0; VW -= 3) {
    wxPoint points[DEGREES / DEGREE_STEP];
    int W, i;
    for(W = 0, i=0; W<DEGREES; W += DEGREE_STEP, i++) {
      double BA, VB;
      boat->Speed(power_slider->GetValue(), W, VW, BA, VB);
      if(VW == 15 && VB > maxVB)
        maxVB = VB;
      points[i] = wxPoint(500*VB*sin(deg2rad(BA)), -500*VB*cos(deg2rad(BA)));
    }

    if(VW == 15)
      scale = scale / (maxVB+1) / 1000;

    for(W = 0, i=0; W<DEGREES; W += DEGREE_STEP, i++)
      points[i] = wxPoint(scale * points[i].x, scale * points[i].y);

    dc.DrawPolygon(i, points, w/2, h/2, 0);
  }
}
