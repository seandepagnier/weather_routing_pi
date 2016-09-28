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
#include "RouteMapOverlay.h"
#include "WeatherRouting.h"

wxString dummy_polar = _T("\
TWA	6	10	14	20\n\
45	5.5	6.6	7.3	7.6\n\
60	5.8	7.0	7.6	7.8\n\
90	6.2	7.3	7.8	8.1\n\
135	5.2	6.4	7.4	7.9\n\
150	4.3	5.4	6.5	7.3\n\
");

enum {spFILENAME};

// for plotting
static const int wind_speeds[] = {0, 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 36, 40, 45, 50, 55, 60};
static const int num_wind_speeds = (sizeof wind_speeds) / (sizeof *wind_speeds);

BoatDialog::BoatDialog(WeatherRouting &weatherrouting)
    : BoatDialogBase(&weatherrouting),
      m_WeatherRouting(weatherrouting), m_PlotScale(0), m_CrossOverRegenerate(false), m_CrossOverGenerationThread(NULL)
{
    // for small screens: don't let boat dialog be larger than screen
    int w,h;
    wxDisplaySize( &w, &h );
    w = wxMin(w, GetMinWidth());
    h = wxMin(h-32, GetMinHeight());
    SetMinSize(wxSize(w, h));
    SetSize(wxSize(w, h));

    m_lPolars->InsertColumn(spFILENAME, _("Filename"));

    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

    m_orientation[0] = pConf->Read ( _T ( "Orientation0" ), 1L );
    m_orientation[1] = pConf->Read ( _T ( "Orientation1" ), 1L );

    // hack to adjust items
    SetSize(wxSize(w, h));
}

BoatDialog::~BoatDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/PlugIns/WeatherRouting/BoatDialog" ) );

    pConf->Read ( _T ( "Orientation0" ), m_orientation[0] );
    pConf->Write ( _T ( "Orientation1" ), m_orientation[1] );
}

void BoatDialog::LoadPolar(const wxString &filename)
{
    m_boatpath = filename;

    SetTitle(m_boatpath);
    wxString error = m_Boat.OpenXML(m_boatpath, false);
    RepopulatePolars();

    /* select first polar if it exists */
    if(m_lPolars->GetItemCount())
        m_lPolars->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );

    if(error.size()) {
        wxMessageDialog md(this, error,
                           _("OpenCPN Weather Routing Plugin"),
                            wxICON_ERROR | wxOK );
        md.ShowModal();
    }

