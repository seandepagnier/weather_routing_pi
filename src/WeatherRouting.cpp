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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 */

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/progdlg.h>
#include <wx/dir.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <wx/glcanvas.h>

#include "tinyxml/tinyxml.h"

#include "Utilities.h"
#include "Boat.h"
#include "BoatDialog.h"
#include "RouteMapOverlay.h"
#include "weather_routing_pi.h"
#include "WeatherRouting.h"
#include "AboutDialog.h"
#include "icons.h"

/* XPM */
static const char *eye[]={
"20 20 7 1",
". c none",
"# c #000000",
"a c #333333",
"b c #666666",
"c c #999999",
"d c #cccccc",
"e c #ffffff",
"....................",
"....................",
"....................",
"....................",
".......######.......",
".....#aabccb#a#.....",
"....#deeeddeebcb#...",
"..#aeeeec##aceaec#..",
".#bedaeee####dbcec#.",
"#aeedbdabc###bcceea#",
".#bedad######abcec#.",
"..#be#d######dadb#..",
"...#abac####abba#...",
".....##acbaca##.....",
".......######.......",
"....................",
"....................",
"....................",
"....................",
"...................."};

WeatherRoute::WeatherRoute() : routemapoverlay(new RouteMapOverlay) {}
WeatherRoute::~WeatherRoute() { delete routemapoverlay; }

const wxString WeatherRouting::column_names[NUM_COLS] = {"Visible", "Boat", "Start", "Start Time",
                                                         "End", "End Time", "Time", "Distance",
                                                         "Avg Speed", "Max Speed",
                                                         "Avg Speed Ground", "Max Speed Ground",
                                                         "Avg Wind", "Max Wind", "Max Wind Gust",
                                                         "Avg Current", "Max Current",
                                                         "Avg Swell", "Max Swell",
                                                         "Upwind Percentage",
                                                         "Port Starboard", "Tacks", "Comfort",
                                                         "State"};

static int sortcol, sortorder = 1;
// sort callback. Sort by body.
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortWeatherRoutes(wxIntPtr item1, wxIntPtr item2, wxIntPtr list)
#else
int wxCALLBACK SortWeatherRoutes(long item1, long item2, long list)
#endif            
{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;

    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(sortcol);
    
    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(sortcol);
    
    lc->GetItem(it1);
    lc->GetItem(it2);
    
    return sortorder * it1.GetText().Cmp(it2.GetText());
}

WeatherRouting::WeatherRouting(wxWindow *parent, weather_routing_pi &plugin)
    : WeatherRoutingBase(parent), m_panel(NULL), m_ConfigurationDialog(*this),
      m_ConfigurationBatchDialog(this), m_CursorPositionDialog(this),
      // CUSTOMIZATION
      m_RoutePositionDialog(this),
      m_BoatDialog(*this),
      m_SettingsDialog(this), m_StatisticsDialog(this), m_ReportDialog(*this),
      m_PlotDialog(*this), m_FilterRoutesDialog(this),
      m_bRunning(false), m_RoutesToRun(0), m_bSkipUpdateCurrentItems(false),
      m_bShowConfiguration(false), m_bShowConfigurationBatch(false),
      m_bShowRoutePosition(false), m_bShowSettings(false),
      m_bShowStatistics(false), m_bShowReport(false), m_bShowPlot(false),
      m_bShowFilter(false), m_weather_routing_pi(plugin),
      m_positionOnRoute(NULL)
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );
    
    wxIcon icon;
    icon.CopyFromBitmap(*_img_WeatherRouting);
    m_ConfigurationDialog.SetIcon(icon);
    m_ConfigurationBatchDialog.SetIcon(icon);
    m_BoatDialog.SetIcon(icon);
    m_SettingsDialog.SetIcon(icon);
    m_StatisticsDialog.SetIcon(icon);
    m_ReportDialog.SetIcon(icon);
    m_PlotDialog.SetIcon(icon);
    m_FilterRoutesDialog.SetIcon(icon);

    m_default_configuration_path = weather_routing_pi::StandardPath()
        + _T("WeatherRoutingConfiguration.xml");

    bool forceCopyBoats = false;
    bool forceCopyPolars = false;
    wxString boatsdir = weather_routing_pi::StandardPath() + wxFileName::GetPathSeparator() + _T("boats");
    if (!wxFileName::DirExists(boatsdir))
        forceCopyBoats = true;
    wxString polarsdir = weather_routing_pi::StandardPath() + wxFileName::GetPathSeparator() + _T("polars");
    if (!wxFileName::DirExists(polarsdir))
        forceCopyPolars = true;

    /* ensure the directories exist */
    wxFileName fn;
    fn.Mkdir(weather_routing_pi::StandardPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    fn.Mkdir(boatsdir);
    fn.Mkdir(polarsdir);

    /* if the boats or polars directories did not previously exist, populate them */
    if (forceCopyBoats)
        CopyDataFiles(*GetpSharedDataLocation() + _T("plugins/weather_routing_pi/data/boats"), boatsdir);
    if (forceCopyPolars)
        CopyDataFiles(*GetpSharedDataLocation() + _T("plugins/weather_routing_pi/data/polars"), polarsdir);

    int confVersion;
    pConf->Read ( _T ( "ConfigVersion" ), &confVersion, 0);

#ifndef __OCPN__ANDROID__
    if (confVersion < PLUGIN_VERSION_MAJOR * 100 + PLUGIN_VERSION_MINOR) {
        wxString title = _("New or updated data available");
        wxString message =
            _("A new version of the Weather Route plugin has been installed.\n\n"
              "\"Import new boats and polars\" will overwrite the standard boats\n"
              "and polars with newer data. If you have modified this data and not\n"
              "changed the names, your modifications will be overwritten, so be\n"
              "sure to backup your changes. If you have added new polars or boats\n"
              "with exclusive names, they will be kept untouched.\n\n"
              "\"Import example configurations\" will overwrite your route\n"
              "configurations with a sample set showing you how WeatherRouting\n"
              "works. Backup your existing configurations if you need.\n\n"
              "Pressing \"OK\" will apply the selected changes, pressing \"Cancel\"\n"
              "will do nothing and you will be asked again on the next launch.");
        wxString confDlgChoices[3] = {
          _("Import new boats and polars"),
          _("Import example configurations")};

        wxMultiChoiceDialog confDlg(this, message, title, 2, confDlgChoices);
	/* check on by default if user starts WR for the first time */
	if (confVersion == 0) {
	    wxArrayInt sel;
	    sel.Add(0);
	    sel.Add(1);
	    confDlg.SetSelections(sel);
	}

        if ( confDlg.ShowModal() == wxID_OK ) {
	    wxArrayInt result = confDlg.GetSelections();
	    for (size_t i = 0; i <
                     result.GetCount();
                 i++) {
	        if (result[i] == 0) {
		    CopyDataFiles(*GetpSharedDataLocation() + _T("plugins/weather_routing_pi/data/boats"), boatsdir);
		    CopyDataFiles(*GetpSharedDataLocation() + _T("plugins/weather_routing_pi/data/polars"), polarsdir);
		}
		else if (result[i] == 1) {
		    wxString cfg = *GetpSharedDataLocation() + _T("plugins/weather_routing_pi/data/") + _T("WeatherRoutingConfiguration.xml");
		    if (wxFileName::FileExists(cfg))
			wxCopyFile(cfg, m_default_configuration_path);
		}
            }
            pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) ); // path can change after modal dialog
            pConf->Write (  _T ( "ConfigVersion" ), PLUGIN_VERSION_MAJOR * 100 + PLUGIN_VERSION_MINOR);
        }
    }
#endif
    m_SettingsDialog.LoadSettings();
    
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) ); // path can change after modal dialog
    pConf->Read ( _T ( "DisableColPane" ), &m_disable_colpane, false);
#ifdef __OCPN__ANDROID__
    m_disable_colpane = true;
#endif
    
    wxBoxSizer* bSizer;
    bSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(bSizer);
    if(!m_disable_colpane) {
        m_colpane = new wxCollapsiblePane(this, wxID_ANY, _("Weather Routing"), wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
        bSizer->Add(m_colpane, 1, wxEXPAND|wxALL, 5);
        m_colpaneWindow = m_colpane->GetPane();
        wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);
        m_colpaneWindow->SetSizer(paneSz);
        m_panel = new WeatherRoutingPanel(m_colpaneWindow);
        paneSz->Add(m_panel, 1, wxEXPAND, 0);
        paneSz->SetSizeHints(m_colpaneWindow);
    } else {
        m_colpane = NULL;
        m_colpaneWindow = this;
        m_panel = new WeatherRoutingPanel(m_colpaneWindow);
        bSizer->Add(m_panel, 1, wxEXPAND, 0);
    }
    bSizer->SetSizeHints(this);
    

    m_panel->m_lPositions->InsertColumn(POSITION_NAME, _("Name"));
    m_panel->m_lPositions->InsertColumn(POSITION_LAT, _("Lat"));
    m_panel->m_lPositions->InsertColumn(POSITION_LON, _("Lon"));

    wxImageList *imglist = new wxImageList(20, 20, true, 1);
    imglist->Add(wxBitmap(eye));
    m_panel->m_lWeatherRoutes->AssignImageList(imglist, wxIMAGE_LIST_SMALL);

    UpdateColumns();

    if(m_colpane)
        m_colpane->Expand();

    OpenXML(m_default_configuration_path, false);

    wxPoint p = GetPosition();
    pConf->Read ( _T ( "DialogX" ), &p.x, p.x);
    pConf->Read ( _T ( "DialogY" ), &p.y, p.y);

    m_size = GetSize();
    pConf->Read ( _T ( "DialogWidth" ), &m_size.x, wxMax(m_size.x, 100));
    pConf->Read ( _T ( "DialogHeight" ), &m_size.y, wxMax(m_size.y, 100));
#ifdef __OCPN__ANDROID__
    wxSize sz = ::wxGetDisplaySize();
    m_size.x = sz.x*3/5;
    m_size.y = sz.y*2/5;
    int y = 2*sz.y/3-40;
    if(m_size.y > y)
        m_size.y = y;
#endif
    SetSize(p.x, p.y, m_size.x, m_size.y);

    pConf->Read ( _T ( "DialogSplit" ), &sashpos, 0);

    /* periodically check for updates from computation thread */
    m_tCompute.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRouting::OnComputationTimer ), NULL, this);

    m_tHideConfiguration.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( WeatherRouting::OnHideConfigurationTimer ), NULL, this);

    Connect(wxEVT_IDLE, wxTimerEventHandler
                       ( WeatherRouting::OnRenderedTimer ), NULL, this);

    SetEnableConfigurationMenu();
    
    // Connect Events
    if(m_colpane)
        m_colpane->Connect( wxEVT_COLLAPSIBLEPANE_CHANGED, wxCollapsiblePaneEventHandler( WeatherRouting::OnCollPaneChanged ), NULL, this );
    //m_panel->m_lPositions->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WeatherRouting::OnEditPositionClick ), NULL, this );
    m_panel->m_lPositions->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( WeatherRouting::OnPositionKeyDown ), NULL, this );
    m_panel->m_lWeatherRoutes->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WeatherRouting::OnEditConfigurationClick ), NULL, this );
    m_panel->m_lWeatherRoutes->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WeatherRouting::OnWeatherRoutesListLeftDown ), NULL, this );
    m_panel->m_lWeatherRoutes->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( WeatherRouting::OnLeftUp ), NULL, this );    
    m_panel->m_lPositions->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WeatherRouting::OnLeftDown ), NULL, this );
    m_panel->m_lPositions->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( WeatherRouting::OnLeftUp ), NULL, this );    
    m_panel->m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( WeatherRouting::OnWeatherRouteSort ), NULL, this );
    m_panel->m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( WeatherRouting::OnWeatherRouteSelected ), NULL, this );
    m_panel->m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( WeatherRouting::OnWeatherRouteSelected ), NULL, this );
    m_panel->m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( WeatherRouting::OnWeatherRouteKeyDown ), NULL, this );
    m_panel->m_bCompute->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRouting::OnCompute ), NULL, this );
    m_panel->m_bExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRouting::OnExport ), NULL, this );


