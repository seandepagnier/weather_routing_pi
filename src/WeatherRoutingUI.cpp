///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WeatherRoutingUI.h"

///////////////////////////////////////////////////////////////////////////

WeatherRoutingBase::WeatherRoutingBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer92->AddGrowableCol( 0 );
	fgSizer92->AddGrowableRow( 0 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( WeatherRoutingBase::m_splitter1OnIdle ), NULL, this );
	
	m_panel11 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer30;
	sbSizer30 = new wxStaticBoxSizer( new wxStaticBox( m_panel11, wxID_ANY, _("Positions") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer93;
	fgSizer93 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer93->AddGrowableCol( 0 );
	fgSizer93->AddGrowableRow( 0 );
	fgSizer93->SetFlexibleDirection( wxBOTH );
	fgSizer93->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lPositions = new wxListCtrl( m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	fgSizer93->Add( m_lPositions, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer30->Add( fgSizer93, 1, wxEXPAND, 5 );
	
	
	m_panel11->SetSizer( sbSizer30 );
	m_panel11->Layout();
	sbSizer30->Fit( m_panel11 );
	m_panel12 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer29;
	sbSizer29 = new wxStaticBoxSizer( new wxStaticBox( m_panel12, wxID_ANY, _("Configurations") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer17->AddGrowableCol( 0 );
	fgSizer17->AddGrowableRow( 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	m_lWeatherRoutes = new wxListCtrl( m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	m_lWeatherRoutes->SetMinSize( wxSize( 600,-1 ) );
	
	fgSizer17->Add( m_lWeatherRoutes, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer116;
	fgSizer116 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer116->AddGrowableCol( 1 );
	fgSizer116->SetFlexibleDirection( wxBOTH );
	fgSizer116->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bCompute = new wxButton( m_panel12, wxID_ANY, _("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer116->Add( m_bCompute, 0, wxALL, 5 );
	
	m_gProgress = new wxGauge( m_panel12, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gProgress->SetValue( 0 ); 
	fgSizer116->Add( m_gProgress, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer17->Add( fgSizer116, 1, wxEXPAND, 5 );
	
	
	sbSizer29->Add( fgSizer17, 1, wxEXPAND, 5 );
	
	
	m_panel12->SetSizer( sbSizer29 );
	m_panel12->Layout();
	sbSizer29->Fit( m_panel12 );
	m_splitter1->SplitVertically( m_panel11, m_panel12, 120 );
	fgSizer92->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer92 );
	this->Layout();
	fgSizer92->Fit( this );
	m_menubar3 = new wxMenuBar( 0 );
	m_mFile = new wxMenu();
	wxMenuItem* m_mOpen;
	m_mOpen = new wxMenuItem( m_mFile, wxID_ANY, wxString( _("Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_mOpen );
	
	wxMenuItem* m_mSave;
	m_mSave = new wxMenuItem( m_mFile, wxID_ANY, wxString( _("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_mSave );
	
	m_mFile->AppendSeparator();
	
	wxMenuItem* m_mClose;
	m_mClose = new wxMenuItem( m_mFile, wxID_ANY, wxString( _("Close") ) + wxT('\t') + wxT("Ctrl+W"), wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_mClose );
	
	m_menubar3->Append( m_mFile, _("File") ); 
	
	m_mPosition = new wxMenu();
	wxMenuItem* m_mAddAtBoat;
	m_mAddAtBoat = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("Add At Boat") ) , wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mAddAtBoat );
	
	wxMenuItem* m_mRemove;
	m_mRemove = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("Remove") ) , wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mRemove );
	
	wxMenuItem* m_mClear;
	m_mClear = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("Clear") ) , wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mClear );
	
	m_menubar3->Append( m_mPosition, _("Position") ); 
	
	m_mConfiguration = new wxMenu();
	wxMenuItem* m_mNew;
	m_mNew = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mNew );
	
	wxMenuItem* m_mBatch;
	m_mBatch = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Batch") ) + wxT('\t') + wxT("Ctrl+B"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mBatch );
	
	wxMenuItem* m_mEdit;
	m_mEdit = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Edit") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mEdit );
	
	wxMenuItem* m_mExport;
	m_mExport = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Export") ) + wxT('\t') + wxT("Ctrl+X"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mExport );
	
	wxMenuItem* m_mDelete;
	m_mDelete = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Delete") ) + wxT('\t') + wxT("Ctrl+D"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mDelete );
	
	m_mConfiguration->AppendSeparator();
	
	wxMenuItem* m_mFilter;
	m_mFilter = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Filter") ) + wxT('\t') + wxT("Ctrl+F"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mFilter );
	
	wxMenuItem* m_mReset;
	m_mReset = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Reset") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mReset );
	
	wxMenuItem* m_mExportAll;
	m_mExportAll = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Export All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mExportAll );
	
	wxMenuItem* m_mDeleteAll;
	m_mDeleteAll = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Delete All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mDeleteAll );
	
	m_menubar3->Append( m_mConfiguration, _("Configuration") ); 
	
	m_mView = new wxMenu();
	wxMenuItem* m_mSettings;
	m_mSettings = new wxMenuItem( m_mView, wxID_ANY, wxString( _("Settings") ) , wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mSettings );
	
	wxMenuItem* m_mStatistics;
	m_mStatistics = new wxMenuItem( m_mView, wxID_ANY, wxString( _("Statistics") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mStatistics );
	
	wxMenuItem* m_mPlot;
	m_mPlot = new wxMenuItem( m_mView, wxID_ANY, wxString( _("Plot") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mPlot );
	
	m_menubar3->Append( m_mView, _("View") ); 
	
	m_mHelp = new wxMenu();
	wxMenuItem* m_mInformation;
	m_mInformation = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("Information") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mInformation );
	
	wxMenuItem* m_mAbout;
	m_mAbout = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mAbout );
	
	m_menubar3->Append( m_mHelp, _("Help") ); 
	
	this->SetMenuBar( m_menubar3 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WeatherRoutingBase::OnClose ) );
	m_lWeatherRoutes->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WeatherRoutingBase::OnEditConfiguration ), NULL, this );
	m_lWeatherRoutes->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WeatherRoutingBase::OnWeatherRoutesListLeftDown ), NULL, this );
	m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( WeatherRoutingBase::OnWeatherRouteSort ), NULL, this );
	m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( WeatherRoutingBase::OnWeatherRouteSelected ), NULL, this );
	m_lWeatherRoutes->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( WeatherRoutingBase::OnWeatherRouteSelected ), NULL, this );
	m_bCompute->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingBase::OnCompute ), NULL, this );
	this->Connect( m_mOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnOpen ) );
	this->Connect( m_mSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnSave ) );
	this->Connect( m_mClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnClose ) );
	this->Connect( m_mAddAtBoat->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnAddAtBoat ) );
	this->Connect( m_mRemove->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnRemovePosition ) );
	this->Connect( m_mClear->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnClearPositions ) );
	this->Connect( m_mNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnNew ) );
	this->Connect( m_mBatch->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnBatch ) );
	this->Connect( m_mEdit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnEditConfiguration ) );
	this->Connect( m_mExport->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnExport ) );
	this->Connect( m_mDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnDelete ) );
	this->Connect( m_mFilter->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnFilter ) );
	this->Connect( m_mReset->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnReset ) );
	this->Connect( m_mExportAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnExportAll ) );
	this->Connect( m_mDeleteAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnDeleteAll ) );
	this->Connect( m_mSettings->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnSettings ) );
	this->Connect( m_mStatistics->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnStatistics ) );
	this->Connect( m_mPlot->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnPlot ) );
	this->Connect( m_mInformation->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnInformation ) );
	this->Connect( m_mAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnAbout ) );
}

WeatherRoutingBase::~WeatherRoutingBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WeatherRoutingBase::OnClose ) );
	m_lWeatherRoutes->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WeatherRoutingBase::OnEditConfiguration ), NULL, this );
	m_lWeatherRoutes->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WeatherRoutingBase::OnWeatherRoutesListLeftDown ), NULL, this );
	m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( WeatherRoutingBase::OnWeatherRouteSort ), NULL, this );
	m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( WeatherRoutingBase::OnWeatherRouteSelected ), NULL, this );
	m_lWeatherRoutes->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( WeatherRoutingBase::OnWeatherRouteSelected ), NULL, this );
	m_bCompute->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingBase::OnCompute ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnSave ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnAddAtBoat ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnRemovePosition ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnClearPositions ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnBatch ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnEditConfiguration ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnExport ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnDelete ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnFilter ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnReset ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnExportAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnDeleteAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnSettings ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnStatistics ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnPlot ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnInformation ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WeatherRoutingBase::OnAbout ) );
	
}