//    m_lBoatPlans->SetItemState(m_SelectedSailPlan, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

    UpdateVMG();
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

        VA = BoatPlan::VelocityApparentWind(VB, W, VW);
        A = rad2posdeg(BoatPlan::DirectionApparentWind(VA, VB, W, VW));
        break;
    case 1:
        A = heading_resolve(B);
        VW = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].SpeedAtApparentWindDirection(A, VW, &W);
        W = positive_degrees(W);

        VA = BoatPlan::VelocityApparentWind(VB, W, VW);
        break;
    case 2:
        W = B;
        VA = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].SpeedAtApparentWindSpeed(W, VA);
        VW = BoatPlan::VelocityTrueWind(VA, VB, W);
        A = rad2posdeg(BoatPlan::DirectionApparentWind(VA, VB, W, VW));
        break;
    case 3:
        A = heading_resolve(B);
        VA = windspeed;
        VB = m_Boat.Plans[m_SelectedSailPlan].SpeedAtApparentWind(A, VA, &W);
        W = positive_degrees(W);
        VW = BoatPlan::VelocityTrueWind(VA, VB, W);
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
    
    int plottype = m_cPlotType->GetSelection();
    int w, h;
    m_PlotWindow->GetSize( &w, &h);
    double maxVB = 0;

    Polar &polar = m_Boat.Polars[index];

    /* plot scale */
    int selection = m_cPlotVariable->GetSelection();

    for(unsigned int VWi = 0; VWi<polar.wind_speeds.size(); VWi++) {
        double windspeed = polar.wind_speeds[VWi].VW;
        for(unsigned int Wi = 0; Wi<polar.degree_steps.size(); Wi++) {
            double W = polar.degree_steps[Wi];
            double VB;
            if(selection < 2)
                VB = polar.Speed(W, windspeed);
            else
                VB = polar.SpeedAtApparentWindSpeed(W, windspeed);
            
            if(VB > maxVB)
                maxVB = VB;
        }
    }

    dc.SetPen(wxPen(wxColor(0, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(wxColour(0, 55, 75));

    if(maxVB <= 0) maxVB = 1; /* avoid lock */
    double Vstep = ceil(maxVB / 5);
    maxVB += Vstep;

    m_PlotScale = (w < h ? w : h)/1.8 / (maxVB+1);
//    m_PlotScale = h/1.8 / (maxVB+1);

    bool full = m_cbFullPlot->GetValue();
    int xc = full ? w / 2 : 0;

    if(plottype == 0) {
        /* polar circles */
        for(double V = Vstep; V <= maxVB; V+=Vstep) {
            dc.DrawCircle(xc, h/2, V*m_PlotScale);
            dc.DrawText(wxString::Format(_T("%.0f"), V), xc, h/2+(int)V*m_PlotScale);
        }
    } else {
        for(double V = Vstep; V <= maxVB; V+=Vstep) {
            int y = h - 2*V*m_PlotScale;
            dc.DrawLine(0, y, w, y);
            dc.DrawText(wxString::Format(_T("%.0f"), V), 0, y);
        }
    }

    dc.SetTextForeground(wxColour(0, 0, 155));

    if(plottype == 0) {
        /* polar meridians */
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
    } else {
        for(int s = 0; s<num_wind_speeds; s++) {
            double windspeed = wind_speeds[s];

            double x = s * w / num_wind_speeds;
            dc.DrawLine(x, 0, x, h);

            wxString str = wxString::Format(_T("%.0f"), windspeed);
            int sw, sh;
            dc.GetTextExtent(str, &sw, &sh);
            dc.DrawText(str, x, 0);
        }
    }

    int cx = (full ? w/2 : 0), cy = h/2;

    dc.SetPen(wxPen(wxColor(255, 0, 0), 2));

    /* boat speeds */
    if(plottype == 0) {
        for(unsigned int VWi = 0; VWi<polar.wind_speeds.size(); VWi++) {
            double VW, VA;
            switch(selection) {
            case 0: case 1: VW = polar.wind_speeds[VWi].VW; break;
                // use grid vw for va steps
            case 2: case 3: VA = polar.wind_speeds[VWi].VW; break;
            }

            bool lastvalid = false;
            int lx, ly;
//            for(unsigned int Wi = 0; Wi<polar.degree_steps.size()+full; Wi++) {
            //              double W = polar.degree_steps[Wi%polar.degree_steps.size()];
            double W0 = polar.degree_steps[0];
            double Wn = polar.degree_steps[polar.degree_steps.size()-1];
            double Wd = Wn - W0, Ws = Wd / floor(Wd);
            for(double W = W0; W <= Wn; W+= Ws) {
                double VB;
                switch(selection) {
                case 0: case 1:
                    VB = polar.Speed(W, VW);
                    break;
                case 2: case 3:
                    VB = polar.SpeedAtApparentWindSpeed(W, VA);
                    VW = Polar::VelocityTrueWind(VA, VB, W);
                    break;
                }
                
                double a;
                
                switch(selection) {
                case 0: case 2: a = W; break;
                case 1: case 3: a = Polar::DirectionApparentWind(VB, W, VW); break;
                }
                
                int px, py;
                px =  m_PlotScale*VB*sin(deg2rad(a)) + cx;
                py = -m_PlotScale*VB*cos(deg2rad(a)) + cy;
                
                if(lastvalid) {
                    dc.DrawLine(lx, ly, px, py);
                    if(full)
                        dc.DrawLine(2*cx-lx, ly, 2*cx-px, py);
                    
//            dc.DrawArc(lx, ly, px, py, cx, cy);
                }

                lx = px, ly = py;
                lastvalid = true;
            }
        }
    } else {
        for(unsigned int Wi = 0; Wi<polar.degree_steps.size(); Wi++) {
            double W = polar.degree_steps[Wi], VB;

            bool lastvalid = false;
            int lx, ly;
            for(unsigned int VWi = 0; VWi<polar.wind_speeds.size(); VWi++) {
                double windspeed = polar.wind_speeds[VWi].VW;
                double VW, VA;
                switch(selection) {
                case 0: case 1: VW = windspeed; break;
                    // use grid vw for va steps
                case 2: case 3: VA = windspeed; break;
                }
                
                switch(selection) {
                case 0: case 1:
                    VB = polar.Speed(W, VW);
                    break;
                case 2: case 3:
                    VB = polar.SpeedAtApparentWindSpeed(W, VA);
                    VW = Polar::VelocityTrueWind(VA, VB, W);
                    break;
                }

                #if 0
                double a;
                
                switch(selection) {
                case 0: case 2: a = W; break;
                case 1: case 3: a = Polar::DirectionApparentWind(VB, W, VW); break;
                }
                #endif
                
                int px, py;
                int s;
                for(s = 0; s<num_wind_speeds-1; s++) {
                    if(wind_speeds[s] > polar.wind_speeds[VWi].VW)
                        break;
                }
                { // interpolate into non-linear windspeed space
                    double x = windspeed, x1 = wind_speeds[s], x2 = wind_speeds[s+1];
                    double y1 = s * w / num_wind_speeds;
                    double y2 = (s+1) * w / num_wind_speeds;

                    px = x2 - x1 ? (y2 - y1)*(x - x1)/(x2 - x1) + y1 : y1;
                }
                py = h - 2*VB*m_PlotScale;
                
                if(lastvalid) {
                    dc.DrawLine(lx, ly, px, py);
                    if(full)
                        dc.DrawLine(2*cx-lx, ly, 2*cx-px, py);
                }

                lx = px, ly = py;
                lastvalid = true;
            }
        }
    }

    /* vmg */

    wxPoint lastp[4];
    bool lastpvalid[4] = {false, false, false, false};
    for(unsigned int VWi = 0; VWi<polar.wind_speeds.size(); VWi++) {
        double VW = polar.wind_speeds[VWi].VW;
        SailingVMG vmg = polar.GetVMGTrueWind(VW);

        for(int i=0; i<4; i++) {
            if(i%2 == 0 && !full)
                continue;

            if(i < 2)
                dc.SetPen(wxPen(wxColor(255, 0, 255), 2));
            else
                dc.SetPen(wxPen(wxColor(255, 255, 0), 2));

            double W = vmg.values[i];
            if(isnan(W))
                continue;

            double VB = polar.Speed(W, VW);

            double a;
            switch(selection) {
            case 0: case 2: a = W; break;
            case 1: case 3: a = Polar::DirectionApparentWind(VB, W, VW); break;
            }

            wxPoint p;
            if(plottype == 0) {
                p.x =  m_PlotScale*VB*sin(deg2rad(a)) + cx;
                p.y = -m_PlotScale*VB*cos(deg2rad(a)) + cy;
            } else {
                p.x = a * w / (full ? 360 : 180) + cx;
                p.y = h - 2*VB*m_PlotScale;
            }

            if(lastpvalid[i])
                dc.DrawLine(lastp[i], p);
            lastp[i] = p;
            lastpvalid[i] = true;
        }
    }
}

static int CalcPolarPoints(wxPoint p0, wxPoint p1)
{
    return floor(fabs((double)p0.x - p1.x) / 5) + 1;
}

void BoatDialog::OnPaintCrossOverChart(wxPaintEvent& event)
{
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window)
        return;
#if wxUSE_GRAPHICS_CONTEXT
    wxGCDC dc(window);
#else    
    wxClientDC dc(window);
#endif    
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
    
    wxColour colors[] = {*wxRED, *wxGREEN, *wxBLUE, *wxCYAN, *wxYELLOW,
                         wxColour(255, 0, 255)};
    int c = 0;
    for(unsigned int i=0; i<m_Boat.Polars.size(); i++) {
        bool bold = i == index;

//        dc.SetPen(wxPen(colors[c], bold ? 1 : 3));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxColour(colors[c].Red(),
                             colors[c].Green(),
                             colors[c].Blue(),
                             bold ? 230 : 60));
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
                    points[j] = wxPoint(H * w / 180, h - VW * h / 40);
                }
                if(polar) {
                    int count[3] = {CalcPolarPoints(points[0], points[1]),
                                    CalcPolarPoints(points[1], points[2]),
                                    CalcPolarPoints(points[2], points[0])};
                    wxPoint *pts = new wxPoint[count[0] + count[1] + count[2]];
                    int c = 0;
                    for(int j = 0; j<3; j++) {
                        int jp1 = j+1 == 3 ? 0 : j+1;
                        for(int k=0; k<count[j]; k++) {
                            double d = (double)k / count[j];
                            double px = points[j].x * (1-d) + points[jp1].x * d;
                            double py = points[j].y * (1-d) + points[jp1].y * d;
                            double H = px / w * 180;
                            double VW = (h - py) / h * 40;
                            pts[c++] = wxPoint(xc + scale*VW*sin(deg2rad(H)),
                                               h/2 - scale*VW*cos(deg2rad(H)));
                        }
                    }
                    dc.DrawPolygon(c, pts);
                    if(full) {
                        for(int j = 0; j<c; j++)
                            pts[j].x = 2*xc - pts[j].x;
                        dc.DrawPolygon(c, pts);
                    }
                    delete [] pts;
                } else {
                    dc.DrawPolygon(3, points);
                }
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

