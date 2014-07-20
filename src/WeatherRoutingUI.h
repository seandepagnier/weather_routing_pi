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
#include <wx/intl.h>
#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/gauge.h>
#include <wx/splitter.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/textctrl.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/listbox.h>
#include <wx/scrolwin.h>
#include <wx/listbook.h>
#include <wx/slider.h>
#include <wx/radiobut.h>
#include <wx/html/htmlwin.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WeatherRoutingBase
///////////////////////////////////////////////////////////////////////////////
class WeatherRoutingBase : public wxFrame 
{
	private:
	
	protected:
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel11;
		wxListCtrl* m_lPositions;
		wxPanel* m_panel12;
		wxListCtrl* m_lWeatherRoutes;
		wxMenuBar* m_menubar3;
		wxMenu* m_mFile;
		wxMenu* m_mPosition;
		wxMenu* m_mConfiguration;
		wxMenuItem* m_mBatch;
		wxMenuItem* m_mEdit;
		wxMenuItem* m_mGoTo;
		wxMenuItem* m_mDelete;
		wxMenuItem* m_mCompute;
		wxMenuItem* m_mComputeAll;
		wxMenuItem* m_mStop;
		wxMenuItem* m_mExport;
		wxMenuItem* m_mExportAll;
		wxMenu* m_mView;
		wxMenu* m_mHelp;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnPositionKeyDown( wxListEvent& event ) { event.Skip(); }
		virtual void OnEditConfigurationClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnWeatherRoutesListLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnWeatherRouteSort( wxListEvent& event ) { event.Skip(); }
		virtual void OnWeatherRouteSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnWeatherRouteKeyDown( wxListEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeletePosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteAllPositions( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBatch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditConfiguration( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGoTo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComputeAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFilter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSettings( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatistics( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxGauge* m_gProgress;
		wxMenuItem* m_mDeleteAll;
		
		WeatherRoutingBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		
		~WeatherRoutingBase();
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 120 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WeatherRoutingBase::m_splitter1OnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SettingsDialogBase
///////////////////////////////////////////////////////////////////////////////
class SettingsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText74;
		wxStaticText* m_staticText73;
		wxStaticText* m_staticText75;
		wxStaticText* m_staticText70;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText115;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Help;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdateColor( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnUpdateSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateColumns( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxColourPickerCtrl* m_cpCursorRoute;
		wxColourPickerCtrl* m_cpDestinationRoute;
		wxSpinCtrl* m_sRouteThickness;
		wxSpinCtrl* m_sIsoChronThickness;
		wxSpinCtrl* m_sAlternateRouteThickness;
		wxCheckBox* m_cbAlternatesForAll;
		wxCheckBox* m_cbSquaresAtSailChanges;
		wxSpinCtrl* m_sConcurrentThreads;
		wxCheckListBox* m_cblFields;
		
		SettingsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~SettingsDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ConfigurationDialogBase
///////////////////////////////////////////////////////////////////////////////
class ConfigurationDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxComboBox* m_cStart;
		wxStaticText* m_staticText28;
		wxButton* m_bGribTime;
		wxStaticText* m_staticText30;
		wxTextCtrl* m_tStartHour;
		wxButton* m_bCurrentTime;
		wxComboBox* m_cEnd;
		wxSpinCtrl* m_sTimeStepHours;
		wxStaticText* m_staticText110;
		wxSpinCtrl* m_sTimeStepMinutes;
		wxStaticText* m_staticText111;
		wxSpinCtrl* m_sTimeStepSeconds;
		wxStaticText* m_staticText112;
		wxButton* m_bEditBoat;
		wxCheckBox* m_cbAllowDataDeficient;
		wxCheckBox* m_cbDetectLand;
		wxCheckBox* m_cbInvertedRegions;
		wxCheckBox* m_cbAnchoring;
		wxCheckBox* m_cbNewton;
		wxCheckBox* m_cbRungeKutta;
		wxNotebook* m_notebook4;
		wxPanel* m_panel11;
		wxStaticText* m_staticText20;
		wxSpinCtrl* m_sMaxDivertedCourse;
		wxStaticText* m_staticText1181;
		wxStaticText* m_staticText119;
		wxSpinCtrl* m_sMaxCourseAngle;
		wxStaticText* m_staticText1251;
		wxStaticText* m_staticText124;
		wxSpinCtrl* m_sMaxSearchAngle;
		wxStaticText* m_staticText125;
		wxStaticText* m_staticText23;
		wxSpinCtrl* m_sMaxWindKnots;
		wxStaticText* m_staticText128;
		wxPanel* m_panel12;
		wxStaticText* m_staticText27;
		wxSpinCtrl* m_sMaxSwellMeters;
		wxStaticText* m_staticText129;
		wxStaticText* m_staticText26;
		wxSpinCtrl* m_sMaxLatitude;
		wxStaticText* m_staticText131;
		wxStaticText* m_staticText24;
		wxSpinCtrl* m_sTackingTime;
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText120;
		wxSpinCtrl* m_sWindVSCurrent;
		wxCheckBox* m_cbAvoidCycloneTracks;
		wxStaticText* m_staticText1281;
		wxSpinCtrl* m_sCycloneMonths;
		wxStaticText* m_staticText1291;
		wxSpinCtrl* m_sCycloneDays;
		wxStaticText* m_staticText130;
		wxListBox* m_lDegreeSteps;
		wxTextCtrl* m_tDegreeStep;
		wxButton* m_bAddDegreeStep;
		wxButton* m_bRemoveDegreeStep;
		wxButton* m_bClearDegreeSteps;
		wxStaticText* m_staticText113;
		wxTextCtrl* m_tFromDegrees;
		wxStaticText* m_staticText114;
		wxStaticText* m_staticText115;
		wxTextCtrl* m_tToDegrees;
		wxStaticText* m_staticText116;
		wxStaticText* m_staticText117;
		wxTextCtrl* m_tByDegrees;
		wxStaticText* m_staticText118;
		wxButton* m_bGenerateDegreeSteps;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateDate( wxDateEvent& event ) { event.Skip(); }
		virtual void OnGribTime( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCurrentTime( wxCommandEvent& event ) { event.Skip(); }
		virtual void EnableSpin( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnUpdateSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdateFile( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnEditBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateIntegratorNewton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateIntegratorRungeKutta( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAvoidCyclones( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddDegreeStep( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveDegreeStep( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearDegreeSteps( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGenerateDegreeSteps( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxDatePickerCtrl* m_dpStartDate;
		wxFilePickerCtrl* m_fpBoat;
		wxCheckBox* m_cbUseGrib;
		wxChoice* m_cClimatologyType;
		wxCheckBox* m_cbCurrents;
		wxPanel* m_pCyclones;
		
		ConfigurationDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~ConfigurationDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BoatDialogBase
///////////////////////////////////////////////////////////////////////////////
class BoatDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxListCtrl* m_lBoatPlans;
		wxButton* m_bNewBoatPlan;
		wxButton* m_bDeleteBoatPlan;
		wxScrolledWindow* m_PlotWindow;
		wxNotebook* m_nNotebook;
		wxPanel* m_panel3;
		wxListbook* m_lPlotType;
		wxPanel* m_panel14;
		wxChoice* m_cPlotType;
		wxSpinCtrl* m_sWindSpeed;
		wxPanel* m_panel16;
		wxSpinCtrl* m_sWindDirection;
		wxPanel* m_panel161;
		wxChoice* m_cPlotVariable;
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
		wxPanel* m_panel17;
		wxStaticText* m_staticText100;
		wxChoice* m_cHullType;
		wxStaticText* m_staticText58;
		wxSpinCtrl* m_sDisplacement;
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText128;
		wxSpinCtrl* m_sSailArea;
		wxStaticText* m_staticText129;
		wxStaticText* m_staticText57;
		wxSpinCtrl* m_sLWL;
		wxStaticText* m_staticText122;
		wxStaticText* m_staticText109;
		wxSpinCtrl* m_sLOA;
		wxStaticText* m_staticText127;
		wxStaticText* m_staticText113;
		wxSpinCtrl* m_sBeam;
		wxStaticText* m_staticText126;
		wxStaticText* m_staticText119;
		wxStaticText* m_stSailAreaDisplacementRatio;
		wxStaticText* m_staticText105;
		wxStaticText* m_stDisplacementLengthRatio;
		wxStaticText* m_staticText92;
		wxStaticText* m_stHullSpeed;
		wxStaticText* m_staticText94;
		wxStaticText* m_stCapsizeRisk;
		wxStaticText* m_staticText96;
		wxStaticText* m_stComfortFactor;
		wxPanel* m_pPolarConfig;
		wxChoice* m_cPolarMethod;
		wxStaticBoxSizer* m_sbComputationTransform;
		wxStaticText* m_staticText961;
		wxSlider* m_sEta;
		wxTextCtrl* m_tEta;
		wxStaticText* m_staticText35;
		wxSpinCtrl* m_sFrictionalDrag;
		wxStaticText* m_staticText112;
		wxSpinCtrl* m_sWakeDrag;
		wxButton* m_bRecomputeDrag;
		wxButton* m_bDragInfo;
		wxStaticText* m_staticText91;
		wxSpinCtrl* m_sLuffAngle;
		wxCheckBox* m_cbWingWingRunning;
		wxButton* m_bSaveCSV;
		wxStaticBoxSizer* m_sbComputationIMF;
		wxStaticText* m_staticText131;
		wxStaticBoxSizer* m_sbCSV;
		wxStaticText* m_staticText93;
		wxFilePickerCtrl* m_fpCSVPath;
		wxStaticText* m_staticText123;
		wxStaticText* m_stWindSpeedStep;
		wxStaticText* m_staticText125;
		wxStaticText* m_stWindDegreeStep;
		wxPanel* m_panel171;
		wxPanel* m_panel311;
		wxStaticText* m_staticText118;
		wxSpinCtrl* m_sVMGWindSpeed;
		wxChoice* m_cVMGTrueApparent;
		wxStaticText* m_staticText3511;
		wxStaticText* m_stBestCourseUpWindPortTack;
		wxStaticText* m_staticText35111;
		wxStaticText* m_stBestCourseUpWindStarboardTack;
		wxStaticText* m_staticText35112;
		wxStaticText* m_stBestCourseDownWindPortTack;
		wxStaticText* m_staticText35113;
		wxStaticText* m_stBestCourseDownWindStarboardTack;
		wxCheckBox* m_cbOptimizeTacking;
		wxPanel* m_panel10;
		wxListBox* m_lSwitchPlans;
		wxButton* m_bNewSwitchPlanRule;
		wxButton* m_bEditSwitchBoatPlan;
		wxButton* m_bDeleteSwitchBoatPlan;
		wxButton* m_bOpen;
		wxButton* m_bSaveAs;
		wxButton* m_bSaveandClose;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSailPlanSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnNewBoatPlan( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteBoatPlan( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMouseEventsPlot( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPlot( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSizePlot( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxListbookEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnRecompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRecomputeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnPolarMethod( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEtaSlider( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnEta( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRecomputeDrag( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDragInfo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveCSV( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPolarCSVFile( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnUpdateVMG( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdateVMG( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOptimizeTacking( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSwitchPlanRules( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewSwitchPlanRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditSwitchPlanRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteSwitchPlanRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		BoatDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Boat"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~BoatDialogBase();
	
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
		wxCheckBox* m_cbDayTime;
		wxCheckBox* m_cbNightTime;
		wxStaticText* m_staticText107;
		wxChoice* m_cPlans;
		wxButton* m_bInformation;
		wxButton* m_bCancel;
		wxButton* m_bDone;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDone( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SwitchPlanDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Switch Sail Plan Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION ); 
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
		wxRadioButton* m_rbCurrentRoute;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMouseEventsPlot( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPlot( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnSizePlot( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlotVariable( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateRoute( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxRadioButton* m_rbCursorRoute;
		
		PlotDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Route Plot"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~PlotDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialogBase
///////////////////////////////////////////////////////////////////////////////
class AboutDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText110;
		wxButton* m_bAboutAuthor;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAboutAuthor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		AboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About Weather Routing"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class StatisticsDialogBase
///////////////////////////////////////////////////////////////////////////////
class StatisticsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText511;
		wxStaticText* m_stRunTime;
		wxStaticText* m_staticText47;
		wxStaticText* m_stState;
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
		wxStdDialogButtonSizer* m_sdbSizer5;
		wxButton* m_sdbSizer5OK;
	
	public:
		
		StatisticsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Statistics"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~StatisticsDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ReportDialogBase
///////////////////////////////////////////////////////////////////////////////
class ReportDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxHtmlWindow* m_htmlConfigurationReport;
		wxHtmlWindow* m_htmlRoutesReport;
		wxButton* m_bInformation;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ReportDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Route Report"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,440 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ReportDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class FilterRoutesDialogBase
///////////////////////////////////////////////////////////////////////////////
class FilterRoutesDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxChoice* m_cCategory;
		wxTextCtrl* m_tFilter;
		wxButton* m_button48;
		wxButton* m_bDone;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCategory( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFilterText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDone( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		FilterRoutesDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Filter Routes"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~FilterRoutesDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ConfigurationBatchDialogBase
///////////////////////////////////////////////////////////////////////////////
class ConfigurationBatchDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebookConfigurations;
		wxPanel* m_panel8;
		wxStaticText* m_staticText108;
		wxStaticText* m_stStartDateTime;
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText122;
		wxStaticText* m_staticText123;
		wxStaticText* m_staticText124;
		wxStaticText* m_staticText125;
		wxStaticText* m_staticText126;
		wxButton* m_button41;
		wxButton* m_button38;
		wxButton* m_button39;
		wxButton* m_button40;
		wxPanel* m_pRoutes;
		wxListBox* m_lSources;
		wxListBox* m_lDestinations;
		wxStaticText* m_staticText1241;
		wxTextCtrl* m_tMiles;
		wxStaticText* m_staticText1251;
		wxButton* m_bConnect;
		wxButton* m_bDisconnectAll;
		wxPanel* m_panel9;
		wxButton* m_bAddBoat;
		wxButton* m_bRemoveBoat;
		wxButton* m_bInformation;
		wxButton* m_bReset;
		wxButton* m_bGenerate;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOnce( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDaily( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWeekly( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMonthly( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSources( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDestinations( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnConnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDisconnectAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGenerate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_tStartDays;
		wxTextCtrl* m_tStartHours;
		wxTextCtrl* m_tStartSpacingDays;
		wxTextCtrl* m_tStartSpacingHours;
		wxListBox* m_lBoats;
		
		ConfigurationBatchDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Configuration Batch"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,400 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ConfigurationBatchDialogBase();
	
};

#endif //__WEATHERROUTINGUI_H__