#ifdef __OCPN__ANDROID__ 
    GetHandle()->setAttribute(Qt::WA_AcceptTouchEvents);
    GetHandle()->grabGesture(Qt::PanGesture);
    GetHandle()->setStyleSheet( qtStyleSheet);

    GetHandle()->setAttribute(Qt::WA_AcceptTouchEvents);
    GetHandle()->grabGesture(Qt::PanGesture);
    Connect( wxEVT_QT_PANGESTURE,
                       (wxObjectEventFunction) (wxEventFunction) &WeatherRouting::OnEvtPanGesture, NULL, this );
    m_tDownTimer.Connect(wxEVT_TIMER, wxTimerEventHandler
                         ( WeatherRouting::OnDownTimer ), NULL, this);
#endif

}

WeatherRouting::~WeatherRouting( )
{
    // Disconnect Events
    if(m_colpane)
        m_colpane->Disconnect( wxEVT_COLLAPSIBLEPANE_CHANGED, wxCollapsiblePaneEventHandler( WeatherRouting::OnCollPaneChanged ), NULL, this );
    //m_panel->m_lPositions->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WeatherRouting::OnEditPositionClick ), NULL, this );
    m_panel->m_lPositions->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( WeatherRouting::OnPositionKeyDown ), NULL, this );
    m_panel->m_lWeatherRoutes->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WeatherRouting::OnEditConfigurationClick ), NULL, this );
    m_panel->m_lWeatherRoutes->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WeatherRouting::OnWeatherRoutesListLeftDown ), NULL, this );
    m_panel->m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( WeatherRouting::OnWeatherRouteSort ), NULL, this );
    m_panel->m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( WeatherRouting::OnWeatherRouteSelected ), NULL, this );
    m_panel->m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( WeatherRouting::OnWeatherRouteSelected ), NULL, this );
    m_panel->m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( WeatherRouting::OnWeatherRouteKeyDown ), NULL, this );
    m_panel->m_bCompute->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRouting::OnCompute ), NULL, this );
    m_panel->m_bExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRouting::OnExport ), NULL, this );
    
    Stop();

    m_SettingsDialog.SaveSettings();

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );

    wxPoint p = GetPosition();
    pConf->Write ( _T ( "DialogX" ), p.x);
    pConf->Write ( _T ( "DialogY" ), p.y);

    pConf->Write ( _T ( "DialogWidth" ), m_size.x);
    pConf->Write ( _T ( "DialogHeight" ), m_size.y);
    pConf->Write ( _T ( "DialogSplit" ), m_panel->m_splitter1->GetSashPosition());

    SaveXML(m_FileName.GetFullPath());

    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++)
        delete *it;
    delete m_panel;
    delete m_colpane;
}

#ifdef __OCPN__ANDROID__ 
void WeatherRouting::OnEvtPanGesture( wxQT_PanGestureEvent &event)
{
    switch(event.GetState()){
        case GestureStarted:
            m_startPos = GetPosition();
            m_startMouse = event.GetCursorPos(); //g_mouse_pos_screen;
            break;
        default:
        {
            wxPoint pos = event.GetCursorPos();
            int x = wxMax(0, pos.x + m_startPos.x - m_startMouse.x);
            int y = wxMax(0, pos.y + m_startPos.y - m_startMouse.y);
            int xmax = ::wxGetDisplaySize().x - GetSize().x;
            x = wxMin(x, xmax);
            int ymax = ::wxGetDisplaySize().y - GetSize().y;          // Some fluff at the bottom
            y = wxMin(y, ymax);
            
            Move(x, y);
            m_tDownTimer.Stop();
        } break;
    }
}
#endif

void WeatherRouting::OnLeftDown( wxMouseEvent& event )
{
    m_tDownTimer.Start(1200, true);
    m_downPos = event.GetPosition();
}

void WeatherRouting::OnLeftUp( wxMouseEvent& event )
{
    m_tDownTimer.Stop();
}

void WeatherRouting::OnDownTimer( wxTimerEvent & )
{
    m_panel->m_lWeatherRoutes->PopupMenu( m_mContextMenu, m_downPos );
}

void WeatherRouting::CopyDataFiles(wxString from, wxString to)
{
    if (from[from.Len() - 1] != '\\' && from[from.Len() - 1] != '/') from += wxFILE_SEP_PATH;
    if (to[to.Len() - 1] != '\\' && to[to.Len() - 1] != '/') to += wxFILE_SEP_PATH;

    if (!wxDirExists(to))
	wxFileName::Mkdir(to, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    wxDir dir(from);
    wxString next = wxEmptyString;
    bool b = dir.GetFirst(&next);
    while (b) {
        const wxString fileFrom = from + next;
        const wxString fileTo = to + next;
        if (wxDirExists(fileFrom))
            CopyDataFiles(fileFrom, fileTo);
        else
	    wxCopyFile(fileFrom, fileTo);
        b = dir.GetNext(&next);
    }
}

void WeatherRouting::Render(piDC &dc, PlugIn_ViewPort &vp)
{
    if (vp.bValid == false)
        return;

    // polling is bad
    bool work = false;
    for(auto &it :RouteMap::Positions) {
        if (it.GUID.IsEmpty())
            continue;

        PlugIn_Waypoint waypoint;
        double lat = it.lat;
        double lon = it.lon;

        if (!GetSingleWaypoint( it.GUID, &waypoint ))
            continue;
        if (lat == waypoint.m_lat && lon == waypoint.m_lon && waypoint.m_MarkName.IsSameAs(it.Name))
            continue;

        long index = m_panel->m_lPositions->FindItem(0, it.ID);
        if (index < 0) {
            // corrupted data
            continue;
        }

        wxString name = waypoint.m_MarkName;
        lat = waypoint.m_lat;
        lon = waypoint.m_lon;
        it.Name = name;
        it.lat = lat;
        it.lon = lon;

        // XXX FIXME there's already this name, update m_ConfigurationDialog source
        m_panel->m_lPositions->SetItem(index, POSITION_NAME, name);
        m_panel->m_lPositions->SetColumnWidth(POSITION_NAME, wxLIST_AUTOSIZE);
        m_panel->m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
        m_panel->m_lPositions->SetColumnWidth(POSITION_LAT, wxLIST_AUTOSIZE);
        m_panel->m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
        m_panel->m_lPositions->SetColumnWidth(POSITION_LON, wxLIST_AUTOSIZE);
        work = true;
    }
    if (work) {
        UpdateConfigurations();
        Reset();
    }

    if(!dc.GetDC()) {
#ifndef __OCPN__ANDROID__
        glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
        glEnable( GL_LINE_SMOOTH );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
#endif
        glEnable( GL_BLEND );
    }

    wxDateTime time = m_ConfigurationDialog.m_GribTimelineTime;
    if(!time.IsValid())
        time = wxDateTime::UNow();

    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
        if(weatherroute->routemapoverlay->m_bEndRouteVisible)
        {
            weatherroute->routemapoverlay->Render(time, m_SettingsDialog, dc, vp, true);
        }
    }

    std::list<RouteMapOverlay *>currentroutemaps = CurrentRouteMaps();
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++) {
        (*it)->Render(time, m_SettingsDialog, dc, vp, false, m_positionOnRoute);
        
        if(it == currentroutemaps.begin() &&
           m_SettingsDialog.m_cbDisplayWindBarbs->GetValue())
            (*it)->RenderWindBarbs(dc, vp);
        if(it == currentroutemaps.begin() &&
           m_SettingsDialog.m_cbDisplayCurrent->GetValue())
            (*it)->RenderCurrent(dc, vp);
    }

    m_ConfigurationBatchDialog.Render(dc, vp);

#ifndef __OCPN__ANDROID__
    if(!dc.GetDC())
        glPopAttrib();
#endif
}

void WeatherRouting::UpdateDisplaySettings()
{
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
        weatherroute->routemapoverlay->m_UpdateOverlay = true;
    }

    GetParent()->Refresh();
}

void WeatherRouting::AddPosition(double lat, double lon)
{
    wxTextEntryDialog pd( this, _("Enter Name"), _("New Position") );
    if(pd.ShowModal() == wxID_OK)
        AddPosition(lat, lon, pd.GetValue());
}

void WeatherRouting::AddPosition(double lat, double lon, wxString name)
{
    for(auto it = RouteMap::Positions.begin(); it != RouteMap::Positions.end(); it++) {
        if((*it).GUID.IsEmpty() && (*it).Name == name) {
            wxMessageDialog mdlg(this, _("This name already exists, replace?\n"),
                                 _("Weather Routing"), wxYES | wxNO | wxICON_WARNING);
            if(mdlg.ShowModal() == wxID_YES) {
                long index = m_panel->m_lPositions->FindItem(0, (*it).ID);
                assert(index >=0);

                (*it).lat = lat;
                (*it).lon = lon;
                (*it).GUID = wxEmptyString;  // no more an O waypoint
                m_panel->m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
                m_panel->m_lPositions->SetColumnWidth(POSITION_LAT, wxLIST_AUTOSIZE);
                m_panel->m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
                m_panel->m_lPositions->SetColumnWidth(POSITION_LON, wxLIST_AUTOSIZE);
                UpdateConfigurations();
            }
            return;
        }
    }
    
    RouteMapPosition p(name, lat, lon);
    RouteMap::Positions.push_back(p);
    UpdateConfigurations();

    wxListItem item;
    long index = m_panel->m_lPositions->InsertItem(m_panel->m_lPositions->GetItemCount(), item);

    m_panel->m_lPositions->SetItem(index, POSITION_NAME, name);
    m_panel->m_lPositions->SetColumnWidth(POSITION_NAME, wxLIST_AUTOSIZE);
    m_panel->m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
    m_panel->m_lPositions->SetColumnWidth(POSITION_LAT, wxLIST_AUTOSIZE);
    m_panel->m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
    m_panel->m_lPositions->SetColumnWidth(POSITION_LON, wxLIST_AUTOSIZE);

    m_panel->m_lPositions->SetItemData(index, p.ID);
    m_ConfigurationDialog.AddSource(name);
    m_ConfigurationBatchDialog.AddSource(name);
}

void WeatherRouting::AddPosition(double lat, double lon, wxString name, wxString GUID)
{
    if (GUID.IsEmpty())
        return AddPosition(lat, lon, name);

    for(auto it = RouteMap::Positions.begin();it != RouteMap::Positions.end(); it++) {
        if((*it).GUID.IsEmpty() /*&& !(*it).NameIsSameAs(name)*/)
            continue;

        if((*it).GUID.IsSameAs(GUID)) {
            // wxMessageDialog mdlg(this, _("This name already exists, replace?\n"),_("Weather Routing"), wxYES | wxNO | wxICON_WARNING);
            long index = m_panel->m_lPositions->FindItem(0, (*it).ID);
            (*it).lat = lat;
            (*it).lon = lon;
            m_panel->m_lPositions->SetItem(index, POSITION_NAME, name);
            m_panel->m_lPositions->SetColumnWidth(POSITION_NAME, wxLIST_AUTOSIZE);
            m_panel->m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
            m_panel->m_lPositions->SetColumnWidth(POSITION_LAT, wxLIST_AUTOSIZE);
            m_panel->m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
            m_panel->m_lPositions->SetColumnWidth(POSITION_LON, wxLIST_AUTOSIZE);
            UpdateConfigurations();
            return;
        }
    }
    
    RouteMapPosition p(name, lat, lon, GUID);
    RouteMap::Positions.push_back(p);
    UpdateConfigurations();

    wxListItem item;
    long index = m_panel->m_lPositions->InsertItem(m_panel->m_lPositions->GetItemCount(), item);
    m_panel->m_lPositions->SetItem(index, POSITION_NAME, name);
    m_panel->m_lPositions->SetColumnWidth(POSITION_NAME, wxLIST_AUTOSIZE);
    
    m_panel->m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
    m_panel->m_lPositions->SetColumnWidth(POSITION_LAT, wxLIST_AUTOSIZE);
    m_panel->m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));
    m_panel->m_lPositions->SetColumnWidth(POSITION_LON, wxLIST_AUTOSIZE);
    m_panel->m_lPositions->SetItemData(index, p.ID);

    m_ConfigurationDialog.AddSource(name);
    m_ConfigurationBatchDialog.AddSource(name);
}

