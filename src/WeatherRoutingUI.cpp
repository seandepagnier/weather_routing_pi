///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WeatherRoutingUI.h"

///////////////////////////////////////////////////////////////////////////

WeatherRoutingDialogBase::WeatherRoutingDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Start") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Lat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer6->Add( m_staticText5, 0, wxALL, 5 );
	
	m_tStartLat = new wxTextCtrl( this, wxID_ANY, wxT("-16"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_tStartLat, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Lon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer6->Add( m_staticText6, 0, wxALL, 5 );
	
	m_tStartLon = new wxTextCtrl( this, wxID_ANY, wxT("166.5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_tStartLon, 0, wxALL, 5 );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer6->Add( m_staticText28, 0, wxALL, 5 );
	
	m_stStartDate = new wxStaticText( this, wxID_ANY, wxT("Start Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stStartDate->Wrap( -1 );
	fgSizer6->Add( m_stStartDate, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( this, wxID_ANY, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer6->Add( m_staticText30, 0, wxALL, 5 );
	
	m_stStartTime = new wxStaticText( this, wxID_ANY, wxT("Start Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stStartTime->Wrap( -1 );
	fgSizer6->Add( m_stStartTime, 0, wxALL, 5 );
	
	
	sbSizer3->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	
	fgSizer17->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("End") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Lat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer61->Add( m_staticText51, 0, wxALL, 5 );
	
	m_tEndLat = new wxTextCtrl( this, wxID_ANY, wxT("-35"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( m_tEndLat, 0, wxALL, 5 );
	
	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("Lon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer61->Add( m_staticText61, 0, wxALL, 5 );
	
	m_tEndLon = new wxTextCtrl( this, wxID_ANY, wxT("175"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( m_tEndLon, 0, wxALL, 5 );
	
	
	sbSizer5->Add( fgSizer61, 1, wxEXPAND, 5 );
	
	
	fgSizer17->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bCompute = new wxButton( this, wxID_ANY, wxT("Compute"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bCompute, 0, wxALL, 5 );
	
	m_bReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bReset, 0, wxALL, 5 );
	
	m_bSettings = new wxButton( this, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bSettings, 0, wxALL, 5 );
	
	m_bBoat = new wxButton( this, wxID_ANY, wxT("Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bBoat, 0, wxALL, 5 );
	
	m_bInformation = new wxButton( this, wxID_ANY, wxT("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bInformation, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer24, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer17 );
	this->Layout();
	fgSizer17->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_tEndLat->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_tEndLon->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_bCompute->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnCompute ), NULL, this );
	m_bReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnReset ), NULL, this );
	m_bSettings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnSettings ), NULL, this );
	m_bBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnBoat ), NULL, this );
	m_bInformation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnInformation ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnClose ), NULL, this );
}

WeatherRoutingDialogBase::~WeatherRoutingDialogBase()
{
	// Disconnect Events
	m_tEndLat->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_tEndLon->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_bCompute->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnCompute ), NULL, this );
	m_bReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnReset ), NULL, this );
	m_bSettings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnSettings ), NULL, this );
	m_bBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnBoat ), NULL, this );
	m_bInformation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnInformation ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnClose ), NULL, this );
	
}