void BoatDialog::OnOverlapPercentage( wxSpinEvent& event )
{
    long i = SelectedPolar();
    if(i != -1)
        m_Boat.Polars[i].m_crossoverpercentage = m_sOverlapPercentage->GetValue()/100.0;
    GenerateCrossOverChart();
}

void BoatDialog::OnOpenBoat ( wxCommandEvent& event )
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

void BoatDialog::SaveBoat()
{
    while(m_CrossOverGenerationThread) {
        wxYield();
        wxThread::Sleep(10);
    }
    
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
            SetTitle(m_boatpath);
        } else
            return;
    }

    wxString error = m_Boat.SaveXML(m_boatpath);
    if(error.empty()) {
        m_WeatherRouting.m_ConfigurationDialog.SetBoatFilename(m_boatpath);
        /* update any configurations that use this boat */
        m_WeatherRouting.UpdateBoatFilename(m_boatpath);
        Update();

        Hide();
    } else {
        wxMessageDialog md(this, error, _("OpenCPN Weather Routing Plugin"),
                           wxICON_ERROR | wxOK );
        md.ShowModal();
    }
}

void BoatDialog::OnSaveAsBoat ( wxCommandEvent& event )
{
    m_boatpath.clear();
    SaveBoat();
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
    int i = SelectedPolar();
    m_bEditPolar->Enable(i != -1);
    m_bRemovePolar->Enable(i != -1);

// not needed if modal    m_EditPolarDialog.SetPolarIndex(i);

    m_sOverlapPercentage->Enable(i != -1);
    if(i != -1)
        m_sOverlapPercentage->SetValue(m_Boat.Polars[i].m_crossoverpercentage*100);
    
    RefreshPlots();
    UpdateVMG();
}

