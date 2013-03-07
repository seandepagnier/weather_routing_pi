///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WEATHERROUTINGUI_H__
#define __WEATHERROUTINGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/scrolwin.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/slider.h>
#include <wx/notebook.h>
#include <wx/html/htmlwin.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WeatherRoutingDialogBase
///////////////////////////////////////////////////////////////////////////////
class WeatherRoutingDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText5;
		wxTextCtrl* m_tStartLat;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_tStartLon;
		wxStaticText* m_staticText51;
		wxTextCtrl* m_tEndLat;
		wxStaticText* m_staticText61;
		wxTextCtrl* m_tEndLon;
		wxButton* m_bCompute;
		wxButton* m_bClear;
		wxStaticText* m_staticText21;
		wxSpinCtrl* m_sDegreeStep;
		wxStaticText* m_staticText22;
		wxSpinCtrl* m_sTimeStep;
		wxStaticText* m_staticText20;
		wxSpinCtrl* m_sMaxDivertedCourse;
		wxStaticText* m_staticText23;
		wxSpinCtrl* m_sMaxWindKnots;
		wxStaticText* m_staticText27;
		wxSpinCtrl* m_sMaxSwellMeters;
		wxStaticText* m_staticText26;
		wxSpinCtrl* m_sMaxLatitude;
		wxStaticText* m_staticText24;
		wxSpinCtrl* m_sTackingTime;
		wxStaticText* m_staticText25;
		wxSpinCtrl* m_sSubSteps;
		wxCheckBox* m_cbDetectLand;
		wxCheckBox* m_cbInvertedRegions;
		wxCheckBox* m_cbAnchoring;
		wxCheckBox* m_cbAllowDataDeficient;
		wxButton* m_bBoat;
		wxButton* m_bInformation;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		WeatherRoutingDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Weather Routing Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~WeatherRoutingDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BoatDialogBase
///////////////////////////////////////////////////////////////////////////////
class BoatDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxScrolledWindow* m_PlotWindow;
		wxNotebook* m_notebook1;
		wxPanel* m_panel3;
		wxChoice* m_cPlotType;
		wxChoice* m_cPlotAxis;
		wxStaticText* m_stTrueWindAngle;
		wxStaticText* m_staticText21;
		wxStaticText* m_stTrueWindKnots;
		wxStaticText* m_staticText23;
		wxStaticText* m_stApparentWindAngle;
		wxStaticText* m_staticText211;
		wxStaticText* m_stApparentWindKnots;
		wxStaticText* m_staticText231;
		wxStaticText* m_stBoatAngle;
		wxStaticText* m_staticText212;
		wxStaticText* m_stBoatKnots;
		wxStaticText* m_staticText232;
		wxButton* m_bOpen;
		wxButton* m_bSave;
		wxPanel* m_panel5;
		wxStaticText* m_staticText12;
		wxSlider* m_sEta;
		wxStaticText* m_staticText13;
		wxSlider* m_sKeelPressure;
		wxStaticText* m_staticText14;
		wxSlider* m_sKeelLift;
		wxButton* m_bOptimizeTacking;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMouseEventsPlot( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPlot( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSizePlot( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRecompute( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnOptimizeTacking( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		BoatDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~BoatDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class InformationDialog
///////////////////////////////////////////////////////////////////////////////
class InformationDialog : public wxDialog 
{
	private:
	
	protected:
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
	
	public:
		wxHtmlWindow* m_htmlInformation;
		
		InformationDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Weather Routing Information"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~InformationDialog();
	
};

#endif //__WEATHERROUTINGUI_H__
