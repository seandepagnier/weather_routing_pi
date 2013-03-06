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
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxVERTICAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Start") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Lat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer6->Add( m_staticText5, 0, wxALL, 5 );
	
	m_tStartLat = new wxTextCtrl( this, wxID_ANY, wxT("-35"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_tStartLat, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Lon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer6->Add( m_staticText6, 0, wxALL, 5 );
	
	m_tStartLon = new wxTextCtrl( this, wxID_ANY, wxT("174"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_tStartLon, 0, wxALL, 5 );
	
	
	sbSizer3->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("End") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 2, 0, 0 );
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
	
	
	fgSizer1->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	m_bCompute = new wxButton( this, wxID_ANY, wxT("Compute"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bCompute, 0, wxALL, 5 );
	
	m_bStep = new wxButton( this, wxID_ANY, wxT("Step"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bStep, 0, wxALL, 5 );
	
	m_bBoat = new wxButton( this, wxID_ANY, wxT("Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bBoat, 0, wxALL, 5 );
	
	m_bClear = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bClear, 0, wxALL, 5 );
	
	m_bAbout = new wxButton( this, wxID_ANY, wxT("About"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bAbout, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bCompute->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnCompute ), NULL, this );
	m_bStep->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnStep ), NULL, this );
	m_bBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnBoat ), NULL, this );
	m_bClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnClear ), NULL, this );
	m_bAbout->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnAbout ), NULL, this );
}

WeatherRoutingDialogBase::~WeatherRoutingDialogBase()
{
	// Disconnect Events
	m_bCompute->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnCompute ), NULL, this );
	m_bStep->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnStep ), NULL, this );
	m_bBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnBoat ), NULL, this );
	m_bClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnClear ), NULL, this );
	m_bAbout->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WeatherRoutingDialogBase::OnAbout ), NULL, this );
	
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
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOpen = new wxButton( m_panel3, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bOpen, 0, wxALL, 5 );
	
	m_bSave = new wxButton( m_panel3, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bSave, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( fgSizer18 );
	m_panel3->Layout();
	fgSizer18->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Plot"), true );
	m_panel5 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText12 = new wxStaticText( m_panel5, wxID_ANY, wxT("Eta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer10->Add( m_staticText12, 0, wxALL, 5 );
	
	m_sEta = new wxSlider( m_panel5, wxID_ANY, 500, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sEta, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText13 = new wxStaticText( m_panel5, wxID_ANY, wxT("Keel Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer10->Add( m_staticText13, 0, wxALL, 5 );
	
	m_sKeelPressure = new wxSlider( m_panel5, wxID_ANY, 30, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sKeelPressure, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText14 = new wxStaticText( m_panel5, wxID_ANY, wxT("Keel Lift"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer10->Add( m_staticText14, 0, wxALL, 5 );
	
	m_sKeelLift = new wxSlider( m_panel5, wxID_ANY, 10, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer10->Add( m_sKeelLift, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_bOptimizeTacking = new wxButton( m_panel5, wxID_ANY, wxT("Optimize Tacking"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_bOptimizeTacking, 0, wxALL, 5 );
	
	
	m_panel5->SetSizer( fgSizer10 );
	m_panel5->Layout();
	fgSizer10->Fit( m_panel5 );
	m_notebook1->AddPage( m_panel5, wxT("Compute"), false );
	
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
	m_bOpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
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
	m_bOpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnOpen ), NULL, this );
	m_bSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoatDialogBase::OnSave ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sEta->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
	m_sKeelPressure->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BoatDialogBase::OnRecompute ), NULL, this );
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
