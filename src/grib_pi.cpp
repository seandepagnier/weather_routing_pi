/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  GRIB Plugin
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include "../../grib_pi/src/grib_pi.h"

grib_pi::grib_pi(void *ppimgr)      :opencpn_plugin_17(ppimgr){}
grib_pi::~grib_pi(void){}
int grib_pi::Init(void){}
bool grib_pi::DeInit(void){}
int grib_pi::GetAPIVersionMajor(){    return 0;}
int grib_pi::GetAPIVersionMinor(){    return 0;}
int grib_pi::GetPlugInVersionMajor(){    return 0;}
int grib_pi::GetPlugInVersionMinor(){    return 0;}
wxBitmap *grib_pi::GetPlugInBitmap(){    return NULL;}
wxString grib_pi::GetCommonName(){      return _("GRIB");}
wxString grib_pi::GetShortDescription(){    return _("");}
wxString grib_pi::GetLongDescription(){    return _("");}
void grib_pi::SetDefaults(void){}
int grib_pi::GetToolbarToolCount(void){      return 1;}
void grib_pi::ShowPreferencesDialog( wxWindow* parent ){}
void grib_pi::OnToolbarToolCallback(int id){}
void grib_pi::OnGribDialogClose(){}
bool grib_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp){}
bool grib_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp){}
void grib_pi::SetCursorLatLon(double lat, double lon){}
void grib_pi::SetPluginMessage(wxString &message_id, wxString &message_body){}
bool grib_pi::LoadConfig(void) {    return true; }
bool grib_pi::SaveConfig(void) {    return true; }
void grib_pi::SetColorScheme(PI_ColorScheme cs) {}
