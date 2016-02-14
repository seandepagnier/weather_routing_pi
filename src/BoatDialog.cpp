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
 */

#include <wx/wx.h>
#include <wx/dcgraph.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "weather_routing_pi.h"
#include "Utilities.h"
#include "Boat.h"
#include "BoatDialog.h"

enum {spFILENAME, spWAVEPOLARFILENAME};

// for plotting
static const int wind_speeds[] = {0, 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 36, 40, 45, 50, 55, 60};
static const int num_wind_speeds = (sizeof wind_speeds) / (sizeof *wind_speeds);

BoatDialog::BoatDialog(wxWindow *parent, wxString boatpath)
    : BoatDialogBase(parent), m_boatpath(boatpath), m_PlotScale(0)
{
    // for small screens: don't let boat dialog be larger than screen
    int w,h;
    wxDisplaySize( &w, &h );
    w = wxMin(w, GetMinWidth());
    h = wxMin(h-32, GetMinHeight());
    SetMinSize(wxSize(w, h));
    SetSize(wxSize(w, h));

    m_lPolars->InsertColumn(spFILENAME, _("Filename"));
    m_lPolars->InsertColumn(spWAVEPOLARFILENAME, _("Wave Polar Filename"));

    wxString error = m_Boat.OpenXML(m_boatpath);
    RepopulatePolars();

    if(error.size()) {
        wxMessageDialog md(this, error,
                           _("OpenCPN Weather Routing Plugin"),
                            wxICON_ERROR | wxOK );
        md.ShowModal();
    }

//    m_lBoatPlans->SetItemState(m_SelectedSailPlan, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

    UpdateVMG();

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

    // hack to adjust items
    SetSize(wxSize(w, h));
}

void BoatDialog::OnMouseEventsPolarPlot( wxMouseEvent& event )
{
#if 0
    if(event.Leaving()) {
        m_stTrueWindAngle->SetLabel(_("N/A"));
        m_stTrueWindKnots->SetLabel(_("N/A"));
        m_stApparentWindAngle->SetLabel(_("N/A"));
        m_stApparentWindKnots->SetLabel(_("N/A"));
        m_stBoatAngle->SetLabel(_("N/A"));
        m_stBoatKnots->SetLabel(_("N/A"));
        return;
    }

    wxPoint p = event.GetPosition();
    int w, h;
    m_PlotWindow->GetSize( &w, &h);

    /* range + to - */
    double W, VW, B, VB, A, VA;
    double windspeed;

    switch(m_lPlotType->GetSelection()) {
    case 0:
        if(m_cPlotType->GetSelection() == 0) { // polar
            if(!m_PlotScale)
                return;
            
            double x = (double)p.x - w/2;
            double y = (double)p.y - h/2;
            
            /* range +- */
            x /= m_PlotScale;
            y /= m_PlotScale;
            
            B = rad2posdeg(atan2(x, -y));
        } else
            B = (double)p.x/w*360;
        
        windspeed = m_sWindSpeed->GetValue();
        break;
    case 1:
    {
        B = m_sWindDirection->GetValue();
        double i = (double)p.x/w*num_wind_speeds;
        int i0 = floor(i), i1 = ceil(i);
        double d = i - i0;
        windspeed = (1-d)*wind_speeds[i0] + d*wind_speeds[i1];
    } break;
    }

    switch(m_cPlotVariable->GetSelection()) {
    case 0: // true wind
        W = B;
        VW = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].Speed(W, VW);

        VA = BoatPlan::VelocityApparentWind(VB, deg2rad(W), VW);
        A = rad2posdeg(BoatPlan::DirectionApparentWind(VA, VB, deg2rad(W), VW));
        break;
    case 1:
        A = heading_resolve(B);
        VW = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].SpeedAtApparentWindDirection(A, VW, &W);
        W = positive_degrees(W);

        VA = BoatPlan::VelocityApparentWind(VB, deg2rad(W), VW);
        break;
    case 2:
        W = B;
        VA = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].SpeedAtApparentWindSpeed(W, VA);
        VW = BoatPlan::VelocityTrueWind(VA, VB, deg2rad(W));
        A = rad2posdeg(BoatPlan::DirectionApparentWind(VA, VB, deg2rad(W), VW));
        break;
    case 3:
        A = heading_resolve(B);
        VA = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].SpeedAtApparentWind(A, VA, &W);
        W = positive_degrees(W);
        VW = BoatPlan::VelocityTrueWind(VA, VB, deg2rad(W));
    }

    m_stBoatAngle->SetLabel(wxString::Format(_T("%03.0f"), B));
    m_stBoatKnots->SetLabel(wxString::Format(_T("%.1f"), VB));

    int newmousew = round(B);
    if(newmousew != m_MouseW) {
        m_MouseW = newmousew;
        RefreshPlots();
    }
    
    m_stTrueWindAngle->SetLabel(wxString::Format(_T("%03.0f"), W));
    m_stTrueWindKnots->SetLabel(wxString::Format(_T("%.1f"), VW));

    m_stApparentWindAngle->SetLabel(wxString::Format(_T("%03.0f"), A));

    m_stApparentWindKnots->SetLabel(wxString::Format(_T("%.1f"), VA));
