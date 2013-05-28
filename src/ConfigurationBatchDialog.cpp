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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 */

#include <wx/wx.h>
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "BatchDialog.h"

BatchDialog::BatchDialog( wxWindow *parent )
    : BatchDialogBase(parent)
{
    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
#ifdef __WXMSW__
    wxString stdPath  = std_path.GetConfigDir();
#endif
#ifdef __WXGTK__
    wxString stdPath  = std_path.GetUserDataDir();
#endif
#ifdef __WXOSX__
    wxString stdPath  = std_path.GetUserConfigDir();   // should be ~/Library/Preferences	
#endif

    wxString batch_path = stdPath + wxFileName::GetPathSeparator() + _T("batchs.xml");
}

BatchDialog::~BatchDialog()
{
}