void WeatherRouting::CursorRouteChanged()
{
    if(m_PlotDialog.IsShown() && m_PlotDialog.m_rbCursorRoute->GetValue())
        m_PlotDialog.SetRouteMapOverlay(FirstCurrentRouteMap());
}

void WeatherRouting::UpdateColumns()
{
    m_panel->m_lWeatherRoutes->DeleteAllColumns();

    for(int i=0; i<NUM_COLS; i++) {
        if(m_SettingsDialog.m_cblFields->IsChecked(i)) {
            columns[i] = m_panel->m_lWeatherRoutes->GetColumnCount();
            wxString name = _(column_names[i]);

            if(i == STARTTIME || i == ENDTIME) {
                name += _T(" (");
                if(m_SettingsDialog.m_cbUseLocalTime->GetValue())
                    name += _("local");
                else
                    name += _T("UTC");
                name += _T(")");
            }

            m_panel->m_lWeatherRoutes->InsertColumn(columns[i], name);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[i], wxLIST_AUTOSIZE);
        } else
            columns[i] = -1;
    }

    std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++, it++) {
        m_panel->m_lWeatherRoutes->SetItemPtrData(i, (wxUIntPtr)*it); // somehow this gets lost
        (*it)->Update(this); // update utc/local switch to strings of start/end time
        UpdateItem(i);
    }

    OnWeatherRouteSelected(); // update utc/local switch if configuration dialog is visible
}

static void CursorPositionDialogMessage(CursorPositionDialog &dlg, wxString msg)
{
    dlg.m_stPosition->SetLabel(msg);
    dlg.m_stPosition->Fit();
    dlg.m_stTime->SetLabel(_T(""));
    dlg.m_stPolar->SetLabel(_T(""));
    dlg.m_stSailChanges->SetLabel(_T(""));
    dlg.m_stTacks->SetLabel(_T(""));
    dlg.m_stWeatherData->SetLabel(_T(""));
    dlg.Fit();
}

static void RoutePositionDialogMessage(RoutePositionDialog &dlg, wxString msg)
{
    dlg.m_stPosition->SetLabel(msg);
    dlg.m_stPosition->Fit();
    dlg.m_stTime->SetLabel(_T(""));
    dlg.m_stPolar->SetLabel(_T(""));
    dlg.m_stSailChanges->SetLabel(_T(""));
    dlg.m_stTacks->SetLabel(_T(""));
    dlg.m_stWeatherData->SetLabel(_T(""));
    dlg.Fit();
}

void WeatherRouting::UpdateCursorPositionDialog()
{
    CursorPositionDialog &dlg = m_CursorPositionDialog;
    if(!dlg.IsShown())
        return;

    std::list<RouteMapOverlay*> currentroutemaps = CurrentRouteMaps();
    if(currentroutemaps.size() != 1) {
        CursorPositionDialogMessage(dlg, _("Select exactly 1 configuration"));
        return;
    }

    RouteMapOverlay *rmo = currentroutemaps.front();
    Position *p = rmo->GetLastCursorPosition();
    if(!p) {
        CursorPositionDialogMessage(dlg, _("Cursor outside computed route map"));
        return;
    }
    wxDateTime display_time = rmo->GetLastCursorTime();

    if(m_SettingsDialog.m_cbUseLocalTime->GetValue())
        display_time = display_time.FromUTC();

    dlg.m_stTime->SetLabel(display_time.Format(_T("%x %H:%M")));

    RouteMapConfiguration configuration = rmo->GetConfiguration();
    wxString pos = wxString::Format(_T("%4.2f%c %4.2f%c"),
                                    fabs(p->lat), p->lat < 0 ? 'S' : 'N',
                                    fabs(p->lon), p->lon < 0 ? 'W' : 'E');
    dlg.m_stPosition->SetLabel(pos);

    if(p->polar == -1)
        dlg.m_stPolar->SetLabel(wxEmptyString);
    else {
        wxFileName fn = configuration.boat.Polars[p->polar].FileName;
        dlg.m_stPolar->SetLabel(fn.GetFullName());
    }

    dlg.m_stSailChanges->SetLabel(wxString::Format(_T("%d"), p->SailChanges()));

    dlg.m_stTacks->SetLabel(wxString::Format(_T("%d"), p->tacks));

    wxString weatherdata;
    wxString grib = _("Grib") + _T(" ");
    wxString climatology = _("Climatology") + _T(" ");
    wxString data_deficient = _("Data Deficient") + _T(" ");
    wxString wind = _("Wind") + _T(" ");
    wxString current = _("Current") + _T(" ");
    
    if(p->data_mask & Position::GRIB_WIND)
        weatherdata += grib + wind;
    if(p->data_mask & Position::CLIMATOLOGY_WIND)
        weatherdata += climatology + wind;
    if(p->data_mask & Position::DATA_DEFICIENT_WIND)
        weatherdata += data_deficient + wind;
    if(p->data_mask & Position::GRIB_CURRENT)
        weatherdata += grib + current;
    if(p->data_mask & Position::CLIMATOLOGY_CURRENT)
        weatherdata += climatology + current;
    if(p->data_mask & Position::DATA_DEFICIENT_CURRENT)
        weatherdata += data_deficient + current;

    dlg.m_stWeatherData->SetLabel(weatherdata);
    dlg.Fit();
}

void WeatherRouting::UpdateRoutePositionDialog()
{
    /* New method to display information on the weather route
     * (like time, duration, position, wind, speed, etc.)
     * based on cursor position of the user.
     * This is complementary with the plot chart.
     */
    
    RoutePositionDialog &dlg = m_RoutePositionDialog;
    if(!dlg.IsShown())
        return;

    m_positionOnRoute = NULL;
    std::list<RouteMapOverlay*> currentroutemaps = CurrentRouteMaps();
    if(currentroutemaps.size() != 1) {
        RoutePositionDialogMessage(dlg, _("Select exactly 1 configuration"));
        return;
    }
    
    RouteMapOverlay *rmo = currentroutemaps.front();
    Position *p = rmo->GetLastCursorPosition();
    if(!p) {
        RoutePositionDialogMessage(dlg, _("Cursor outside computed route map"));
        return;
    }
    wxDateTime display_time = rmo->GetLastCursorTime();
    
    if(m_SettingsDialog.m_cbUseLocalTime->GetValue())
        display_time = display_time.FromUTC();
    
    dlg.m_stTime->SetLabel(display_time.Format(_T("%x %H:%M")));
    
    RouteMapConfiguration configuration = rmo->GetConfiguration();
    
    // CUSTOMIZATION
    // -------------------------
    // Get access to the closest point between the cursor location
    // and the weather routing computed point.
    PlotData data;
    Position *closestPosition = rmo->getClosestRoutePositionFromCursor(m_weather_routing_pi.m_cursor_lat,
                                          m_weather_routing_pi.m_cursor_lon,
                                          data);
    // Store position to display it
    m_positionOnRoute = closestPosition;

    // TRIP DURATION
    wxDateTime startTime = configuration.StartTime;
    wxDateTime cursorTime = data.time;
    
    if(m_SettingsDialog.m_cbUseLocalTime->GetValue())
    {
        startTime = startTime.FromUTC();
        cursorTime = data.time.FromUTC();
    }
    wxString duration = calculateTimeDelta(startTime, cursorTime);
    dlg.m_stDuration->SetLabel(duration);
    
    // POSITION
    wxString pos = wxString::Format(_T("%4.2f%c %4.2f%c"),
                                    fabs(data.lat), data.lat < 0 ? 'S' : 'N',
                                    fabs(data.lon), data.lon < 0 ? 'W' : 'E');
    dlg.m_stPosition->SetLabel(pos);
    
    // POLAR
    if(closestPosition->polar == -1)
        dlg.m_stPolar->SetLabel(wxEmptyString);
    else {
        wxFileName fn = configuration.boat.Polars[closestPosition->polar].FileName;
        dlg.m_stPolar->SetLabel(fn.GetFullName());
    }
    
    // SAIL CHANGES
    dlg.m_stSailChanges->SetLabel(wxString::Format(_T("%d"), closestPosition->SailChanges()));
    
    // TACKS
    dlg.m_stTacks->SetLabel(wxString::Format(_T("%d"), closestPosition->tacks));
    
    // BOAT SPEED
    dlg.m_stBoatSpeed->SetLabel(wxString::Format(_T("%.1f knts"), data.VB));
    
    // WIND SPEED
    //RouteInfo(RouteMapOverlay::COMFORT);
    dlg.m_stTWS->SetLabel(wxString::Format(_T("%.0f knts"), data.VW));
    
    // WIND: TRUE WIND ANGLE
    // For wind direction, specify if it is
    // coming from starboard or port side.
    double windDirection = heading_resolve(data.B - data.W);
    wxString windDirectionLabel;
    if (windDirection <= 0)
        windDirectionLabel = wxString::Format(_T("%.0f\u00B0 starboard"), fabs(windDirection));
    else
        windDirectionLabel = wxString::Format(_T("%.0f\u00B0 port"), fabs(windDirection));
    dlg.m_stTWA->SetLabel(windDirectionLabel);
    
    // WIND: APPARENT WIND SPEED
    float apparentWindSpeed = Polar::VelocityApparentWind(data.VB, windDirection, data.VW);
    dlg.m_stAWS->SetLabel(wxString::Format(_T("%.0f knts"), apparentWindSpeed));
    
    // WIND: APPARENT WIND SPEED
    float apparentWindDirection = Polar::DirectionApparentWind(apparentWindSpeed, data.VB,
                                                               windDirection, data.VW);
    wxString apparentWindDirectionLabel;
    if (apparentWindDirection <= 0)
        apparentWindDirectionLabel = wxString::Format(_T("%.0f\u00B0 starboard"),
                                                      fabs(apparentWindDirection));
    else
        apparentWindDirectionLabel = wxString::Format(_T("%.0f\u00B0 port"),
                                                      fabs(apparentWindDirection));
    dlg.m_stAWA->SetLabel(apparentWindDirectionLabel);
    
    // WAVES
    dlg.m_stWaves->SetLabel(wxString::Format(_T("%.0f m"), data.WVHT));
    
    // WIND GUST
    dlg.m_stWindGust->SetLabel(wxString::Format(_T("%.0f knts"), data.VW_GUST));
    
    // CLIMATOLOGY DATA
    wxString weatherdata;
    wxString grib = _("Grib") + _T(" ");
    wxString climatology = _("Climatology") + _T(" ");
    wxString data_deficient = _("Data Deficient") + _T(" ");
    wxString wind = _("Wind") + _T(" ");
    wxString current = _("Current") + _T(" ");
    
    if(closestPosition->data_mask & Position::GRIB_WIND)
        weatherdata += grib + wind;
    if(closestPosition->data_mask & Position::CLIMATOLOGY_WIND)
        weatherdata += climatology + wind;
    if(closestPosition->data_mask & Position::DATA_DEFICIENT_WIND)
        weatherdata += data_deficient + wind;
    if(closestPosition->data_mask & Position::GRIB_CURRENT)
        weatherdata += grib + current;
    if(closestPosition->data_mask & Position::CLIMATOLOGY_CURRENT)
        weatherdata += climatology + current;
    if(closestPosition->data_mask & Position::DATA_DEFICIENT_CURRENT)
        weatherdata += data_deficient + current;
    
    dlg.m_stWeatherData->SetLabel(weatherdata);
    dlg.Fit();
}