#endif
}

#if 0
void BoatDialog::PlotVMG(wxPaintDC &dc, double W, double s)
{
    if(isnan(W))
        return;

    int w, h;
    m_PlotWindow->GetSize( &w, &h);

    double H;
    if((m_cPlotVariable->GetSelection() & 1) == 0) // true wind
        H = W;
    else { // apparent wind
        Polar &polar = m_Boat.Polars[m_SelectedPolar];
        int VW = m_sWindSpeed->GetValue();
        H = rad2posdeg(BoatPlan::DirectionApparentWind(plan.Speed(W, VW), deg2rad(W), VW));
    }

    if(m_cPlotType->GetSelection() == 0) { // polar
        int px = s*sin(deg2rad(H));
        int py = s*cos(deg2rad(H));
        dc.DrawLine(w/2, h/2, w/2 + px, h/2 - py);
    } else {
        int px = H * w / DEGREES;
        dc.DrawLine(px, 0, px, h);
    }
}
#endif

static void PlotPolarData(wxDC &dc, double *values, int count, double scale, int w, int h, bool full)
{
    int lx, ly;
    bool lastvalid = false;
    int total = full ? count + 1 : count / 2;
    for(int i = 0; i<total; i++) {
        int j = i%count;
        if(isnan(values[j])) {
            lastvalid = false;
            continue;
        }

        int px =  scale*values[j]*sin(deg2rad(i)) + (full ? w/2 : 0);
        int py = -scale*values[j]*cos(deg2rad(i)) + h/2;

        if(lastvalid)
            dc.DrawLine(lx, ly, px, py);

        lx = px, ly = py;
        lastvalid = true;
    }
}

static void PlotRectangularData(wxDC &dc, double *values, int count, double scale, int w, int h)
{
    int lx, ly;
    bool lastvalid = false;
    for(int i = 0; i<count; i++) {
        if(isnan(values[i])) {
            lastvalid = false;
            continue;
        }

        int px = i * w / count, py = h - scale*values[i];

        if(lastvalid)
            dc.DrawLine(lx, ly, px, py);

        lx = px, ly = py;
        lastvalid = true;
    }
}

