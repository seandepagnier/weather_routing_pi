///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul 15 2016)
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
#include <wx/button.h>
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
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/notebook.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/slider.h>
#include <wx/radiobut.h>
#include <wx/statline.h>
#include <wx/html/htmlwin.h>
#include <wx/listbox.h>
#include <wx/valgen.h>
#include <wx/grid.h>

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
		wxButton* m_bCompute;
		wxButton* m_bExport;
		wxMenu* m_mFile;
		wxMenuBar* m_menubar3;
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
		virtual void OnCompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { event.Skip(); }
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
		virtual void OnComputeAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFilter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSettings( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatistics( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCursorPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnManual( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxGauge* m_gProgress;
		wxMenuItem* m_mDeleteAll;
		
		WeatherRoutingBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		
		~WeatherRoutingBase();
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 120 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WeatherRoutingBase::m_splitter1OnIdle ), NULL, this );
		}
		
		void WeatherRoutingBaseOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_mFile, event.GetPosition() );
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
		wxCheckBox* m_cbMarkAtPolarChange;
		wxCheckBox* m_cbDisplayWindBarbs;
		wxCheckBox* m_cbDisplayCurrent;
		wxSpinCtrl* m_sConcurrentThreads;
		wxCheckListBox* m_cblFields;
		wxCheckBox* m_cbUseLocalTime;
		
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
		wxStaticText* m_staticText113;
		wxStaticText* m_staticText115;
		wxStaticText* m_staticText117;
		wxStaticText* m_staticText118;
		wxNotebook* m_notebook4;
		wxPanel* m_panel11;
		wxStaticText* m_staticText20;
		wxSpinCtrl* m_sMaxDivertedCourse;
		wxStaticText* m_staticText1181;
		wxStaticText* m_staticText23;
		wxSpinCtrl* m_sMaxTrueWindKnots;
		wxStaticText* m_staticText128;
		wxStaticText* m_staticText136;
		wxSpinCtrl* m_sMaxApparentWindKnots;
		wxStaticText* m_staticText1282;
		wxStaticText* m_staticText27;
		wxSpinCtrl* m_sMaxSwellMeters;
		wxStaticText* m_staticText129;
		wxPanel* m_panel12;
		wxStaticText* m_staticText26;
		wxSpinCtrl* m_sMaxLatitude;
		wxStaticText* m_staticText131;
		wxStaticText* m_staticText24;
		wxSpinCtrl* m_sTackingTime;
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText120;
		wxSpinCtrl* m_sWindVSCurrent;
		wxStaticText* m_staticText119;
		wxSpinCtrl* m_sMaxCourseAngle;
		wxStaticText* m_staticText1251;
		wxStaticText* m_staticText124;
		wxSpinCtrl* m_sMaxSearchAngle;
		wxStaticText* m_staticText125;
		wxCheckBox* m_cbAvoidCycloneTracks;
		wxStaticText* m_staticText1281;
		wxSpinCtrl* m_sCycloneMonths;
		wxStaticText* m_staticText1291;
		wxSpinCtrl* m_sCycloneDays;
		wxStaticText* m_staticText130;
		wxComboBox* m_cEnd;
		wxSpinCtrl* m_sTimeStepHours;
		wxStaticText* m_staticText110;
		wxSpinCtrl* m_sTimeStepMinutes;
		wxStaticText* m_staticText111;
		wxSpinCtrl* m_sTimeStepSeconds;
		wxStaticText* m_staticText112;
		wxNotebook* m_notebook41;
		wxPanel* m_panel18;
		wxCheckBox* m_cbDetectLand;
		wxCheckBox* m_cbDetectBoundary;
		wxStaticText* m_staticText1292;
		wxSpinCtrl* m_sWindStrength;
		wxStaticText* m_staticText1301;
		wxPanel* m_panel19;
		wxCheckBox* m_cbInvertedRegions;
		wxCheckBox* m_cbAnchoring;
		wxStaticText* m_staticText139;
		wxComboBox* m_cIntegrator;
		wxTextCtrl* m_tBoat;
		wxButton* m_bBoatFilename;
		wxButton* m_bEditBoat;
		wxCheckBox* m_cbAllowDataDeficient;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateDate( wxDateEvent& event ) { event.Skip(); }
		virtual void OnGribTime( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCurrentTime( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void EnableSpin( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnAvoidCyclones( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnBoatFilename( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxDatePickerCtrl* m_dpStartDate;
		wxSpinCtrl* m_sFromDegree;
		wxSpinCtrl* m_sToDegree;
		wxTextCtrl* m_tByDegrees;
		wxPanel* m_pCyclones;
		wxCheckBox* m_cbCurrents;
		wxCheckBox* m_cbUseGrib;
		wxChoice* m_cClimatologyType;
		
		ConfigurationDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~ConfigurationDialogBase();
	
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
		wxStaticText* m_staticText135;
		wxStaticText* m_stVersion;
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
/// Class BoatDialogBase
///////////////////////////////////////////////////////////////////////////////
class BoatDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxFlexGridSizer* m_fgSizer;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel20;
		wxNotebook* m_nNotebook;
		wxPanel* m_plot;
		wxScrolledWindow* m_PlotWindow;
		wxPanel* m_panel10;
		wxScrolledWindow* m_CrossOverChart;
		wxStaticText* m_staticText137;
		wxSpinCtrl* m_sOverlapPercentage;
		wxStaticText* m_staticText138;
		wxPanel* m_panel24;
		wxStaticText* m_staticText125;
		wxStaticText* m_stBestCourseUpWindPortTack;
		wxStaticText* m_staticText1251;
		wxStaticText* m_stBestCourseUpWindStarboardTack;
		wxStaticText* m_staticText1252;
		wxStaticText* m_stBestCourseDownWindPortTack;
		wxStaticText* m_staticText12511;
		wxStaticText* m_stBestCourseDownWindStarboardTack;
		wxStaticText* m_staticText133;
		wxSpinCtrl* m_sVMGWindSpeed;
		wxChoice* m_cPlotType;
		wxChoice* m_cPlotVariable;
		wxCheckBox* m_cbFullPlot;
		wxCheckBox* m_cbOrientation;
		wxPanel* m_panel21;
		wxListCtrl* m_lPolars;
		wxButton* m_bUp;
		wxButton* m_bDown;
		wxButton* m_bEditPolar;
		wxButton* m_bAddPolar;
		wxButton* m_bRemovePolar;
		wxStaticLine* m_staticline1;
		wxButton* m_bOpenBoat;
		wxButton* m_bSaveBoat;
		wxButton* m_bSaveAsBoat;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMouseEventsPolarPlot( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPaintPlot( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnPaintCrossOverChart( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnOverlapPercentage( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnVMGWindSpeed( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdatePlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOrientation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPolarSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnUpPolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDownPolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditPolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddPolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpenBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveAsBoat( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxGauge* m_gCrossOverChart;
		
		BoatDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Boat"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~BoatDialogBase();
		
		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 400 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( BoatDialogBase::m_splitter2OnIdle ), NULL, this );
		}
	
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
		wxPanel* m_panel17;
		wxStaticText* m_staticText131;
		wxStaticText* m_staticText134;
		wxStaticText* m_staticText132;
		wxStaticText* m_staticText1341;
		wxStaticText* m_staticText133;
		wxStaticText* m_staticText1342;
		wxButton* m_button46;
		wxButton* m_button47;
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
		virtual void On100( wxCommandEvent& event ) { event.Skip(); }
		virtual void On80to120( wxCommandEvent& event ) { event.Skip(); }
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
		wxSpinCtrl* m_sWindStrengthMin;
		wxSpinCtrl* m_sWindStrengthMax;
		wxSpinCtrl* m_sWindStrengthStep;
		
		ConfigurationBatchDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Configuration Batch"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ConfigurationBatchDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NewPositionDialog
///////////////////////////////////////////////////////////////////////////////
class NewPositionDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText140;
		wxStaticText* m_staticText142;
		wxStaticText* m_staticText143;
		wxStaticText* m_staticText144;
		wxStaticText* m_staticText145;
		wxStaticText* m_staticText146;
		wxStaticText* m_staticText147;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4OK;
		wxButton* m_sdbSizer4Cancel;
	
	public:
		wxTextCtrl* m_tName;
		wxTextCtrl* m_tLatitudeDegrees;
		wxTextCtrl* m_tLatitudeMinutes;
		wxTextCtrl* m_tLongitudeDegrees;
		wxTextCtrl* m_tLongitudeMinutes;
		
		NewPositionDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("New Weather Routing Position"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~NewPositionDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CursorPositionDialog
///////////////////////////////////////////////////////////////////////////////
class CursorPositionDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText128;
		wxStaticText* m_staticText124;
		wxStaticText* m_staticText130;
		wxStaticText* m_staticText126;
		wxStaticText* m_staticText122;
		wxStdDialogButtonSizer* m_sdbSizer5;
		wxButton* m_sdbSizer5OK;
	
	public:
		wxStaticText* m_stPosition;
		wxStaticText* m_stPolar;
		wxStaticText* m_stSailChanges;
		wxStaticText* m_stTacks;
		wxStaticText* m_stWeatherData;
		
		CursorPositionDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Cursor Position"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~CursorPositionDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EditPolarDialogBase
///////////////////////////////////////////////////////////////////////////////
class EditPolarDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook6;
		wxPanel* m_panel19;
		wxGrid* m_gPolar;
		wxPanel* m_panel20;
		wxTextCtrl* m_tTrueWindAngle;
		wxListBox* m_lTrueWindAngles;
		wxButton* m_bAddTrueWindAngle;
		wxButton* m_bRemoveTrueWindAngle;
		wxTextCtrl* m_tTrueWindSpeed;
		wxListBox* m_lTrueWindSpeeds;
		wxButton* m_bAddTrueWindSpeed;
		wxButton* m_bRemoveTrueWindSpeed;
		wxPanel* m_panel21;
		wxNotebook* m_notebook61;
		wxPanel* m_panel22;
		wxRadioButton* m_rbApparentWind;
		wxRadioButton* m_rbTrueWind;
		wxStaticText* m_staticText133;
		wxTextCtrl* m_tWindSpeed;
		wxStaticText* m_staticText134;
		wxTextCtrl* m_tWindDirection;
		wxStaticText* m_staticText135;
		wxTextCtrl* m_tBoatSpeed;
		wxButton* m_button46;
		wxListCtrl* m_lMeasurements;
		wxButton* m_bRemoveMeasurement;
		wxButton* m_bRemoveAllMeasurements;
		wxButton* m_button48;
		wxButton* m_button50;
		wxPanel* m_panel23;
		wxPanel* m_panel17;
		wxStaticText* m_staticText139;
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
		wxPanel* m_panel221;
		wxGrid* m_grid1;
		wxButton* m_button42;
		wxButton* m_button43;
		wxButton* m_button44;
		wxButton* m_button45;
		wxButton* m_button461;
		wxStdDialogButtonSizer* m_sdbSizer6;
		wxButton* m_sdbSizer6Save;
		wxButton* m_sdbSizer6Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPolarGridChanged( wxGridEvent& event ) { event.Skip(); }
		virtual void d( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddTrueWindAngle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveTrueWindAngle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddTrueWindSpeed( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveTrueWindSpeed( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddMeasurement( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveMeasurement( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveAllMeasurements( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGeneratePolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRecompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRecomputeSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EditPolarDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Polar"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~EditPolarDialogBase();
	
};

#endif //__WEATHERROUTINGUI_H__