void WeatherRouting::OnNewPosition( wxCommandEvent& event )
{
    NewPositionDialog dlg(this);
    if(dlg.ShowModal() == wxID_OK) {
        double lat=0, lon=0, lat_minutes=0, lon_minutes=0;

        wxString latitude_degrees = dlg.m_tLatitudeDegrees->GetValue();
        wxString latitude_minutes = dlg.m_tLatitudeMinutes->GetValue();
        latitude_degrees.ToDouble(&lat);
        latitude_minutes.ToDouble(&lat_minutes);
        lat_minutes = fabs(lat_minutes);
        if(lat < 0)
            lat_minutes = -lat_minutes;
        lat += lat_minutes / 60;

        wxString longitude_degrees = dlg.m_tLongitudeDegrees->GetValue();
        wxString longitude_minutes = dlg.m_tLongitudeMinutes->GetValue();
        longitude_degrees.ToDouble(&lon);
        longitude_minutes.ToDouble(&lon_minutes);
        lon_minutes = fabs(lon_minutes);
        if(lon < 0)
            lon_minutes = -lon_minutes;
        lon += lon_minutes / 60;

        AddPosition(lat, lon, dlg.m_tName->GetValue());
    }
}

void WeatherRouting::OnUpdateBoat( wxCommandEvent& event )
{
    double lat = m_weather_routing_pi.m_boat_lat;
    double lon = m_weather_routing_pi.m_boat_lon;

    long index = 0;
    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++, index++)
        if((*it).Name == _("Boat")) {
            m_panel->m_lPositions->SetItem(index, POSITION_LAT, wxString::Format(_T("%.5f"), lat));
            m_panel->m_lPositions->SetItem(index, POSITION_LON, wxString::Format(_T("%.5f"), lon));

            (*it).lat = lat, (*it).lon = lon;
            UpdateConfigurations();
            return;
        }

    AddPosition(lat, lon, _("Boat"));
}

#if 0 /* wx widgets is shit, can only allow users
         to edit the first column, so this doesn't work */
void WeatherRouting::OnListLabelEdit( wxListEvent& event )
{
    long index = event.GetIndex();
    int col = event.GetColumn();
    
    long i = 0;
    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++, i++)
        if(i == index) {
            if(col == POSITION_NAME) {
                (*it).Name = event.GetText(); 
            } else {
                double value;
                event.GetText().ToDouble(&value);
                if(col == POSITION_LAT)
                    (*it).lat = value;
                else if(col == POSITION_LON)
                    (*it).lon = value;

                m_lPositions->SetItem(index, col, wxString::Format(_T("%.5f"), value));
                UpdateConfigurations();
            }
        }
}
#endif

void WeatherRouting::OnDeletePosition( wxCommandEvent& event )
{
    long index = m_panel->m_lPositions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if(index < 0)
        return;

    wxListItem item;
    item.SetId(index);
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT); // Note use of the mask, somehow it's required for this to work correctly on windows
    m_panel->m_lPositions->GetItem(item);

    long ID = m_panel->m_lPositions->GetItemData(index);
    assert(ID >=0);

    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++) {
        if((*it).ID == ID) {
            wxString name = (*it).Name;
            m_ConfigurationDialog.RemoveSource(name);
            m_ConfigurationBatchDialog.RemoveSource(name);
            RouteMap::Positions.erase(it);
            break;
        }
    }
    m_panel->m_lPositions->DeleteItem(index);

    UpdateConfigurations();
}

void WeatherRouting::OnDeleteAllPositions( wxCommandEvent& event )
{
    RouteMap::Positions.clear();
    m_ConfigurationDialog.ClearSources();
    m_ConfigurationBatchDialog.ClearSources();
    m_panel->m_lPositions->DeleteAllItems();
}

void WeatherRouting::OnPositionKeyDown( wxListEvent& event )
{
    switch(event.GetKeyCode()) {
    case WXK_DELETE:
    {
        wxCommandEvent event;
        OnDeletePosition(event);
    } break;
    default:
        event.Skip();
    }
}

void WeatherRouting::OnEditConfiguration()
{
    std::list<RouteMapOverlay *>routemapoverlays = CurrentRouteMaps(true);
    if(routemapoverlays.empty())
        return;

    m_ConfigurationDialog.Show();

#if 0
    /* if boat filename doesn't exist open boat dialog immediately */
    wxString boatfilename = m_ConfigurationDialog.m_fpBoat->GetPath();
    if(!boatfilename.empty() && !wxFileName::FileExists(boatfilename))
        m_ConfigurationDialog.EditBoat();
#endif
}

void WeatherRouting::OnGoTo( wxCommandEvent& event )
{
    std::list<RouteMapOverlay *>currentroutemaps = CurrentRouteMaps(true);
    if(currentroutemaps.empty())
        return;

    double avg_lat=0, avg_lonx=0, avg_lony=0, total = 0;
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++) {
        RouteMapConfiguration configuration = (*it)->GetConfiguration();
        if(wxIsNaN(configuration.StartLat)) continue;
        avg_lat += configuration.StartLat + configuration.EndLat;
        avg_lonx = cos(deg2rad(configuration.StartLon)) + cos(deg2rad(configuration.EndLon));
        avg_lony = sin(deg2rad(configuration.StartLon)) + sin(deg2rad(configuration.EndLon));

        total += 2;
    }

    avg_lat /= total, avg_lonx /= total, avg_lony /= total;
    double avg_lon = rad2deg(atan2(avg_lony, avg_lonx));

    double max_distance = 0;
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++) {
        RouteMapConfiguration configuration = (*it)->GetConfiguration();
        if(wxIsNaN(configuration.StartLat)) continue;
        double distance;
        DistanceBearingMercator_Plugin(avg_lat, avg_lon,
                                       configuration.StartLat, configuration.StartLon,
                                       NULL, &distance);
        max_distance = wxMax(distance, max_distance);
        DistanceBearingMercator_Plugin(avg_lat, avg_lon,
                                       configuration.EndLat, configuration.EndLon,
                                       NULL, &distance);
        max_distance = wxMax(distance, max_distance);
    }

    if(max_distance > 1e-4)
        JumpToPosition(avg_lat, avg_lon, .125/max_distance);
    else {
        wxMessageDialog mdlg(this, _("Cannot goto invalid route(s)."),
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void WeatherRouting::OnDelete( wxCommandEvent& event )
{
    long index = m_panel->m_lWeatherRoutes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index < 0) return;

    DeleteRouteMaps(CurrentRouteMaps());

    /* select map just after the first one selected */
    int cnt = m_panel->m_lWeatherRoutes->GetItemCount();
    m_panel->m_lWeatherRoutes->SetItemState(index == cnt ? index - 1 : index,
                                   wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    GetParent()->Refresh();
}

void WeatherRouting::OnDeleteAll( wxCommandEvent& event )
{
    std::list<RouteMapOverlay *>allroutemapoverlays;
    for(int i=0; i< m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
        allroutemapoverlays.push_back(weatherroute->routemapoverlay);
    }

    DeleteRouteMaps(allroutemapoverlays);

    GetParent()->Refresh();
}

void WeatherRouting::OnWeatherRouteSort( wxListEvent& event )
{
    sortcol = event.GetColumn();
    sortorder = -sortorder;

    if(sortcol == 0) {
        for(int index=0; index<m_panel->m_lWeatherRoutes->GetItemCount(); index++) {
            WeatherRoute *weatherroute =
                reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)));
            weatherroute->routemapoverlay->m_bEndRouteVisible = sortorder == 1;
            UpdateItem(index);
        }
        RequestRefresh( GetParent() );
    } else {
#if wxCHECK_VERSION(2, 9, 0)
        m_panel->m_lWeatherRoutes->SortItems(SortWeatherRoutes, (wxIntPtr)m_panel->m_lWeatherRoutes);
#else
        m_panel->m_lWeatherRoutes->SortItems(SortWeatherRoutes, (long)m_panel->m_lWeatherRoutes);
#endif
    }
}

void WeatherRouting::OnWeatherRouteSelected( )
{
    GetParent()->Refresh();

    std::list<RouteMapOverlay*>currentroutemaps = CurrentRouteMaps();
    std::list<RouteMapConfiguration> currentconfigurations;
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++) {
        (*it)->SetCursorLatLon(m_weather_routing_pi.m_cursor_lat,
                               m_weather_routing_pi.m_cursor_lon);
        currentconfigurations.push_back((*it)->GetConfiguration());
    }

    if(currentroutemaps.empty())
        m_tHideConfiguration.Start(25, true);
    else {
        m_tHideConfiguration.Stop();
        m_bSkipUpdateCurrentItems = true;
        m_ConfigurationDialog.SetConfigurations(currentconfigurations);
        m_bSkipUpdateCurrentItems = false;
    }

    UpdateDialogs();

    SetEnableConfigurationMenu();
}

void WeatherRouting::OnWeatherRouteKeyDown( wxListEvent& event )
{
    switch(event.GetKeyCode()) {
    case WXK_DELETE:
    {
        wxCommandEvent event;
        OnDelete(event);
    } break;
    default:
        event.Skip();
    }
}

void WeatherRouting::OnWeatherRoutesListLeftDown(wxMouseEvent &event)
{
    OnLeftDown(event);
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long index = m_panel->m_lWeatherRoutes->HitTest(pos, flags);
    
    // Do we have the Visibility column?
    if(columns[VISIBLE] >= 0) {
        int minx = 0, maxx = m_panel->m_lWeatherRoutes->GetColumnWidth(columns[VISIBLE]);

        //    Clicking Visibility column?
        if (index >= 0 && event.GetX() >= minx && event.GetX() < maxx)
        {
            // Process the clicked item
            WeatherRoute *weatherroute =
                reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)));
            weatherroute->routemapoverlay->m_bEndRouteVisible =
                !weatherroute->routemapoverlay->m_bEndRouteVisible;
            UpdateItem(index);
            RequestRefresh( GetParent() );
        }
    }

    // Allow wx to process...
    event.Skip();
}

void WeatherRouting::UpdateComputeState()
{
    m_panel->m_gProgress->SetRange(m_RoutesToRun);

    if(m_bRunning)
        return;

    m_bRunning = true;
    m_panel->m_gProgress->SetValue(0);
    
    m_mCompute->Enable();
    m_panel->m_bCompute->Enable();
    m_StartTime = wxDateTime::Now();
    m_tCompute.Start(1, true);
}

void WeatherRouting::OnCompute( wxCommandEvent& event )
{
    std::list<RouteMapOverlay*> currentroutemaps = CurrentRouteMaps();
    for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
        it != currentroutemaps.end(); it++)
        Start(*it);
    UpdateComputeState();
}

void WeatherRouting::OnComputeAll ( wxCommandEvent& event )
{
    StartAll();
    UpdateComputeState();
}

void WeatherRouting::OnStop( wxCommandEvent& event )
{
    Stop();
}