ConfigurationDialogBase::ConfigurationDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer62;
	fgSizer62 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer62->SetFlexibleDirection( wxBOTH );
	fgSizer62->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer95;
	fgSizer95 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer95->SetFlexibleDirection( wxBOTH );
	fgSizer95->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer83;
	fgSizer83 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer83->SetFlexibleDirection( wxBOTH );
	fgSizer83->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Start") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer60;
	fgSizer60 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer60->SetFlexibleDirection( wxBOTH );
	fgSizer60->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer6->AddGrowableCol( 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cStart = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer6->Add( m_cStart, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer60->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer94;
	fgSizer94 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer94->SetFlexibleDirection( wxBOTH );
	fgSizer94->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, _("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer94->Add( m_staticText28, 0, wxALL, 5 );
	
	m_dpStartDate = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	fgSizer94->Add( m_dpStartDate, 0, wxALL, 5 );
	
	m_bGribTime = new wxButton( this, wxID_ANY, _("Grib Time"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_bGribTime, 0, wxLEFT|wxRIGHT, 5 );
	
	m_staticText30 = new wxStaticText( this, wxID_ANY, _("Hour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer94->Add( m_staticText30, 0, wxALL, 5 );
	
	m_tStartHour = new wxTextCtrl( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_tStartHour, 0, wxALL, 5 );
	
	m_bCurrentTime = new wxButton( this, wxID_ANY, _("Current Time"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_bCurrentTime, 0, wxLEFT|wxRIGHT, 5 );
	
	
	fgSizer60->Add( fgSizer94, 1, wxEXPAND, 5 );
	
	
	sbSizer3->Add( fgSizer60, 1, wxEXPAND, 5 );
	
	
	fgSizer83->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("End") ), wxVERTICAL );
	
	m_cEnd = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sbSizer5->Add( m_cEnd, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer83->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer251;
	sbSizer251 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Time Step") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer921;
	fgSizer921 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer921->SetFlexibleDirection( wxBOTH );
	fgSizer921->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_sTimeStepHours = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 1 );
	fgSizer921->Add( m_sTimeStepHours, 0, wxALL, 5 );
	
	m_staticText110 = new wxStaticText( this, wxID_ANY, _("h"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( -1 );
	fgSizer921->Add( m_staticText110, 0, wxALL, 5 );
	
	m_sTimeStepMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 60, 0 );
	fgSizer921->Add( m_sTimeStepMinutes, 0, wxALL, 5 );
	
	m_staticText111 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer921->Add( m_staticText111, 0, wxALL, 5 );
	
	m_sTimeStepSeconds = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 60, 0 );
	fgSizer921->Add( m_sTimeStepSeconds, 0, wxALL, 5 );
	
	m_staticText112 = new wxStaticText( this, wxID_ANY, _("s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText112->Wrap( -1 );
	fgSizer921->Add( m_staticText112, 0, wxALL, 5 );
	
	
	sbSizer251->Add( fgSizer921, 1, wxEXPAND, 5 );
	
	
	fgSizer83->Add( sbSizer251, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer24;
	sbSizer24 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Boat") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer98;
	fgSizer98 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer98->AddGrowableCol( 0 );
	fgSizer98->SetFlexibleDirection( wxBOTH );
	fgSizer98->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_fpBoat = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	fgSizer98->Add( m_fpBoat, 0, wxALL|wxEXPAND, 5 );
	
	m_bEditBoat = new wxButton( this, wxID_ANY, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer98->Add( m_bEditBoat, 0, wxALL, 5 );
	
	
	sbSizer24->Add( fgSizer98, 1, wxEXPAND, 5 );
	
	
	fgSizer83->Add( sbSizer24, 1, wxEXPAND, 5 );
	
	
	fgSizer95->Add( fgSizer83, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer112;
	fgSizer112 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer112->SetFlexibleDirection( wxBOTH );
	fgSizer112->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer23;
	sbSizer23 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Constraints") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer110;
	fgSizer110 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer110->SetFlexibleDirection( wxBOTH );
	fgSizer110->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, _("Max Diverted Course"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer110->Add( m_staticText20, 0, wxALL, 5 );
	
	m_sMaxDivertedCourse = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 180 );
	fgSizer110->Add( m_sMaxDivertedCourse, 0, wxALL, 5 );
	
	m_staticText1181 = new wxStaticText( this, wxID_ANY, _("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1181->Wrap( -1 );
	fgSizer110->Add( m_staticText1181, 0, wxALL, 5 );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, _("Max Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer110->Add( m_staticText23, 0, wxALL, 5 );
	
	m_sMaxWindKnots = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 200, 60 );
	fgSizer110->Add( m_sMaxWindKnots, 0, wxALL, 5 );
	
	m_staticText128 = new wxStaticText( this, wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText128->Wrap( -1 );
	fgSizer110->Add( m_staticText128, 0, wxALL, 5 );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, _("Max Swell"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer110->Add( m_staticText27, 0, wxALL, 5 );
	
	m_sMaxSwellMeters = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 20 );
	fgSizer110->Add( m_sMaxSwellMeters, 0, wxALL, 5 );
	
	m_staticText129 = new wxStaticText( this, wxID_ANY, _("meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText129->Wrap( -1 );
	fgSizer110->Add( m_staticText129, 0, wxALL, 5 );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, _("Max Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	fgSizer110->Add( m_staticText26, 0, wxALL, 5 );
	
	m_sMaxLatitude = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 90, 90 );
	fgSizer110->Add( m_sMaxLatitude, 0, wxALL, 5 );
	
	m_staticText131 = new wxStaticText( this, wxID_ANY, _("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	fgSizer110->Add( m_staticText131, 0, wxALL, 5 );
	
	m_staticText1131 = new wxStaticText( this, wxID_ANY, _("Max # of Tacks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1131->Wrap( -1 );
	fgSizer110->Add( m_staticText1131, 0, wxALL, 5 );
	
	m_sMaxTacks = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1, 10000, -1 );
	fgSizer110->Add( m_sMaxTacks, 0, wxALL, 5 );
	
	m_staticText1141 = new wxStaticText( this, wxID_ANY, _("(-1 for unlimited)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1141->Wrap( -1 );
	fgSizer110->Add( m_staticText1141, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("Tacking Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer110->Add( m_staticText24, 0, wxALL, 5 );
	
	m_sTackingTime = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 10 );
	fgSizer110->Add( m_sTackingTime, 0, wxALL, 5 );
	
	m_staticText1191 = new wxStaticText( this, wxID_ANY, _("seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1191->Wrap( -1 );
	fgSizer110->Add( m_staticText1191, 0, wxALL, 5 );
	
	
	sbSizer23->Add( fgSizer110, 1, wxEXPAND, 5 );
	
	
	fgSizer112->Add( sbSizer23, 1, wxEXPAND, 5 );
	
	
	fgSizer95->Add( fgSizer112, 1, wxEXPAND, 5 );
	
	
	fgSizer62->Add( fgSizer95, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer84;
	fgSizer84 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer84->SetFlexibleDirection( wxBOTH );
	fgSizer84->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Degree Steps") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer105;
	fgSizer105 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer105->SetFlexibleDirection( wxBOTH );
	fgSizer105->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
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
	m_lDegreeSteps->Append( _("-45") );
	m_lDegreeSteps->Append( _("45") );
	m_lDegreeSteps->Append( _("-60") );
	m_lDegreeSteps->Append( _("60") );
	m_lDegreeSteps->Append( _("-90") );
	m_lDegreeSteps->Append( _("90") );
	m_lDegreeSteps->Append( _("-120") );
	m_lDegreeSteps->Append( _("120") );
	m_lDegreeSteps->Append( _("-150") );
	m_lDegreeSteps->Append( _("150") );
	m_lDegreeSteps->SetMinSize( wxSize( 60,-1 ) );
	
	fgSizer22->Add( m_lDegreeSteps, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer20->Add( fgSizer22, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tDegreeStep = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_tDegreeStep, 0, wxALL, 5 );
	
	m_bAddDegreeStep = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bAddDegreeStep, 0, wxALL, 5 );
	
	m_bRemoveDegreeStep = new wxButton( this, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bRemoveDegreeStep, 0, wxALL, 5 );
	
	m_bClearDegreeSteps = new wxButton( this, wxID_ANY, _("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_bClearDegreeSteps, 0, wxALL, 5 );
	
	
	fgSizer20->Add( fgSizer21, 1, wxEXPAND, 5 );
	
	
	fgSizer105->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer27;
	sbSizer27 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Generate") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer107;
	fgSizer107 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer107->SetFlexibleDirection( wxBOTH );
	fgSizer107->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText113 = new wxStaticText( this, wxID_ANY, _("From"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113->Wrap( -1 );
	fgSizer107->Add( m_staticText113, 0, wxALL, 5 );
	
	m_tFromDegrees = new wxTextCtrl( this, wxID_ANY, _("45"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer107->Add( m_tFromDegrees, 0, wxALL, 5 );
	
	m_staticText114 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText114->Wrap( -1 );
	fgSizer107->Add( m_staticText114, 0, wxALL, 5 );
	
	m_staticText115 = new wxStaticText( this, wxID_ANY, _("To"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	fgSizer107->Add( m_staticText115, 0, wxALL, 5 );
	
	m_tToDegrees = new wxTextCtrl( this, wxID_ANY, _("170"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer107->Add( m_tToDegrees, 0, wxALL, 5 );
	
	m_staticText116 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText116->Wrap( -1 );
	fgSizer107->Add( m_staticText116, 0, wxALL, 5 );
	
	m_staticText117 = new wxStaticText( this, wxID_ANY, _("By"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText117->Wrap( -1 );
	fgSizer107->Add( m_staticText117, 0, wxALL, 5 );
	
	m_tByDegrees = new wxTextCtrl( this, wxID_ANY, _("5"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer107->Add( m_tByDegrees, 0, wxALL, 5 );
	
	m_staticText118 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText118->Wrap( -1 );
	fgSizer107->Add( m_staticText118, 0, wxALL, 5 );
	
	
	sbSizer27->Add( fgSizer107, 1, wxEXPAND, 5 );
	
	m_bGenerateDegreeSteps = new wxButton( this, wxID_ANY, _("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer27->Add( m_bGenerateDegreeSteps, 0, wxALL, 5 );
	
	
	fgSizer105->Add( sbSizer27, 1, wxEXPAND, 5 );
	
	
	sbSizer7->Add( fgSizer105, 1, wxEXPAND, 5 );
	
	
	fgSizer84->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer57;
	fgSizer57 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer57->SetFlexibleDirection( wxBOTH );
	fgSizer57->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer15;
	sbSizer15 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Data Source") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer58;
	fgSizer58 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer58->SetFlexibleDirection( wxBOTH );
	fgSizer58->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer59;
	fgSizer59 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer59->SetFlexibleDirection( wxBOTH );
	fgSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbUseGrib = new wxCheckBox( this, wxID_ANY, _("Grib"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbUseGrib->SetValue(true); 
	m_cbUseGrib->Enable( false );
	
	fgSizer59->Add( m_cbUseGrib, 0, wxALL, 5 );
	
	m_cbUseClimatology = new wxCheckBox( this, wxID_ANY, _("Climatology"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbUseClimatology->Enable( false );
	
	fgSizer59->Add( m_cbUseClimatology, 0, wxALL, 5 );
	
	
	fgSizer58->Add( fgSizer59, 1, wxEXPAND, 5 );
	
	m_cbAllowDataDeficient = new wxCheckBox( this, wxID_ANY, _("Last Valid if Data Deficient"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer58->Add( m_cbAllowDataDeficient, 0, wxALL, 5 );
	
	
	sbSizer15->Add( fgSizer58, 1, wxEXPAND, 5 );
	
	
	fgSizer57->Add( sbSizer15, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer28;
	sbSizer28 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Options") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer23->SetFlexibleDirection( wxBOTH );
	fgSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbDetectLand = new wxCheckBox( this, wxID_ANY, _("Detect Land"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbDetectLand->SetValue(true); 
	fgSizer23->Add( m_cbDetectLand, 0, wxALL, 5 );
	
	m_cbCurrents = new wxCheckBox( this, wxID_ANY, _("Currents"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCurrents->SetValue(true); 
	fgSizer23->Add( m_cbCurrents, 0, wxALL, 5 );
	
	m_cbInvertedRegions = new wxCheckBox( this, wxID_ANY, _("Inverted Regions"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_cbInvertedRegions, 0, wxALL, 5 );
	
	m_cbAnchoring = new wxCheckBox( this, wxID_ANY, _("Anchoring"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_cbAnchoring, 0, wxALL, 5 );
	
	
	sbSizer28->Add( fgSizer23, 1, wxEXPAND, 5 );
	
	
	fgSizer57->Add( sbSizer28, 1, wxEXPAND, 5 );
	
	
	fgSizer18->Add( fgSizer57, 1, wxEXPAND, 5 );
	
	
	fgSizer84->Add( fgSizer18, 1, wxEXPAND, 5 );
	
	
	fgSizer62->Add( fgSizer84, 1, wxEXPAND, 5 );
	
	
	fgSizer92->Add( fgSizer62, 1, wxEXPAND, 5 );
	
	m_sdbSizer5 = new wxStdDialogButtonSizer();
	m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	
	fgSizer92->Add( m_sdbSizer5, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer92 );
	this->Layout();
	fgSizer92->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cStart->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_dpStartDate->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bGribTime->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnGribTime ), NULL, this );
	m_tStartHour->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bCurrentTime->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnCurrentTime ), NULL, this );
	m_cEnd->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTimeStepHours->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTimeStepMinutes->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTimeStepSeconds->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_fpBoat->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bEditBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnEditBoat ), NULL, this );
	m_sMaxDivertedCourse->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxWindKnots->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxSwellMeters->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxLatitude->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxTacks->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTackingTime->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bAddDegreeStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnAddDegreeStep ), NULL, this );
	m_bRemoveDegreeStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnRemoveDegreeStep ), NULL, this );
	m_bClearDegreeSteps->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnClearDegreeSteps ), NULL, this );
	m_bGenerateDegreeSteps->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnGenerateDegreeSteps ), NULL, this );
	m_cbUseGrib->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbUseClimatology->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbAllowDataDeficient->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbDetectLand->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbCurrents->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbInvertedRegions->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbAnchoring->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
}

ConfigurationDialogBase::~ConfigurationDialogBase()
{
	// Disconnect Events
	m_cStart->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_dpStartDate->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bGribTime->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnGribTime ), NULL, this );
	m_tStartHour->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bCurrentTime->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnCurrentTime ), NULL, this );
	m_cEnd->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTimeStepHours->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTimeStepMinutes->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTimeStepSeconds->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_fpBoat->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bEditBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnEditBoat ), NULL, this );
	m_sMaxDivertedCourse->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxWindKnots->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxSwellMeters->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxLatitude->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sMaxTacks->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_sTackingTime->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_bAddDegreeStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnAddDegreeStep ), NULL, this );
	m_bRemoveDegreeStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnRemoveDegreeStep ), NULL, this );
	m_bClearDegreeSteps->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnClearDegreeSteps ), NULL, this );
	m_bGenerateDegreeSteps->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnGenerateDegreeSteps ), NULL, this );
	m_cbUseGrib->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbUseClimatology->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbAllowDataDeficient->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbDetectLand->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbCurrents->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbInvertedRegions->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	m_cbAnchoring->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnUpdate ), NULL, this );
	
}

SettingsDialogBase::SettingsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,290 ), wxDefaultSize );
	
	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer25;
	sbSizer25 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Display") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText74 = new wxStaticText( this, wxID_ANY, _("Cursor Route Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer42->Add( m_staticText74, 0, wxALL, 5 );
	
	m_cpCursorRoute = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_cpCursorRoute, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( this, wxID_ANY, _("Destination Route Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer42->Add( m_staticText73, 0, wxALL, 5 );
	
	m_cpDestinationRoute = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_cpDestinationRoute, 0, wxALL, 5 );
	
	m_staticText75 = new wxStaticText( this, wxID_ANY, _("Route Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer42->Add( m_staticText75, 0, wxALL, 5 );
	
	m_sRouteThickness = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 4 );
	fgSizer42->Add( m_sRouteThickness, 0, wxALL, 5 );
	
	m_staticText70 = new wxStaticText( this, wxID_ANY, _("Iso Chron Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText70->Wrap( -1 );
	fgSizer42->Add( m_staticText70, 0, wxALL, 5 );
	
	m_sIsoChronThickness = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
	fgSizer42->Add( m_sIsoChronThickness, 0, wxALL, 5 );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, _("Alternate Routes Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer42->Add( m_staticText71, 0, wxALL, 5 );
	
	m_sAlternateRouteThickness = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer42->Add( m_sAlternateRouteThickness, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer42, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer82;
	fgSizer82 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer82->SetFlexibleDirection( wxBOTH );
	fgSizer82->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbAlternatesForAll = new wxCheckBox( this, wxID_ANY, _("Alternates for all IsoChrons"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_cbAlternatesForAll, 0, wxALL, 5 );
	
	m_cbSquaresAtSailChanges = new wxCheckBox( this, wxID_ANY, _("Squares at Sail Changes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSquaresAtSailChanges->SetValue(true); 
	fgSizer82->Add( m_cbSquaresAtSailChanges, 0, wxALL, 5 );
	
	m_cbFilterbyClimatology = new wxCheckBox( this, wxID_ANY, _("Filer Routes by Climatology"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_cbFilterbyClimatology, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer82, 1, wxEXPAND, 5 );
	
	
	sbSizer25->Add( fgSizer18, 1, wxEXPAND, 5 );
	
	
	fgSizer92->Add( sbSizer25, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer26;
	sbSizer26 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Computation") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer93;
	fgSizer93 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer93->SetFlexibleDirection( wxBOTH );
	fgSizer93->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText115 = new wxStaticText( this, wxID_ANY, _("Number of Concurrent threads"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	fgSizer93->Add( m_staticText115, 0, wxALL, 5 );
	
	m_sConcurrentThreads = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 64, 1 );
	fgSizer93->Add( m_sConcurrentThreads, 0, wxALL, 5 );
	
	
	sbSizer26->Add( fgSizer93, 1, wxEXPAND, 5 );
	
	
	fgSizer92->Add( sbSizer26, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	fgSizer92->Add( m_sdbSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer92 );
	this->Layout();
	fgSizer92->Fit( this );
	
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
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("Plot Type") ), wxHORIZONTAL );
	
	wxString m_cPlotTypeChoices[] = { _("Polar"), _("Rect") };
	int m_cPlotTypeNChoices = sizeof( m_cPlotTypeChoices ) / sizeof( wxString );
	m_cPlotType = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cPlotTypeNChoices, m_cPlotTypeChoices, 0 );
	m_cPlotType->SetSelection( 0 );
	m_cPlotType->Enable( false );
	
	sbSizer7->Add( m_cPlotType, 0, wxALL, 5 );
	
	wxString m_cPlotAxisChoices[] = { _("Boat"), _("True Wind"), _("Apparent Wind") };
	int m_cPlotAxisNChoices = sizeof( m_cPlotAxisChoices ) / sizeof( wxString );
	m_cPlotAxis = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cPlotAxisNChoices, m_cPlotAxisChoices, 0 );
	m_cPlotAxis->SetSelection( 0 );
	sbSizer7->Add( m_cPlotAxis, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("Wind Kts") ), wxVERTICAL );
	
	m_sWindSpeed = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 1, 59, 15 );
	sbSizer8->Add( m_sWindSpeed, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer8, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("Sig Wv Ht") ), wxVERTICAL );
	
	m_spinCtrl20 = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 10, 0 );
	m_spinCtrl20->Enable( false );
	
	sbSizer11->Add( m_spinCtrl20, 0, wxALL, 5 );
	
	
	fgSizer24->Add( sbSizer11, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("Wv Period") ), wxVERTICAL );
	
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
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("True Wind") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stTrueWindAngle = new wxStaticText( m_panel3, wxID_ANY, _("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stTrueWindAngle->Wrap( -1 );
	fgSizer20->Add( m_stTrueWindAngle, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( m_panel3, wxID_ANY, _("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer20->Add( m_staticText21, 0, wxALL, 5 );
	
	m_stTrueWindKnots = new wxStaticText( m_panel3, wxID_ANY, _("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stTrueWindKnots->Wrap( -1 );
	fgSizer20->Add( m_stTrueWindKnots, 0, wxALL, 5 );
	
	m_staticText23 = new wxStaticText( m_panel3, wxID_ANY, _("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer20->Add( m_staticText23, 0, wxALL, 5 );
	
	
	sbSizer6->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("Apparent Wind") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer201;
	fgSizer201 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer201->SetFlexibleDirection( wxBOTH );
	fgSizer201->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stApparentWindAngle = new wxStaticText( m_panel3, wxID_ANY, _("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stApparentWindAngle->Wrap( -1 );
	fgSizer201->Add( m_stApparentWindAngle, 0, wxALL, 5 );
	
	m_staticText211 = new wxStaticText( m_panel3, wxID_ANY, _("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer201->Add( m_staticText211, 0, wxALL, 5 );
	
	m_stApparentWindKnots = new wxStaticText( m_panel3, wxID_ANY, _("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stApparentWindKnots->Wrap( -1 );
	fgSizer201->Add( m_stApparentWindKnots, 0, wxALL, 5 );
	
	m_staticText231 = new wxStaticText( m_panel3, wxID_ANY, _("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText231->Wrap( -1 );
	fgSizer201->Add( m_staticText231, 0, wxALL, 5 );
	
	
	sbSizer61->Add( fgSizer201, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( sbSizer61, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer62;
	sbSizer62 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, _("Boat") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer202;
	fgSizer202 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer202->SetFlexibleDirection( wxBOTH );
	fgSizer202->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stBoatAngle = new wxStaticText( m_panel3, wxID_ANY, _("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stBoatAngle->Wrap( -1 );
	fgSizer202->Add( m_stBoatAngle, 0, wxALL, 5 );
	
	m_staticText212 = new wxStaticText( m_panel3, wxID_ANY, _("Deg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText212->Wrap( -1 );
	fgSizer202->Add( m_staticText212, 0, wxALL, 5 );
	
	m_stBoatKnots = new wxStaticText( m_panel3, wxID_ANY, _("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_stBoatKnots->Wrap( -1 );
	fgSizer202->Add( m_stBoatKnots, 0, wxALL, 5 );
	
	m_staticText232 = new wxStaticText( m_panel3, wxID_ANY, _("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText232->Wrap( -1 );
	fgSizer202->Add( m_staticText232, 0, wxALL, 5 );
	
	
	sbSizer62->Add( fgSizer202, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( sbSizer62, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer94;
	fgSizer94 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer94->SetFlexibleDirection( wxBOTH );
	fgSizer94->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOpen = new wxButton( m_panel3, wxID_ANY, _("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_bOpen, 0, wxALL, 5 );
	
	m_bSaveAs = new wxButton( m_panel3, wxID_ANY, _("Save As"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_bSaveAs, 0, wxALL, 5 );
	
	m_bSave = new wxButton( m_panel3, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_bSave, 0, wxALL, 5 );
	
	m_bCancel = new wxButton( m_panel3, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer94->Add( m_bCancel, 0, wxALL, 5 );
	
	
	fgSizer23->Add( fgSizer94, 1, wxEXPAND, 5 );
	
	
	fgSizer18->Add( fgSizer23, 1, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( fgSizer18 );
	m_panel3->Layout();
	fgSizer18->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, _("Plot Details"), false );
	m_panel31 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer181;
	fgSizer181 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer181->AddGrowableCol( 0 );
	fgSizer181->SetFlexibleDirection( wxBOTH );
	fgSizer181->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel31, wxID_ANY, _("CSV Settings") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer34;
	fgSizer34 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer34->SetFlexibleDirection( wxBOTH );
	fgSizer34->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer54;
	fgSizer54 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer54->AddGrowableCol( 1 );
	fgSizer54->SetFlexibleDirection( wxBOTH );
	fgSizer54->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText93 = new wxStaticText( m_panel31, wxID_ANY, _("FileName"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText93->Wrap( -1 );
	fgSizer54->Add( m_staticText93, 0, wxALL, 5 );
	
	m_stCSVFile = new wxStaticText( m_panel31, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stCSVFile->Wrap( -1 );
	fgSizer54->Add( m_stCSVFile, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer34->Add( fgSizer54, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer241;
	fgSizer241 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer241->SetFlexibleDirection( wxBOTH );
	fgSizer241->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText351 = new wxStaticText( m_panel31, wxID_ANY, _("Wind Speed Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText351->Wrap( -1 );
	fgSizer241->Add( m_staticText351, 0, wxALL, 5 );
	
	m_sFileCSVWindSpeedStep = new wxSpinCtrl( m_panel31, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20, 5 );
	fgSizer241->Add( m_sFileCSVWindSpeedStep, 0, wxALL, 5 );
	
	m_staticText36 = new wxStaticText( m_panel31, wxID_ANY, _("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer241->Add( m_staticText36, 0, wxALL, 5 );
	
	m_staticText37 = new wxStaticText( m_panel31, wxID_ANY, _("Wind Degree Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer241->Add( m_staticText37, 0, wxALL, 5 );
	
	m_sFileCSVWindDegreeStep = new wxSpinCtrl( m_panel31, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 30, 10 );
	fgSizer241->Add( m_sFileCSVWindDegreeStep, 0, wxALL, 5 );
	
	m_staticText38 = new wxStaticText( m_panel31, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer241->Add( m_staticText38, 0, wxALL, 5 );
	
	
	fgSizer34->Add( fgSizer241, 1, wxEXPAND, 5 );
	
	
	sbSizer9->Add( fgSizer34, 1, wxEXPAND, 5 );
	
	
	fgSizer181->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer142;
	fgSizer142 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer142->SetFlexibleDirection( wxBOTH );
	fgSizer142->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOpenCSV = new wxButton( m_panel31, wxID_ANY, _("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_bOpenCSV, 0, wxALL, 5 );
	
	m_bSaveCSV = new wxButton( m_panel31, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_bSaveCSV, 0, wxALL, 5 );
	
	
	fgSizer181->Add( fgSizer142, 1, wxEXPAND, 5 );
	
	
	m_panel31->SetSizer( fgSizer181 );
	m_panel31->Layout();
	fgSizer181->Fit( m_panel31 );
	m_notebook1->AddPage( m_panel31, _("Polar File"), true );
	m_panel311 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1811;
	fgSizer1811 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1811->SetFlexibleDirection( wxBOTH );
	fgSizer1811->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer2411;
	fgSizer2411 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2411->SetFlexibleDirection( wxBOTH );
	fgSizer2411->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3511 = new wxStaticText( m_panel311, wxID_ANY, _("Best Course Up Wind Port Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3511->Wrap( -1 );
	fgSizer2411->Add( m_staticText3511, 0, wxALL, 5 );
	
	m_stBestCourseUpWindPortTack = new wxStaticText( m_panel311, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseUpWindPortTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseUpWindPortTack, 0, wxALL, 5 );
	
	m_staticText35111 = new wxStaticText( m_panel311, wxID_ANY, _("Best Course Up Wind Starboard Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35111->Wrap( -1 );
	fgSizer2411->Add( m_staticText35111, 0, wxALL, 5 );
	
	m_stBestCourseUpWindStarboardTack = new wxStaticText( m_panel311, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseUpWindStarboardTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseUpWindStarboardTack, 0, wxALL, 5 );
	
	m_staticText35112 = new wxStaticText( m_panel311, wxID_ANY, _("Best Course Down Wind Port Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35112->Wrap( -1 );
	fgSizer2411->Add( m_staticText35112, 0, wxALL, 5 );
	
	m_stBestCourseDownWindPortTack = new wxStaticText( m_panel311, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseDownWindPortTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseDownWindPortTack, 0, wxALL, 5 );
	
	m_staticText35113 = new wxStaticText( m_panel311, wxID_ANY, _("Best Course Down Wind Starboard Tack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35113->Wrap( -1 );
	fgSizer2411->Add( m_staticText35113, 0, wxALL, 5 );
	
	m_stBestCourseDownWindStarboardTack = new wxStaticText( m_panel311, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBestCourseDownWindStarboardTack->Wrap( -1 );
	fgSizer2411->Add( m_stBestCourseDownWindStarboardTack, 0, wxALL, 5 );
	
	
	fgSizer1811->Add( fgSizer2411, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer141;
	fgSizer141 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer141->SetFlexibleDirection( wxBOTH );
	fgSizer141->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOptimizeTacking = new wxButton( m_panel311, wxID_ANY, _("Optimize Tacking"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer141->Add( m_bOptimizeTacking, 0, wxALL, 5 );
	
	m_bResetOptimizeTacking = new wxButton( m_panel311, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer141->Add( m_bResetOptimizeTacking, 0, wxALL, 5 );
	
	
	fgSizer1811->Add( fgSizer141, 1, wxEXPAND, 5 );
	
	
	m_panel311->SetSizer( fgSizer1811 );
	m_panel311->Layout();
	fgSizer1811->Fit( m_panel311 );
	m_notebook1->AddPage( m_panel311, _("VMG"), false );
	m_panel3111 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer46;
	fgSizer46 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer46->SetFlexibleDirection( wxBOTH );
	fgSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer58;
	fgSizer58 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer58->SetFlexibleDirection( wxBOTH );
	fgSizer58->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText100 = new wxStaticText( m_panel3111, wxID_ANY, _("type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	fgSizer58->Add( m_staticText100, 0, wxALL, 5 );
	
	wxString m_cHullTypeChoices[] = { _("Mono"), _("Catamaran"), _("Trimaran"), _("Proa"), _("Submarine") };
	int m_cHullTypeNChoices = sizeof( m_cHullTypeChoices ) / sizeof( wxString );
	m_cHullType = new wxChoice( m_panel3111, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), m_cHullTypeNChoices, m_cHullTypeChoices, 0 );
	m_cHullType->SetSelection( 0 );
	fgSizer58->Add( m_cHullType, 0, wxALL, 5 );
	
	m_staticText58 = new wxStaticText( m_panel3111, wxID_ANY, _("disp tons"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer58->Add( m_staticText58, 0, wxALL, 5 );
	
	m_sDisplacement = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxSP_ARROW_KEYS, 0, 100000, 4 );
	fgSizer58->Add( m_sDisplacement, 0, wxALL, 5 );
	
	m_staticText57 = new wxStaticText( m_panel3111, wxID_ANY, _("lwl ft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer58->Add( m_staticText57, 0, wxALL, 5 );
	
	m_sLWL = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 24 );
	fgSizer58->Add( m_sLWL, 0, wxALL, 5 );
	
	
	fgSizer46->Add( fgSizer58, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer14;
	sbSizer14 = new wxStaticBoxSizer( new wxStaticBox( m_panel3111, wxID_ANY, _("Drag") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer57;
	fgSizer57 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer57->SetFlexibleDirection( wxBOTH );
	fgSizer57->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText35 = new wxStaticText( m_panel3111, wxID_ANY, _("Frictional"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer57->Add( m_staticText35, 0, wxALL, 5 );
	
	m_sFrictionalDrag = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer57->Add( m_sFrictionalDrag, 0, wxALL, 5 );
	
	m_staticText112 = new wxStaticText( m_panel3111, wxID_ANY, _("Wake"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText112->Wrap( -1 );
	fgSizer57->Add( m_staticText112, 0, wxALL, 5 );
	
	m_sWakeDrag = new wxSpinCtrl( m_panel3111, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer57->Add( m_sWakeDrag, 0, wxALL, 5 );
	
	m_bRecomputeDrag = new wxButton( m_panel3111, wxID_ANY, _("Recompute"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer57->Add( m_bRecomputeDrag, 0, wxALL, 5 );
	
	m_bDragInfo = new wxButton( m_panel3111, wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer57->Add( m_bDragInfo, 0, wxALL, 5 );
	
	
	sbSizer14->Add( fgSizer57, 1, wxEXPAND, 5 );
	
	
	fgSizer46->Add( sbSizer14, 1, wxEXPAND, 5 );
	
	
	m_panel3111->SetSizer( fgSizer46 );
	m_panel3111->Layout();
	fgSizer46->Fit( m_panel3111 );
	m_notebook1->AddPage( m_panel3111, _("Hull"), false );
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
	
	m_staticText961 = new wxStaticText( m_panel311111, wxID_ANY, _("Eta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText961->Wrap( -1 );
	fgSizer53->Add( m_staticText961, 0, wxALL, 5 );
	
	m_sEta = new wxSlider( m_panel311111, wxID_ANY, 400, 1, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer53->Add( m_sEta, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer49->Add( fgSizer53, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer541;
	fgSizer541 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer541->SetFlexibleDirection( wxBOTH );
	fgSizer541->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText91 = new wxStaticText( m_panel311111, wxID_ANY, _("Max Point Angle"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_bNewBoatPlan = new wxButton( m_panel311111, wxID_ANY, _("New"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_bNewBoatPlan, 0, wxALL, 5 );
	
	m_bEditBoatPlan = new wxButton( m_panel311111, wxID_ANY, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_bEditBoatPlan, 0, wxALL, 5 );
	
	m_bDeleteBoatPlan = new wxButton( m_panel311111, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_bDeleteBoatPlan, 0, wxALL, 5 );
	
	
	fgSizer1811111->Add( fgSizer40, 1, wxEXPAND, 5 );
	
	
	m_panel311111->SetSizer( fgSizer1811111 );
	m_panel311111->Layout();
	fgSizer1811111->Fit( m_panel311111 );
	m_notebook1->AddPage( m_panel311111, _("Sail Plan"), false );
	m_panel5 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer59;
	fgSizer59 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer59->SetFlexibleDirection( wxBOTH );
	fgSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText113 = new wxStaticText( m_panel5, wxID_ANY, _("beam"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113->Wrap( -1 );
	fgSizer59->Add( m_staticText113, 0, wxALL, 5 );
	
	m_sBeam = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 100, 8 );
	fgSizer59->Add( m_sBeam, 0, wxALL, 5 );
	
	m_staticText109 = new wxStaticText( m_panel5, wxID_ANY, _("loa ft"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText109->Wrap( -1 );
	fgSizer59->Add( m_staticText109, 0, wxALL, 5 );
	
	m_sLOA = new wxSpinCtrl( m_panel5, wxID_ANY, wxT("27"), wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0 );
	fgSizer59->Add( m_sLOA, 0, wxALL, 5 );
	
	
	fgSizer52->Add( fgSizer59, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText92 = new wxStaticText( m_panel5, wxID_ANY, _("Hull Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	fgSizer43->Add( m_staticText92, 0, wxALL, 5 );
	
	m_stHullSpeed = new wxStaticText( m_panel5, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stHullSpeed->Wrap( -1 );
	fgSizer43->Add( m_stHullSpeed, 0, wxALL, 5 );
	
	m_staticText94 = new wxStaticText( m_panel5, wxID_ANY, _("Capsize Risk"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText94->Wrap( -1 );
	fgSizer43->Add( m_staticText94, 0, wxALL, 5 );
	
	m_stCapsizeRisk = new wxStaticText( m_panel5, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stCapsizeRisk->Wrap( -1 );
	fgSizer43->Add( m_stCapsizeRisk, 0, wxALL, 5 );
	
	m_staticText96 = new wxStaticText( m_panel5, wxID_ANY, _("Comfort Factor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	fgSizer43->Add( m_staticText96, 0, wxALL, 5 );
	
	m_stComfortFactor = new wxStaticText( m_panel5, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stComfortFactor->Wrap( -1 );
	fgSizer43->Add( m_stComfortFactor, 0, wxALL, 5 );
	
	m_staticText105 = new wxStaticText( m_panel5, wxID_ANY, _("Displacement Length Ratio"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText105->Wrap( -1 );
	fgSizer43->Add( m_staticText105, 0, wxALL, 5 );
	
	m_stDisplacementLengthRatio = new wxStaticText( m_panel5, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stDisplacementLengthRatio->Wrap( -1 );
	fgSizer43->Add( m_stDisplacementLengthRatio, 0, wxALL, 5 );
	
	
	fgSizer52->Add( fgSizer43, 1, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( fgSizer52 );
	m_panel5->Layout();
	fgSizer52->Fit( m_panel5 );
	m_notebook1->AddPage( m_panel5, _("Statistics"), false );
	
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
	m_bOpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSaveAs->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSaveAs ), NULL, this );
	m_bSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_bCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnCancel ), NULL, this );
	m_bOpenCSV->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpenCSV ), NULL, this );
	m_bSaveCSV->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSaveCSV ), NULL, this );
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
	m_bOpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSaveAs->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSaveAs ), NULL, this );
	m_bSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_bCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnCancel ), NULL, this );
	m_bOpenCSV->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpenCSV ), NULL, this );
	m_bSaveCSV->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSaveCSV ), NULL, this );
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
	
	m_staticText87 = new wxStaticText( this, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_bNewSwitchPlanRule = new wxButton( m_panel10, wxID_ANY, _("New"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer46->Add( m_bNewSwitchPlanRule, 0, wxALL, 5 );
	
	m_bEditSwitchBoatPlan = new wxButton( m_panel10, wxID_ANY, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer46->Add( m_bEditSwitchBoatPlan, 0, wxALL, 5 );
	
	m_bDeleteSwitchBoatPlan = new wxButton( m_panel10, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer46->Add( m_bDeleteSwitchBoatPlan, 0, wxALL, 5 );
	
	
	fgSizer45->Add( fgSizer46, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( fgSizer45 );
	m_panel10->Layout();
	fgSizer45->Fit( m_panel10 );
	m_notebook2->AddPage( m_panel10, _("Switch Plan Rules"), false );
	
	fgSizer41->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bDone = new wxButton( this, wxID_ANY, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
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
	sbSizer15 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Conditions") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbMaxWindSpeed = new wxCheckBox( this, wxID_ANY, _("Wind Speed >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMaxWindSpeed, 0, wxALL, 5 );
	
	m_sMaxWindSpeed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer52->Add( m_sMaxWindSpeed, 0, wxALL, 5 );
	
	m_staticText101 = new wxStaticText( this, wxID_ANY, _("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer52->Add( m_staticText101, 0, wxALL, 5 );
	
	m_cbMinWindSpeed = new wxCheckBox( this, wxID_ANY, _("Wind Speed <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMinWindSpeed, 0, wxALL, 5 );
	
	m_sMinWindSpeed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	fgSizer52->Add( m_sMinWindSpeed, 0, wxALL, 5 );
	
	m_staticText102 = new wxStaticText( this, wxID_ANY, _("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	fgSizer52->Add( m_staticText102, 0, wxALL, 5 );
	
	m_cbMaxWindDirection = new wxCheckBox( this, wxID_ANY, _("Wind Direction >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMaxWindDirection, 0, wxALL, 5 );
	
	m_sMaxWindDirection = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 0 );
	fgSizer52->Add( m_sMaxWindDirection, 0, wxALL, 5 );
	
	m_staticText103 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText103->Wrap( -1 );
	fgSizer52->Add( m_staticText103, 0, wxALL, 5 );
	
	m_cbMinWindDirection = new wxCheckBox( this, wxID_ANY, _("Wind Direction <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMinWindDirection, 0, wxALL, 5 );
	
	m_sMinWindDirection = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 0 );
	fgSizer52->Add( m_sMinWindDirection, 0, wxALL, 5 );
	
	m_staticText104 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText104->Wrap( -1 );
	fgSizer52->Add( m_staticText104, 0, wxALL, 5 );
	
	m_cbMaxWaveHeight = new wxCheckBox( this, wxID_ANY, _("Wave Height >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMaxWaveHeight, 0, wxALL, 5 );
	
	m_sMaxWaveHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer52->Add( m_sMaxWaveHeight, 0, wxALL, 5 );
	
	m_staticText105 = new wxStaticText( this, wxID_ANY, _("Meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText105->Wrap( -1 );
	fgSizer52->Add( m_staticText105, 0, wxALL, 5 );
	
	m_cbMinWaveHeight = new wxCheckBox( this, wxID_ANY, _("Wave Height <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer52->Add( m_cbMinWaveHeight, 0, wxALL, 5 );
	
	m_sMinWaveHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer52->Add( m_sMinWaveHeight, 0, wxALL, 5 );
	
	m_staticText106 = new wxStaticText( this, wxID_ANY, _("Meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText106->Wrap( -1 );
	fgSizer52->Add( m_staticText106, 0, wxALL, 5 );
	
	m_cbDayTime = new wxCheckBox( this, wxID_ANY, _("Day Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbDayTime->SetValue(true); 
	fgSizer52->Add( m_cbDayTime, 0, wxALL, 5 );
	
	m_cbNightTime = new wxCheckBox( this, wxID_ANY, _("Night Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbNightTime->SetValue(true); 
	fgSizer52->Add( m_cbNightTime, 0, wxALL, 5 );
	
	
	sbSizer15->Add( fgSizer52, 1, wxEXPAND, 5 );
	
	
	fgSizer55->Add( sbSizer15, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer56;
	fgSizer56 = new wxFlexGridSizer( 2, 0, 0, 0 );
	fgSizer56->SetFlexibleDirection( wxBOTH );
	fgSizer56->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText107 = new wxStaticText( this, wxID_ANY, _("Then Switch to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText107->Wrap( -1 );
	fgSizer56->Add( m_staticText107, 0, wxALL, 5 );
	
	wxArrayString m_cPlansChoices;
	m_cPlans = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cPlansChoices, 0 );
	m_cPlans->SetSelection( 0 );
	fgSizer56->Add( m_cPlans, 0, wxALL, 5 );
	
	
	fgSizer56->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_bDone = new wxButton( this, wxID_ANY, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer56->Add( m_bDone, 0, wxALL, 5 );
	
	m_bCancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer56->Add( m_bCancel, 0, wxALL, 5 );
	
	m_bInformation = new wxButton( this, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_staticText138 = new wxStaticText( this, wxID_ANY, _("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText138->Wrap( -1 );
	fgSizer77->Add( m_staticText138, 0, wxALL, 5 );
	
	m_sPosition = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer77->Add( m_sPosition, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText139 = new wxStaticText( this, wxID_ANY, _("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText139->Wrap( -1 );
	fgSizer77->Add( m_staticText139, 0, wxALL, 5 );
	
	m_sScale = new wxSlider( this, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer77->Add( m_sScale, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer77->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer78;
	fgSizer78 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer78->SetFlexibleDirection( wxBOTH );
	fgSizer78->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText140 = new wxStaticText( this, wxID_ANY, _("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText140->Wrap( -1 );
	m_staticText140->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer78->Add( m_staticText140, 0, wxALL, 5 );
	
	wxString m_cVariable1Choices[] = { _("Boat Velocity Ground"), _("Boat Course Ground"), _("Boat Velocity Water"), _("Boat Course Water"), _("Wind Velocity"), _("Wind Direction"), _("Wind Course"), _("Wind Velocity Ground"), _("Wind Direction Ground"), _("Wind Course Ground"), _("Apparent Wind Velocity"), _("Apparent Wind Direction"), _("Current Velocity"), _("Current Direction"), _("Sig Wave Height") };
	int m_cVariable1NChoices = sizeof( m_cVariable1Choices ) / sizeof( wxString );
	m_cVariable1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariable1NChoices, m_cVariable1Choices, 0 );
	m_cVariable1->SetSelection( 3 );
	fgSizer78->Add( m_cVariable1, 0, wxALL, 5 );
	
	m_stMousePosition1 = new wxStaticText( this, wxID_ANY, _("        N/A         "), wxDefaultPosition, wxDefaultSize, 0 );
	m_stMousePosition1->Wrap( -1 );
	fgSizer78->Add( m_stMousePosition1, 0, wxALL, 5 );
	
	m_staticText1401 = new wxStaticText( this, wxID_ANY, _("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1401->Wrap( -1 );
	m_staticText1401->SetForegroundColour( wxColour( 0, 255, 0 ) );
	
	fgSizer78->Add( m_staticText1401, 0, wxALL, 5 );
	
	wxString m_cVariable2Choices[] = { _("Boat Velocity Ground"), _("Boat Course Ground"), _("Boat Velocity Water"), _("Boat Course Water"), _("Wind Velocity"), _("Wind Direction"), _("Wind Course"), _("Wind Velocity Ground"), _("Wind Direction Ground"), _("Wind Course Ground"), _("Apparent Wind Velocity"), _("Apparent Wind Direction"), _("Current Velocity"), _("Current Direction"), _("Sig Wave Height") };
	int m_cVariable2NChoices = sizeof( m_cVariable2Choices ) / sizeof( wxString );
	m_cVariable2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariable2NChoices, m_cVariable2Choices, 0 );
	m_cVariable2->SetSelection( 5 );
	fgSizer78->Add( m_cVariable2, 0, wxALL, 5 );
	
	m_stMousePosition2 = new wxStaticText( this, wxID_ANY, _("        N/A         "), wxDefaultPosition, wxDefaultSize, 0 );
	m_stMousePosition2->Wrap( -1 );
	fgSizer78->Add( m_stMousePosition2, 0, wxALL, 5 );
	
	m_staticText14011 = new wxStaticText( this, wxID_ANY, _("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14011->Wrap( -1 );
	m_staticText14011->SetForegroundColour( wxColour( 0, 0, 255 ) );
	
	fgSizer78->Add( m_staticText14011, 0, wxALL, 5 );
	
	wxString m_cVariable3Choices[] = { _("Boat Velocity Ground"), _("Boat Course Ground"), _("Boat Velocity Water"), _("Boat Course Water"), _("Wind Velocity"), _("Wind Direction"), _("Wind Course"), _("Wind Velocity Ground"), _("Wind Direction Ground"), _("Wind Course Ground"), _("Apparent Wind Velocity"), _("Apparent Wind Direction"), _("Current Velocity"), _("Current Direction"), _("Sig Wave Height") };
	int m_cVariable3NChoices = sizeof( m_cVariable3Choices ) / sizeof( wxString );
	m_cVariable3 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariable3NChoices, m_cVariable3Choices, 0 );
	m_cVariable3->SetSelection( 11 );
	fgSizer78->Add( m_cVariable3, 0, wxALL, 5 );
	
	m_stMousePosition3 = new wxStaticText( this, wxID_ANY, _("        N/A         "), wxDefaultPosition, wxDefaultSize, 0 );
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
	m_sdbSizer2->Realize();
	
	fgSizer15->Add( m_sdbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer15 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

InformationDialog::~InformationDialog()
{
}

AboutDialogBase::AboutDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer90;
	fgSizer90 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer90->SetFlexibleDirection( wxBOTH );
	fgSizer90->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText110 = new wxStaticText( this, wxID_ANY, _("The weather routing plugin for opencpn is intended to aid in planning sailing routes based on computerized weather data and boat sailing ability.\n\nThis also means to educate users about weather trends, to \"discover\" new cruising routes, and to avoid adverse conditions.\n\nLicense: GPLv3+\n\nSource Code:\nhttps://github.com/seandepagnier/weather_routing_pi\n\nAuthor:\nSean D'Epagnier\n\nMany thanks to all of the translators and testers."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( 400 );
	fgSizer90->Add( m_staticText110, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bDonate = new wxButton( this, wxID_ANY, _("Donate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_bDonate, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer90->Add( fgSizer91, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer90 );
	this->Layout();
	fgSizer90->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bDonate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnDonate ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnClose ), NULL, this );
}

AboutDialogBase::~AboutDialogBase()
{
	// Disconnect Events
	m_bDonate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnDonate ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnClose ), NULL, this );
	
}

StatisticsDialogBase::StatisticsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer55;
	fgSizer55 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer55->SetFlexibleDirection( wxBOTH );
	fgSizer55->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer27;
	sbSizer27 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Overall") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer94;
	fgSizer94 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer94->SetFlexibleDirection( wxBOTH );
	fgSizer94->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText511 = new wxStaticText( this, wxID_ANY, _("Run Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText511->Wrap( -1 );
	fgSizer94->Add( m_staticText511, 0, wxALL, 5 );
	
	m_stRunTime = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stRunTime->Wrap( -1 );
	fgSizer94->Add( m_stRunTime, 0, wxALL, 5 );
	
	
	sbSizer27->Add( fgSizer94, 1, wxEXPAND, 5 );
	
	
	fgSizer55->Add( sbSizer27, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Current Route") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText116 = new wxStaticText( this, wxID_ANY, _("Upwind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText116->Wrap( -1 );
	fgSizer29->Add( m_staticText116, 0, wxALL, 5 );
	
	m_stPercentageUpwind = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stPercentageUpwind->Wrap( -1 );
	fgSizer29->Add( m_stPercentageUpwind, 0, wxALL, 5 );
	
	m_staticText125 = new wxStaticText( this, wxID_ANY, _("Port/Starboard"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText125->Wrap( -1 );
	fgSizer29->Add( m_staticText125, 0, wxALL, 5 );
	
	m_stPortStarboard = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stPortStarboard->Wrap( -1 );
	fgSizer29->Add( m_stPortStarboard, 0, wxALL, 5 );
	
	m_staticText121 = new wxStaticText( this, wxID_ANY, _("Average Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	fgSizer29->Add( m_staticText121, 0, wxALL, 5 );
	
	m_stAverageWindKnots = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stAverageWindKnots->Wrap( -1 );
	fgSizer29->Add( m_stAverageWindKnots, 0, wxALL, 5 );
	
	m_staticText123 = new wxStaticText( this, wxID_ANY, _("Average Wave (M)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText123->Wrap( -1 );
	fgSizer29->Add( m_staticText123, 0, wxALL, 5 );
	
	m_stAverageWaveHeight = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stAverageWaveHeight->Wrap( -1 );
	fgSizer29->Add( m_stAverageWaveHeight, 0, wxALL, 5 );
	
	m_staticText47 = new wxStaticText( this, wxID_ANY, _("State"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer29->Add( m_staticText47, 0, wxALL, 5 );
	
	m_stState = new wxStaticText( this, wxID_ANY, _("Stopped"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stState->Wrap( -1 );
	fgSizer29->Add( m_stState, 0, wxALL, 5 );
	
	m_staticText53 = new wxStaticText( this, wxID_ANY, _("Iso Chrons"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer29->Add( m_staticText53, 0, wxALL, 5 );
	
	m_stIsoChrons = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stIsoChrons->Wrap( -1 );
	fgSizer29->Add( m_stIsoChrons, 0, wxALL, 5 );
	
	m_staticText55 = new wxStaticText( this, wxID_ANY, _("Routes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer29->Add( m_staticText55, 0, wxALL, 5 );
	
	m_stRoutes = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stRoutes->Wrap( -1 );
	fgSizer29->Add( m_stRoutes, 0, wxALL, 5 );
	
	m_staticText57 = new wxStaticText( this, wxID_ANY, _("InvRoutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer29->Add( m_staticText57, 0, wxALL, 5 );
	
	m_stInvRoutes = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stInvRoutes->Wrap( -1 );
	fgSizer29->Add( m_stInvRoutes, 0, wxALL, 5 );
	
	m_staticText90 = new wxStaticText( this, wxID_ANY, _("Skip Positions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText90->Wrap( -1 );
	fgSizer29->Add( m_staticText90, 0, wxALL, 5 );
	
	m_stSkipPositions = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stSkipPositions->Wrap( -1 );
	fgSizer29->Add( m_stSkipPositions, 0, wxALL, 5 );
	
	m_staticText49 = new wxStaticText( this, wxID_ANY, _("Positions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer29->Add( m_staticText49, 0, wxALL, 5 );
	
	m_stPositions = new wxStaticText( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
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

FilterRoutesDialogBase::FilterRoutesDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer95;
	fgSizer95 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer95->SetFlexibleDirection( wxBOTH );
	fgSizer95->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer29;
	sbSizer29 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Category") ), wxVERTICAL );
	
	wxArrayString m_cCategoryChoices;
	m_cCategory = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cCategoryChoices, 0 );
	m_cCategory->SetSelection( 0 );
	sbSizer29->Add( m_cCategory, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer95->Add( sbSizer29, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer28;
	sbSizer28 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Filter") ), wxVERTICAL );
	
	m_tFilter = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer28->Add( m_tFilter, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer95->Add( sbSizer28, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer99;
	fgSizer99 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer99->SetFlexibleDirection( wxBOTH );
	fgSizer99->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button48 = new wxButton( this, wxID_ANY, _("Reset All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer99->Add( m_button48, 0, wxALL, 5 );
	
	m_bDone = new wxButton( this, wxID_ANY, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer99->Add( m_bDone, 0, wxALL, 5 );
	
	
	fgSizer95->Add( fgSizer99, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer95 );
	this->Layout();
	fgSizer95->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cCategory->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FilterRoutesDialogBase::OnCategory ), NULL, this );
	m_tFilter->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FilterRoutesDialogBase::OnFilterText ), NULL, this );
	m_button48->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterRoutesDialogBase::OnResetAll ), NULL, this );
	m_bDone->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterRoutesDialogBase::OnDone ), NULL, this );
}

FilterRoutesDialogBase::~FilterRoutesDialogBase()
{
	// Disconnect Events
	m_cCategory->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FilterRoutesDialogBase::OnCategory ), NULL, this );
	m_tFilter->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FilterRoutesDialogBase::OnFilterText ), NULL, this );
	m_button48->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterRoutesDialogBase::OnResetAll ), NULL, this );
	m_bDone->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterRoutesDialogBase::OnDone ), NULL, this );
	
}

ConfigurationBatchDialogBase::ConfigurationBatchDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer76;
	fgSizer76 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer76->SetFlexibleDirection( wxBOTH );
	fgSizer76->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebookConfigurations = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel8 = new wxPanel( m_notebookConfigurations, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer77;
	fgSizer77 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer77->SetFlexibleDirection( wxBOTH );
	fgSizer77->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText108 = new wxStaticText( m_panel8, wxID_ANY, _("Starting"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText108->Wrap( -1 );
	fgSizer77->Add( m_staticText108, 0, wxALL, 5 );
	
	m_stStartDateTime = new wxStaticText( m_panel8, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stStartDateTime->Wrap( -1 );
	fgSizer77->Add( m_stStartDateTime, 0, wxALL, 5 );
	
	
	fgSizer77->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText121 = new wxStaticText( m_panel8, wxID_ANY, _("For"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	fgSizer77->Add( m_staticText121, 0, wxALL, 5 );
	
	m_tStartDays = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer77->Add( m_tStartDays, 0, wxALL, 5 );
	
	m_staticText122 = new wxStaticText( m_panel8, wxID_ANY, _("day(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText122->Wrap( -1 );
	fgSizer77->Add( m_staticText122, 0, wxALL, 5 );
	
	
	fgSizer77->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_tStartHours = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer77->Add( m_tStartHours, 0, wxALL, 5 );
	
	m_staticText123 = new wxStaticText( m_panel8, wxID_ANY, _("hour(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText123->Wrap( -1 );
	fgSizer77->Add( m_staticText123, 0, wxALL, 5 );
	
	m_staticText124 = new wxStaticText( m_panel8, wxID_ANY, _("Spaced"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText124->Wrap( -1 );
	fgSizer77->Add( m_staticText124, 0, wxALL, 5 );
	
	m_tStartSpacingDays = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer77->Add( m_tStartSpacingDays, 0, wxALL, 5 );
	
	m_staticText125 = new wxStaticText( m_panel8, wxID_ANY, _("day(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText125->Wrap( -1 );
	fgSizer77->Add( m_staticText125, 0, wxALL, 5 );
	
	
	fgSizer77->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_tStartSpacingHours = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer77->Add( m_tStartSpacingHours, 0, wxALL, 5 );
	
	m_staticText126 = new wxStaticText( m_panel8, wxID_ANY, _("Hour(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText126->Wrap( -1 );
	fgSizer77->Add( m_staticText126, 0, wxALL, 5 );
	
	
	m_panel8->SetSizer( fgSizer77 );
	m_panel8->Layout();
	fgSizer77->Fit( m_panel8 );
	m_notebookConfigurations->AddPage( m_panel8, _("Start Time"), false );
	m_pRoutes = new wxPanel( m_notebookConfigurations, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer62;
	fgSizer62 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer62->AddGrowableCol( 0 );
	fgSizer62->SetFlexibleDirection( wxBOTH );
	fgSizer62->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( m_pRoutes, wxID_ANY, _("Sources") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer64;
	fgSizer64 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer64->AddGrowableCol( 0 );
	fgSizer64->AddGrowableCol( 1 );
	fgSizer64->AddGrowableRow( 0 );
	fgSizer64->SetFlexibleDirection( wxBOTH );
	fgSizer64->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer66;
	fgSizer66 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer66->AddGrowableCol( 0 );
	fgSizer66->AddGrowableRow( 0 );
	fgSizer66->SetFlexibleDirection( wxBOTH );
	fgSizer66->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lSources = new wxListBox( m_pRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer66->Add( m_lSources, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer64->Add( fgSizer66, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer19;
	sbSizer19 = new wxStaticBoxSizer( new wxStaticBox( m_pRoutes, wxID_ANY, _("Destinations") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer72;
	fgSizer72 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer72->AddGrowableCol( 0 );
	fgSizer72->AddGrowableRow( 0 );
	fgSizer72->SetFlexibleDirection( wxBOTH );
	fgSizer72->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lDestinations = new wxListBox( m_pRoutes, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE ); 
	m_lDestinations->SetMinSize( wxSize( 100,-1 ) );
	
	fgSizer72->Add( m_lDestinations, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer19->Add( fgSizer72, 1, wxEXPAND, 5 );
	
	
	fgSizer64->Add( sbSizer19, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer28;
	sbSizer28 = new wxStaticBoxSizer( new wxStaticBox( m_pRoutes, wxID_ANY, _("Auto Connect") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer97;
	fgSizer97 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer97->SetFlexibleDirection( wxBOTH );
	fgSizer97->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1241 = new wxStaticText( m_pRoutes, wxID_ANY, _("Within"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1241->Wrap( -1 );
	fgSizer97->Add( m_staticText1241, 0, wxALL, 5 );
	
	m_tMiles = new wxTextCtrl( m_pRoutes, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer97->Add( m_tMiles, 0, wxALL, 5 );
	
	m_staticText1251 = new wxStaticText( m_pRoutes, wxID_ANY, _("Nautical Miles"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1251->Wrap( -1 );
	fgSizer97->Add( m_staticText1251, 0, wxALL, 5 );
	
	m_bConnect = new wxButton( m_pRoutes, wxID_ANY, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer97->Add( m_bConnect, 0, wxALL, 5 );
	
	
	sbSizer28->Add( fgSizer97, 1, wxEXPAND, 5 );
	
	
	fgSizer64->Add( sbSizer28, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer67;
	fgSizer67 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer67->SetFlexibleDirection( wxBOTH );
	fgSizer67->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bDisconnectAll = new wxButton( m_pRoutes, wxID_ANY, _("Disconnect All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer67->Add( m_bDisconnectAll, 0, wxALL, 5 );
	
	
	fgSizer64->Add( fgSizer67, 1, wxEXPAND, 5 );
	
	
	sbSizer11->Add( fgSizer64, 1, wxEXPAND, 5 );
	
	
	fgSizer62->Add( sbSizer11, 1, wxEXPAND, 5 );
	
	
	m_pRoutes->SetSizer( fgSizer62 );
	m_pRoutes->Layout();
	fgSizer62->Fit( m_pRoutes );
	m_notebookConfigurations->AddPage( m_pRoutes, _("Routes"), true );
	m_panel9 = new wxPanel( m_notebookConfigurations, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer81;
	fgSizer81 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer81->AddGrowableCol( 0 );
	fgSizer81->AddGrowableRow( 0 );
	fgSizer81->SetFlexibleDirection( wxBOTH );
	fgSizer81->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lBoats = new wxListBox( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer81->Add( m_lBoats, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer82;
	fgSizer82 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer82->SetFlexibleDirection( wxBOTH );
	fgSizer82->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bAddBoat = new wxButton( m_panel9, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_bAddBoat, 0, wxALL, 5 );
	
	m_bRemoveBoat = new wxButton( m_panel9, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_bRemoveBoat, 0, wxALL, 5 );
	
	
	fgSizer81->Add( fgSizer82, 1, wxEXPAND, 5 );
	
	
	m_panel9->SetSizer( fgSizer81 );
	m_panel9->Layout();
	fgSizer81->Fit( m_panel9 );
	m_notebookConfigurations->AddPage( m_panel9, _("Boats"), false );
	
	fgSizer76->Add( m_notebookConfigurations, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer78;
	fgSizer78 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer78->SetFlexibleDirection( wxBOTH );
	fgSizer78->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bInformation = new wxButton( this, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer78->Add( m_bInformation, 0, wxALL, 5 );
	
	m_bReset = new wxButton( this, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer78->Add( m_bReset, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer78->Add( m_bClose, 0, wxALL, 5 );
	
	m_bGenerate = new wxButton( this, wxID_ANY, _("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer78->Add( m_bGenerate, 0, wxALL, 5 );
	
	
	fgSizer76->Add( fgSizer78, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer76 );
	this->Layout();
	fgSizer76->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_lSources->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnSources ), NULL, this );
	m_lDestinations->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnDestinations ), NULL, this );
	m_bConnect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnConnect ), NULL, this );
	m_bDisconnectAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnDisconnectAll ), NULL, this );
	m_bAddBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnAddBoat ), NULL, this );
	m_bRemoveBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnRemoveBoat ), NULL, this );
	m_bInformation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnInformation ), NULL, this );
	m_bReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnReset ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnClose ), NULL, this );
	m_bGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnGenerate ), NULL, this );
}

ConfigurationBatchDialogBase::~ConfigurationBatchDialogBase()
{
	// Disconnect Events
	m_lSources->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnSources ), NULL, this );
	m_lDestinations->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnDestinations ), NULL, this );
	m_bConnect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnConnect ), NULL, this );
	m_bDisconnectAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnDisconnectAll ), NULL, this );
	m_bAddBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnAddBoat ), NULL, this );
	m_bRemoveBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnRemoveBoat ), NULL, this );
	m_bInformation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnInformation ), NULL, this );
	m_bReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnReset ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnClose ), NULL, this );
	m_bGenerate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationBatchDialogBase::OnGenerate ), NULL, this );
	
}