void BoatDialog::OnUpdatePlot()
{
    int vert = m_orientation[m_cPlotType->GetSelection()];
    m_cbOrientation->SetValue(vert);
    if(m_fgSizer->GetRows() != vert) {
        m_fgSizer->SetRows(vert);
        m_fgSizer->SetCols(!vert);
        Fit();
    }
    RefreshPlots();
}

void BoatDialog::OnOrientation( wxCommandEvent& event )
{
    m_orientation[m_cPlotType->GetSelection()] = m_cbOrientation->GetValue();
    OnUpdatePlot();
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
    //m_EditPolarDialog.Show();

    int i = SelectedPolar();
    if(i == -1)
        return;

    EditPolarDialog dlg(this);
         
    dlg.SetPolarIndex(i);
    wxString filename = m_Boat.Polars[i].FileName;
    if(dlg.ShowModal() == wxID_SAVE) {
        if(!m_Boat.Polars[i].Save(filename))
            wxMessageBox(_("Failed to save") + _T(": ") + filename,
                           _("OpenCPN Weather Routing Plugin"),
                         wxICON_ERROR | wxOK );
    } else {
        wxString message;
        if(!m_Boat.Polars[i].Open(filename, message))
            wxMessageBox(_("Failed to revert") + _T(": ") + filename + _T("\n")
                         + message, _("OpenCPN Weather Routing Plugin"),
                         wxICON_ERROR | wxOK );
    }

    GenerateCrossOverChart();
    RefreshPlots();
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
          wxFD_OPEN | wxFD_MULTIPLE );

    if( openDialog.ShowModal() != wxID_OK )
        return;

    pConf->Write( _T ( "FilePath" ), openDialog.GetPath());

    wxArrayString paths;
    openDialog.GetPaths(paths);

    bool generate = false, existed = true;
    for(unsigned int i=0; i<paths.GetCount(); i++) {
        wxString filename = paths[i], message;
        Polar polar;

        for(unsigned int j=0; j<m_Boat.Polars.size(); j++)
            if(m_Boat.Polars[j].FileName == filename)
                goto skip;

        existed = wxFileName::Exists(filename);

        // write dummy file
        if(!existed) {
            wxFile file;
            if(file.Open(filename, wxFile::write))
                file.Write(dummy_polar);
        }
        
        if(polar.Open(filename, message)) {
            m_Boat.Polars.push_back(polar);
            RepopulatePolars();
            m_lPolars->SetItemState(m_Boat.Polars.size()-1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            generate = true;
        } else {
            wxMessageDialog md(this, message,
                               _("OpenCPN Weather Routing Plugin"),
                               wxICON_ERROR | wxOK );
            md.ShowModal();
        }
    skip:;
    }

    if(generate)
        GenerateCrossOverChart();
            
    if(!existed)
        OnEditPolar(event);
}

