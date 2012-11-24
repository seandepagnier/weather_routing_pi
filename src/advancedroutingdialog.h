/******************************************************************************
 * $Id: advancedroutingdialog.h,v 1.8 2010/06/21 01:54:37 bdbcat Exp $
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
 */

#ifndef _ADVANCED_ROUTING_H_
#define _ADVANCED_ROUTING_H_


#include <wx/treectrl.h>
#include <wx/fileconf.h>
#include <wx/glcanvas.h>

class advanced_routing_pi;
class BoatDialog;

class AdvancedRoutingThread : public wxThread
{
public:
AdvancedRoutingThread(RouteMap &r, BoatSpeed &b) : routemap(r), boat(b), stop(false) {}
  void End() { stop = true; }
  void *Entry();
  RouteMap &routemap;
  BoatSpeed &boat;
  wxMutex routemutex;
  bool stop;
};

//----------------------------------------------------------------------------------------------------------
//    ADVANCED_ROUTING Selector/Control Dialog Specification
//----------------------------------------------------------------------------------------------------------
class AdvancedRoutingDialog : public wxDialog
{
      DECLARE_CLASS( AdvancedRoutingDialog )
                  DECLARE_EVENT_TABLE()
      public:

            AdvancedRoutingDialog( );
            ~AdvancedRoutingDialog( );

            bool Create(  wxWindow *parent, advanced_routing_pi *ppi, wxWindowID id = wxID_ANY,
                         const wxString& caption = _("ADVANCED_ROUTING Display Control"),
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );

           void CreateControls();

           void SetBoatLatLon(double lat, double lon);
           void SetCursorLatLon(double lat, double lon);

           void RenderRouteMap(PlugIn_ViewPort *vp);

            RouteMap routemap;

      private:
            void OnClose(wxCloseEvent& event);
            void OnIdBoatClick( wxCommandEvent& event );
            void OnIdComputeClick( wxCommandEvent& event );
            void OnIdClearClick( wxCommandEvent& event );
            void OnIdAboutClick( wxCommandEvent& event );
            void OnIdCloseClick( wxCommandEvent& event );
            void OnMove( wxMoveEvent& event );
            void OnSize( wxSizeEvent& event );

            //    Data
            wxWindow          *pParent;
            advanced_routing_pi   *pPlugIn;

            wxFont            *m_dFont;
            double            m_boat_lat, m_boat_lon;

            BoatSpeed boat;

            wxButton* m_bCompute;
            bool m_bComputing;

            AdvancedRoutingThread m_thCompute;

            bool m_bShowBoatDialog;
            BoatDialog *m_pBoatDialog;
};

#endif