void BoatDialog::PaintPolar(wxPaintDC &dc, long index)
{
    int w, h;
    m_PlotWindow->GetSize( &w, &h);
    double maxVB = 0;

    Polar &polar = m_Boat.Polars[index];
    int windspeed = 15; //m_sWindSpeed->GetValue();

    int H, i;
    /* plot scale */
    int selection = m_cPlotVariable->GetSelection();

    for(H = 0, i=0; H<DEGREES; H += 3, i++) {
        double VB;
        if(selection < 2)
            VB = polar.Speed(H, windspeed);
        else
            VB = polar.SpeedAtApparentWindSpeed(H, windspeed);

        if(VB > maxVB)
            maxVB = VB;
    }
    
    dc.SetPen(wxPen(wxColor(0, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(wxColour(0, 55, 75));

    if(maxVB <= 0) maxVB = 1; /* avoid lock */
    double Vstep = ceil(maxVB / 5);
    maxVB += Vstep;

    m_PlotScale = (w < h ? w : h)/1.8 / (maxVB+1);

    bool full = m_cbFullPlot->GetValue();
    int xc = full ? w / 2 : 0;

    /* polar circles */
    for(double V = Vstep; V <= maxVB; V+=Vstep) {
        dc.DrawCircle(xc, h/2, V*m_PlotScale);
        dc.DrawText(wxString::Format(_T("%.0f"), V), xc, h/2+(int)V*m_PlotScale);
    }

    /* polar meridians */
    dc.SetTextForeground(wxColour(0, 0, 155));
    for(double B = 0; B < DEGREES; B+=15) {
        double x = maxVB*m_PlotScale*sin(deg2rad(B));
        double y = maxVB*m_PlotScale*cos(deg2rad(B));
        if(B < 180)
            dc.DrawLine(xc - x, h/2 + y, xc + x, h/2 - y);

        wxString str = wxString::Format(_T("%.0f"), B);
        int sw, sh;
        dc.GetTextExtent(str, &sw, &sh);
        dc.DrawText(str, xc + .9*x - sw/2, h/2 - .9*y - sh/2);
    }

    /* vmg */
//    double s = maxVB*m_PlotScale;
//    SailingVMG vmg = selection < 2 ? polar.GetVMGTrueWind(windspeed)
//                                 : polar.GetVMGApparentWind(windspeed);

    dc.SetPen(wxPen(wxColor(255, 0, 255), 2));

//    PlotVMG(dc, vmg.values[SailingVMG::PORT_UPWIND], s);
//    PlotVMG(dc, vmg.values[SailingVMG::STARBOARD_UPWIND], s);

    dc.SetPen(wxPen(wxColor(255, 255, 0), 2));

//    PlotVMG(dc, vmg.values[SailingVMG::PORT_DOWNWIND], s);
//    PlotVMG(dc, vmg.values[SailingVMG::STARBOARD_DOWNWIND], s);

    /* boat speeds */
    double values[DEGREES + 1];
    for(int H = 0; H<=DEGREES; H++) {
        switch(selection) {
        case 0: values[H] = polar.Speed(H, windspeed); break;
        case 1: values[H] = polar.SpeedAtApparentWindDirection(H, windspeed); break;
        case 2: values[H] = polar.SpeedAtApparentWindSpeed(H, windspeed); break;
        case 3: values[H] = polar.SpeedAtApparentWind(H, windspeed); break;
        }
    }

    dc.SetPen(wxPen(wxColor(255, 0, 0), 2));
    PlotPolarData(dc, values, DEGREES+1, m_PlotScale, w, h, full);
}

void BoatDialog::PaintSpeed(wxPaintDC &dc, long index)
{
    int w, h;
    m_PlotWindow->GetSize( &w, &h);
    double maxVB = 0;

    Polar &polar = m_Boat.Polars[index];
    int H = 90;//m_sWindDirection->GetValue();

    /* plot scale */
    int selection = m_cPlotVariable->GetSelection();

    for(int s = 0; s<num_wind_speeds; s++) {
        double windspeed = wind_speeds[s], VB;

        if(selection < 2)
            VB = polar.Speed(H, windspeed);
        else
            VB = polar.SpeedAtApparentWindSpeed(H, windspeed);

        if(VB > maxVB)
            maxVB = VB;
    }
    
    dc.SetPen(wxPen(wxColor(0, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(wxColour(0, 55, 75));

    if(maxVB <= 0) maxVB = 1; /* avoid lock */
    double Vstep = ceil(maxVB / 5);
    maxVB += Vstep;

    m_PlotScale = h/1.8 / (maxVB+1);

    for(double V = Vstep; V <= maxVB; V+=Vstep) {
        int y = h - 2*V*m_PlotScale;
        dc.DrawLine(0, y, w, y);
        dc.DrawText(wxString::Format(_T("%.0f"), V), 0, y);
    }

    dc.SetTextForeground(wxColour(0, 0, 155));

    for(int s = 0; s<num_wind_speeds; s++) {
        double windspeed = wind_speeds[s];

        double x = s * w / num_wind_speeds;
        dc.DrawLine(x, 0, x, h);

        wxString str = wxString::Format(_T("%.0f"), windspeed);
        int sw, sh;
        dc.GetTextExtent(str, &sw, &sh);
        dc.DrawText(str, x, 0);
    }

    /* boat speeds */
    double *values = new double[num_wind_speeds];
    for(int s = 0; s<num_wind_speeds; s++) {
        double windspeed = wind_speeds[s];
        switch(selection) {
        case 0: values[s] = polar.Speed(H, windspeed); break;
        case 1: values[s] = polar.SpeedAtApparentWindSpeed(H, windspeed); break;
        case 2: values[s] = polar.SpeedAtApparentWindDirection(H, windspeed); break;
        case 3: values[s] = polar.SpeedAtApparentWind(H, windspeed); break;
        }
    }

    dc.SetPen(wxPen(wxColor(255, 0, 0), 2));
    PlotRectangularData(dc, values, num_wind_speeds, 2*m_PlotScale, w, h);
    delete [] values;
}
#if 0
void BoatDialog::PaintVMG(wxPaintDC &dc)
{
    int w, h;
    m_PlotWindow->GetSize( &w, &h);

    m_PlotScale = 0; // don't do mouse over

    int mul = 5, count = mul*num_wind_speeds - 1;

    olar &polar = m_Boat.Polars[m_SelectedSailPolar];

    /* plot scale */
    int selection = m_cPlotVariable->GetSelection();

    double min = 0, max = 190, step = 10;
    
    dc.SetPen(wxPen(wxColor(0, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(wxColour(0, 55, 75));

    double scale = h/(max - min);

    /* draw angles lines and text */
    for(double V = step; V < max; V+=step) {
        int y = h - V*scale;
        dc.DrawLine(0, y, w, y);
        dc.DrawText(wxString::Format(_T("%.0f"), V), 0, y);
    }

    /* draw wind speeds and lines and text */
    dc.SetTextForeground(wxColour(0, 0, 155));
    for(int s = 0; s<num_wind_speeds; s++) {
        double windspeed = wind_speeds[s];

        double x = s * w / num_wind_speeds;
        dc.DrawLine(x, 0, x, h);

        wxString str = wxString::Format(_T("%.0f"), windspeed);
        int sw, sh;
        dc.GetTextExtent(str, &sw, &sh);
        dc.DrawText(str, x, 0);
    }

    // Only render port tack.. later we could do starboard if the polar is not symmetric
    double *valuesup = new double[count], *valuesdown = new double[count];
    for(int s = 0; s<count; s++) {
        int s0 = s/mul;
        double d = (double)s/mul - s0, v;
        double windspeed1 = wind_speeds[s0];
        double windspeed2 = wind_speeds[s0+1];
        double windspeed = (1-d)*windspeed1 + d*windspeed2;

        SailingVMG vmg = selection < 2 ? polar.GetVMGTrueWind(windspeed) :
            polar.GetVMGApparentWind(windspeed);

        v = vmg.values[SailingVMG::PORT_UPWIND];
        if(selection & 1 && !isnan(v))
            v = rad2posdeg(BoatPolar::DirectionApparentWind
                                   (polar.Speed(v, windspeed), deg2rad(v), windspeed));
        valuesup[s] = v;

        v = vmg.values[SailingVMG::PORT_DOWNWIND];
        if(selection & 1 && !isnan(v))
            v = rad2posdeg(BoatPolar::DirectionApparentWind
                                   (polar.Speed(v, windspeed), deg2rad(v), windspeed));
        valuesdown[s] = v;
    }

    dc.SetPen(wxPen(wxColor(255, 0, 255), 2));
    PlotRectangularData(dc, valuesup, count, scale, w, h);

    dc.SetPen(wxPen(wxColor(255, 255, 0), 2));
    PlotRectangularData(dc, valuesdown, count, scale, w, h);
    delete [] valuesup;
    delete [] valuesdown;
}
#endif
void BoatDialog::OnPaintPlot(wxPaintEvent& event)
{
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window)
        return;

    wxPaintDC dc(window);
    dc.SetBackgroundMode(wxTRANSPARENT);

    long index = SelectedPolar();
    if(index < 0) {
        dc.SetTextForeground(*wxBLACK);
        wxString str = _("Select a polar to view plot");
        int sw, sh;
        dc.GetTextExtent(str, &sw, &sh);
        dc.DrawText(str, 0, 0);
        return;
    }
    
    switch(m_cPlotType->GetSelection()) {
    case 0: PaintPolar(dc, index); break;
    case 1: PaintSpeed(dc, index); break;
    }
}

void BoatDialog::OnPaintCrossOverChart(wxPaintEvent& event)
{
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window)
        return;

    wxGCDC dc(window);
    dc.SetBackgroundMode(wxTRANSPARENT);

    long index = SelectedPolar();
    bool polar = !m_cPlotType->GetSelection();

    int w, h;
    m_CrossOverChart->GetSize( &w, &h);

    dc.SetPen(wxPen(wxColor(0, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(wxColour(0, 55, 75));

    bool full = m_cbFullPlot->GetValue();
    double scale;
    int xc = full ? w / 2 : 0;
    if(polar) {
        scale = wxMin(full ? w/2 : w, h/2) / 40.0;
    }
    
    for(double VW = 0; VW < 40; VW += 10) {
        if(polar) {
            dc.DrawCircle(xc, h/2, VW * scale);
            dc.DrawText(wxString::Format(_T("%.0f"), VW), xc, h/2+(int)VW*scale);
        } else {
            int y = h - VW * h / 40;
            dc.DrawLine(0, y, w, y);
            dc.DrawText(wxString::Format(_T("%.0f"), VW), 0, y);
        }
    }

    for(double H = 0; H < 180; H += 10) {
        if(polar) {
            double x = scale*sin(deg2rad(H));
            double y = scale*cos(deg2rad(H));
            if(H < 180)
                dc.DrawLine(xc - x, h/2 + y, xc + x, h/2 - y);

            wxString str = wxString::Format(_T("%.0f"), H);
            int sw, sh;
            dc.GetTextExtent(str, &sw, &sh);
            dc.DrawText(str, xc + .9*x - sw/2, h/2 - .9*y - sh/2);
        } else {
            int x = H * w / 180;
            dc.DrawLine(x, 0, x, h);
            dc.DrawText(wxString::Format(_T("%.0f"), H), x, 0);
        }
    }
    
    wxColour colors[] = {*wxRED, *wxGREEN, *wxBLUE, *wxCYAN, *wxYELLOW, *wxWHITE};
    int c = 0;
    for(unsigned int i=0; i<m_Boat.Polars.size(); i++) {
        bool bold = i == index;

//        dc.SetPen(wxPen(colors[c], bold ? 1 : 3));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxColour(colors[c].Red(),
                             colors[c].Green(),
                             colors[c].Blue(),
                             bold ? 220 : 140));
        if(++c == (sizeof colors) / (sizeof *colors))
            c = 0;

        bool tri = true;
        TESStesselator *tess = m_Boat.Polars[i].CrossOverRegion.Tesselate(tri);

        if(!tess)
            continue;
          
        const float* verts = tessGetVertices(tess);
//        const int* vinds = tessGetVertexIndices(tess);
        const int* elems = tessGetElements(tess);
//        const int nverts = tessGetVertexCount(tess);
        const int nelems = tessGetElementCount(tess);
	
        // Draw polygons.
        for (int i = 0; i < nelems; ++i)
        {
            if(tri) {
                const int* p = &elems[i*3];
                wxPoint points[3];
                for (unsigned j = 0; j < 3 && p[j] != TESS_UNDEF; ++j) {
                    double H = verts[p[j]*2+0];
                    double VW = verts[p[j]*2+1];
                    if(polar)
                        points[j] = wxPoint(xc + scale*VW*sin(deg2rad(H)),
                                            h/2 - scale*VW*cos(deg2rad(H)));
                    else
                        points[j] = wxPoint(H * w / 180, h - VW * h / 40);
                }
                dc.DrawPolygon(3, points);
            } else {
                int b = elems[i*2];
                int n = elems[i*2+1];

                wxPoint pl;
                for(int j = 0; j<=n; j++) {
                    int k = j < n ? j : 0;
                    float H = verts[2*(b + k)+0], VW = verts[2*(b + k)+1];
                    wxPoint p0;
                    if(polar)
                        p0 = wxPoint(xc + scale*VW*sin(deg2rad(H)),
                                            h/2 - scale*VW*cos(deg2rad(H)));
                    else
                        p0 = wxPoint(H * w / 180, h - VW * h / 40);

                    if(j > 0)
                        dc.DrawLine(pl, p0);
                    pl = p0;
                }
            }
        }

        tessDeleteTess(tess);
    }
}

void BoatDialog::OnGenerateCrossOverChart( wxCommandEvent& event )
{
    m_Boat.GenerateCrossOverChart();
    RefreshPlots();
}

void BoatDialog::OnOpen ( wxCommandEvent& event )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

    wxString path;
    pConf->Read ( _T ( "Path" ), &path, weather_routing_pi::StandardPath());

    wxFileDialog openDialog
        ( this, _( "Select Boat" ), path, wxT ( "" ),
          wxT ( "Boat polar (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
          wxFD_OPEN  );

    if( openDialog.ShowModal() == wxID_OK ) {
        wxString filename = openDialog.GetPath();
        pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );
        pConf->Write ( _T ( "Path" ), wxFileName(filename).GetPath() );

        wxString error = m_Boat.OpenXML(filename);
        if(error.empty()) {
            RepopulatePolars();
        } else {
            wxMessageDialog md(this, error, _("OpenCPN Weather Routing Plugin"),
                               wxICON_ERROR | wxOK );
            md.ShowModal();
            return;
        }

        UpdateVMG();
        RefreshPlots();
    }
}

void BoatDialog::Save()
{
    if(m_boatpath.empty()) {
        wxFileConfig *pConf = GetOCPNConfigObject();
        pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

        wxString path;
        pConf->Read ( _T ( "Path" ), &path, weather_routing_pi::StandardPath());

        wxFileDialog saveDialog( this, _( "Select Boat" ), path, wxT ( "" ),
                                 wxT ( "Boat files (*.xml)|*.XML;*.xml|All files (*.*)|*.*" ),
                                 wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

        if( saveDialog.ShowModal() == wxID_OK ) {
            wxString filename = wxFileDialog::AppendExtension(saveDialog.GetPath(), _T("*.xml"));

            pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );
            pConf->Write ( _T ( "Path" ), wxFileName(filename).GetPath() );

            m_boatpath = filename;
        } else
            return;
    }

    wxString error = m_Boat.SaveXML(m_boatpath);
    if(error.empty())
        EndModal(wxID_OK);
    else {
        wxMessageDialog md(this, error, _("OpenCPN Weather Routing Plugin"),
                           wxICON_ERROR | wxOK );
        md.ShowModal();
    }
}

void BoatDialog::OnSaveAs ( wxCommandEvent& event )
{
    m_boatpath.clear();
    Save();
}

void BoatDialog::OnClose ( wxCommandEvent& event )
{
    EndModal(wxID_CANCEL);
}

void BoatDialog::OnSaveFile ( wxCommandEvent& event )
{
    long index = SelectedPolar();
    if(index < 0)
        return;

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

    wxString path;
    pConf->Read ( _T ( "FilePath" ), &path, weather_routing_pi::StandardPath());

    wxFileDialog saveDialog( this, _( "Select Polar" ), path, wxT ( "" ),
                             wxT ( "Boat Polar files (*.file)|*.FILE;*.file|All files (*.*)|*.*" ), wxFD_SAVE  );

    if( saveDialog.ShowModal() == wxID_OK ) {
        wxString filename = saveDialog.GetPath();
        pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );
        pConf->Write ( _T ( "FILEPath" ), wxFileName(filename).GetPath() );

        Polar &polar = m_Boat.Polars[index];
        if(!polar.Save(saveDialog.GetPath().mb_str())) {
            wxMessageDialog md(this, _("Failed saving boat polar to file"), _("OpenCPN Weather Routing Plugin"),
                               wxICON_ERROR | wxOK );
            md.ShowModal();
        }
    }
}

void BoatDialog::OnPolarSelected()
{
    bool enable = SelectedPolar() != -1;
    m_bEditPolar->Enable(enable);
    m_bRemovePolar->Enable(enable);

    RefreshPlots();
}

void BoatDialog::OnUpdatePlot()
{
    m_fgSizer->SetCols(m_cPlotType->GetSelection());
    m_fgSizer->SetRows(!m_cPlotType->GetSelection());
//    m_fgSizer->Fit();
    Fit();
    RefreshPlots();
}

void BoatDialog::OnUpPolar( wxCommandEvent& event )
{
    long index = SelectedPolar();
    if(index < 1)
        return;

    m_Boat.Polars.insert(m_Boat.Polars.begin() + index - 1,
                         m_Boat.Polars.at(index));
    m_Boat.Polars.erase(m_Boat.Polars.begin() + index + 1);

    RepopulatePolars();

    m_lPolars->SetItemState( index - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
}

void BoatDialog::OnDownPolar( wxCommandEvent& event )
{
    long index = SelectedPolar();
    if(index < 0 || index + 1 >= (long)m_Boat.Polars.size())
        return;

    m_Boat.Polars.insert(m_Boat.Polars.begin() + index + 2,
                         m_Boat.Polars.at(index));
    m_Boat.Polars.erase(m_Boat.Polars.begin() + index);

    RepopulatePolars();

    m_lPolars->SetItemState( index + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
}

void BoatDialog::OnEditPolar( wxCommandEvent& event )
{
    
}

void BoatDialog::OnAddPolar( wxCommandEvent& event )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

    wxString path;
    pConf->Read ( _T ( "FilePath" ), &path, *GetpSharedDataLocation()
                  + _T("plugins/weather_routing_pi/data/polars"));
    path = wxFileName(path).GetPath();

    wxFileDialog openDialog
        ( this, _( "Select Polar File" ), path, wxT ( "" ),
          wxT ( "CSV, POL, TXT (*.csv, *.pol, *.txt)|*.CSV;*.csv;*.csv.gz;*.csv.bz2;*.POL;*.pol;*.pol.gz;*.pol.bz2;*.TXT;*.txt;*.txt.gz;*.txt.bz2|All files (*.*)|*.*" ),
          wxFD_OPEN  );

    if( openDialog.ShowModal() != wxID_OK )
        return;

    pConf->Write( _T ( "FilePath" ), openDialog.GetPath());
    
    wxString filename = openDialog.GetPath(), message;
    Polar polar;
    bool success = polar.Open(filename, message);

    if(success) {
        m_Boat.Polars.push_back(polar);
        RepopulatePolars();
        RefreshPlots();
    }
}

void BoatDialog::OnRemovePolar( wxCommandEvent& event )
{
    long index = SelectedPolar();
    if(index < 0)
        return;

    m_Boat.Polars.erase(m_Boat.Polars.begin() + index);
    RepopulatePolars();
    RefreshPlots();
    m_bRemovePolar->Disable();
}

void BoatDialog::RepopulatePolars()
{
    m_lPolars->DeleteAllItems();
#if 0
    if(m_Boat.Polars.size() == 0) {
        Polar generic_polar;
        wxString message, generic_polar_path = *GetpSharedDataLocation()
            + _T("plugins/weather_routing_pi/data/polars/60ft_mono.pol");
        bool success = generic_polar.Open(generic_polar_path, message);
        if(success)
            m_Boat.Polars.push_back(generic_polar);
        if(message.size())
            wxLogMessage(wxT("weather_routing_pi: ") + wxString(success ? _T("warning") : _T("error")) +
                         _T(" loading generic polar \"") + generic_polar_path + _T("\""));
    }
#endif
    for(unsigned int i=0; i<m_Boat.Polars.size(); i++) {
        wxListItem info;
        info.SetId(i);
        info.SetData(i);
        long idx = m_lPolars->InsertItem(info);
        Polar &polar = m_Boat.Polars[i];
        m_lPolars->SetItem(idx, spFILENAME, wxFileName(polar.FileName).GetFullName());
        m_lPolars->SetItem(idx, spWAVEPOLARFILENAME, polar.WavePolarFileName);
    }

//        m_lPolars->SetColumnWidth(spFILENAME, wxLIST_AUTOSIZE);
//    m_lPolars->SetColumnWidth(spFILENAME, 80);

    bool enable = m_Boat.Polars.size();
    m_bRemovePolar->Enable(enable);
}

wxString BoatDialog::FormatVMG(double W, double VW)
{
    long index = SelectedPolar();
    Polar &polar = m_Boat.Polars[index];
    double A = isnan(W) ? NAN :
        rad2posdeg(Polar::DirectionApparentWind(polar.Speed(W, VW), deg2rad(W), VW));
    return wxString::Format(_T("%.1f True %.1f Apparent"), W, A);
}

void BoatDialog::UpdateVMG()
{ 
    long index = SelectedPolar();
    if(index < 0)
        return;

    int windspeed = m_sVMGWindSpeed->GetValue();
    Polar &polar = m_Boat.Polars[index];
    SailingVMG vmg = /*m_cVMGTrueApparent->GetSelection()*/ 0 ?
        polar.GetVMGApparentWind(windspeed) :
        polar.GetVMGTrueWind(windspeed);

    m_stBestCourseUpWindPortTack->SetLabel(FormatVMG(vmg.values[SailingVMG::PORT_UPWIND], windspeed));
    m_stBestCourseUpWindStarboardTack->SetLabel(FormatVMG(vmg.values[SailingVMG::STARBOARD_UPWIND], windspeed));
    m_stBestCourseDownWindPortTack->SetLabel(FormatVMG(vmg.values[SailingVMG::PORT_DOWNWIND], windspeed));
    m_stBestCourseDownWindStarboardTack->SetLabel(FormatVMG(vmg.values[SailingVMG::STARBOARD_DOWNWIND], windspeed));
}