WeatherRoutingSettingsDialogBase::WeatherRoutingSettingsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Degree Steps") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->AddGrowableCol( 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer22->AddGrowableCol( 0 );
	fgSizer22->AddGrowableRow( 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lDegreeSteps = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer22->Add( m_lDegreeSteps, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer20->Add( fgSizer22, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tDegreeStep = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_tDegreeStep, 0, wxALL, 5 );
	
	m_bAddDegreeStep = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bAddDegreeStep, 0, wxALL, 5 );
	
	m_bRemoveDegreeStep = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bRemoveDegreeStep, 0, wxALL, 5 );
	
	m_bGenerateDegreeSteps = new wxButton( this, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bGenerateDegreeSteps, 0, wxALL, 5 );
	
	m_bClearDegreeSteps = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bClearDegreeSteps, 0, wxALL, 5 );
	
	
	fgSizer20->Add( fgSizer21, 1, wxEXPAND, 5 );
	
	
	sbSizer7->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	
	fgSizer18->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxVERTICAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	m_staticText22 = new wxStaticText( this, wxID_ANY, wxT("Time Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer1->Add( m_staticText22, 0, wxALL, 5 );
	
	m_sTimeStep = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000, 3600 );
	fgSizer1->Add( m_sTimeStep, 0, wxALL, 5 );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, wxT("Max Diverted Course"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer1->Add( m_staticText20, 0, wxALL, 5 );
	
	m_sMaxDivertedCourse = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 180 );
	fgSizer1->Add( m_sMaxDivertedCourse, 0, wxALL, 5 );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, wxT("Max Wind Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer1->Add( m_staticText23, 0, wxALL, 5 );
	
	m_sMaxWindKnots = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 100 );
	fgSizer1->Add( m_sMaxWindKnots, 0, wxALL, 5 );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("Max Swell Meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer1->Add( m_staticText27, 0, wxALL, 5 );
	
	m_sMaxSwellMeters = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 20 );
	fgSizer1->Add( m_sMaxSwellMeters, 0, wxALL, 5 );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("Max Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	fgSizer1->Add( m_staticText26, 0, wxALL, 5 );
	
	m_sMaxLatitude = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 90, 90 );
	fgSizer1->Add( m_sMaxLatitude, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Tacking Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer1->Add( m_staticText24, 0, wxALL, 5 );
	
	m_sTackingTime = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 10 );
	fgSizer1->Add( m_sTackingTime, 0, wxALL, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Substeps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer1->Add( m_staticText25, 0, wxALL, 5 );
	
	m_sSubSteps = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_sSubSteps->Enable( false );
	
	fgSizer1->Add( m_sSubSteps, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer23->SetFlexibleDirection( wxBOTH );
	fgSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbDetectLand = new wxCheckBox( this, wxID_ANY, wxT("Detect Land"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbDetectLand->SetValue(true); 
	fgSizer23->Add( m_cbDetectLand, 0, wxALL, 5 );
	
	m_cbInvertedRegions = new wxCheckBox( this, wxID_ANY, wxT("Inverted Regions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbInvertedRegions->SetValue(true); 
	fgSizer23->Add( m_cbInvertedRegions, 0, wxALL, 5 );
	
	m_cbAnchoring = new wxCheckBox( this, wxID_ANY, wxT("Anchoring"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_cbAnchoring, 0, wxALL, 5 );
	
	m_cbAllowDataDeficient = new wxCheckBox( this, wxID_ANY, wxT("Allow Data Deficient"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_cbAllowDataDeficient, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer23, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	fgSizer18->Add( m_sdbSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer18 );
	this->Layout();
	fgSizer18->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bAddDegreeStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnAddDegreeStep ), NULL, this );
	m_bRemoveDegreeStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnRemoveDegreeStep ), NULL, this );
	m_bGenerateDegreeSteps->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnGenerateDegreeSteps ), NULL, this );
	m_bClearDegreeSteps->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnClearDegreeSteps ), NULL, this );
}

WeatherRoutingSettingsDialogBase::~WeatherRoutingSettingsDialogBase()
{
	// Disconnect Events
	m_bAddDegreeStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnAddDegreeStep ), NULL, this );
	m_bRemoveDegreeStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnRemoveDegreeStep ), NULL, this );
	m_bGenerateDegreeSteps->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnGenerateDegreeSteps ), NULL, this );
	m_bClearDegreeSteps->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingSettingsDialogBase::OnClearDegreeSteps ), NULL, this );
	
}