#define FAIL(X) do { error = X; goto failed; } while(0)
void WeatherRouting::OnOpen( wxCommandEvent& event )
{
    wxString error;
    wxFileDialog openDialog
        ( this, _( "Select Configuration" ), m_FileName.GetPath(), m_FileName.GetName(),
          wxT ( "XML files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_OPEN  );

    if( openDialog.ShowModal() == wxID_OK ) {
        wxCommandEvent event;
        OnDeleteAllPositions( event );
        OnDeleteAll( event );
        OpenXML(openDialog.GetPath());
    }
}

void WeatherRouting::OnSave( wxCommandEvent& event )
{
    wxString error;
    wxFileDialog saveDialog
        ( this, _( "Select Configuration" ), m_FileName.GetPath(), m_FileName.GetName(),
          wxT ( "XML files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_SAVE  );

    if( saveDialog.ShowModal() == wxID_OK )
        SaveXML(saveDialog.GetPath());
}

void WeatherRouting::OnClose( wxCommandEvent& event )
{
    Hide();
}

void WeatherRouting::OnCollPaneChanged( wxCollapsiblePaneEvent& event )
{
    if(m_colpane && m_colpane->IsExpanded())
        SetSize(m_size);
    else
        if(m_colpane)
            Fit();
    Update();
    Layout();
}


void WeatherRouting::OnSize( wxSizeEvent& event )
{
    if(m_colpane && m_colpane->IsExpanded ()) {
        Update();
        Layout();
        m_size = GetSize();
    } else {
        if(m_colpane)
            Fit();
    }
    event.Skip();
}

void WeatherRouting::OnNew( wxCommandEvent& event )
{
    RouteMapConfiguration configuration;
    if(FirstCurrentRouteMap())
        configuration = FirstCurrentRouteMap()->GetConfiguration();
    else
        configuration = DefaultConfiguration();

    AddConfiguration(configuration);

    // deselect all
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++)
        m_panel->m_lWeatherRoutes->SetItemState(i, 0, wxLIST_STATE_SELECTED);

    m_panel->m_lWeatherRoutes->SetItemState(m_panel->m_lWeatherRoutes->GetItemCount() - 1,
                                   wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    OnEditConfiguration();
}

void WeatherRouting::OnBatch( wxCommandEvent& event )
{
    if(m_ConfigurationBatchDialog.IsShown())
        return;

    m_ConfigurationBatchDialog.Reset();
    m_ConfigurationBatchDialog.Show();
}

void WeatherRouting::GenerateBatch()
{
    std::list<RouteMapOverlay *>routemapoverlays = CurrentRouteMaps(true);

    wxProgressDialog *progressdialog = NULL;
    int count = routemapoverlays.size(), c = 0;
    int times = 0;

    wxTimeSpan StartSpan, StartSpacingSpan;
    double days, hours;

    ConfigurationBatchDialog &dlg = m_ConfigurationBatchDialog;        
    dlg.m_tStartDays->GetValue().ToDouble(&days);
    StartSpan = wxTimeSpan::Days(days);

    dlg.m_tStartHours->GetValue().ToDouble(&hours);
    StartSpan += wxTimeSpan::Seconds(3600*hours);

    dlg.m_tStartSpacingDays->GetValue().ToDouble(&days);
    StartSpacingSpan = wxTimeSpan::Days(days);
        
    dlg.m_tStartSpacingHours->GetValue().ToDouble(&hours);
    StartSpacingSpan += wxTimeSpan::Seconds(3600*hours);

    if(!StartSpacingSpan.GetSeconds().ToLong()) {
        wxMessageDialog mdlg(this, _("Zero time span forbidden, aborting."),
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
        return;
    }

    wxDateTime StartTime = wxDateTime::Now(), EndTime = StartTime+StartSpan;

    for(wxDateTime start = StartTime; start <= EndTime; start += StartSpacingSpan)
        times++;
    
    int sources = 0;
    for(std::vector<BatchSource*>::iterator it = dlg.sources.begin();
        it != dlg.sources.end(); it++)
        for(std::list<BatchDestination*>::iterator it2 = (*it)->destinations.begin();
            it2 != (*it)->destinations.end(); it2++)
            sources++;
    
    count *= sources;
    count *= dlg.m_lBoats->GetCount();
    
    if(count > 10) {
        progressdialog = new wxProgressDialog(
            _("Batch configuration"), _("Weather Routing"), count, this,
            wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
    }

    for(std::list<RouteMapOverlay *>::iterator it = routemapoverlays.begin();
        it != routemapoverlays.end(); it++) {
        RouteMapConfiguration configuration = (*it)->GetConfiguration();

        EndTime = configuration.StartTime+StartSpan;

        for(; configuration.StartTime <= EndTime; configuration.StartTime += StartSpacingSpan) {
            for(std::vector<BatchSource*>::iterator it = dlg.sources.begin();
                it != dlg.sources.end(); it++) {
                configuration.Start = (*it)->Name;

                for(std::list<BatchDestination*>::iterator it2 = (*it)->destinations.begin();
                    it2 != (*it)->destinations.end(); it2++) {
                    configuration.End = (*it2)->Name;

                    for(unsigned int boatindex = 0; boatindex < dlg.m_lBoats->GetCount(); boatindex++) {
                        configuration.boatFileName = dlg.m_lBoats->GetString(boatindex);

                        for(int windstrength = dlg.m_sWindStrengthMin->GetValue();
                            windstrength <= dlg.m_sWindStrengthMax->GetValue();
                            windstrength += dlg.m_sWindStrengthStep->GetValue()) {

                            configuration.WindStrength = windstrength / 100.0;

                            AddConfiguration(configuration);
                            m_WeatherRoutes.back()->routemapoverlay->LoadBoat();
                            configuration = m_WeatherRoutes.back()->routemapoverlay->GetConfiguration();

                            if(progressdialog && !progressdialog->Update(c++))
                                goto abort;
                        }
                    }
                }
            }
        }
    }
abort:
    DeleteRouteMaps(routemapoverlays);

    delete progressdialog;
}

bool WeatherRouting::Show(bool show)
{
    m_weather_routing_pi.ShowMenuItems(show);

    if(show) {
        m_ConfigurationDialog.Show(m_bShowConfiguration);
        m_ConfigurationBatchDialog.Show(m_bShowConfigurationBatch);
        m_SettingsDialog.Show(m_bShowSettings);
        m_StatisticsDialog.Show(m_bShowStatistics);
        m_ReportDialog.Show(m_bShowReport);
        m_PlotDialog.Show(m_bShowPlot);
        m_FilterRoutesDialog.Show(m_bShowFilter);
        m_RoutePositionDialog.Show(m_bShowRoutePosition);
    } else {
        m_bShowConfiguration = m_ConfigurationDialog.IsShown();
        m_ConfigurationDialog.Hide();

        m_bShowConfigurationBatch = m_ConfigurationBatchDialog.IsShown();
        m_ConfigurationBatchDialog.Hide();

        m_bShowSettings = m_SettingsDialog.IsShown();
        m_SettingsDialog.Hide();

        m_bShowStatistics = m_StatisticsDialog.IsShown();
        m_StatisticsDialog.Hide();

        m_bShowReport = m_ReportDialog.IsShown();
        m_ReportDialog.Hide();

        m_bShowPlot = m_PlotDialog.IsShown();
        m_PlotDialog.Hide();

        m_bShowFilter = m_FilterRoutesDialog.IsShown();
        m_FilterRoutesDialog.Hide();

        m_bShowRoutePosition = m_RoutePositionDialog.IsShown();
        m_RoutePositionDialog.Hide();
    }

    return WeatherRoutingBase::Show(show);
}

void WeatherRouting::OnFilter( wxCommandEvent& event )
{
    m_FilterRoutesDialog.Show();
}

void WeatherRouting::OnResetAll( wxCommandEvent& event )
{
    m_StatisticsDialog.SetRunTime(m_RunTime = wxTimeSpan(0));
    Reset();
    UpdateStates();
}

void WeatherRouting::OnExport ( wxCommandEvent& event )
{
    std::list<RouteMapOverlay *>routemapoverlays = CurrentRouteMaps(true);
    for(std::list<RouteMapOverlay *>::iterator it = routemapoverlays.begin();
        it != routemapoverlays.end(); it++)
        Export(**it);
}

void WeatherRouting::OnExportAll( wxCommandEvent& event )
{
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++)
        Export(*reinterpret_cast<WeatherRoute*>
               (wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)))->routemapoverlay);
}

void WeatherRouting::OnSettings( wxCommandEvent& event )
{
    m_SettingsDialog.Show();
}

void WeatherRouting::OnStatistics( wxCommandEvent& event )
{
    m_StatisticsDialog.SetRouteMapOverlays(CurrentRouteMaps());
    m_StatisticsDialog.Show();
}

void WeatherRouting::OnReport( wxCommandEvent& event )
{
    m_ReportDialog.SetRouteMapOverlays(CurrentRouteMaps());
    m_ReportDialog.Show();
}

void WeatherRouting::OnPlot ( wxCommandEvent& event )
{
    m_PlotDialog.SetRouteMapOverlay(FirstCurrentRouteMap());
    m_PlotDialog.Show();
}

void WeatherRouting::OnCursorPosition( wxCommandEvent& event )
{
    m_CursorPositionDialog.Show(!m_CursorPositionDialog.IsShown());
    UpdateCursorPositionDialog();
}

// CUSTOMIZATION
void WeatherRouting::OnRoutePosition( wxCommandEvent& event )
{
    m_RoutePositionDialog.Show(!m_RoutePositionDialog.IsShown());
    UpdateRoutePositionDialog();
}

void WeatherRouting::OnManual ( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser("https://opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:opencpn_user_manual:plugins:weather:weather_routing");    
}

void WeatherRouting::OnInformation ( wxCommandEvent& event )
{
    wxString infolocation = *GetpSharedDataLocation()
        + _T("plugins/weather_routing_pi/data/") + _("WeatherRoutingInformation.html");
    wxLaunchDefaultBrowser(_T("file://") + infolocation);
}

void WeatherRouting::OnAbout ( wxCommandEvent& event )
{
    AboutDialog dlg(GetParent());
    dlg.ShowModal();
}

void WeatherRouting::OnComputationTimer( wxTimerEvent & )
{
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); ) {
        RouteMapOverlay *routemapoverlay = *it;
        if(!routemapoverlay->Running()) {
            routemapoverlay->DeleteThread();

            it = m_RunningRouteMaps.erase(it);

            m_panel->m_gProgress->SetValue(m_RoutesToRun - m_WaitingRouteMaps.size() - m_RunningRouteMaps.size());
            UpdateRouteMap(routemapoverlay);

            /* update report if needed */
            m_ReportDialog.m_bReportStale = true;
            if(m_ReportDialog.IsShown()) {
                std::list<RouteMapOverlay *>routemapoverlays = CurrentRouteMaps();
                for(std::list<RouteMapOverlay *>::iterator it = routemapoverlays.begin();
                    it != routemapoverlays.end(); it++)
                    if(routemapoverlay == *it) {
                        m_ReportDialog.SetRouteMapOverlays(routemapoverlays);
                        break;
                    }
            }

            continue;
        } else
            it++;

        /* get a new grib for the route map if needed */
        if(routemapoverlay->NeedsGrib() && !routemapoverlay->Finished()) {
            m_RouteMapOverlayNeedingGrib = routemapoverlay;
            routemapoverlay->RequestGrib(routemapoverlay->NewTime());
            m_RouteMapOverlayNeedingGrib = NULL;
        }
    }

    if((int)m_RunningRouteMaps.size() < m_SettingsDialog.m_sConcurrentThreads->GetValue()
       && m_WaitingRouteMaps.size()) {
        RouteMapOverlay *routemapoverlay = m_WaitingRouteMaps.front();
        m_WaitingRouteMaps.pop_front();
        wxString error;
        if(routemapoverlay->Start(error))
            m_RunningRouteMaps.push_back(routemapoverlay);
        else {
            wxMessageDialog mdlg(this, _("Failed to start configuration: ") + error,
                                 _("Weather Routing"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
        }

        UpdateRouteMap(routemapoverlay);
    }
        
    static int cycles; /* don't refresh all the time */
    if(++cycles > 50 || !m_RunningRouteMaps.size()) {
        cycles = 0;

        std::list<RouteMapOverlay*> currentroutemaps = CurrentRouteMaps();
        for(std::list<RouteMapOverlay*>::iterator it = currentroutemaps.begin();
            it != currentroutemaps.end(); it++)
            if((*it)->Updated()) {

                m_StatisticsDialog.SetRunTime(m_RunTime += wxDateTime::Now() - m_StartTime);
                if(m_StatisticsDialog.IsShown())
                    m_StatisticsDialog.SetRouteMapOverlays(CurrentRouteMaps());
                if(m_PlotDialog.IsShown())
                    m_PlotDialog.SetRouteMapOverlay(FirstCurrentRouteMap());

                m_StartTime = wxDateTime::Now();
                GetParent()->Refresh();
                break;
            }
    }

    if(m_RunningRouteMaps.size()) {
        /* todo, instead of respawning the funky timer here,
           maybe we can do it from the thread instead to eliminate the delay */
        m_tCompute.Start(25, true);
        return;
    }

    Stop();
}

void WeatherRouting::OnHideConfigurationTimer( wxTimerEvent & )
{
    m_ConfigurationDialog.Hide();
}

void WeatherRouting::OnRenderedTimer ( wxTimerEvent & )
{
    // don't do it until the window system is up and running
    if ( GetClientSize().GetWidth() > 20 ) {
        if (!sashpos)
	    sashpos = GetClientSize().GetWidth() / 5;
        m_panel->m_splitter1->SetSashPosition(sashpos, true);
        Disconnect(wxEVT_IDLE, wxTimerEventHandler(WeatherRouting::OnRenderedTimer ), NULL, this );
    }
}

bool WeatherRouting::OpenXML(wxString filename, bool reportfailure)
{
    TiXmlDocument doc;
    wxString error;

    wxFileName fn(filename);
    SetTitle(_("Weather Routing") + wxString(_T(" - ")) + fn.GetFullName());
    m_FileName = fn;

    wxProgressDialog *progressdialog = NULL;
    wxDateTime start = wxDateTime::UNow();

    wxString lastboatFileName;
    Boat lastboat;

    if(!doc.LoadFile(filename.mb_str()))
        FAIL(_("Failed to load file."));
    else {
        TiXmlHandle root(doc.RootElement());

        if(strcmp(root.Element()->Value(), "OpenCPNWeatherRoutingConfiguration"))
            FAIL(_("Invalid xml file"));

        RouteMap::Positions.clear();

        int count = 0;
        for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement())
            count++;
    
        int i=0;
        for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement(), i++) {
            if(progressdialog) {
                if(!progressdialog->Update(i)) {
                    delete progressdialog;
                    return true;
                }
            } else {
                wxDateTime now = wxDateTime::UNow();
                /* if it's going to take more than a half second, show a progress dialog */
                if((now-start).GetMilliseconds() > 250 && i < count/2) {
                    progressdialog = new wxProgressDialog(
                        _("Load"), _("Weather Routing"), count, this,
                        wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
                }
            }
        
            if(!strcmp(e->Value(), "Position")) {
                wxString name = wxString::FromUTF8(e->Attribute("Name"));
                wxString GUID = wxString::FromUTF8(e->Attribute("GUID"));
                double lat = AttributeDouble(e, "Latitude", NAN);
                double lon = AttributeDouble(e, "Longitude", NAN);
            
                if (GUID.IsEmpty()) for(auto it : RouteMap::Positions) {
                    if (it.Name == name) {
                        static bool warnonce = true;
                        if(warnonce) {
                            warnonce = false;
                            wxMessageDialog mdlg(this, _("File contains duplicate position name, discaring\n"),
                                                 _("Weather Routing"), wxOK | wxICON_WARNING);
                            mdlg.ShowModal();
                        }
                    
                        goto skipadd;
                    }
                }
                AddPosition(lat, lon, name, GUID);
            
            skipadd:;
            } 
            else if(!strcmp(e->Value(), "Configuration")) {
                RouteMapConfiguration configuration;
                configuration.RouteGUID = wxString::FromUTF8(e->Attribute("GUID"));
                configuration.Start = wxString::FromUTF8(e->Attribute("Start"));
                wxDateTime date;
                date.ParseISODate(wxString::FromUTF8(e->Attribute("StartDate")));
                wxDateTime time;
                time.ParseISOTime(wxString::FromUTF8(e->Attribute("StartTime")));
                if(date.IsValid()) {
                    if(time.IsValid()) {
                        date.SetHour(time.GetHour());
                        date.SetMinute(time.GetMinute());
                        date.SetSecond(time.GetSecond());
                    }
                    configuration.StartTime = date;
                } else
                    configuration.StartTime = wxDateTime::Now();
            
                configuration.End = wxString::FromUTF8(e->Attribute("End"));
                configuration.DeltaTime = AttributeDouble(e, "dt", 0);
            
                configuration.boatFileName = wxString::FromUTF8(e->Attribute("Boat"));
                if(!wxFileName::FileExists(configuration.boatFileName)) {
                    configuration.boatFileName = weather_routing_pi::StandardPath() +
			                             _T("boats") +
						     wxFileName::GetPathSeparator() +
						     configuration.boatFileName;
                    if(!wxFileName::FileExists(configuration.boatFileName)) {

                        configuration.boatFileName = _T("");
			}
		}
            
                configuration.Integrator = (RouteMapConfiguration::IntegratorType)
                    AttributeInt(e, "Integrator", 0);

                configuration.MaxDivertedCourse = AttributeDouble(e, "MaxDivertedCourse", 90);
                configuration.MaxCourseAngle = AttributeDouble(e, "MaxCourseAngle", 180);
                configuration.MaxSearchAngle = AttributeDouble(e, "MaxSearchAngle", 120);
                configuration.MaxTrueWindKnots = AttributeDouble(e, "MaxTrueWindKnots", 100);
                configuration.MaxApparentWindKnots = AttributeDouble(e, "MaxApparentWindKnots", 100);

                configuration.MaxSwellMeters = AttributeDouble(e, "MaxSwellMeters", 20);
                configuration.MaxLatitude = AttributeDouble(e, "MaxLatitude", 90);
                configuration.TackingTime = AttributeDouble(e, "TackingTime", 0);
                configuration.WindVSCurrent = AttributeDouble(e, "WindVSCurrent", 0);

                configuration.AvoidCycloneTracks = AttributeBool(e, "AvoidCycloneTracks", false);
                configuration.CycloneMonths = AttributeInt(e, "CycloneMonths", 2);
                configuration.CycloneDays = AttributeInt(e, "CycloneDays", 0);

                configuration.UseGrib = AttributeBool(e, "UseGrib", true);
                configuration.ClimatologyType = (RouteMapConfiguration::ClimatologyDataType)
                    AttributeInt(e, "ClimatologyType", RouteMapConfiguration::CUMULATIVE_MAP);
                configuration.AllowDataDeficient = AttributeBool(e, "AllowDataDeficient", false);
                configuration.WindStrength = AttributeDouble(e, "WindStrength", 1);

                configuration.DetectLand = AttributeBool(e, "DetectLand", true);
                configuration.SafetyMarginLand = AttributeDouble(e, "SafetyMarginLand", 2.);
                configuration.DetectBoundary = AttributeBool(e, "DetectBoundary", false);
                configuration.Currents = AttributeBool(e, "Currents", true);
                configuration.OptimizeTacking = AttributeBool(e, "OptimizeTacking", false);
                
                configuration.InvertedRegions = AttributeBool(e, "InvertedRegions", false);
                configuration.Anchoring = AttributeBool(e, "Anchoring", false);

                configuration.FromDegree = AttributeDouble(e, "FromDegree", 0);
                configuration.ToDegree = AttributeDouble(e, "ToDegree", 180);
                configuration.ByDegrees = AttributeDouble(e, "ByDegrees", 5);

                if(configuration.boatFileName == lastboatFileName)
                    configuration.boat = lastboat;
            
                AddConfiguration(configuration);

                lastboatFileName = configuration.boatFileName;
                m_WeatherRoutes.back()->routemapoverlay->LoadBoat();
                lastboat = m_WeatherRoutes.back()->routemapoverlay->GetConfiguration().boat;
            } else
                FAIL(_("Unrecognized xml node"));
        }
    }

    delete progressdialog;
    return true;
failed:

    delete progressdialog;
    if(reportfailure) {
        wxMessageDialog mdlg(this, error, _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
    return false;
}

void WeatherRouting::SaveXML(wxString filename)
{
    wxFileName fn(filename);
    SetTitle(_("Weather Routing") + wxString(_T(" - ")) + fn.GetFullName());
    m_FileName = fn;

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * root = new TiXmlElement( "OpenCPNWeatherRoutingConfiguration" );
    doc.LinkEndChild( root );

    char version[24];
    sprintf(version, "%d.%d", PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR);
    root->SetAttribute("version", version);
    root->SetAttribute("creator", "Opencpn Weather Routing plugin");

    for(std::list<RouteMapPosition>::iterator it = RouteMap::Positions.begin();
        it != RouteMap::Positions.end(); it++) {
        TiXmlElement *c = new TiXmlElement( "Position" );

        c->SetAttribute("Name", (*it).Name.mb_str());
        c->SetAttribute("Latitude", wxString::Format(_T("%.5f"), (*it).lat).mb_str());
        c->SetAttribute("Longitude", wxString::Format(_T("%.5f"), (*it).lon).mb_str());
        if (!(*it).GUID.IsEmpty())
            c->SetAttribute("GUID", (*it).GUID.mb_str());

        root->LinkEndChild(c);
    }

    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++) {
        TiXmlElement *c = new TiXmlElement( "Configuration" );

        RouteMapConfiguration configuration =
            (*it)->routemapoverlay->GetConfiguration();

        if (!configuration.RouteGUID.IsEmpty())
            c->SetAttribute("GUID", configuration.RouteGUID.mb_str());

        c->SetAttribute("Start", configuration.Start.mb_str());
        c->SetAttribute("StartDate", configuration.StartTime.FormatISODate().mb_str());
        c->SetAttribute("StartTime", configuration.StartTime.FormatISOTime().mb_str());
        c->SetAttribute("End", configuration.End.mb_str());
        c->SetAttribute("dt", configuration.DeltaTime);

        c->SetAttribute("Boat", configuration.boatFileName.ToUTF8());

        c->SetAttribute("Integrator", configuration.Integrator);

        c->SetAttribute("MaxDivertedCourse", configuration.MaxDivertedCourse);
        c->SetAttribute("MaxCourseAngle", configuration.MaxCourseAngle);
        c->SetAttribute("MaxSearchAngle", configuration.MaxSearchAngle);
        c->SetAttribute("MaxTrueWindKnots", configuration.MaxTrueWindKnots);
        c->SetAttribute("MaxApparentWindKnots", configuration.MaxApparentWindKnots);

        c->SetAttribute("MaxSwellMeters", configuration.MaxSwellMeters);
        c->SetAttribute("MaxLatitude", configuration.MaxLatitude);
        c->SetAttribute("TackingTime", configuration.TackingTime);
        c->SetAttribute("WindVSCurrent", configuration.WindVSCurrent);

        c->SetAttribute("AvoidCycloneTracks", configuration.AvoidCycloneTracks);
        c->SetAttribute("CycloneMonths", configuration.CycloneMonths);
        c->SetAttribute("CycloneDays", configuration.CycloneDays);

        c->SetAttribute("UseGrib", configuration.UseGrib);
        c->SetAttribute("ClimatologyType", configuration.ClimatologyType);
        c->SetAttribute("AllowDataDeficient", configuration.AllowDataDeficient);
        c->SetDoubleAttribute("WindStrength", configuration.WindStrength);

        c->SetAttribute("DetectLand", configuration.DetectLand);
        c->SetAttribute("SafetyMarginLand", configuration.SafetyMarginLand);
        c->SetAttribute("DetectBoundary", configuration.DetectBoundary);
        c->SetAttribute("Currents", configuration.Currents);
        c->SetAttribute("OptimizeTacking", configuration.OptimizeTacking);

        c->SetAttribute("InvertedRegions", configuration.InvertedRegions);
        c->SetAttribute("Anchoring", configuration.Anchoring);

        c->SetDoubleAttribute("FromDegree", configuration.FromDegree);
        c->SetDoubleAttribute("ToDegree", configuration.ToDegree);
        c->SetDoubleAttribute("ByDegrees", configuration.ByDegrees);

        root->LinkEndChild(c);
    }

    if(!doc.SaveFile(filename.mb_str())) {
        wxMessageDialog mdlg(this, _("Failed to save xml file: ") + filename,
                             _("Weather Routing"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void WeatherRouting::SetEnableConfigurationMenu()
{
    bool current = FirstCurrentRouteMap() != NULL;
    m_mBatch->Enable(current);
    m_mEdit->Enable(current);
    m_mGoTo->Enable(current);
    m_mDelete->Enable(current);
    m_mCompute->Enable(current);
    m_panel->m_bCompute->Enable(current);
    m_mExport->Enable(current);
    m_panel->m_bExport->Enable(current);

    m_mStop->Enable(m_WaitingRouteMaps.size() + m_RunningRouteMaps.size() > 0);

    bool cnt = m_panel->m_lWeatherRoutes->GetItemCount() > 0;
    m_mDeleteAll->Enable(cnt);
    m_mComputeAll->Enable(cnt);
    m_mExportAll->Enable(cnt);
}

void WeatherRouting::UpdateConfigurations()
{
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));

        /* get and set configuration to update start/end positions */
        RouteMapConfiguration c = weatherroute->routemapoverlay->GetConfiguration();
        weatherroute->routemapoverlay->SetConfiguration(c);

        weatherroute->Update(this, true);
        UpdateItem(i);
    }
}

void WeatherRouting::UpdateDialogs()
{
    std::list<RouteMapOverlay *>routemapoverlays = CurrentRouteMaps();
    if(m_StatisticsDialog.IsShown())
        m_StatisticsDialog.SetRouteMapOverlays(routemapoverlays);

    if(m_ReportDialog.IsShown())
        m_ReportDialog.SetRouteMapOverlays(routemapoverlays);

    if(m_PlotDialog.IsShown())
        m_PlotDialog.SetRouteMapOverlay(FirstCurrentRouteMap());
}

void WeatherRouting::AddConfiguration(const RouteMapConfiguration &configuration)
{
    WeatherRoute *weatherroute = new WeatherRoute;
    RouteMapOverlay *routemapoverlay = weatherroute->routemapoverlay;
    routemapoverlay->SetConfiguration(configuration);
    routemapoverlay->Reset();
    weatherroute->Update(this);

    m_WeatherRoutes.push_back(weatherroute);

    wxListItem item;
    item.SetId(m_panel->m_lWeatherRoutes->GetItemCount());
    item.SetData(weatherroute);
    if(m_panel->m_lWeatherRoutes->GetColumnCount())
        UpdateItem(m_panel->m_lWeatherRoutes->InsertItem(item));

    m_mDeleteAll->Enable();
    m_mComputeAll->Enable();
    m_mExportAll->Enable();
}

void WeatherRouting::UpdateRouteMap(RouteMapOverlay *routemapoverlay)
{
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
        if(weatherroute->routemapoverlay == routemapoverlay) {
            weatherroute->Update(this);
            UpdateItem(i);
            return;
        }
    }
}

/* we could speed this up more with another flag for when we need to update
   parameters but not computed route information */
void WeatherRoute::Update(WeatherRouting *wr, bool stateonly)
{
    if(!stateonly) {
        RouteMapConfiguration configuration = routemapoverlay->GetConfiguration();

        BoatFilename = configuration.boatFileName;
        Start = configuration.Start;

        wxDateTime starttime = configuration.StartTime;

        if(wr->m_SettingsDialog.m_cbUseLocalTime->GetValue())
            starttime = starttime.FromUTC();
        StartTime = starttime.Format(_T("%x %H:%M"));

        End = configuration.End;

        wxDateTime endtime = routemapoverlay->EndTime();
        if(endtime.IsValid()) {
            if(wr->m_SettingsDialog.m_cbUseLocalTime->GetValue())
                endtime = endtime.FromUTC();
            EndTime = endtime.Format(_T("%x %H:%M"));
        } else
            EndTime = _T("N/A");
        
        // REFACTORING
        // I decided to dedicate a function for displaying the difference
        // between two TimeDate as it is usefull in some other part of the code.
        Time = calculateTimeDelta(starttime, endtime);

        Distance =  wxString::Format
            (_T("%.0f/%.0f"), routemapoverlay->RouteInfo(RouteMapOverlay::DISTANCE),
             DistGreatCircle_Plugin(configuration.StartLat, configuration.StartLon,
                                    configuration.EndLat, configuration.EndLon));
    
        AvgSpeed = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::AVGSPEED));

        MaxSpeed = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::MAXSPEED));
    
        AvgSpeedGround = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::AVGSPEEDGROUND));

        MaxSpeedGround = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::MAXSPEEDGROUND));

        AvgWind = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::AVGWIND));

        MaxWind = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::MAXWIND));

        MaxWindGust = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::MAXWINDGUST));
        
        AvgCurrent = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::AVGCURRENT));

        MaxCurrent = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::MAXCURRENT));

        AvgSwell = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::AVGSWELL));

        MaxSwell = wxString::Format
            (_T("%.2f"), routemapoverlay->RouteInfo(RouteMapOverlay::MAXSWELL));

        UpwindPercentage = wxString::Format(_T("%.2f%%"), routemapoverlay->RouteInfo
                                            (RouteMapOverlay::PERCENTAGE_UPWIND));

        double ps = routemapoverlay->RouteInfo(RouteMapOverlay::PORT_STARBOARD);
        PortStarboard = wxString::Format(_T("%.0f/%.0f"), ps, 100-ps);

        Tacks = wxString::Format(_T("%.0f"), routemapoverlay->RouteInfo(RouteMapOverlay::TACKS));
        
        // CUSTOMIZATION
        // Display sailing comfort
        int comfort_level = routemapoverlay->RouteInfo(RouteMapOverlay::COMFORT);
        Comfort = RouteMapOverlay::sailingConditionText(comfort_level);
    }

    if(!routemapoverlay->Valid())
        State = _("Invalid Start/End ") + routemapoverlay->GetError();
    else
    if(routemapoverlay->Running())
        State = _("Computing...");
    else {
        if(routemapoverlay->Finished()) {
            if(routemapoverlay->ReachedDestination())
                State = _("Complete");
            else {
                State = _T("");
                if(routemapoverlay->GribFailed()) {
                    State += _("Grib");
                    State += _T(": ");
                }
                if(routemapoverlay->PolarFailed()) {
                    State += _("Polar");
                    State += _T(": ");
                }
                if(routemapoverlay->NoData()) {
                    State += _("No Data");
                    State += _T(": ");
                }
                if(routemapoverlay->LandCrossing()) {
                    State += _("Land");
                    State += _T(": ");
                }
                if(routemapoverlay->BoundaryCrossing()) {
                    State += _("Boundary");
                    State += _T(": ");
                }

                State += _("Failed");
            }
        } else {
            for(std::list<RouteMapOverlay*>::iterator it = wr->m_WaitingRouteMaps.begin();
                it != wr->m_WaitingRouteMaps.end(); it++)
                if(*it == routemapoverlay) {
                    State = _("Waiting...");
                    return;
                }
            State = _("Not Computed");
        }
    }
}

