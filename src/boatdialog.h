/******************************************************************************
 * $Id: boatdialog.h,v 1.8 2010/06/21 01:54:37 bdbcat Exp $
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

#ifndef _BOAT_DIALOG_H_
#define _BOAT_DIALOG_H_

#include <wx/treectrl.h>
#include <wx/fileconf.h>


#define ID_BOAT                        10001
#define ID_COMPUTE                     10002
#define ID_CLEAR                       10003
#define ID_ABOUT                       10004
#define ID_CLOSE                       10005

class advanced_routing_pi;

//----------------------------------------------------------------------------------------------------------
//    BOAT_DIALOG Selector/Control Dialog Specification
//----------------------------------------------------------------------------------------------------------
class BoatDialog : public wxDialog
{
      DECLARE_CLASS( BoatDialog )
                  DECLARE_EVENT_TABLE()
      public:

            BoatDialog( BoatSpeed &boat );
            ~BoatDialog( );

            bool Create(  wxWindow *parent, wxWindowID id = wxID_ANY,
                         const wxString& caption = _("BOAT_DIALOG Display Control"),
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );

           void CreateControls();

      private:
            void OnClose(wxCloseEvent& event);
            void OnIdComputeClick( wxCommandEvent& event );
            void OnIdCloseClick( wxCommandEvent& event );
            void OnMove( wxMoveEvent& event );
            void OnSize( wxSizeEvent& event );

            //    Data
            wxWindow          *pParent;

};

class BoatCanvas : public wxWindow
{
                  DECLARE_EVENT_TABLE()
public:
  BoatCanvas(wxWindow *parent) : wxWindow(parent, wxID_ANY) {}
  void OnPaint(wxPaintEvent &event);
};

#endif