void BoatDialog::OnRemovePolar( wxCommandEvent& event )
{
    long index = -1, lastindex = -1, count = 0;

    while((index = m_lPolars->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
        m_Boat.Polars.erase(m_Boat.Polars.begin() + index - count++);
        lastindex = index;
    }

    if(lastindex == -1)
        return;
    
    RepopulatePolars();

    lastindex -= count;
    if(lastindex == (int)m_Boat.Polars.size())
        lastindex--;

    m_lPolars->SetItemState(lastindex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    GenerateCrossOverChart();
    m_bRemovePolar->Enable(lastindex != -1);
}

static void status(void *arg, int p, int s)
{
    if(s == 0)
        p = s = 1;

    wxThreadEvent event(wxEVT_THREAD, 100*p/s);
    ((wxEvtHandler*)arg)->AddPendingEvent(event);
}

class CrossOverGenerationThread : public wxThread
{
public:
    CrossOverGenerationThread(Boat &boat, BoatDialog &dlg)
        : wxThread(wxTHREAD_JOINABLE), m_Boat(boat),
          m_BoatDialog(dlg)
        {
            Create();
        }

    void *Entry() {
        m_Boat.GenerateCrossOverChart(&m_BoatDialog, status);
        return 0;
    }

    Boat m_Boat;
    BoatDialog &m_BoatDialog;
};

void BoatDialog::GenerateCrossOverChart()
{
    if(m_CrossOverGenerationThread) {
        m_CrossOverRegenerate = true; // regenerate again when done
        return;
    }

    m_gCrossOverChart->Enable();

    m_CrossOverGenerationThread = new CrossOverGenerationThread(m_Boat, *this);

    Connect( wxEVT_THREAD, (wxEventFunction)&BoatDialog::OnEvtThread);
    m_CrossOverGenerationThread->Run();
}

void BoatDialog::OnEvtThread( wxThreadEvent & event )
{
    int id = event.GetId();
    m_gCrossOverChart->SetValue(id);
    if(id < 100)
        return;

    m_gCrossOverChart->Disable();

    m_CrossOverGenerationThread->Wait();
    Boat &tboat = m_CrossOverGenerationThread->m_Boat;
    for(unsigned int i=0; i<m_Boat.Polars.size() &&
            i < tboat.Polars.size(); i++)
        m_Boat.Polars[i].CrossOverRegion = tboat.Polars[i].CrossOverRegion;
    delete m_CrossOverGenerationThread;
    m_CrossOverGenerationThread = NULL;
    RefreshPlots();

    if(m_CrossOverRegenerate) {
        m_CrossOverRegenerate = false;
        GenerateCrossOverChart();
    }
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
        m_lPolars->SetColumnWidth(spFILENAME, wxLIST_AUTOSIZE);
    }

//        m_lPolars->SetColumnWidth(spFILENAME, wxLIST_AUTOSIZE);
//    m_lPolars->SetColumnWidth(spFILENAME, 80);

    int enable = m_Boat.Polars.size();
    m_bRemovePolar->Enable(enable);
}

wxString BoatDialog::FormatVMG(double W, double VW)
{
    long index = SelectedPolar();
    Polar &polar = m_Boat.Polars[index];
    double A = isnan(W) ? NAN :
        positive_degrees(Polar::DirectionApparentWind(polar.Speed(W, VW), W, VW));
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