void WeatherRouting::UpdateItem(long index, bool stateonly)
{
    WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
        (wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)));
    if( !weatherroute )
        return;

    if(!stateonly) {
        if(columns[VISIBLE] >= 0) {
            m_panel->m_lWeatherRoutes->SetItemImage(index, weatherroute->routemapoverlay->m_bEndRouteVisible ? 0 : -1);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[VISIBLE], 28);
        }

        if(columns[BOAT] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[BOAT], wxFileName(weatherroute->BoatFilename).GetName());
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[BOAT], wxLIST_AUTOSIZE);
        }

        if(columns[START] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[START], weatherroute->Start);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[START], wxLIST_AUTOSIZE);
        }

        if(columns[STARTTIME] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[STARTTIME], weatherroute->StartTime);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[STARTTIME], wxLIST_AUTOSIZE);
        }

        if(columns[END] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[END], weatherroute->End);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[END], wxLIST_AUTOSIZE);
        }

        if(columns[ENDTIME] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[ENDTIME], weatherroute->EndTime);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[ENDTIME], wxLIST_AUTOSIZE);
        }

        if(columns[TIME] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[TIME], weatherroute->Time);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[TIME], wxLIST_AUTOSIZE);
        }

        if(columns[DISTANCE] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[DISTANCE], weatherroute->Distance);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[DISTANCE], wxLIST_AUTOSIZE);
        }

        if(columns[AVGSPEED] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[AVGSPEED], weatherroute->AvgSpeed);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[AVGSPEED], wxLIST_AUTOSIZE);
        }

        if(columns[MAXSPEED] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[MAXSPEED], weatherroute->MaxSpeed);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[MAXSPEED], wxLIST_AUTOSIZE);
        }

        if(columns[AVGSPEEDGROUND] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[AVGSPEEDGROUND], weatherroute->AvgSpeedGround);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[AVGSPEEDGROUND], wxLIST_AUTOSIZE);
        }

        if(columns[MAXSPEEDGROUND] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[MAXSPEEDGROUND], weatherroute->MaxSpeedGround);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[MAXSPEEDGROUND], wxLIST_AUTOSIZE);
        }

        if(columns[AVGWIND] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[AVGWIND], weatherroute->AvgWind);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[AVGWIND], wxLIST_AUTOSIZE);
        }

        if(columns[MAXWIND] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[MAXWIND], weatherroute->MaxWind);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[MAXWIND], wxLIST_AUTOSIZE);
        }

        if(columns[MAXWINDGUST] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[MAXWINDGUST], weatherroute->MaxWindGust);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[MAXWINDGUST], wxLIST_AUTOSIZE);
        }
        
        if(columns[AVGCURRENT] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[AVGCURRENT], weatherroute->AvgCurrent);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[AVGCURRENT], wxLIST_AUTOSIZE);
        }

        if(columns[MAXCURRENT] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[MAXCURRENT], weatherroute->MaxCurrent);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[MAXCURRENT], wxLIST_AUTOSIZE);
        }

        if(columns[AVGSWELL] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[AVGSWELL], weatherroute->AvgSwell);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[AVGSWELL], wxLIST_AUTOSIZE);
        }

        if(columns[MAXSWELL] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[MAXSWELL], weatherroute->MaxSwell);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[MAXSWELL], wxLIST_AUTOSIZE);
        }

        if(columns[UPWIND_PERCENTAGE] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[UPWIND_PERCENTAGE], weatherroute->UpwindPercentage);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[UPWIND_PERCENTAGE], wxLIST_AUTOSIZE);
        }

        if(columns[PORT_STARBOARD] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[PORT_STARBOARD], weatherroute->PortStarboard);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[PORT_STARBOARD], wxLIST_AUTOSIZE);
        }
        
        if(columns[TACKS] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[TACKS], weatherroute->Tacks);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[TACKS], wxLIST_AUTOSIZE);
        }
        
        if(columns[COMFORT] >= 0) {
            m_panel->m_lWeatherRoutes->SetItem(index, columns[COMFORT], weatherroute->Comfort);
            m_panel->m_lWeatherRoutes->SetColumnWidth(columns[COMFORT], wxLIST_AUTOSIZE);
        }
    }

    if(columns[STATE] >= 0) {
        m_panel->m_lWeatherRoutes->SetItem(index, columns[STATE], weatherroute->State);
        m_panel->m_lWeatherRoutes->SetColumnWidth(columns[STATE], wxLIST_AUTOSIZE);
    }
}