BoatDialogBase::BoatDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_PlotWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_PlotWindow->SetScrollRate( 5, 5 );
	fgSizer3->Add( m_PlotWindow, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Plot Type") ), wxHORIZONTAL );
	
	wxString m_cPlotTypeChoices[] = { wxT("Polar"), wxT("Rect") };
	int m_cPlotTypeNChoices = sizeof( m_cPlotTypeChoices ) / sizeof( wxString );
	m_cPlotType = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cPlotTypeNChoices, m_cPlotTypeChoices, 0 );
	m_cPlotType->SetSelection( 0 );
	m_cPlotType->Enable( false );
	
	sbSizer7->Add( m_cPlotType, 0, wxALL, 5 );
	
	wxString m_cPlotAxisChoices[] = { wxT("Boat"), wxT("True Wind"), wxT("Apparent Wind") };
	int m_cPlotAxisNChoices = sizeof( m_cPlotAxisChoices ) / sizeof( wxString );
	m_cPlotAxis = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cPlotAxisNChoices, m_cPlotAxisChoices, 0 );
	m_cPlotAxis->SetSelection( 0 );
	sbSizer7->Add( m_cPlotAxis, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Wind Speed") ), wxVERTICAL );
	
	m_sWindSpeed = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 10 );
	sbSizer8->Add( m_sWindSpeed, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer8, 1, wxEXPAND, 5 );
	
	
	fgSizer18->Add( fgSizer24, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer23->SetFlexibleDirection( wxBOTH );
	fgSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("True Wind") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stTrueWindAngle = new wxStaticText( m_panel3, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stTrueWindAngle->Wrap( -1 );
	fgSizer20->Add( m_stTrueWindAngle, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( m_panel3, wxID_ANY, wxT("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer20->Add( m_staticText21, 0, wxALL, 5 );
	
	m_stTrueWindKnots = new wxStaticText( m_panel3, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stTrueWindKnots->Wrap( -1 );
	fgSizer20->Add( m_stTrueWindKnots, 0, wxALL, 5 );
	
	m_staticText23 = new wxStaticText( m_panel3, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer20->Add( m_staticText23, 0, wxALL, 5 );
	
	
	sbSizer6->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Apparent Wind") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer201;
	fgSizer201 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer201->SetFlexibleDirection( wxBOTH );
	fgSizer201->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stApparentWindAngle = new wxStaticText( m_panel3, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stApparentWindAngle->Wrap( -1 );
	fgSizer201->Add( m_stApparentWindAngle, 0, wxALL, 5 );
	
	m_staticText211 = new wxStaticText( m_panel3, wxID_ANY, wxT("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer201->Add( m_staticText211, 0, wxALL, 5 );
	
	m_stApparentWindKnots = new wxStaticText( m_panel3, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stApparentWindKnots->Wrap( -1 );
	fgSizer201->Add( m_stApparentWindKnots, 0, wxALL, 5 );
	
	m_staticText231 = new wxStaticText( m_panel3, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText231->Wrap( -1 );
	fgSizer201->Add( m_staticText231, 0, wxALL, 5 );
	
	
	sbSizer61->Add( fgSizer201, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( sbSizer61, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer62;
	sbSizer62 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Boat") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer202;
	fgSizer202 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer202->SetFlexibleDirection( wxBOTH );
	fgSizer202->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stBoatAngle = new wxStaticText( m_panel3, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stBoatAngle->Wrap( -1 );
	fgSizer202->Add( m_stBoatAngle, 0, wxALL, 5 );
	
	m_staticText212 = new wxStaticText( m_panel3, wxID_ANY, wxT("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText212->Wrap( -1 );
	fgSizer202->Add( m_staticText212, 0, wxALL, 5 );
	
	m_stBoatKnots = new wxStaticText( m_panel3, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stBoatKnots->Wrap( -1 );
	fgSizer202->Add( m_stBoatKnots, 0, wxALL, 5 );
	
	m_staticText232 = new wxStaticText( m_panel3, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText232->Wrap( -1 );
	fgSizer202->Add( m_staticText232, 0, wxALL, 5 );
	
	
	sbSizer62->Add( fgSizer202, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( sbSizer62, 1, wxEXPAND, 5 );
	
	
	fgSizer18->Add( fgSizer23, 1, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( fgSizer18 );
	m_panel3->Layout();
	fgSizer18->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Plot Details"), false );
	m_panel31 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer181;
	fgSizer181 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer181->SetFlexibleDirection( wxBOTH );
	fgSizer181->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOpen = new wxButton( m_panel31, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bOpen, 0, wxALL, 5 );
	
	m_bSave = new wxButton( m_panel31, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bSave, 0, wxALL, 5 );
	
	m_bClose = new wxButton( m_panel31, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer181->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	
	m_panel31->SetSizer( fgSizer181 );
	m_panel31->Layout();
	fgSizer181->Fit( m_panel31 );
	m_notebook1->AddPage( m_panel31, wxT("Polar File"), false );
	m_panel5 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText12 = new wxStaticText( m_panel5, wxID_ANY, wxT("Eta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer10->Add( m_staticText12, 0, wxALL, 5 );
	
	m_sEta = new wxSlider( m_panel5, wxID_ANY, 127, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sEta, 0, wxEXPAND, 5 );
	
	m_staticText57 = new wxStaticText( m_panel5, wxID_ANY, wxT("lwl ft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer10->Add( m_staticText57, 0, wxALL, 5 );
	
	m_sLWL = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 24 );
	fgSizer10->Add( m_sLWL, 0, wxALL, 5 );
	
	m_staticText35 = new wxStaticText( m_panel5, wxID_ANY, wxT("Hull Drag"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer10->Add( m_staticText35, 0, wxALL, 5 );
	
	m_sHullDrag = new wxSlider( m_panel5, wxID_ANY, 11, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sHullDrag, 0, wxEXPAND, 5 );
	
	m_staticText58 = new wxStaticText( m_panel5, wxID_ANY, wxT("disp lbs"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer10->Add( m_staticText58, 0, wxALL, 5 );
	
	m_sDisplacement = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100000, 8000 );
	fgSizer10->Add( m_sDisplacement, 0, wxALL, 5 );
	
	m_staticText13 = new wxStaticText( m_panel5, wxID_ANY, wxT("Keel Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer10->Add( m_staticText13, 0, wxALL, 5 );
	
	m_sKeelPressure = new wxSlider( m_panel5, wxID_ANY, 30, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sKeelPressure, 0, wxEXPAND, 5 );
	
	m_staticText60 = new wxStaticText( m_panel5, wxID_ANY, wxT("plane"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer10->Add( m_staticText60, 0, wxALL, 5 );
	
	m_sPlaningConstant = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer10->Add( m_sPlaningConstant, 0, wxALL, 5 );
	
	m_staticText14 = new wxStaticText( m_panel5, wxID_ANY, wxT("Keel Lift"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer10->Add( m_staticText14, 0, wxALL, 5 );
	
	m_sKeelLift = new wxSlider( m_panel5, wxID_ANY, 10, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sKeelLift, 0, wxEXPAND, 5 );
	
	
	fgSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_bOptimizeTacking = new wxButton( m_panel5, wxID_ANY, wxT("Optimize Tacking"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_bOptimizeTacking, 0, wxALL, 5 );
	
	
	m_panel5->SetSizer( fgSizer10 );
	m_panel5->Layout();
	fgSizer10->Fit( m_panel5 );
	m_notebook1->AddPage( m_panel5, wxT("Compute"), true );
	
	fgSizer12->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer12->Add( fgSizer16, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( fgSizer12, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_PlotWindow->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MOTION, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_RIGHT_DCLICK, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_PAINT, wxPaintEventHandler( BoatDialogBase::OnPaintPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_SIZE, wxSizeEventHandler( BoatDialogBase::OnSizePlot ), NULL, this );
	m_cPlotType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BoatDialogBase::OnUpdatePlot ), NULL, this );
	m_cPlotAxis->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BoatDialogBase::OnUpdatePlot ), NULL, this );
	m_sWindSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdatePlot ), NULL, this );
	m_bOpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnClose ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sLWL->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sDisplacement->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sPlaningConstant->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_bOptimizeTacking->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOptimizeTacking ), NULL, this );
}

BoatDialogBase::~BoatDialogBase()
{
	// Disconnect Events
	m_PlotWindow->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MIDDLE_UP, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MOTION, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_RIGHT_DCLICK, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( BoatDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( BoatDialogBase::OnPaintPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_SIZE, wxSizeEventHandler( BoatDialogBase::OnSizePlot ), NULL, this );
	m_cPlotType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BoatDialogBase::OnUpdatePlot ), NULL, this );
	m_cPlotAxis->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BoatDialogBase::OnUpdatePlot ), NULL, this );
	m_sWindSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdatePlot ), NULL, this );
	m_bOpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnClose ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sLWL->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sHullDrag->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sDisplacement->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sPlaningConstant->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelLift->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_bOptimizeTacking->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOptimizeTacking ), NULL, this );
	
}

InformationDialog::InformationDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer15->AddGrowableCol( 0 );
	fgSizer15->AddGrowableRow( 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_htmlInformation = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	fgSizer15->Add( m_htmlInformation, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	
	fgSizer15->Add( m_sdbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer15 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

InformationDialog::~InformationDialog()
{
}
