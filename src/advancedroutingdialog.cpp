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

#include "boatspeed.h"
#include "boatdialog.h"
#include "routemap.h"
#include "advanced_routing_pi.h"

//---------------------------------------------------------------------------------------
//          Advanced Routing Dialog Implementation
//---------------------------------------------------------------------------------------
IMPLEMENT_CLASS ( AdvancedRoutingDialog, wxDialog )

BEGIN_EVENT_TABLE ( AdvancedRoutingDialog, wxDialog )

            EVT_CLOSE ( AdvancedRoutingDialog::OnClose )
            EVT_BUTTON ( ID_BOAT, AdvancedRoutingDialog::OnIdBoatClick )
            EVT_BUTTON ( ID_COMPUTE, AdvancedRoutingDialog::OnIdComputeClick )
            EVT_BUTTON ( ID_CLEAR, AdvancedRoutingDialog::OnIdClearClick )
            EVT_BUTTON ( ID_ABOUT, AdvancedRoutingDialog::OnIdAboutClick )
            EVT_BUTTON ( ID_CLOSE, AdvancedRoutingDialog::OnIdCloseClick )
            EVT_MOVE ( AdvancedRoutingDialog::OnMove )
            EVT_SIZE ( AdvancedRoutingDialog::OnSize )

END_EVENT_TABLE()

AdvancedRoutingDialog::AdvancedRoutingDialog( )
: m_boat_lat(180), m_boat_lon(0), m_thCompute(routemap, boat)
{
}

AdvancedRoutingDialog::~AdvancedRoutingDialog( )
{
}

bool AdvancedRoutingDialog::Create ( wxWindow *parent, advanced_routing_pi *ppi, wxWindowID id,
                              const wxString& caption,
                              const wxPoint& pos, const wxSize& size, long style )
{
      pParent = parent;
      pPlugIn = ppi;

      long wstyle = wxDEFAULT_FRAME_STYLE;

      wxSize size_min = size;
      if ( !wxDialog::Create ( parent, id, caption, pos, size_min, wstyle ) )
            return false;

      CreateControls();

      DimeWindow(this);

      Fit();
      SetMinSize(GetBestSize());

      return true;
}

