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

#define ODAPI_VERSION_MAJOR 1
#define ODAPI_VERSION_MINOR 1

#include    <list>

enum {
    EXCLUSION_BOUNDARY = 0,
    INCLUSION_BOUNDARY,
    NEITHER_BOUNDARY
};

enum {
    TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS = 0,
    TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER,
    TEXTPOINT_DISPLAY_TEXT_SHOW_NEVER,
    TEXTPOINT_DISPLAY_TEXT_SHOW_DEFAULT,
    
    TEXTPOINT_DISPLAY_TEXT_SHOW_LAST
};

enum {
    TEXT_TOP = 0,
    TEXT_CENTRE_TOP,
    TEXT_BOTTOM,
    TEXT_CENTRE_BOTTOM,
    TEXT_CENTRE,
    TEXT_RIGHT,
    TEXT_LEFT,
    TEXT_DEFAULT,
    
    TEXT_POSTION_LAST
};

enum {
    TEXTPOINT_TEXT_BACKGROUND_TRANSPARANCY_DEFAULT = -1
};

struct ODAPIVersion_t {
    int         ODAPIVersionMajor;
    int         ODAPIVersionMinor;
    ODAPIVersion_t() : ODAPIVersionMajor(ODAPI_VERSION_MAJOR), ODAPIVersionMinor(ODAPI_VERSION_MINOR) {}
};

struct HyperLinkList_t {
    wxString    sDescription;
    wxString    sLink;
    wxString    sType;
};

struct FindPointInAnyBoundary_t {
    double      dLat;
    double      dLon;
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

struct CreateBoundaryPoint_t : public ODAPIVersion_t {
    wxString    name;
    wxString    iconname;
    double      lat;
    double      lon;
    int         type;
    bool        visible;
    bool        ringsvisible;
    int         ringsnumber;
    double      ringssteps;
    int         ringsunits;
    bool        defaultRingColour;
    wxColour    ringscolour;
    wxString    GUID;
    std::list   <HyperLinkList_t *> BoundaryPointHyperLinkList;
};

struct DeleteBoundaryPoint_t : public ODAPIVersion_t {
    wxString    GUID;
};

struct CreateBoundary_t : public ODAPIVersion_t {
    wxString    name;
    int         type;
    bool        pathIsActive;
    bool        visible;
    bool        defaultLineColour;
    wxColour    lineColour;
    bool        defaultFillColour;
    wxColour    fillColour;
    wxString    GUID;
    bool        temporary;
    std::list   <CreateBoundaryPoint_t *> BoundaryPointsList;
};

struct DeleteBoundary_t : public ODAPIVersion_t {
    wxString    GUID;
};

struct CreateTextPoint_t : public ODAPIVersion_t{
    wxString    name;               //Point name
    wxString    iconname;           //Icon Name
    double      lat;
    double      lon;
    wxString    description;
    wxString    TextToDisplay;
    bool        Visible;            //Point visible
    bool        ShowName;           //Show name
    int         TextPosition;       //enum, i.e. TEXT_CENTRE
    wxString    TextColour;
    wxString    BackgroundColour;
    int         BackgroundTransparancy;
    bool        defaultFont;
    wxFont      TextFont;
    int         TextPointDisplayTextWhen;
    bool        defaultRangeRings;
    bool        ringsvisible;
    int         ringsnumber;
    double      ringssteps;
    int         ringsunits;
    bool        defaultRingColour;
    wxString    ringscolour;
    wxString    GUID;
    std::list   <HyperLinkList_t *> TextPointHyperLinkList;
    bool        temporary;
};

struct DeleteTextPoint_t : public ODAPIVersion_t {
    wxString    GUID;
};

struct AddPointIcon_t : ODAPIVersion_t {
    wxBitmap    PointIcon;
    wxString    PointIconName;
    wxString    PointIconDescription;
};

struct DeletePointIcon_t :ODAPIVersion_t {
    wxString    PointIconName;
};

class ODAPI
{
public:
    ODAPI();
    ~ODAPI();
    static bool OD_FindPathByGUID( wxString sGUID, wxString *sName, wxString *sDescription );
    static bool OD_FindPointInAnyBoundary( FindPointInAnyBoundary_t *pFPIAB );
    static bool OD_FindClosestBoundaryLineCrossing( FindClosestBoundaryLineCrossing_t *pFCBLC );
    static bool OD_FindFirstBoundaryLineCrossing( FindClosestBoundaryLineCrossing_t *pFCBLC );
    static bool OD_CreateBoundary( CreateBoundary_t *pCB );
    static bool OD_CreateBoundaryPoint( CreateBoundaryPoint_t *pCBP );
    static bool OD_CreateTextPoint( CreateTextPoint_t *pCTP );
    static bool OD_AddPointIcon( AddPointIcon_t *pAPI );
    static bool OD_DeletePointIcon( DeletePointIcon_t *pDPI );
    static bool OD_DeleteBoundary( DeleteBoundary_t *pDB );
    static bool OD_DeleteBoundaryPoint( DeleteBoundaryPoint_t *pDBP );
    static bool OD_DeleteTextPoint( DeleteTextPoint_t *pDTP );
    
protected:
    
private:
    
};

typedef bool (*OD_FindPathByGUID) (wxString, wxString *, wxString *);
typedef bool (*OD_FindPointInAnyBoundary) (FindPointInAnyBoundary_t *);
typedef bool (*OD_FindClosestBoundaryLineCrossing) (FindClosestBoundaryLineCrossing_t *);
typedef bool (*OD_FindFirstBoundaryLineCrossing) (FindClosestBoundaryLineCrossing_t *);
typedef bool (*OD_CreateBoundary) (CreateBoundary_t *);
typedef bool (*OD_CreateBoundaryPoint) (CreateBoundaryPoint_t *);
typedef bool (*OD_CreateTextPoint) (CreateTextPoint_t *);
typedef bool (*OD_AddPointIcon) (AddPointIcon_t *);
typedef bool (*OD_DeletePointIcon) (DeletePointIcon_t *);
typedef bool (*OD_DeleteBoundary) (DeleteBoundary_t *);
typedef bool (*OD_DeleteBoundaryPoint) (DeleteBoundaryPoint_t *);
typedef bool (*OD_DeleteTextPoint) (DeleteTextPoint_t *);

#endif //_ODAPI_H_