// The configuration changed, so stop computation and update the display in the list
void WeatherRouting::SetConfigurationRoute(WeatherRoute *weatherroute)
{
    if(m_bSkipUpdateCurrentItems)
        return;

    RouteMapOverlay *rmo = weatherroute->routemapoverlay;
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++)
        if(*it == rmo && rmo->Running())
            rmo->DeleteThread();

    weatherroute->Update(this);

    for(long index = 0; index<m_panel->m_lWeatherRoutes->GetItemCount(); index++) {
        WeatherRoute *wr = reinterpret_cast<WeatherRoute*>
            (wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)));
        if(weatherroute == wr) {
            UpdateItem(index);
            break;
        }
    }
}

void WeatherRouting::UpdateBoatFilename(wxString boatFileName)
{
    for(long index = 0; index<m_panel->m_lWeatherRoutes->GetItemCount(); index++) {
        WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
            (wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)));

        RouteMapConfiguration c = weatherroute->routemapoverlay->GetConfiguration();
        if(c.boatFileName == boatFileName) {
            RouteMapOverlay *rmo = weatherroute->routemapoverlay;
            rmo->ResetFinished();
            SetConfigurationRoute(weatherroute);
        }
    }
}

void WeatherRouting::UpdateCurrentConfigurations()
{
    long index = -1;
    for(;;) {
        index = m_panel->m_lWeatherRoutes->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(index == -1)
            break;
        WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>
            (wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)));
        SetConfigurationRoute(weatherroute);
    }
}

