/******************************************************************************
 * $Id: ODAPI.h,v 1.0 2016/06/17 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin API
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 **************************************************************************/

#ifndef _ODAPI_H_
#define _ODAPI_H_

#ifndef DECL_EXP
#ifdef __WXMSW__
#  define DECL_EXP     __declspec(dllexport)
#else
# ifdef __GNUC__
# define DECL_EXP       __attribute__((visibility("default")))
# endif
#endif
#endif

struct FindPointInAnyBoundary_t {
    double  dLat;
    double  dLon;
    wxString    sBoundaryType;
    wxString    sBoundaryState;
    wxString    sName;
    wxString    sDescription;
    wxString    sGUID;
    wxString    sBoundaryObjectType;
};

struct FindClosestBoundaryLineCrossing_t {
    double      dStartLat;
    double      dStartLon;
    double      dEndLat;
    double      dEndLon;
    double      dCrossingLat;
    double      dCrossingLon;
    double      dCrossingDistance;
    wxString    sBoundaryType;
    wxString    sBoundaryState;
    wxString    sName;
    wxString    sDescription;
    wxString    sGUID;
    wxString    sBoundaryObjectType;
};

class ODAPI
{
public:
    ODAPI();
    ~ODAPI();
    static bool OD_FindPathByGUID( wxString sGUID, wxString sName, wxString sDescription );
    static bool OD_FindPointInAnyBoundary( FindPointInAnyBoundary_t *pFPIAB );
    static bool OD_FindClosestBoundaryLineCrossing( FindClosestBoundaryLineCrossing_t *pFCBLC );
    
protected:
    
private:
};

typedef bool (*OD_FindPathByGUID) (wxString, wxString *, wxString *);
typedef bool (*OD_FindPointInAnyBoundary) (FindPointInAnyBoundary_t *);
typedef bool (*OD_FindClosestBoundaryLineCrossing) (FindClosestBoundaryLineCrossing_t *);


#endif //_ODAPI_H_
