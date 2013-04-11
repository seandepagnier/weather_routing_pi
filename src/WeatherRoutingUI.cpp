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
	fgSizer17 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
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
	
	m_tStartLon = new wxTextCtrl( this, wxID_ANY, wxT("166.5"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
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
	
	m_tEndLat = new wxTextCtrl( this, wxID_ANY, wxT("-35"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	fgSizer61->Add( m_tEndLat, 0, wxALL, 5 );
	
	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("Lon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer61->Add( m_staticText61, 0, wxALL, 5 );
	
	m_tEndLon = new wxTextCtrl( this, wxID_ANY, wxT("175"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	fgSizer61->Add( m_tEndLon, 0, wxALL, 5 );
	
	
	sbSizer5->Add( fgSizer61, 1, wxEXPAND, 5 );
	
	
	fgSizer17->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bCompute = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bCompute, 0, wxALL, 5 );
	
	m_bReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bReset, 0, wxALL, 5 );
	
	m_bStatistics = new wxButton( this, wxID_ANY, wxT("Statistics"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bStatistics, 0, wxALL, 5 );
	
	m_bConfiguration = new wxButton( this, wxID_ANY, wxT("Configuration"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bConfiguration, 0, wxALL, 5 );
	
	m_bSettings = new wxButton( this, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bSettings, 0, wxALL, 5 );
	
	m_bBoat = new wxButton( this, wxID_ANY, wxT("Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bBoat, 0, wxALL, 5 );
	
	m_bPlot = new wxButton( this, wxID_ANY, wxT("Plot"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bPlot, 0, wxALL, 5 );
	
	m_bExport = new wxButton( this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bExport, 0, wxALL, 5 );
	
	m_bInformation = new wxButton( this, wxID_ANY, wxT("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_bInformation, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer24, 0, 0, 5 );
	
	
	this->SetSizer( fgSizer17 );
	this->Layout();
	fgSizer17->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_tEndLat->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_tEndLon->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_bCompute->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnCompute ), NULL, this );
	m_bReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnReset ), NULL, this );
	m_bStatistics->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnStatistics ), NULL, this );
	m_bConfiguration->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnConfiguration ), NULL, this );
	m_bSettings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnSettings ), NULL, this );
	m_bBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnBoat ), NULL, this );
	m_bPlot->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnPlot ), NULL, this );
	m_bExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnExport ), NULL, this );
	m_bInformation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnInformation ), NULL, this );
}

WeatherRoutingDialogBase::~WeatherRoutingDialogBase()
{
	// Disconnect Events
	m_tEndLat->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_tEndLon->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WeatherRoutingDialogBase::OnUpdateEnd ), NULL, this );
	m_bCompute->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnCompute ), NULL, this );
	m_bReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnReset ), NULL, this );
	m_bStatistics->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnStatistics ), NULL, this );
	m_bConfiguration->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnConfiguration ), NULL, this );
	m_bSettings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnSettings ), NULL, this );
	m_bBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnBoat ), NULL, this );
	m_bPlot->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnPlot ), NULL, this );
	m_bExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnExport ), NULL, this );
	m_bInformation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnInformation ), NULL, this );
	
}

ConfigurationDialogBase::ConfigurationDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	fgSizer20->AddGrowableRow( 0 );
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
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
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
	m_bAddDegreeStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnAddDegreeStep ), NULL, this );
	m_bRemoveDegreeStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnRemoveDegreeStep ), NULL, this );
	m_bGenerateDegreeSteps->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnGenerateDegreeSteps ), NULL, this );
	m_bClearDegreeSteps->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnClearDegreeSteps ), NULL, this );
}

ConfigurationDialogBase::~ConfigurationDialogBase()
{
	// Disconnect Events
	m_bAddDegreeStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnAddDegreeStep ), NULL, this );
	m_bRemoveDegreeStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnRemoveDegreeStep ), NULL, this );
	m_bGenerateDegreeSteps->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnGenerateDegreeSteps ), NULL, this );
	m_bClearDegreeSteps->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnClearDegreeSteps ), NULL, this );
	
}