void WeatherRouting::UpdateStates()
{
    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++)
        (*it)->Update(this, true);
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++)
        UpdateItem(i, true);
}

std::list <RouteMapOverlay *>WeatherRouting::CurrentRouteMaps(bool messagedialog)
{
    std::list<RouteMapOverlay *>routemapoverlays;
    long index = -1;
    if(m_panel)
        for(;;) {
            index = m_panel->m_lWeatherRoutes->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            if(index == -1)
                break;
            routemapoverlays.push_back(reinterpret_cast<WeatherRoute*>
                                       (wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(index)))->routemapoverlay);
        }

    if(messagedialog && routemapoverlays.empty()) {
        wxMessageDialog mdlg(this, _("No Weather Route selected"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }

    return routemapoverlays;
}

RouteMapOverlay *WeatherRouting::FirstCurrentRouteMap()
{
    std::list<RouteMapOverlay*> currentroutemaps = CurrentRouteMaps();
    return currentroutemaps.empty() ? NULL : currentroutemaps.front();
}

void WeatherRouting::RebuildList()
{
    m_panel->m_lWeatherRoutes->DeleteAllItems();
    for(std::list<WeatherRoute*>::iterator it = m_WeatherRoutes.begin();
        it != m_WeatherRoutes.end(); it++) {
        if(!(*it)->Filtered) {
            wxListItem item;
            item.SetId(m_panel->m_lWeatherRoutes->GetItemCount());
            item.SetData(*it);
            UpdateItem(m_panel->m_lWeatherRoutes->InsertItem(item));
        }
    }
}

void WeatherRouting::Export(RouteMapOverlay &routemapoverlay)
{
    std::list<PlotData> plotdata = routemapoverlay.GetPlotData(false);

    if(plotdata.empty()) {
        wxMessageDialog mdlg(this, _("Empty Route, nothing to export\n"),
                             _("Weather Routing"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
        return;
    }

    PlugIn_Track* newTrack = new PlugIn_Track;
    wxDateTime display_time = routemapoverlay.StartTime();
    if(m_SettingsDialog.m_cbUseLocalTime->GetValue())
        display_time = display_time.FromUTC();

    newTrack->m_NameString = _("Weather Route ") + " ("  +display_time.Format(_T("%x %H:%M")) + ")";

    // XXX double check time is really end time, not start time off by one.
    RouteMapConfiguration c = routemapoverlay.GetConfiguration();

    for(std::list<PlotData>::iterator it = plotdata.begin(); it != plotdata.end(); it++) {
        PlugIn_Waypoint*  newPoint = new PlugIn_Waypoint
            ((*it).lat, heading_resolve((*it).lon), _T("circle"), _("Weather Route Point"));

        newPoint->m_CreateTime = (*it).time;
        newTrack->pWaypointList->Append(newPoint);
    }

    // last point, missing if config didn't succeed
    Position *p = routemapoverlay.GetDestination();
    if (p) {
        PlugIn_Waypoint*  newPoint = new PlugIn_Waypoint
            (p->lat, p->lon, _T("circle"), _("Weather Route Destination"));
        newPoint->m_CreateTime =  routemapoverlay.EndTime();
        newTrack->pWaypointList->Append(newPoint);
    }

    AddPlugInTrack(newTrack);
    // not done PlugIn_Track DTOR
    newTrack->pWaypointList->DeleteContents( true );
    newTrack->pWaypointList->Clear();

    delete newTrack;

    GetParent()->Refresh();
}

void WeatherRouting::Start(RouteMapOverlay *routemapoverlay)
{
    if(!routemapoverlay ||
       (routemapoverlay->Finished() &&
        !routemapoverlay->GribFailed()))
        return;

    RouteMapConfiguration configuration = routemapoverlay->GetConfiguration();

    if(configuration.DeltaTime <= 0) {
        routemapoverlay->SetError(_("Zero Time Step"));
        return;
    }
    if (configuration.DetectBoundary) {
        if (m_weather_routing_pi.InBoundary(configuration.EndLat, configuration.EndLon)
             ||
            m_weather_routing_pi.InBoundary(configuration.StartLat, configuration.StartLon)) 
        {
            routemapoverlay->SetError(_("inside exclusion boundary"));
            return;
        }
    }

    if(fabs(configuration.StartLat) > configuration.MaxLatitude ||
       fabs(configuration.EndLat) > configuration.MaxLatitude) {
            routemapoverlay->SetError(_("lies outside of Max Latitude constraint"));
            return;
    }

    /* initialize crossing land routine from main thread as it is
       not re-entrant, and cannot be done by worker-threads later */
    if(configuration.DetectLand)
        PlugIn_GSHHS_CrossesLand(0, 0, 0, 0);
    if(configuration.ClimatologyType != RouteMapConfiguration::DISABLED) {
        /* query climatology to load it from main thread */
        double dir, speed;
        if(RouteMap::ClimatologyData)
            RouteMap::ClimatologyData(0, wxDateTime::Now(), 0, 0, dir, speed);
    }

    // already running?
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++)
        if(*it == routemapoverlay)
            return;

    if(!m_bRunning)
        m_StatisticsDialog.SetRunTime(m_RunTime = wxTimeSpan(0));

    // already waiting?
    for(std::list<RouteMapOverlay*>::iterator it = m_WaitingRouteMaps.begin();
        it != m_WaitingRouteMaps.end(); it++)
        if(*it == routemapoverlay)
            return;

    routemapoverlay->Reset();
    m_RoutesToRun++;
    m_WaitingRouteMaps.push_back(routemapoverlay);
    SetEnableConfigurationMenu();
    UpdateRouteMap(routemapoverlay);
}

void WeatherRouting::StartAll()
{
    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute = reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
        Start(weatherroute->routemapoverlay);
    }
}

void WeatherRouting::Stop()
{
    /* stop all the threads at once, rather than waiting for each one before
       telling the next to stop */
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++)
        (*it)->Stop();

    wxProgressDialog *progressdialog = NULL;

    int c = 0;
    for(std::list<RouteMapOverlay*>::iterator it = m_RunningRouteMaps.begin();
        it != m_RunningRouteMaps.end(); it++) {
        // Wait for threads to finish
        while((*it)->Running())
            wxThread::Sleep(100);

        (*it)->ResetFinished();
        (*it)->DeleteThread();

        if(progressdialog)
            progressdialog->Update(c++);
    }

    delete progressdialog;

    m_RunningRouteMaps.clear();
    m_WaitingRouteMaps.clear();

    UpdateStates();

    m_RoutesToRun = 0;
    m_panel->m_gProgress->SetValue(0);
    m_bRunning = false;

    SetEnableConfigurationMenu();
    if(m_StartTime.IsValid())
        m_StatisticsDialog.SetRunTime(m_RunTime += wxDateTime::Now() - m_StartTime);
}

void WeatherRouting::Reset()
{
    if(m_bRunning)
        Stop();

    for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
        WeatherRoute *weatherroute =
            reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
        weatherroute->routemapoverlay->Reset();
    }
    m_positionOnRoute = NULL;
    UpdateDialogs();

    GetParent()->Refresh();
}

void WeatherRouting::DeleteRouteMaps(std::list<RouteMapOverlay *>routemapoverlays)
{
    bool current = false;
    for(std::list<RouteMapOverlay*>::iterator it = routemapoverlays.begin();
        it != routemapoverlays.end(); it++) {
        std::list<RouteMapOverlay *>currentroutemaps = CurrentRouteMaps();
        for(std::list<RouteMapOverlay*>::iterator cit = currentroutemaps.begin();
            cit != currentroutemaps.end(); cit++)
            if(*it == *cit) {
                current = true;
                break;
            }

        for(std::list<RouteMapOverlay*>::iterator wit = m_WaitingRouteMaps.begin();
            wit != m_WaitingRouteMaps.end(); wit++)
            if(*it == *wit) {
                m_WaitingRouteMaps.erase(wit);
                break;
            }

        for(std::list<RouteMapOverlay*>::iterator rit = m_RunningRouteMaps.begin();
            rit != m_RunningRouteMaps.end(); rit++)
            if(*it == *rit) {
                m_RunningRouteMaps.erase(rit);
                break;
            }

        for(int i=0; i<m_panel->m_lWeatherRoutes->GetItemCount(); i++) {
            WeatherRoute *weatherroute =
                reinterpret_cast<WeatherRoute*>(wxUIntToPtr(m_panel->m_lWeatherRoutes->GetItemData(i)));
            if(weatherroute->routemapoverlay == *it) {
                m_panel->m_lWeatherRoutes->DeleteItem(i);
                break;
            }
        }

        for(std::list<WeatherRoute*>::iterator writ = m_WeatherRoutes.begin();
            writ != m_WeatherRoutes.end(); writ++)
            if((*writ)->routemapoverlay == *it) {
                delete *writ;
                m_WeatherRoutes.erase(writ);
                break;
            }
    }

    m_ReportDialog.m_bReportStale = true;

    SetEnableConfigurationMenu();

    if(current)
        UpdateDialogs();
}

RouteMapConfiguration WeatherRouting::DefaultConfiguration()
{
    RouteMapConfiguration configuration;

    if(RouteMap::Positions.size() >= 1) {
        RouteMapPosition &p = *RouteMap::Positions.begin();
        configuration.Start = p.Name;
        configuration.StartLat = p.lat, configuration.StartLon = p.lon;
    } else
        configuration.StartLat = 0, configuration.StartLon = 0;

    configuration.StartTime = wxDateTime::Now();
    configuration.DeltaTime = 3600;

    if(RouteMap::Positions.size() >= 2) {
        RouteMapPosition &p = *(++RouteMap::Positions.begin());
        configuration.End = p.Name;
        configuration.EndLat = p.lat, configuration.EndLon = p.lon;
    } else
        configuration.EndLat = 0, configuration.EndLon = 0;
    
    configuration.boatFileName = weather_routing_pi::StandardPath() +
        _T("boats") + wxFileName::GetPathSeparator() + _T("Boat.xml");
    
    configuration.Integrator = RouteMapConfiguration::NEWTON;

    configuration.MaxDivertedCourse = 90;
    configuration.MaxCourseAngle = 180;
    configuration.MaxSearchAngle = 120;
    configuration.MaxTrueWindKnots = 100;
    configuration.MaxApparentWindKnots = 100;

    configuration.MaxSwellMeters = 20;
    configuration.MaxLatitude = 90;
    configuration.TackingTime = 0;
    configuration.WindVSCurrent = 0;
    
    configuration.AvoidCycloneTracks = false;
    configuration.CycloneMonths = 1;
    configuration.CycloneDays = 0;

    configuration.UseGrib = true;
    configuration.ClimatologyType = RouteMapConfiguration::MOST_LIKELY;
    configuration.AllowDataDeficient = false;
    configuration.WindStrength = 1;
    configuration.DetectLand = true;
    configuration.SafetyMarginLand = 0.;
    configuration.DetectBoundary = false;
    configuration.Currents = false;
    configuration.OptimizeTacking = false;
    configuration.InvertedRegions = false;
    configuration.Anchoring = false;

    configuration.FromDegree = 0;
    configuration.ToDegree = 180;
    configuration.ByDegrees = 5;

    return configuration;
}