void AdvancedRoutingDialog::CreateControls()
{
      int border_size = 4;

// A top-level sizer
      wxBoxSizer* topSizer = new wxBoxSizer ( wxVERTICAL );
      SetSizer ( topSizer );

// A second box sizer to give more space around the controls
      wxBoxSizer* boxSizer = new wxBoxSizer ( wxVERTICAL );
      topSizer->Add ( boxSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 2 );

  /*  User interface:
      Tabs Control
             Routing
                 Button to start computing routing changes to stop, continue and restart
                 Buttons load/save current routing
                 Timeline showing each iso region with data, can render each one and go back
                             rewind, clear before etc..
                             min and max lat/lon, area covered,
                             average speed
                             selectable to display
                                    route with min/max boat-speed,wind-speed,wave-height,
                                                       current-speed,temperature,cloud-cover etc..
                                    

             Configuration
                 Save Load buttons for configuration
                 Timespan
                      In seconds
                         Fixed
                         Variable (is based on the curvature of the isoregion/walls etc..
                 Allow inversions (current overpowering for a time to reverse later (maybe)
                 Rendering Colors/Enabled for
                         Positive/Negative Routes
                         
             Boat
                 List of boats with current boat selected
                 Buttons - New Edit Save Load Clone Simulate
                   New/Edit Dialog Tabs
                     Performance with
                       Polar graph showing boat performance with and without tacking optimizations
                       with each sail plan different color selectable wind speed and thrust level
                       Graph showing boat speed over wind speed and over thrust level with selectable
                       angle of attack.
                       Graph showing apparent wind vs true wind 
                       Setup hull characteristics drag etc...
                       Can precompute these from a few values for actual measured performance
                     Sailing
                       Can add/remove sail configurations from table (reefs different sails etc..)
                       Has rules for when to switch to another configuration from each configuration
                       Always have "none" when sails are down
                     Electric Power
                       Timeline over current route graph
                       Renewable Storage
                         Battery capacity, efficiency leakage etc type etc..
                                  initial charge allow negative value?
                                        with type have coefficents for temp charge discharge tetc..
                         hydrogen tanks capacity efficiency etc..
                         pressurized air tanks  efficiency and capacity
                       
                       Photo Voltaic coefficients for power vs light
                       Hydro Generator coefficients for efficiency, use if
                                               wind speed is > x, boat speed is > x, battery charge
                                               will slow boat down
                       Wind Generator coefficients for efficiency wind-loading etc..
                                               can slow beating and help running
                       Fuel Generator coefficients for efficiency and capacity (will cut on in fuel shortage)
                       Human Generator coefficients for efficiency, human and willingness
                   Other propulsion...

                 Overall boat has 
                   Simulate mode renders the boats sailing various selectable patterns
                   with various winds
                 Interactive mode allows you to sail in realtime
             Goal
                  Save Load goal
                  Start Point1 ... End
                      Lat Lon Time  Reaching a point effectively kills the last iso and restarts from there

             Route Blockers
                  Configure areas to avoid
                  Can draw them, import coastline data etc, have a list of areas
                  Can select how to render them or not
                  Collisions with other vessels ais etc..
                  Rules to prevent certain boat conditions from occuring

                      If (VB VW VA A E B Fuel etc...) (< > = != etc..) 
                         (VB VW VA A E B Fuel etc...) (AND OR 

                         So if the swell is > 7 meters we may choose that 50 < VA < 65 || VA > 120
             Weather/Sea
                  Can import from grib file
                  Selecting either Wind or Current
                     Also allow user to specifyd speed and direction for a given:
                      timespan, and distance from cursor
                      Can render on screen speed and direction at a given time

                  Allow generated weather systems to simulate dynamic weather
                  Can use high and low pressure positions with value and weight
                  to create isobar map and from this generate wind map

                  Can import current data

                  Coefficients to reduce boat speed given swell data

            Demo
                  Loads or Saves all possible settings from a file.  Select
                  demos in the lsit.
                  Can create a demo.  Demos have a sequence so whenever you
                  do something it is recorded as an event in the table.  Certain
                  events have automatic delays when replayed

  */
//      Data Box
      wxStaticBox* itemStaticBoxData = new wxStaticBox(this, wxID_ANY, _("Advanced Routing"));
      wxStaticBoxSizer* itemStaticBoxSizerData= new wxStaticBoxSizer(itemStaticBoxData, wxVERTICAL);
                          boxSizer->Add(itemStaticBoxSizerData, 0, wxALL|wxEXPAND, border_size);

      wxFlexGridSizer *pDataGrid = new wxFlexGridSizer(3);
      pDataGrid->AddGrowableCol(1);
      itemStaticBoxSizerData->Add(pDataGrid, 0, wxALL|wxEXPAND, border_size);

// A horizontal box sizer to contain close
      wxBoxSizer* AckBox = new wxBoxSizer ( wxHORIZONTAL );
      boxSizer->Add ( AckBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

// The Boat button
      wxButton* bBoat = new wxButton ( this, ID_BOAT, _( "&Boat" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      AckBox->Add ( bBoat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

// The recompute button
      m_bCompute = new wxButton ( this, ID_COMPUTE, _( "&Compute" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      AckBox->Add ( m_bCompute, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

// The recompute button
      wxButton* bClear = new wxButton ( this, ID_CLEAR, _( "&Clear" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      AckBox->Add ( bClear, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

// The about button
      wxButton* bAbout = new wxButton ( this, ID_ABOUT, _( "&About" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      AckBox->Add ( bAbout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

// The close button
      wxButton* bClose = new wxButton ( this, ID_CLOSE, _( "&Close" ),
                                     wxDefaultPosition, wxDefaultSize, 0 );
      AckBox->Add ( bClose, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

      m_pBoatDialog = new BoatDialog(boat);
      m_pBoatDialog->Create( pParent, -1, _("Boat Configuration"),
                             wxPoint(100, 50), wxSize(600, 500));
      m_pBoatDialog->Hide();
}

void AdvancedRoutingDialog::SetBoatLatLon(double lat, double lon)
{
      m_boat_lon = lon;
      m_boat_lat = lat;
}

void AdvancedRoutingDialog::SetCursorLatLon(double lat, double lon)
{
  if(routemap.SetCursorLatLon(lat, lon))
    RequestRefresh(pParent);

  /* if we don't get nmea messages use last cursor */
  if(m_boat_lon == 0 && m_boat_lat==180) {
      m_boat_lon = lon;
      m_boat_lat = lat;
  }
}

void AdvancedRoutingDialog::RenderRouteMap(PlugIn_ViewPort *vp)
{
    m_thCompute.routemutex.Lock();
    routemap.Render(vp);
    m_thCompute.routemutex.Unlock();
}

void AdvancedRoutingDialog::OnClose ( wxCloseEvent& event )
{
      pPlugIn->OnAdvanced_RoutingDialogClose();
}

void AdvancedRoutingDialog::OnIdBoatClick ( wxCommandEvent& event )
{
      //Toggle Boat Dialog display
      m_bShowBoatDialog = !m_bShowBoatDialog;

      if(m_bShowBoatDialog)
            m_pBoatDialog->Show();
      else
            m_pBoatDialog->Hide();
}

void AdvancedRoutingDialog::OnIdComputeClick ( wxCommandEvent& event )
{
  if(m_thCompute.IsRunning()) {
    m_bCompute->SetLabel(_( "&Compute" ));
    m_thCompute.End();
    m_thCompute.Wait();
  } else {
//    m_bCompute->SetLabel(_( "&Stop" ));
//    m_thCompute.Run();
      if(routemap.origin.size() == 0) {
        OnIdClearClick(event);
      }
    routemap.Propagate(boat);
  }

  RequestRefresh(pParent);
}

void AdvancedRoutingDialog::OnIdClearClick ( wxCommandEvent& event )
{
  routemap.Reset(m_boat_lat, m_boat_lon, wxDateTime::Now());
  boat.ComputeBoatSpeeds(1, 1, 1);
//        boat.OptimizeTackingSpeed();
}

void AdvancedRoutingDialog::OnIdAboutClick ( wxCommandEvent& event )
{
  wxString msg(_("Advanced routing plugin\n"));
  msg.Append(_("Version "));
  msg.Append(wxString::Format(wxT("%i"),PLUGIN_VERSION_MAJOR));  msg.Append(_("."));
  msg.Append(wxString::Format(wxT("%i"),PLUGIN_VERSION_MINOR));  msg.Append(_("\n"));
  msg.Append(_("Compute iteratively positions the boat could possibly make it to by"
               " a certain time and merge the results to form iso routes to eventually"
               " create a map giving the quickest route to any given location\n"));
  msg.Append(_("The advanced routing plugin makes it possible to pre-compute the "
               "route you would have sailed that you would have been in to select"
               "the best route before leaving.\n"));

  msg.Append(_("Written by Sean D'Epagnier email <sean at depagnier dot com>"));
  wxMessageDialog md(this, msg, _("OpenCPN Advanced Routing Plugin"),
                       wxICON_INFORMATION | wxOK );
  md.ShowModal();
}

void AdvancedRoutingDialog::OnIdCloseClick ( wxCommandEvent& event )
{
      Close();
}

void AdvancedRoutingDialog::OnMove ( wxMoveEvent& event )
{
      //    Record the dialog position
      wxPoint p = event.GetPosition();
      pPlugIn->SetAdvancedRoutingDialogX(p.x);
      pPlugIn->SetAdvancedRoutingDialogY(p.y);

      event.Skip();
}

void AdvancedRoutingDialog::OnSize ( wxSizeEvent& event )
{
      //    Record the dialog size
      wxSize p = event.GetSize();
      pPlugIn->SetAdvancedRoutingDialogSizeX(p.x);
      pPlugIn->SetAdvancedRoutingDialogSizeY(p.y);

      event.Skip();
}

void *AdvancedRoutingThread::Entry()
{
  while(!stop) {
    routemutex.Lock();
    routemap.Propagate(boat);
    routemutex.Unlock();
    wxThread::Yield();
  }
  return 0;
}