SettingsDialogBase::SettingsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,290 ), wxDefaultSize );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText74 = new wxStaticText( this, wxID_ANY, wxT("Cursor Route Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer42->Add( m_staticText74, 0, wxALL, 5 );
	
	m_cpCursorRoute = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_cpCursorRoute, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( this, wxID_ANY, wxT("Destination Route Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer42->Add( m_staticText73, 0, wxALL, 5 );
	
	m_cpDestinationRoute = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_cpDestinationRoute, 0, wxALL, 5 );
	
	m_staticText75 = new wxStaticText( this, wxID_ANY, wxT("Route Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer42->Add( m_staticText75, 0, wxALL, 5 );
	
	m_sRouteThickness = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 4 );
	fgSizer42->Add( m_sRouteThickness, 0, wxALL, 5 );
	
	m_staticText70 = new wxStaticText( this, wxID_ANY, wxT("Iso Chron Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText70->Wrap( -1 );
	fgSizer42->Add( m_staticText70, 0, wxALL, 5 );
	
	m_sIsoChronThickness = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
	fgSizer42->Add( m_sIsoChronThickness, 0, wxALL, 5 );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, wxT("Alternate Routes Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer42->Add( m_staticText71, 0, wxALL, 5 );
	
	m_sAlternateRouteThickness = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer42->Add( m_sAlternateRouteThickness, 0, wxALL, 5 );
	
	m_cbAlternatesForAll = new wxCheckBox( this, wxID_ANY, wxT("Alternates for all IsoChrons"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer42->Add( m_cbAlternatesForAll, 0, wxALL, 5 );
	
	
	fgSizer42->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_cbSquaresAtSailChanges = new wxCheckBox( this, wxID_ANY, wxT("Squares at Sail Changes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSquaresAtSailChanges->SetValue(true); 
	fgSizer42->Add( m_cbSquaresAtSailChanges, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer42, 1, wxEXPAND, 5 );
	
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
}

SettingsDialogBase::~SettingsDialogBase()
{
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
	fgSizer12 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer12->AddGrowableCol( 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 4, 0, 0 );
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
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Wind Kts") ), wxVERTICAL );
	
	m_sWindSpeed = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 1, 59, 15 );
	sbSizer8->Add( m_sWindSpeed, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer8, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Sig Wv Ht") ), wxVERTICAL );
	
	m_spinCtrl20 = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 10, 0 );
	m_spinCtrl20->Enable( false );
	
	sbSizer11->Add( m_spinCtrl20, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer11, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Wv Period") ), wxVERTICAL );
	
	m_spinCtrl21 = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 10, 0 );
	m_spinCtrl21->Enable( false );
	
	sbSizer12->Add( m_spinCtrl21, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer12, 1, wxEXPAND, 5 );
	
	
	fgSizer18->Add( fgSizer24, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 4, 0, 0 );
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
	
	m_bClose = new wxButton( m_panel3, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer23, 1, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( fgSizer18 );
	m_panel3->Layout();
	fgSizer18->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Plot Details"), true );
	m_panel31 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer181;
	fgSizer181 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer181->AddGrowableCol( 0 );
	fgSizer181->SetFlexibleDirection( wxBOTH );
	fgSizer181->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel31, wxID_ANY, wxT("CSV Settings") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer34;
	fgSizer34 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer34->SetFlexibleDirection( wxBOTH );
	fgSizer34->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer54;
	fgSizer54 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer54->AddGrowableCol( 1 );
	fgSizer54->SetFlexibleDirection( wxBOTH );
	fgSizer54->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText93 = new wxStaticText( m_panel31, wxID_ANY, wxT("FileName"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText93->Wrap( -1 );
	fgSizer54->Add( m_staticText93, 0, wxALL, 5 );
	
	m_stCSVFile = new wxStaticText( m_panel31, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stCSVFile->Wrap( -1 );
	fgSizer54->Add( m_stCSVFile, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer34->Add( fgSizer54, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer241;
	fgSizer241 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer241->SetFlexibleDirection( wxBOTH );
	fgSizer241->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText351 = new wxStaticText( m_panel31, wxID_ANY, wxT("Wind Speed Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText351->Wrap( -1 );
	fgSizer241->Add( m_staticText351, 0, wxALL, 5 );
	
	m_sFileCSVWindSpeedStep = new wxSpinCtrl( m_panel31, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20, 5 );
	fgSizer241->Add( m_sFileCSVWindSpeedStep, 0, wxALL, 5 );
	
	m_staticText36 = new wxStaticText( m_panel31, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer241->Add( m_staticText36, 0, wxALL, 5 );
	
	m_staticText37 = new wxStaticText( m_panel31, wxID_ANY, wxT("Wind Degree Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer241->Add( m_staticText37, 0, wxALL, 5 );
	
	m_sFileCSVWindDegreeStep = new wxSpinCtrl( m_panel31, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 30, 10 );
	fgSizer241->Add( m_sFileCSVWindDegreeStep, 0, wxALL, 5 );
	
	m_staticText38 = new wxStaticText( m_panel31, wxID_ANY, wxT("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer241->Add( m_staticText38, 0, wxALL, 5 );
	
	
	fgSizer34->Add( fgSizer241, 1, wxEXPAND, 5 );
	
	
	sbSizer9->Add( fgSizer34, 1, wxEXPAND, 5 );
	
	
	fgSizer181->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer142;
	fgSizer142 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer142->SetFlexibleDirection( wxBOTH );
	fgSizer142->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOpen = new wxButton( m_panel31, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_bOpen, 0, wxALL, 5 );
	
	m_bSave = new wxButton( m_panel31, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_bSave, 0, wxALL, 5 );
	
	
	fgSizer181->Add( fgSizer142, 1, wxEXPAND, 5 );
	
	
	m_panel31->SetSizer( fgSizer181 );
	m_panel31->Layout();
	fgSizer181->Fit( m_panel31 );
	m_notebook1->AddPage( m_panel31, wxT("Polar File"), false );
	m_panel311 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1811;
	fgSizer1811 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1811->SetFlexibleDirection( wxBOTH );
	fgSizer1811->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer2411;
	fgSizer2411 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2411->SetFlexibleDirection( wxBOTH );
	fgSizer2411->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3511 = new wxStaticText( m_panel311, wxID_ANY, wxT("Best Course Up Wind Port Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3511->Wrap( -1 );
	fgSizer2411->Add( m_staticText3511, 0, wxALL, 5 );
	
	m_stBestCourseUpWindPortTack = new wxStaticText( m_panel311, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseUpWindPortTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseUpWindPortTack, 0, wxALL, 5 );
	
	m_staticText35111 = new wxStaticText( m_panel311, wxID_ANY, wxT("Best Course Up Wind Starboard Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35111->Wrap( -1 );
	fgSizer2411->Add( m_staticText35111, 0, wxALL, 5 );
	
	m_stBestCourseUpWindStarboardTack = new wxStaticText( m_panel311, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseUpWindStarboardTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseUpWindStarboardTack, 0, wxALL, 5 );
	
	m_staticText35112 = new wxStaticText( m_panel311, wxID_ANY, wxT("Best Course Down Wind Port Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35112->Wrap( -1 );
	fgSizer2411->Add( m_staticText35112, 0, wxALL, 5 );
	
	m_stBestCourseDownWindPortTack = new wxStaticText( m_panel311, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseDownWindPortTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseDownWindPortTack, 0, wxALL, 5 );
	
	m_staticText35113 = new wxStaticText( m_panel311, wxID_ANY, wxT("Best Course Down Wind Starboard Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35113->Wrap( -1 );
	fgSizer2411->Add( m_staticText35113, 0, wxALL, 5 );
	
	m_stBestCourseDownWindStarboardTack = new wxStaticText( m_panel311, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseDownWindStarboardTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseDownWindStarboardTack, 0, wxALL, 5 );
	
	
	fgSizer1811->Add( fgSizer2411, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer141;
	fgSizer141 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer141->SetFlexibleDirection( wxBOTH );
	fgSizer141->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOptimizeTacking = new wxButton( m_panel311, wxID_ANY, wxT("Optimize Tacking"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer141->Add( m_bOptimizeTacking, 0, wxALL, 5 );
	
	m_bResetOptimizeTacking = new wxButton( m_panel311, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer141->Add( m_bResetOptimizeTacking, 0, wxALL, 5 );
	
	
	fgSizer1811->Add( fgSizer141, 1, wxEXPAND, 5 );
	
	
	m_panel311->SetSizer( fgSizer1811 );
	m_panel311->Layout();
	fgSizer1811->Fit( m_panel311 );
	m_notebook1->AddPage( m_panel311, wxT("VMG"), false );
	m_panel3111 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer46;
	fgSizer46 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer46->SetFlexibleDirection( wxBOTH );
	fgSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer58;
	fgSizer58 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer58->SetFlexibleDirection( wxBOTH );
	fgSizer58->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText100 = new wxStaticText( m_panel3111, wxID_ANY, wxT("type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	fgSizer58->Add( m_staticText100, 0, wxALL, 5 );
	
	wxString m_cHullTypeChoices[] = { wxT("Mono"), wxT("Catamaran"), wxT("Trimaran"), wxT("Proa"), wxT("Submarine") };
	int m_cHullTypeNChoices = sizeof( m_cHullTypeChoices ) / sizeof( wxString );
	m_cHullType = new wxChoice( m_panel3111, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), m_cHullTypeNChoices, m_cHullTypeChoices, 0 );
	m_cHullType->SetSelection( 0 );
	fgSizer58->Add( m_cHullType, 0, wxALL, 5 );
	
	m_staticText58 = new wxStaticText( m_panel3111, wxID_ANY, wxT("disp tons"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer58->Add( m_staticText58, 0, wxALL, 5 );
	
	m_sDisplacement = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 100000, 4 );
	fgSizer58->Add( m_sDisplacement, 0, wxALL, 5 );
	
	m_staticText57 = new wxStaticText( m_panel3111, wxID_ANY, wxT("lwl ft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer58->Add( m_staticText57, 0, wxALL, 5 );
	
	m_sLWL = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 24 );
	fgSizer58->Add( m_sLWL, 0, wxALL, 5 );
	
	
	fgSizer46->Add( fgSizer58, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer14;
	sbSizer14 = new wxStaticBoxSizer( new wxStaticBox( m_panel3111, wxID_ANY, wxT("Drag") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer57;
	fgSizer57 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer57->SetFlexibleDirection( wxBOTH );
	fgSizer57->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText35 = new wxStaticText( m_panel3111, wxID_ANY, wxT("Frictional"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer57->Add( m_staticText35, 0, wxALL, 5 );
	
	m_sFrictionalDrag = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer57->Add( m_sFrictionalDrag, 0, wxALL, 5 );
	
	m_staticText112 = new wxStaticText( m_panel3111, wxID_ANY, wxT("Wake"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText112->Wrap( -1 );
	fgSizer57->Add( m_staticText112, 0, wxALL, 5 );
	
	m_sWakeDrag = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer57->Add( m_sWakeDrag, 0, wxALL, 5 );
	
	m_bRecomputeDrag = new wxButton( m_panel3111, wxID_ANY, wxT("Recompute"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer57->Add( m_bRecomputeDrag, 0, wxALL, 5 );
	
	m_bDragInfo = new wxButton( m_panel3111, wxID_ANY, wxT("Info"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer57->Add( m_bDragInfo, 0, wxALL, 5 );
	
	
	sbSizer14->Add( fgSizer57, 1, wxEXPAND, 5 );
	
	
	fgSizer46->Add( sbSizer14, 1, wxEXPAND, 5 );
	
	
	m_panel3111->SetSizer( fgSizer46 );
	m_panel3111->Layout();
	fgSizer46->Fit( m_panel3111 );
	m_notebook1->AddPage( m_panel3111, wxT("Hull"), false );
	m_panel311111 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1811111;
	fgSizer1811111 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer1811111->AddGrowableCol( 0 );
	fgSizer1811111->AddGrowableRow( 0 );
	fgSizer1811111->SetFlexibleDirection( wxBOTH );
	fgSizer1811111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lBoatPlans = new wxListCtrl( m_panel311111, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	fgSizer1811111->Add( m_lBoatPlans, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer49;
	fgSizer49 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer49->AddGrowableCol( 1 );
	fgSizer49->AddGrowableRow( 0 );
	fgSizer49->SetFlexibleDirection( wxBOTH );
	fgSizer49->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer53;
	fgSizer53 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer53->AddGrowableCol( 1 );
	fgSizer53->SetFlexibleDirection( wxBOTH );
	fgSizer53->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText961 = new wxStaticText( m_panel311111, wxID_ANY, wxT("Eta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText961->Wrap( -1 );
	fgSizer53->Add( m_staticText961, 0, wxALL, 5 );
	
	m_sEta = new wxSlider( m_panel311111, wxID_ANY, 400, 1, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer53->Add( m_sEta, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer49->Add( fgSizer53, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer541;
	fgSizer541 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer541->SetFlexibleDirection( wxBOTH );
	fgSizer541->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText91 = new wxStaticText( m_panel311111, wxID_ANY, wxT("Max Point Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	fgSizer541->Add( m_staticText91, 0, wxALL, 5 );
	
	m_sLuffAngle = new wxSpinCtrl( m_panel311111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 180, 15 );
	fgSizer541->Add( m_sLuffAngle, 0, wxALL, 5 );
	
	
	fgSizer49->Add( fgSizer541, 1, wxEXPAND, 5 );
	
	
	fgSizer1811111->Add( fgSizer49, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer40;
	fgSizer40 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer40->SetFlexibleDirection( wxBOTH );
	fgSizer40->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bNewBoatPlan = new wxButton( m_panel311111, wxID_ANY, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_bNewBoatPlan, 0, wxALL, 5 );
	
	m_bEditBoatPlan = new wxButton( m_panel311111, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_bEditBoatPlan, 0, wxALL, 5 );
	
	m_bDeleteBoatPlan = new wxButton( m_panel311111, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_bDeleteBoatPlan, 0, wxALL, 5 );
	
	
	fgSizer1811111->Add( fgSizer40, 1, wxEXPAND, 5 );
	
	
	m_panel311111->SetSizer( fgSizer1811111 );
	m_panel311111->Layout();
	fgSizer1811111->Fit( m_panel311111 );
	m_notebook1->AddPage( m_panel311111, wxT("Sail Plan"), false );
	m_panel5 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer59;
	fgSizer59 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer59->SetFlexibleDirection( wxBOTH );
	fgSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText113 = new wxStaticText( m_panel5, wxID_ANY, wxT("beam"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113->Wrap( -1 );
	fgSizer59->Add( m_staticText113, 0, wxALL, 5 );
	
	m_sBeam = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100, 8 );
	fgSizer59->Add( m_sBeam, 0, wxALL, 5 );
	
	m_staticText109 = new wxStaticText( m_panel5, wxID_ANY, wxT("loa ft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText109->Wrap( -1 );
	fgSizer59->Add( m_staticText109, 0, wxALL, 5 );
	
	m_sLOA = new wxSpinCtrl( m_panel5, wxID_ANY, wxT("27"), wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0 );
	fgSizer59->Add( m_sLOA, 0, wxALL, 5 );
	
	
	fgSizer52->Add( fgSizer59, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText92 = new wxStaticText( m_panel5, wxID_ANY, wxT("Hull Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	fgSizer43->Add( m_staticText92, 0, wxALL, 5 );
	
	m_stHullSpeed = new wxStaticText( m_panel5, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stHullSpeed->Wrap( -1 );
	fgSizer43->Add( m_stHullSpeed, 0, wxALL, 5 );
	
	m_staticText94 = new wxStaticText( m_panel5, wxID_ANY, wxT("Capsize Risk"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText94->Wrap( -1 );
	fgSizer43->Add( m_staticText94, 0, wxALL, 5 );
	
	m_stCapsizeRisk = new wxStaticText( m_panel5, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stCapsizeRisk->Wrap( -1 );
	fgSizer43->Add( m_stCapsizeRisk, 0, wxALL, 5 );
	
	m_staticText96 = new wxStaticText( m_panel5, wxID_ANY, wxT("Comfort Factor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	fgSizer43->Add( m_staticText96, 0, wxALL, 5 );
	
	m_stComfortFactor = new wxStaticText( m_panel5, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stComfortFactor->Wrap( -1 );
	fgSizer43->Add( m_stComfortFactor, 0, wxALL, 5 );
	
	m_staticText105 = new wxStaticText( m_panel5, wxID_ANY, wxT("Displacement Length Ratio"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText105->Wrap( -1 );
	fgSizer43->Add( m_staticText105, 0, wxALL, 5 );
	
	m_stDisplacementLengthRatio = new wxStaticText( m_panel5, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stDisplacementLengthRatio->Wrap( -1 );
	fgSizer43->Add( m_stDisplacementLengthRatio, 0, wxALL, 5 );
	
	
	fgSizer52->Add( fgSizer43, 1, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( fgSizer52 );
	m_panel5->Layout();
	fgSizer52->Fit( m_panel5 );
	m_notebook1->AddPage( m_panel5, wxT("Statistics"), false );
	
	fgSizer12->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	
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
	m_sWindSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateWindSpeed ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnClose ), NULL, this );
	m_bOpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_bOptimizeTacking->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOptimizeTacking ), NULL, this );
	m_bResetOptimizeTacking->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnResetOptimalTackingSpeed ), NULL, this );
	m_cHullType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	m_sDisplacement->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	m_sLWL->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sFrictionalDrag->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sWakeDrag->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_bRecomputeDrag->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnRecomputeDrag ), NULL, this );
	m_bDragInfo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnDragInfo ), NULL, this );
	m_lBoatPlans->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( BoatDialogBase::OnSailPlanSelected ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sLuffAngle->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_bNewBoatPlan->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnNewBoatPlan ), NULL, this );
	m_bEditBoatPlan->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnEditBoatPlan ), NULL, this );
	m_bDeleteBoatPlan->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnDeleteBoatPlan ), NULL, this );
	m_sBeam->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	m_sLOA->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
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
	m_sWindSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateWindSpeed ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnClose ), NULL, this );
	m_bOpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_bOptimizeTacking->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOptimizeTacking ), NULL, this );
	m_bResetOptimizeTacking->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnResetOptimalTackingSpeed ), NULL, this );
	m_cHullType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	m_sDisplacement->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	m_sLWL->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sFrictionalDrag->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sWakeDrag->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_bRecomputeDrag->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnRecomputeDrag ), NULL, this );
	m_bDragInfo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnDragInfo ), NULL, this );
	m_lBoatPlans->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( BoatDialogBase::OnSailPlanSelected ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnEta ), NULL, this );
	m_sLuffAngle->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_bNewBoatPlan->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnNewBoatPlan ), NULL, this );
	m_bEditBoatPlan->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnEditBoatPlan ), NULL, this );
	m_bDeleteBoatPlan->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnDeleteBoatPlan ), NULL, this );
	m_sBeam->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	m_sLOA->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BoatDialogBase::OnUpdateStatistics ), NULL, this );
	
}

BoatPlanDialogBase::BoatPlanDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer41->AddGrowableCol( 0 );
	fgSizer41->AddGrowableRow( 1 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer44;
	fgSizer44 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer44->AddGrowableCol( 1 );
	fgSizer44->SetFlexibleDirection( wxBOTH );
	fgSizer44->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText87 = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText87->Wrap( -1 );
	fgSizer44->Add( m_staticText87, 0, wxALL, 5 );
	
	m_tBoatPlanName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer44->Add( m_tBoatPlanName, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer41->Add( fgSizer44, 1, wxEXPAND, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel10 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer45->AddGrowableCol( 0 );
	fgSizer45->AddGrowableRow( 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lSwitchPlans = new wxListBox( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	fgSizer45->Add( m_lSwitchPlans, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer46;
	fgSizer46 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer46->SetFlexibleDirection( wxBOTH );
	fgSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bNewSwitchPlanRule = new wxButton( m_panel10, wxID_ANY, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer46->Add( m_bNewSwitchPlanRule, 0, wxALL, 5 );
	
	m_bEditSwitchBoatPlan = new wxButton( m_panel10, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer46->Add( m_bEditSwitchBoatPlan, 0, wxALL, 5 );
	
	m_bDeleteSwitchBoatPlan = new wxButton( m_panel10, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer46->Add( m_bDeleteSwitchBoatPlan, 0, wxALL, 5 );
	
	
	fgSizer45->Add( fgSizer46, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( fgSizer45 );
	m_panel10->Layout();
	fgSizer45->Fit( m_panel10 );
	m_notebook2->AddPage( m_panel10, wxT("Switch Plan Rules"), false );
	
	fgSizer41->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bDone = new wxButton( this, wxID_ANY, wxT("Done"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_bDone, 0, wxALL, 5 );
	
	
	fgSizer41->Add( fgSizer52, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer41 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bNewSwitchPlanRule->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnNewSwitchPlanRule ), NULL, this );
	m_bEditSwitchBoatPlan->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnEditSwitchPlanRule ), NULL, this );
	m_bDeleteSwitchBoatPlan->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnDeleteSwitchPlanRule ), NULL, this );
	m_bDone->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnDone ), NULL, this );
}

BoatPlanDialogBase::~BoatPlanDialogBase()
{
	// Disconnect Events
	m_bNewSwitchPlanRule->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnNewSwitchPlanRule ), NULL, this );
	m_bEditSwitchBoatPlan->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnEditSwitchPlanRule ), NULL, this );
	m_bDeleteSwitchBoatPlan->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnDeleteSwitchPlanRule ), NULL, this );
	m_bDone->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatPlanDialogBase::OnDone ), NULL, this );
	
}

SwitchPlanDialogBase::SwitchPlanDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer55;
	fgSizer55 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer55->SetFlexibleDirection( wxBOTH );
	fgSizer55->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer15;
	sbSizer15 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Conditions") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbMaxWindSpeed = new wxCheckBox( this, wxID_ANY, wxT("Wind Speed >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMaxWindSpeed, 0, wxALL, 5 );
	
	m_sMaxWindSpeed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer52->Add( m_sMaxWindSpeed, 0, wxALL, 5 );
	
	m_staticText101 = new wxStaticText( this, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer52->Add( m_staticText101, 0, wxALL, 5 );
	
	m_cbMinWindSpeed = new wxCheckBox( this, wxID_ANY, wxT("Wind Speed <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMinWindSpeed, 0, wxALL, 5 );
	
	m_sMinWindSpeed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer52->Add( m_sMinWindSpeed, 0, wxALL, 5 );
	
	m_staticText102 = new wxStaticText( this, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	fgSizer52->Add( m_staticText102, 0, wxALL, 5 );
	
	m_cbMaxWindDirection = new wxCheckBox( this, wxID_ANY, wxT("Wind Direction >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMaxWindDirection, 0, wxALL, 5 );
	
	m_sMaxWindDirection = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 0 );
	fgSizer52->Add( m_sMaxWindDirection, 0, wxALL, 5 );
	
	m_staticText103 = new wxStaticText( this, wxID_ANY, wxT("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText103->Wrap( -1 );
	fgSizer52->Add( m_staticText103, 0, wxALL, 5 );
	
	m_cbMinWindDirection = new wxCheckBox( this, wxID_ANY, wxT("Wind Direction <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMinWindDirection, 0, wxALL, 5 );
	
	m_sMinWindDirection = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 0 );
	fgSizer52->Add( m_sMinWindDirection, 0, wxALL, 5 );
	
	m_staticText104 = new wxStaticText( this, wxID_ANY, wxT("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText104->Wrap( -1 );
	fgSizer52->Add( m_staticText104, 0, wxALL, 5 );
	
	m_cbMaxWaveHeight = new wxCheckBox( this, wxID_ANY, wxT("Wave Height >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMaxWaveHeight, 0, wxALL, 5 );
	
	m_sMaxWaveHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer52->Add( m_sMaxWaveHeight, 0, wxALL, 5 );
	
	m_staticText105 = new wxStaticText( this, wxID_ANY, wxT("Meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText105->Wrap( -1 );
	fgSizer52->Add( m_staticText105, 0, wxALL, 5 );
	
	m_cbMinWaveHeight = new wxCheckBox( this, wxID_ANY, wxT("Wave Height <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMinWaveHeight, 0, wxALL, 5 );
	
	m_sMinWaveHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer52->Add( m_sMinWaveHeight, 0, wxALL, 5 );
	
	m_staticText106 = new wxStaticText( this, wxID_ANY, wxT("Meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText106->Wrap( -1 );
	fgSizer52->Add( m_staticText106, 0, wxALL, 5 );
	
	
	sbSizer15->Add( fgSizer52, 1, wxEXPAND, 5 );
	
	
	fgSizer55->Add( sbSizer15, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer56;
	fgSizer56 = new wxFlexGridSizer( 2, 0, 0, 0 );
	fgSizer56->SetFlexibleDirection( wxBOTH );
	fgSizer56->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText107 = new wxStaticText( this, wxID_ANY, wxT("Then Switch to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText107->Wrap( -1 );
	fgSizer56->Add( m_staticText107, 0, wxALL, 5 );
	
	wxArrayString m_cPlansChoices;
	m_cPlans = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cPlansChoices, 0 );
	m_cPlans->SetSelection( 0 );
	fgSizer56->Add( m_cPlans, 0, wxALL, 5 );
	
	
	fgSizer56->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_bDone = new wxButton( this, wxID_ANY, wxT("Done"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer56->Add( m_bDone, 0, wxALL, 5 );
	
	m_bCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer56->Add( m_bCancel, 0, wxALL, 5 );
	
	m_bInformation = new wxButton( this, wxID_ANY, wxT("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer56->Add( m_bInformation, 0, wxALL, 5 );
	
	
	fgSizer55->Add( fgSizer56, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer55 );
	this->Layout();
	fgSizer55->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bDone->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SwitchPlanDialogBase::OnDone ), NULL, this );
	m_bCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SwitchPlanDialogBase::OnCancel ), NULL, this );
	m_bInformation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SwitchPlanDialogBase::OnInformation ), NULL, this );
}

SwitchPlanDialogBase::~SwitchPlanDialogBase()
{
	// Disconnect Events
	m_bDone->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SwitchPlanDialogBase::OnDone ), NULL, this );
	m_bCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SwitchPlanDialogBase::OnCancel ), NULL, this );
	m_bInformation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SwitchPlanDialogBase::OnInformation ), NULL, this );
	
}

PlotDialogBase::PlotDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	fgSizer12 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer12->AddGrowableCol( 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer77;
	fgSizer77 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer77->AddGrowableCol( 1 );
	fgSizer77->SetFlexibleDirection( wxBOTH );
	fgSizer77->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText138 = new wxStaticText( this, wxID_ANY, wxT("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText138->Wrap( -1 );
	fgSizer77->Add( m_staticText138, 0, wxALL, 5 );
	
	m_sPosition = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer77->Add( m_sPosition, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText139 = new wxStaticText( this, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText139->Wrap( -1 );
	fgSizer77->Add( m_staticText139, 0, wxALL, 5 );
	
	m_sScale = new wxSlider( this, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer77->Add( m_sScale, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer77->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer78;
	fgSizer78 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer78->SetFlexibleDirection( wxBOTH );
	fgSizer78->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText140 = new wxStaticText( this, wxID_ANY, wxT("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText140->Wrap( -1 );
	m_staticText140->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer78->Add( m_staticText140, 0, wxALL, 5 );
	
	wxString m_cVariable1Choices[] = { wxT("Boat Velocity Ground"), wxT("Boat Course Ground"), wxT("Boat Velocity Water"), wxT("Boat Course Water"), wxT("Wind Velocity"), wxT("Wind Direction"), wxT("Wind Course"), wxT("Wind Velocity Ground"), wxT("Wind Direction Ground"), wxT("Wind Course Ground"), wxT("Apparent Wind Velocity"), wxT("Apparent Wind Direction"), wxT("Current Velocity"), wxT("Current Direction"), wxT("Sig Wave Height") };
	int m_cVariable1NChoices = sizeof( m_cVariable1Choices ) / sizeof( wxString );
	m_cVariable1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariable1NChoices, m_cVariable1Choices, 0 );
	m_cVariable1->SetSelection( 3 );
	fgSizer78->Add( m_cVariable1, 0, wxALL, 5 );
	
	m_stMousePosition1 = new wxStaticText( this, wxID_ANY, wxT("        N/A         "), wxDefaultPosition, wxDefaultSize, 0 );
	m_stMousePosition1->Wrap( -1 );
	fgSizer78->Add( m_stMousePosition1, 0, wxALL, 5 );
	
	m_staticText1401 = new wxStaticText( this, wxID_ANY, wxT("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1401->Wrap( -1 );
	m_staticText1401->SetForegroundColour( wxColour( 0, 255, 0 ) );
	
	fgSizer78->Add( m_staticText1401, 0, wxALL, 5 );
	
	wxString m_cVariable2Choices[] = { wxT("Boat Velocity Ground"), wxT("Boat Course Ground"), wxT("Boat Velocity Water"), wxT("Boat Course Water"), wxT("Wind Velocity"), wxT("Wind Direction"), wxT("Wind Course"), wxT("Wind Velocity Ground"), wxT("Wind Direction Ground"), wxT("Wind Course Ground"), wxT("Apparent Wind Velocity"), wxT("Apparent Wind Direction"), wxT("Current Velocity"), wxT("Current Direction"), wxT("Sig Wave Height") };
	int m_cVariable2NChoices = sizeof( m_cVariable2Choices ) / sizeof( wxString );
	m_cVariable2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariable2NChoices, m_cVariable2Choices, 0 );
	m_cVariable2->SetSelection( 5 );
	fgSizer78->Add( m_cVariable2, 0, wxALL, 5 );
	
	m_stMousePosition2 = new wxStaticText( this, wxID_ANY, wxT("        N/A         "), wxDefaultPosition, wxDefaultSize, 0 );
	m_stMousePosition2->Wrap( -1 );
	fgSizer78->Add( m_stMousePosition2, 0, wxALL, 5 );
	
	m_staticText14011 = new wxStaticText( this, wxID_ANY, wxT("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14011->Wrap( -1 );
	m_staticText14011->SetForegroundColour( wxColour( 0, 0, 255 ) );
	
	fgSizer78->Add( m_staticText14011, 0, wxALL, 5 );
	
	wxString m_cVariable3Choices[] = { wxT("Boat Velocity Ground"), wxT("Boat Course Ground"), wxT("Boat Velocity Water"), wxT("Boat Course Water"), wxT("Wind Velocity"), wxT("Wind Direction"), wxT("Wind Course"), wxT("Wind Velocity Ground"), wxT("Wind Direction Ground"), wxT("Wind Course Ground"), wxT("Apparent Wind Velocity"), wxT("Apparent Wind Direction"), wxT("Current Velocity"), wxT("Current Direction"), wxT("Sig Wave Height") };
	int m_cVariable3NChoices = sizeof( m_cVariable3Choices ) / sizeof( wxString );
	m_cVariable3 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariable3NChoices, m_cVariable3Choices, 0 );
	m_cVariable3->SetSelection( 11 );
	fgSizer78->Add( m_cVariable3, 0, wxALL, 5 );
	
	m_stMousePosition3 = new wxStaticText( this, wxID_ANY, wxT("        N/A         "), wxDefaultPosition, wxDefaultSize, 0 );
	m_stMousePosition3->Wrap( -1 );
	fgSizer78->Add( m_stMousePosition3, 0, wxALL, 5 );
	
	
	fgSizer77->Add( fgSizer78, 1, wxEXPAND, 5 );
	
	
	fgSizer12->Add( fgSizer77, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4OK = new wxButton( this, wxID_OK );
	m_sdbSizer4->AddButton( m_sdbSizer4OK );
	m_sdbSizer4->Realize();
	
	fgSizer16->Add( m_sdbSizer4, 1, wxEXPAND, 5 );
	
	
	fgSizer12->Add( fgSizer16, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( fgSizer12, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_PlotWindow->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MOTION, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_RIGHT_DCLICK, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_PAINT, wxPaintEventHandler( PlotDialogBase::OnPaintPlot ), NULL, this );
	m_PlotWindow->Connect( wxEVT_SIZE, wxSizeEventHandler( PlotDialogBase::OnSizePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_cVariable1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlotDialogBase::OnUpdatePlotVariable ), NULL, this );
	m_cVariable2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlotDialogBase::OnUpdatePlotVariable ), NULL, this );
	m_cVariable3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlotDialogBase::OnUpdatePlotVariable ), NULL, this );
}

PlotDialogBase::~PlotDialogBase()
{
	// Disconnect Events
	m_PlotWindow->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MIDDLE_UP, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MOTION, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_RIGHT_DCLICK, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( PlotDialogBase::OnMouseEventsPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( PlotDialogBase::OnPaintPlot ), NULL, this );
	m_PlotWindow->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PlotDialogBase::OnSizePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sPosition->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_sScale->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( PlotDialogBase::OnUpdatePlot ), NULL, this );
	m_cVariable1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlotDialogBase::OnUpdatePlotVariable ), NULL, this );
	m_cVariable2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlotDialogBase::OnUpdatePlotVariable ), NULL, this );
	m_cVariable3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PlotDialogBase::OnUpdatePlotVariable ), NULL, this );
	
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

StatisticsDialogBase::StatisticsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer55;
	fgSizer55 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer55->SetFlexibleDirection( wxBOTH );
	fgSizer55->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Statistics") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText47 = new wxStaticText( this, wxID_ANY, wxT("State"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer29->Add( m_staticText47, 0, wxALL, 5 );
	
	m_stState = new wxStaticText( this, wxID_ANY, wxT("Stopped"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stState->Wrap( -1 );
	fgSizer29->Add( m_stState, 0, wxALL, 5 );
	
	m_staticText511 = new wxStaticText( this, wxID_ANY, wxT("Run Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText511->Wrap( -1 );
	fgSizer29->Add( m_staticText511, 0, wxALL, 5 );
	
	m_stRunTime = new wxStaticText( this, wxID_ANY, wxT("0000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stRunTime->Wrap( -1 );
	fgSizer29->Add( m_stRunTime, 0, wxALL, 5 );
	
	m_staticText53 = new wxStaticText( this, wxID_ANY, wxT("Iso Chrons"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer29->Add( m_staticText53, 0, wxALL, 5 );
	
	m_stIsoChrons = new wxStaticText( this, wxID_ANY, wxT("0000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stIsoChrons->Wrap( -1 );
	fgSizer29->Add( m_stIsoChrons, 0, wxALL, 5 );
	
	
	fgSizer29->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer29->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText55 = new wxStaticText( this, wxID_ANY, wxT("Routes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer29->Add( m_staticText55, 0, wxALL, 5 );
	
	m_stRoutes = new wxStaticText( this, wxID_ANY, wxT("0000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stRoutes->Wrap( -1 );
	fgSizer29->Add( m_stRoutes, 0, wxALL, 5 );
	
	m_staticText57 = new wxStaticText( this, wxID_ANY, wxT("InvRoutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer29->Add( m_staticText57, 0, wxALL, 5 );
	
	m_stInvRoutes = new wxStaticText( this, wxID_ANY, wxT("0000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stInvRoutes->Wrap( -1 );
	fgSizer29->Add( m_stInvRoutes, 0, wxALL, 5 );
	
	m_staticText90 = new wxStaticText( this, wxID_ANY, wxT("Skip Positions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText90->Wrap( -1 );
	fgSizer29->Add( m_staticText90, 0, wxALL, 5 );
	
	m_stSkipPositions = new wxStaticText( this, wxID_ANY, wxT("0000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stSkipPositions->Wrap( -1 );
	fgSizer29->Add( m_stSkipPositions, 0, wxALL, 5 );
	
	m_staticText49 = new wxStaticText( this, wxID_ANY, wxT("Positions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer29->Add( m_staticText49, 0, wxALL, 5 );
	
	m_stPositions = new wxStaticText( this, wxID_ANY, wxT("0000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stPositions->Wrap( -1 );
	fgSizer29->Add( m_stPositions, 0, wxALL, 5 );
	
	
	sbSizer10->Add( fgSizer29, 1, wxEXPAND, 5 );
	
	
	fgSizer55->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	m_sdbSizer5 = new wxStdDialogButtonSizer();
	m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	m_sdbSizer5->Realize();
	
	fgSizer55->Add( m_sdbSizer5, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer55 );
	this->Layout();
	fgSizer55->Fit( this );
	
	this->Centre( wxBOTH );
}

StatisticsDialogBase::~StatisticsDialogBase()
{
}
