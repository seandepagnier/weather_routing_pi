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
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/scrolwin.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listctrl.h>
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
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText28;
		wxStaticText* m_stStartDate;
		wxStaticText* m_staticText30;
		wxStaticText* m_stStartTime;
		wxStaticText* m_staticText51;
		wxStaticText* m_staticText61;
		wxStaticText* m_staticText47;
		wxStaticText* m_stState;
		wxStaticText* m_staticText511;
		wxStaticText* m_stRunTime;
		wxStaticText* m_staticText53;
		wxStaticText* m_stIsoChrons;
		wxStaticText* m_staticText55;
		wxStaticText* m_stRoutes;
		wxStaticText* m_staticText57;
		wxStaticText* m_stInvRoutes;
		wxStaticText* m_staticText90;
		wxStaticText* m_stSkipPositions;
		wxStaticText* m_staticText49;
		wxStaticText* m_stPositions;
		wxButton* m_bCompute;
		wxButton* m_bReset;
		wxButton* m_bConfiguration;
		wxButton* m_bSettings;
		wxButton* m_bBoat;
		wxButton* m_bPlot;
		wxButton* m_bExport;
		wxButton* m_bInformation;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdateEnd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnConfiguration( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSettings( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_tStartLat;
		wxTextCtrl* m_tStartLon;
		wxTextCtrl* m_tEndLat;
		wxTextCtrl* m_tEndLon;
		
		WeatherRoutingDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Weather Routing Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxRESIZE_BORDER ); 
		~WeatherRoutingDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WeatherRoutingConfigurationDialogBase
///////////////////////////////////////////////////////////////////////////////
class WeatherRoutingConfigurationDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxListBox* m_lDegreeSteps;
		wxTextCtrl* m_tDegreeStep;
		wxButton* m_bAddDegreeStep;
		wxButton* m_bRemoveDegreeStep;
		wxButton* m_bGenerateDegreeSteps;
		wxButton* m_bClearDegreeSteps;
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
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAddDegreeStep( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveDegreeStep( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGenerateDegreeSteps( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearDegreeSteps( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		WeatherRoutingConfigurationDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Weather Routing Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~WeatherRoutingConfigurationDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WeatherRoutingSettingsDialogBase
///////////////////////////////////////////////////////////////////////////////
class WeatherRoutingSettingsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText74;
		wxStaticText* m_staticText73;
		wxStaticText* m_staticText75;
		wxStaticText* m_staticText70;
		wxStaticText* m_staticText71;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		wxColourPickerCtrl* m_cpCursorRoute;
		wxColourPickerCtrl* m_cpDestinationRoute;
		wxSpinCtrl* m_sRouteThickness;
		wxSpinCtrl* m_sIsoChronThickness;
		wxSpinCtrl* m_sAlternateRouteThickness;
		wxCheckBox* m_cbAlternatesForAll;
		
		WeatherRoutingSettingsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Weather Routing Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION ); 
		~WeatherRoutingSettingsDialogBase();
	
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
		wxSpinCtrl* m_sWindSpeed;
		wxSpinCtrl* m_spinCtrl20;
		wxSpinCtrl* m_spinCtrl21;
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
		wxButton* m_bClose;
		wxPanel* m_panel31;
		wxStaticText* m_staticText93;
		wxStaticText* m_stCSVFile;
		wxStaticText* m_staticText351;
		wxSpinCtrl* m_sFileCSVWindSpeedStep;
		wxStaticText* m_staticText36;
		wxStaticText* m_staticText37;
		wxSpinCtrl* m_sFileCSVWindDegreeStep;
		wxStaticText* m_staticText38;
		wxButton* m_bOpen;
		wxButton* m_bSave;
		wxPanel* m_panel311;
		wxStaticText* m_staticText3511;
		wxStaticText* m_stBestCourseUpWindPortTack;
		wxStaticText* m_staticText35111;
		wxStaticText* m_stBestCourseUpWindStarboardTack;
		wxStaticText* m_staticText35112;
		wxStaticText* m_stBestCourseDownWindPortTack;
		wxStaticText* m_staticText35113;
		wxStaticText* m_stBestCourseDownWindStarboardTack;
		wxButton* m_bOptimizeTacking;
		wxButton* m_bResetOptimizeTacking;
		wxPanel* m_panel3111;
		wxStaticText* m_staticText100;
		wxChoice* m_cHullType;
		wxStaticText* m_staticText58;
		wxSpinCtrl* m_sDisplacement;
		wxStaticText* m_staticText57;
		wxSpinCtrl* m_sLWL;
		wxStaticText* m_staticText35;
		wxSpinCtrl* m_sFrictionalDrag;
		wxStaticText* m_staticText112;
		wxSpinCtrl* m_sWakeDrag;
		wxButton* m_bRecomputeDrag;
		wxButton* m_bDragInfo;
		wxPanel* m_panel311111;
		wxListCtrl* m_lBoatPlans;
		wxStaticText* m_staticText961;
		wxSlider* m_sEta;
		wxStaticText* m_staticText91;
		wxSpinCtrl* m_sLuffAngle;
		wxButton* m_bNewBoatPlan;
		wxButton* m_bEditBoatPlan;
		wxButton* m_bDeleteBoatPlan;
		wxPanel* m_panel5;
		wxStaticText* m_staticText113;
		wxSpinCtrl* m_sBeam;
		wxStaticText* m_staticText109;
		wxSpinCtrl* m_sLOA;
		wxStaticText* m_staticText92;
		wxStaticText* m_stHullSpeed;
		wxStaticText* m_staticText94;
		wxStaticText* m_stCapsizeRisk;
		wxStaticText* m_staticText96;
		wxStaticText* m_stComfortFactor;
		wxStaticText* m_staticText105;
		wxStaticText* m_stDisplacementLengthRatio;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMouseEventsPlot( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPlot( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSizePlot( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateWindSpeed( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOptimizeTacking( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetOptimalTackingSpeed( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateStatistics( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateStatistics( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnRecompute( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnRecomputeDrag( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDragInfo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSailPlanSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnEta( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnNewBoatPlan( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditBoatPlan( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteBoatPlan( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		BoatDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Boat"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~BoatDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BoatPlanDialogBase
///////////////////////////////////////////////////////////////////////////////
class BoatPlanDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText87;
		wxTextCtrl* m_tBoatPlanName;
		wxNotebook* m_notebook2;
		wxPanel* m_panel10;
		wxListBox* m_lSwitchPlans;
		wxButton* m_bNewSwitchPlanRule;
		wxButton* m_bEditSwitchBoatPlan;
		wxButton* m_bDeleteSwitchBoatPlan;
		wxButton* m_bDone;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNewSwitchPlanRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditSwitchPlanRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteSwitchPlanRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDone( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		BoatPlanDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sail Plan"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,300 ), long style = wxCAPTION|wxRESIZE_BORDER ); 
		~BoatPlanDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SwitchPlanDialogBase
///////////////////////////////////////////////////////////////////////////////
class SwitchPlanDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxCheckBox* m_cbMaxWindSpeed;
		wxSpinCtrl* m_sMaxWindSpeed;
		wxStaticText* m_staticText101;
		wxCheckBox* m_cbMinWindSpeed;
		wxSpinCtrl* m_sMinWindSpeed;
		wxStaticText* m_staticText102;
		wxCheckBox* m_cbMaxWindDirection;
		wxSpinCtrl* m_sMaxWindDirection;
		wxStaticText* m_staticText103;
		wxCheckBox* m_cbMinWindDirection;
		wxSpinCtrl* m_sMinWindDirection;
		wxStaticText* m_staticText104;
		wxCheckBox* m_cbMaxWaveHeight;
		wxSpinCtrl* m_sMaxWaveHeight;
		wxStaticText* m_staticText105;
		wxCheckBox* m_cbMinWaveHeight;
		wxSpinCtrl* m_sMinWaveHeight;
		wxStaticText* m_staticText106;
		wxStaticText* m_staticText107;
		wxChoice* m_cPlans;
		wxButton* m_bDone;
		wxButton* m_bCancel;
		wxButton* m_bInformation;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDone( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SwitchPlanDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Switch Sail Plan Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION ); 
		~SwitchPlanDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PlotDialogBase
///////////////////////////////////////////////////////////////////////////////
class PlotDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxScrolledWindow* m_PlotWindow;
		wxStaticText* m_staticText138;
		wxSlider* m_sPosition;
		wxStaticText* m_staticText139;
		wxSlider* m_sScale;
		wxStaticText* m_staticText140;
		wxChoice* m_cVariable1;
		wxStaticText* m_stMousePosition1;
		wxStaticText* m_staticText1401;
		wxChoice* m_cVariable2;
		wxStaticText* m_stMousePosition2;
		wxStaticText* m_staticText14011;
		wxChoice* m_cVariable3;
		wxStaticText* m_stMousePosition3;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMouseEventsPlot( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPlot( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSizePlot( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlotVariable( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		PlotDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Plot"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~PlotDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class InformationDialog
///////////////////////////////////////////////////////////////////////////////
class InformationDialog : public wxDialog 
{
	private:
	
	protected:
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
	
	public:
		wxHtmlWindow* m_htmlInformation;
		
		InformationDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Weather Routing Information"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~InformationDialog();
	
};

#endif //__WEATHERROUTINGUI_H__
