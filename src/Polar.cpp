/******************************************************************************
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
#include <wx/filename.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "zuFile.h"

#include "Utilities.h"
#include "Boat.h"

PolarMeasurement::PolarMeasurement(double v, double d, double vb, bool apparent)
{
    VB = vb;
    if(apparent) {
        VA = v;
        A = d;
    } else {
        double VW = v, W = d;
        /*
           ________________________
          /  2    2
   VA =  / VW + VB  + 2 VW VB cos(W)
       \/

           ________________________
          /  2    2
   VW =  / VA + VB - 2 VA VB cos(A)
       \/

      sin A   sin W
      ----- = --------
       VW        VA   

        */
        VA = sqrt(VW*VW + VB*VB + 2*VW*VB*cos(deg2rad(W)));
//        A = rad2deg(asin(VW*sin(deg2rad(W))/VA));
        A = rad2deg(acos((VA*VA + VB*VB - VW*VW) / (2*VA*VB)));
                  
    }
    VB = vb;
    eta = VA*(1-cos(deg2rad(A)))/(2*VB*VB);
}

double PolarMeasurement::VW() const
{
    return sqrt(VA*VA + VB*VB - 2*VA*VB*cos(deg2rad(A)));
}

double PolarMeasurement::W() const
{
//        VB*sin(W)=VA*sin(W-A);
//        sin(W)/(cos(A)*sin(W)-sin(A)*cos(W)) = VA/ VB;
//        W = atan(VA*sin(A) / (VA*cos(A)-VB))
//
//    return rad2deg(atan(VA*sin(deg2rad(A)) / (VA*cos(deg2rad(A))-VB)));
//
//    VA = sqrt(VW^2 + VB^2 + 2*VW*VB*cos(W))
//    VA*VA = VW*VW + VB*VB + 2*VW*VB*cos(W)
//    (VA*VA - VW*VW - VB*VB)/(2*VW*VB) = cos(W)
//    W = acos(VA*VA - VW*VW - VB*VB)/(2*VW*VB))
    double vw = VW();
    return rad2deg(acos((VA*VA - vw*vw - VB*VB)/(2*vw*VB)));
}

/* give value for y at a given x location on a segment */
double interp_value(double x, double x1, double x2, double y1, double y2)
{
/*
    y = m*x+b, y1 = m*x1+b, y2 = m*x2+b
    y = m*x+b, y1 - m*x1=b, y2 = m*x2+y1 - m*x1
    y = m*x+y1 - m*x1, y2 -y1= m*(x2 - x1)
    y = m*(x-x1)+y1, m = (y2 - y1)/(x2 - x1)
    y = (y2 - y1)*(x - x1)/(x2 - x1) + y1
*/
    if(x == x1) return y1; // handle partial nan edge case
    if(x == x2) return y2;
    return x2 - x1 ? (y2 - y1)*(x - x1)/(x2 - x1) + y1 : y1;
}

/* compute apparent wind speed from boat speed and true wind */
double Polar::VelocityApparentWind(double VB, double W, double VW)
{
    return sqrt(VW*VW + VB*VB + 2*VW*VB*cos(deg2rad(W))); /* law of cosines, W is flipped by 180 */
}

/*
   VA == 0
     A = undefined
   VB == 0
     A = W
             /   2    2    2 \
         -1 |  VA + VB - VW   |
   A = cos  |  ------------   |
             \   2 VA VB     /

        -1 / VW      \
  A = sin |  -- sin W |
           \ VA      /

*/
double Polar::DirectionApparentWind(double VA, double VB, double W, double VW)
{
  if(VA == 0) /* apparent wind direction is not defined */
    return 0;
  if(VB == 0) /* trig identity breaks down, but if we aren't */
    return W; /*   moving, apparent wind is true wind */
  double cosA = (VA*VA + VB*VB - VW*VW) / (2*VA*VB);
  if(cosA > 1) cosA = 1; else if(cosA < -1) cosA = -1; /* slight arithmatic errors */
  double ac = acos(cosA);
  while(W > 180)  W-=360;
  while(W < -180) W+=360;
  return rad2deg(W > 0 ? ac : -ac);
}

double Polar::DirectionApparentWind(double VB, double W, double VW)
{
    double VA = VelocityApparentWind(VB, W, VW);
    return DirectionApparentWind(VA, VB, W, VW);
}

/*
     2      2    2
   VA  =  VW + VB  + 2 VW VB cos(W)

     2                        2    2
   VW + 2 VW VB cos(W) + VB - VA  = 0
*/

double Polar::VelocityTrueWind(double VA, double VB, double W)
{
    double a = 1, b = 2 * VB * cos(deg2rad(W)), c = VB*VB - VA*VA;
    double det = b*b - 4*a*c;

    if(-b - sqrt(det) > 0) // sometimes there are 2 possible true winds
        printf("ambiguous true wind.\n");

    return (-b + sqrt(det)) / (2 * a);
}

/*
            Law of cosines;
           ________________________
          /  2    2
   VW =  / VA + VB - 2 VA VB cos(A)
       \/
*/
double Polar::VelocityTrueWind2(double VA, double VB, double A)
{
    return sqrt(VA*VA + VB*VB - 2*VA*VB*cos(deg2rad(A)));
}

Polar::Polar()
{
    m_crossoverpercentage = 0;
}

static char *strtok_polar(const char *line, char **saveptr)
{
    const char delim[] = " ;,\t\r\n";
    if(line)
        *saveptr = (char*)line;

    char *start = *saveptr, *c = start;
    while(*c == ' ') c++; // chomp spaces
    while(*c) {
        for(unsigned int i=0; i<(sizeof delim) / (sizeof *delim); i++)
            if(*c == delim[i]) {
                if(*c == '\r' || *c == '\n')
                    c[1] = 0;
                *c = 0;
                *saveptr = c+1;
                return start;
            }
        c++;
    }
    return NULL;
}

#define MAX_WINDSPEEDS_IN_TABLE 200
#define MESSAGE(S) (wxString::FromUTF8(wxFileName(filename).GetFullName()) \
                    + (linenum > 0 ? (_(" line ") + wxString::Format("%d", linenum)) : "") + ": " + S + "\n")
#define PARSE_WARNING(S) do { if(message.size() < 512) message += MESSAGE(S); } while (0)
#define PARSE_ERROR(S) do { message = _("Boat polar failed") + wxString("\n") \
                                  + MESSAGE(S); goto failed; } while (0)
bool Polar::Open(const wxString &filename, wxString &message)
{
    wind_speeds.clear();
    degree_steps.clear();

    if(filename[0] == 0)
        return false;
    
    int linenum = 0;
    ZUFILE *f = zu_open(filename, "r");
    char line[1024];
    double lastentryW = -1;
    char *token, *saveptr;
    double lastspeed = -1;
    bool warn_zeros = false;

    if(!f)
        PARSE_ERROR(_("Failed to open."));

    // polar file has optional first line which is description
    for(;;) {
        if(!zu_gets(f, line, sizeof line))
            PARSE_ERROR(_("Failed to read."));
        linenum++;

        token = strtok_polar(line, &saveptr);
        if(!token)
            PARSE_ERROR(_("Invalid data."));

        /* chomp invisible bytes */
        while(*token < 0) token++;

        if(!strcasecmp(token, "twa/tws") ||
           !strcasecmp(token, "twa\\tws") ||
           !strcasecmp(token, "twa"))
            break;

        if(linenum == 2)
            PARSE_ERROR(_("Unrecognized format."));
    }

    while((token = strtok_polar(NULL, &saveptr))) {
        double speed = strtod(token, 0);
        if(speed > lastspeed)
            wind_speeds.push_back(SailingWindSpeed(speed));
        else
            PARSE_ERROR(_("Invalid wind speeds.  Wind speeds must be increasing."));
        if(wind_speeds.size() > MAX_WINDSPEEDS_IN_TABLE)
            PARSE_ERROR(_("Too many wind speeds."));
        lastspeed = speed;
    }

    while(zu_gets(f, line, sizeof line)) {
        linenum++;

        if(!(token = strtok_polar(line, &saveptr)))
            break;

        double W = strtod(token, 0);

        if(W < 0 || W > 180) {
            PARSE_WARNING(_("Wind direction out of range."));
            continue;
        }

        if(W <= lastentryW) {
            PARSE_WARNING(_("Wind direction out of order."));
            continue;
        }

        // add zero speed for all wind speeds going against wind if not specified
#if 0        
        if(degree_steps.empty() && W > 0)
        {
            degree_steps.push_back(0);

            for(int VWi = 0; VWi < (int)wind_speeds.size(); VWi++)
                wind_speeds[VWi].speeds.push_back(0);
        }
#endif        

        degree_steps.push_back(W);
        lastentryW = W;

        {
            for(int VWi = 0; VWi < (int)wind_speeds.size(); VWi++) {
                double s = NAN;
                if((token = strtok_polar(NULL, &saveptr))) {
                    if(!strcmp(token, "0") && !warn_zeros) {
                        PARSE_WARNING(_("Warning: 0 values found in polar.\n"
                                        "These measurements will be interpolated.\n"
                                        "To specify interpolated, leave blank values.\n"
                                        "To specify course as 'invalid' use 0.0 rather than 0\n"));
                        warn_zeros = true;
                    } else if(*token) {
                        s = strtod(token, 0);
                        if(s < .05)
                            s = 0;
                    }
                } else
                    PARSE_WARNING(_("Too few measurements."));

                wind_speeds[VWi].orig_speeds.push_back(s);
            }

            if(strtok_polar(NULL, &saveptr))
                PARSE_WARNING(_("Too many measurements."));
        }
    }

    zu_close(f);


    UpdateSpeeds();

    FileName = wxString::FromUTF8(filename);
    return true;
    
failed:
    wind_speeds.clear();
    degree_steps.clear();

    zu_close(f);
    return false;
}

bool Polar::Save(const wxString &filename)
{
    FILE *f = fopen(filename, "w");
    if(!f)
        return false;

    // save zero wind speed column only if non-zero speeds
    int vwi0 = 1;
    if(wind_speeds[0].VW == 0) {
        for(unsigned int i=0; i<degree_steps.size(); i++)
            if(wind_speeds[0].speeds[i] != 0)
                vwi0 = 0;
    } else
        vwi0 = 0;

    
    fputs("twa/tws", f);
    for(unsigned int VWi = vwi0; VWi<wind_speeds.size(); VWi++)
        fprintf(f, ";%.4g", wind_speeds[VWi].VW);
    fputs("\n", f);

    for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
        if(degree_steps[Wi] > 180)
            break;
        fprintf(f, "%.5g", degree_steps[Wi]);
        for(unsigned int VWi = vwi0; VWi<wind_speeds.size(); VWi++)
            if(wxIsNaN(wind_speeds[VWi].orig_speeds[Wi]))
                fprintf(f, ";");
            else if(wind_speeds[VWi].speeds[Wi] == 0) // if we actually want zero?
                fprintf(f, ";0.01");
            else
                fprintf(f, ";%.5g", wind_speeds[VWi].speeds[Wi]);
        fputs("\n", f);
    }
    fclose(f);

    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
        CalculateVMG(VWi);

    return true;
}

/* instead of traveling in the direction given, allow traveling at angles
   closer to +- 90.  Basically we are trying every two tack angles and combining
   to determine if alternating between these two courses is actually faster than
   the current strategy for the given course, and if so, use it.

   TODO: Make this this work for asymmetrical polars correctly
*/
#if 0
void Polar::OptimizeTackingSpeed()
{
    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++) {
        CalculateVMG(VWi);
        for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
            double at = degree_steps[Wi];
            double bt, ct;
            
            if(at > 90 && at < 270) {
                bt = wind_speeds[VWi].VMG.values[SailingVMG::STARBOARD_DOWNWIND];
                ct = wind_speeds[VWi].VMG.values[SailingVMG::PORT_DOWNWIND];
            } else {
                bt = wind_speeds[VWi].VMG.values[SailingVMG::STARBOARD_UPWIND];
                ct = wind_speeds[VWi].VMG.values[SailingVMG::PORT_UPWIND];
            }

            if(wxIsNaN(bt) || wxIsNaN(ct))
                continue;
            
            double a = wind_speeds[VWi].speeds[Wi];
            double b = Speed(bt, wind_speeds[VWi].VW);
            double c = Speed(ct, wind_speeds[VWi].VW);
            
            /* w is the weight between b and c tacks (.5 for equal time on each
               t is the improvement factor]
               bcVB * sin(a) = w*b.VB*sin(b) + (1-w)*c.VB*sin(c)
               bcVB * cos(a) = w*b.VB*cos(b) + (1-w)*c.VB*cos(c) */
            double ar = deg2rad(at);
            double br = deg2rad(bt);
            double cr = deg2rad(ct);
            double sa = sin(ar), ca = cos(ar);
            double sb = sin(br), cb = cos(br);
            double sc = sin(cr), cc = cos(cr);
            double X = ca*sc-sa*cc, Y = sa*cb-ca*sb;
            double d = (X*c + Y*b);
            double w = X*c / d;
            
            if(w > 0 && w < 1) {
                double Z = cb*sc-sb*cc;
                double bcVB = Z*b*c / d;
                if(bcVB > a) {
                    a = bcVB;
//                    if(at > 180)
//                        a.b = bt, a.c = ct, a.w = w;
//                    else
//                        a.b = ct, a.c = bt, a.w = (1-w);
                }
            }           
        }
    }
}
#endif

// return index of wind speed in table which less than our wind speed
void Polar::ClosestVWi(double VW, int &VW1i, int &VW2i)
{
    for(unsigned int VWi = 1; VWi < wind_speeds.size()-1; VWi++)
        if(wind_speeds[VWi].VW > VW) {
            VW1i = VWi - 1;
            VW2i = VWi;
            return;
        }

    VW2i = wind_speeds.size()-1;
    VW1i = VW2i > 0 ? VW2i - 1 : 0;
}


bool Polar::VMGAngle(SailingWindSpeed &ws1, SailingWindSpeed &ws2, float VW, float &W)
{
    // optimization
    SailingVMG vmg1 = ws1.VMG, vmg2 = ws2.VMG;
    if(W >= vmg1.values[SailingVMG::STARBOARD_UPWIND] &&
       W >= vmg2.values[SailingVMG::STARBOARD_UPWIND] &&
       W <= vmg1.values[SailingVMG::STARBOARD_DOWNWIND] &&
       W <= vmg2.values[SailingVMG::STARBOARD_DOWNWIND])
        return false;
    if(W >= vmg1.values[SailingVMG::PORT_DOWNWIND] &&
       W >= vmg2.values[SailingVMG::PORT_DOWNWIND] &&
       W <= vmg1.values[SailingVMG::PORT_UPWIND] &&
       W <= vmg2.values[SailingVMG::PORT_UPWIND])
        return false;

    SailingVMG vmg = GetVMGTrueWind(VW);
    if(W < vmg.values[SailingVMG::STARBOARD_UPWIND] ||
       W > vmg.values[SailingVMG::PORT_UPWIND])
        W = vmg.values[SailingVMG::STARBOARD_UPWIND];
    else if(W > vmg.values[SailingVMG::STARBOARD_DOWNWIND] &&
            W < vmg.values[SailingVMG::STARBOARD_DOWNWIND])
        W = vmg.values[SailingVMG::STARBOARD_DOWNWIND];
    else
        return false;
    return true;
}

/* compute boat speed from true wind angle and true wind speed
 */
double Polar::Speed(double W, double VW, bool bound, bool optimize_tacking)
{
    if(VW < 0)
        return NAN;

    if(!degree_steps.size() || !wind_speeds.size())
        return NAN;

    W = positive_degrees(W);

    // assume symmetric
    if(W > 180)
        W = 360 - W;
    
    if(!optimize_tacking && (W < degree_steps[0] || W > degree_steps[degree_steps.size()-1]))
        return NAN;

    if(bound && (VW < wind_speeds[0].VW || VW > wind_speeds[wind_speeds.size()-1].VW))
        return NAN;

    unsigned int W1i = degree_step_index[(int)floor(W)];
    unsigned int W2i = W1i +1;
    if (W2i > degree_steps.size() - 1)
        W2i = W1i;

    double W1 = degree_steps[W1i];
    double W2 = degree_steps[W2i];

    int VW1i, VW2i;
    ClosestVWi(VW, VW1i, VW2i);
    SailingWindSpeed &ws1 = wind_speeds[VW1i], &ws2 = wind_speeds[VW2i];

    if(optimize_tacking) {
        float vmgW = W;
        if(VMGAngle(ws1, ws2, VW, vmgW))
            return Speed(vmgW, VW, bound)*cos(deg2rad(vmgW))/cos(deg2rad(W));
    }

    double VW1 = ws1.VW, VW2 = ws2.VW;

    double VB11 = ws1.speeds[W1i], VB12 = ws1.speeds[W2i];
    double VB21 = ws2.speeds[W1i], VB22 = ws2.speeds[W2i];

    double VB1 = interp_value(VW, VW1, VW2, VB11, VB21);
    double VB2 = interp_value(VW, VW1, VW2, VB12, VB22);

    double VB  = interp_value(W, W1, W2, VB1, VB2);

    if(VB < 0) // with faulty polars, extrapolation, sometimes results in negative boat speed
        return NAN;

    return VB;
}

double Polar::SpeedAtApparentWindDirection(double A, double VW, double *pW)
{
    int iters = 0;
    double VB = 0, W = A; // initial guess
    double lp = 1;
    for(;;) {
        double cVB = Speed(W, VW);
        VB -= (VB - cVB) * lp;

        double VA = VelocityApparentWind(VB, W, VW);
        double cA = positive_degrees(DirectionApparentWind(VA, VB, W, VW));

        if(wxIsNaN(cA) || iters++ > 256) {
            if(pW) *pW = NAN;
            return NAN;
        }

        if(fabs(cA - A) < 2e-2) {
            if(pW) *pW = W;
            return cVB;
        }

        W -= (cA - A) * lp;
        lp *= .97;
    }
}

double Polar::SpeedAtApparentWindSpeed(double W, double VA)
{
    int iters = 0;
    double VW = VA, VB = 0; // initial guess
    double lp = 1;
    for(;;) {
        double cVB = Speed(W, VW);
        VB -= (VB - cVB) * lp;

        double cVA = VelocityApparentWind(VB, W, VW);
        if(wxIsNaN(cVA) || iters++ > 256)
            return NAN;

        if(fabsf(cVA - VA) < 2e-2)
            return cVB;

        VW -= (cVA - VA) * lp;
        lp *= .97;
    }
}

double Polar::SpeedAtApparentWind(double A, double VA, double *pW)
{
    int iters = 0;
    double VW = VA, W = A, VB = 0; // initial guess
    double lp = 1;
    for(;;) {
        double cVB = Speed(W, VW);
        VB -= (VB - cVB) * lp;

        double cVA = VelocityApparentWind(VB, W, VW);
        double cA = positive_degrees(DirectionApparentWind(cVA, VB, W, VW));

        if(wxIsNaN(cVA) || wxIsNaN(cA) || iters++ > 256) {
            if(pW) *pW = NAN;
            return NAN;
        }

        if(fabsf(cVA - VA) < 2e-2 && fabsf(cA - A) < 2e-2) {
            if(pW) *pW = W;
            return cVB;
        }

        VW -= (cVA - VA) * lp;
        W -= (cA - A) * lp;
        lp *= .97;
    }
}

SailingVMG Polar::GetVMGTrueWind(double VW)
{
    int VW1i, VW2i;
    ClosestVWi(VW, VW1i, VW2i);

    SailingWindSpeed &ws1 = wind_speeds[VW1i], &ws2 = wind_speeds[VW2i];
    double VW1 = ws1.VW, VW2 = ws2.VW;
    SailingVMG vmg, vmg1 = ws1.VMG, vmg2 = ws2.VMG;

    for(int i=0; i<4; i++) {
        if(VW < VW1 || VW > VW2)
            vmg.values[i] = NAN;
        else
            vmg.values[i] = interp_value(VW, VW1, VW2, vmg1.values[i], vmg2.values[i]);
    }

    return vmg;
}

SailingVMG Polar::GetVMGApparentWind(double VA)
{
    SailingVMG avmg;

    for(int i=0; i<4; i++) {
        int iters = 0;
        double VW = VA, lp = 1;
        for(;;) {
            SailingVMG vmg = GetVMGTrueWind(VW);
            double W = vmg.values[i];
            if(wxIsNaN(W) || iters++ > 128) {
                avmg.values[i] = NAN;
                break;
            }

            double VB = Speed(W, VW);
            double cVA = VelocityApparentWind(VB, W, VW);            
            if(fabsf(cVA - VA) < 2e-1) {
                avmg.values[i] = W;
                break;
            }

            VW -= (cVA - VA) * lp;
            lp *= .97;
        }
    }

    return avmg;
}

/* find true wind speed from boat speed and true wind direction, because often at high
   winds the polar inverts and boat speed is lower, we can specify the max VW to search */
double Polar::TrueWindSpeed(double VB, double W, double maxVW)
{
    if(!degree_steps.size())
        return NAN;

    W = positive_degrees(W);
    // assume symmetric
    if(W > 180)
        W = 360 - W;

    unsigned int W1i = degree_step_index[(int)floor(W)];
    unsigned int W2i = degree_steps.size() == 1 ? 0 : W1i+1;

    double W1 = degree_steps[W1i], W2 = degree_steps[W2i];

    double VB1min = INFINITY, VW1min = NAN, VB1max = 0, VW1max = NAN;
    double VB2min = INFINITY, VW2min = NAN, VB2max = 0, VW2max = NAN;

    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++) {
        if(wind_speeds[VWi].VW > maxVW)
            break;

        SailingWindSpeed &ws = wind_speeds[VWi];
        double VB1 = ws.speeds[W1i];
        if(VB1 > VB && VB1 < VB1min) VB1min = VB1, VW1min = ws.VW;
        if(VB1 < VB && VB1 > VB1max) VB1max = VB1, VW1max = ws.VW;

        double VB2 = ws.speeds[W2i];
        if(VB2 > VB && VB2 < VB2min) VB2min = VB2, VW2min = ws.VW;
        if(VB2 < VB && VB2 > VB2max) VB2max = VB2, VW2max = ws.VW;
    }

    double VBmin = interp_value(W, W1, W2, VB1min, VB2min);
    double VWmin = interp_value(W, W1, W2, VW1min, VW2min);
    double VBmax = interp_value(W, W1, W2, VB1max, VB2max);
    double VWmax = interp_value(W, W1, W2, VW1max, VW2max);

    return interp_value(VB, VBmin, VBmax, VWmin, VWmax);
}

bool Polar::InterpolateSpeeds()
{
    bool ret = false;
    // interpolate wind speeds
    for(unsigned int j=0; j<degree_steps.size(); j++)
        for(unsigned int i=0; i<wind_speeds.size(); i++) {
            if(wxIsNaN(wind_speeds[i].speeds[j])) {
                // first try higher and lower wind speed
                for(int i0=i-1; i0>=0; i0--)
                    if(!wxIsNaN(wind_speeds[i0].speeds[j]))
                        for(unsigned int i1=i+1; i1<wind_speeds.size(); i1++)
                            if(!wxIsNaN(wind_speeds[i1].speeds[j])) {
                                wind_speeds[i].speeds[j] =
                                    interp_value(wind_speeds[i].VW,
                                                 wind_speeds[i0].VW,
                                                 wind_speeds[i1].VW,
                                                 wind_speeds[i0].speeds[j],
                                                 wind_speeds[i1].speeds[j]);
                                ret = true;
                                goto next;
                            }
                // now higher and lower angle
                for(int j0=j-1; j0>=0; j0--)
                    if(!wxIsNaN(wind_speeds[i].speeds[j0]))
                        for(unsigned int j1=j+1; j1<degree_steps.size(); j1++)
                            if(!wxIsNaN(wind_speeds[i].speeds[j1])) {
                                double W0 = degree_steps[j0], W1 = degree_steps[j1];
                                double W = degree_steps[j];
                                double VW = wind_speeds[i].VW;
                                double VB0 = wind_speeds[i].speeds[j0];
                                double VB1 = wind_speeds[i].speeds[j1];
                                double VA0 = VelocityApparentWind(VB0, W0, VW);
                                double VA1 = VelocityApparentWind(VB0, W1, VW);
                                double A0  = DirectionApparentWind(VA0, VB0, W0, VW);
                                double A1  = DirectionApparentWind(VA1, VB1, W1, VW);

                                // for different wind angles, use sailboat transform to
                                // interpolate the boat speed, that is
                                // VB = sin(A/2) * x
                                double x0 = VB0 / sin(deg2rad(A0/2));
                                double x1 = VB1 / sin(deg2rad(A1/2));

                                double A = A0; // initial
                                double VB = VB1;
                                int c;
                                for(c=0; c<24; c++) { // limit iterations
                                    double x = interp_value(A, A0, A1, x0, x1);
                                    double lastVB = sin(deg2rad(A/2)) * x;
                                    if(fabs(lastVB - VB) < .001)
                                        break;
                                    VB = lastVB;
                                    double VA  = VelocityApparentWind(VB, W, VW);
                                    A = DirectionApparentWind(VA, VB, W, VW);
                                }

                                // if iteration falls outside range, use simple interpolation
                                if(A < A0 || A > A1 || c == 24)
                                    wind_speeds[i].speeds[j] = interp_value(W, W0, W1, VB0, VB1);
                                else
                                    wind_speeds[i].speeds[j] = VB;

                                ret = true;
                                goto next;
                            }
            next:;
            }
        }
    return ret;
}

void Polar::UpdateSpeeds()
{
    // interpolate wind speeds
    for(unsigned int i=0; i<wind_speeds.size(); i++) {
        wind_speeds[i].speeds.clear();
        for(unsigned int j=0; j<degree_steps.size(); j++)
            wind_speeds[i].speeds.push_back(wind_speeds[i].orig_speeds[j]);
    }

    while(InterpolateSpeeds());

    // add invalid speed in 0 wind in all directions, if not specified
#if 0
    if(wind_speeds[0].VW > 0) {
        std::vector<SailingWindSpeed> speeds;
        speeds.push_back(SailingWindSpeed(0));
        for(unsigned int i=0; i<degree_steps.size(); i++)
            speeds[0].speeds.push_back(NAN);
        for(unsigned int i=0; i<wind_speeds.size(); i++)
            speeds.push_back(wind_speeds[i]);
        wind_speeds = speeds;
    }
#endif

    UpdateDegreeStepLookup();

    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
        CalculateVMG(VWi);
}

void Polar::UpdateDegreeStepLookup()
{
    unsigned int Wi = 0;
    for(int d = 0; d < DEGREES; d++) {
        while(Wi < degree_steps.size()-1) {
            if(d <= degree_steps[Wi+1])
                break;
            Wi++;
        }

        degree_step_index[d] = Wi;
    }
}

// Determine if our current state is satisfied by the current cross over contour
bool Polar::InsideCrossOverContour(float H, float VW, bool optimize_tacking)
{
    if(optimize_tacking) {
        int VW1i, VW2i;
        ClosestVWi(VW, VW1i, VW2i);
        SailingWindSpeed &ws1 = wind_speeds[VW1i], &ws2 = wind_speeds[VW2i];
        VMGAngle(ws1, ws2, VW, H);
    }
    // rounding error, XXX what about overlapping ?
    if(VW < wind_speeds[0].VW || VW > wind_speeds[wind_speeds.size()-1].VW)
        return false;

    // XXX?
    H = fabs(H);
    if (H > 180.)
        H -= 180.;
    // yeah motor boat...
    if (VW == 0.)
        VW = 0.01;
    return CrossOverRegion.Contains(H, VW);
}

float SailboatTransformSpeed(double W, double VW, double eta)
{
    /* starting out not moving */
    double VB = 0, A = W, VA = VW;
    double lp = .1;

    const int count = 128;
    double bucket = 0;
    int bcount = 0;

    for(;;) {
        double v = sin(deg2rad(A/2)) * sqrt(VA / eta);

        if(v == 0) // we cannot sail this way
            return 0;

        double a = v - VB;

        if(bcount == count) {
            VB = bucket / count;
            a = 0;
        }

        if(fabs(a) < 1e-2 || lp < 1e-2) {
            if(VB < 0) // not allowing backwards sailing
                VB = 0;
            return VB; /* reached steady state */
        }

        if(a < 0) {
            bucket += VB;
            bcount++;
//            lp *= .97;
        }

        VB = (1-lp)*VB + lp*(VB+a); /* lowpass to get a smooth update */
        VA = Polar::VelocityApparentWind(VB, W, VW);
        A =  Polar::DirectionApparentWind(VA, VB, W, VW);
    }
}

float BoatSpeedFromMeasurements(const std::list<PolarMeasurement> &measurements,
                                double W, double VW)
{
#if 1
    // find 3 closest measurements
    double dist[3] = {INFINITY, INFINITY, INFINITY};
    std::list<PolarMeasurement>::const_iterator pos[3] =
        {measurements.end(), measurements.end(), measurements.end()};
    for(std::list<PolarMeasurement>::const_iterator it = measurements.begin();
        it != measurements.end(); it++) {

        double mVW = it->VW(), mW = it->W();

        // could add constants multiplier for one axis here...
        double d = pow(mW - W, 2) + pow(mVW - VW, 2);

        if(d < dist[2]) {
            if(d < dist[1]) {
                if(d < dist[0]) {
                    dist[2] = dist[1], dist[1] = dist[0], dist[0] = d;
                    pos[2] = pos[1], pos[1] = pos[0], pos[0] = it;
                } else {
                    dist[2] = dist[1], dist[1] = d;
                    pos[2] = pos[1], pos[1] = it;
                }
            } else {
                dist[2] = d;
                pos[2] = it;
            }
        }

        // eta = VA/VB^2*(1-cos(A))/2
//            besteta = mVA*(1-cos(deg2rad(mA)))/(2*mVB*mVB);
//            bestVW = mVW;
    }

    double besteta;
    if(pos[0] != measurements.end()) {
        if(pos[1] != measurements.end() && pos[2] != measurements.end()) {
            // plane fit
            double mVW[3], mW[3], meta[3];
            for(int i=0; i<3; i++)
                mVW[i] = pos[i]->VW(), mW[i] = pos[i]->W(), meta[i] = pos[i]->eta;

            /*
            m.eta0 = dVW*m0.VW + dW*m0.W + ceta;
            m.eta1 = dVW*m1.VW + dW*m1.W + ceta;
            m.eta2 = dVW*m2.VW + dW*m2.W + ceta;
            */
            double det = mVW[0]*(mW[2]-mW[1])-mVW[1]*mW[2]+mW[1]*mVW[2]+mW[0]*(mVW[1]-mVW[2]);
            double dVW = (meta[0]*(mW[2]-mW[1])-
                          meta[1]*(mW[2])+
                          meta[2]*mW[1]+(meta[1]-meta[2])*mW[0])
                /det;
            double dW = -(meta[0]*(mVW[2]-mVW[1])-
                          meta[1]*mVW[2]+meta[2]*mVW[1]+
                          (meta[1]-meta[2])*mVW[0])
                /det;
            double ceta = (meta[0]*(mW[1]*mVW[2]-mVW[1]*mW[2])+
                           mVW[0]*(meta[1]*mW[2]-meta[2]*mW[1])+
                           mW[0]*(meta[2]*mVW[1]-meta[1]*mVW[2]))
                /det;

            besteta = dVW*VW + dW*W + ceta;
            if(det == 0 || wxIsNaN(besteta))
                besteta = pos[0]->eta;
             
            if((besteta < pos[0]->eta &&
                besteta < pos[1]->eta &&
                besteta < pos[2]->eta) ||
               (besteta > pos[0]->eta &&
                besteta > pos[1]->eta &&
                besteta > pos[2]->eta))
                besteta = pos[0]->eta;
        } else
            besteta = pos[0]->eta;
    } else
        besteta = 1;
#else
        // find 1 closest measurements
    double dist = INFINITY;
    std::list<PolarMeasurement>::const_iterator pos = measurements.end();
    for(std::list<PolarMeasurement>::const_iterator it = measurements.begin();
        it != measurements.end(); it++) {

        double mVW = it->VW(), mW = it->W();

        // could add constants multiplier for one axis here...
        double d = pow(mW - W, 2) + pow(mVW - VW, 2);

        if(d < dist) {
            pos = it;
            dist = d;
        }

        // eta = VA/VB^2*(1-cos(A))/2
//            besteta = mVA*(1-cos(deg2rad(mA)))/(2*mVB*mVB);
//            bestVW = mVW;
    }

    double besteta;
    if(pos != measurements.end())
        besteta = pos->eta;
    else
        besteta = 1;
#endif
    if(besteta < .001)
        besteta = .001;
    if(besteta > 1)
        besteta = 1;

    return SailboatTransformSpeed(W, VW, besteta);
}
    
void Polar::Generate(const std::list<PolarMeasurement> &measurements)
{
    for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
        double W = degree_steps[Wi];
        for(unsigned int VWi = 0; VWi<wind_speeds.size(); VWi++) {
            double VW = wind_speeds[VWi].VW;
            wind_speeds[VWi].speeds[Wi] = BoatSpeedFromMeasurements(measurements, W, VW);
        }
    }
}

void Polar::CalculateVMG(int VWi)
{
    SailingWindSpeed &ws = wind_speeds[VWi];
    // limits for port/starboard upwind/downwind
    const double limits[4][2] = {{0, 90}, {270, 360}, {90, 180}, {180, 270}};
    for(int i=0; i<4; i++) {
        double upwind = i < 2 ? 1 : -1;
        double maxVB = 0;
        double maxW = NAN;
        unsigned int maxWi = 0;
        for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
            double W = degree_steps[Wi];

            if(W < limits[i][0] || W > limits[i][1])
                continue;

            double VB = upwind*cos(deg2rad(W))*ws.speeds[Wi];// original speed before optimizing
            if(VB > maxVB) {
                maxVB = VB;
                maxW = W;
                maxWi = Wi;
            }
        }

        // interpolate the best vmg (as it often lies between entries in the polar
        if(!wxIsNaN(maxW)) {
            unsigned int Wi1 = maxWi > 0 ? maxWi - 1 : maxWi;
            unsigned int Wi2 = maxWi < degree_steps.size() - 1 ? maxWi + 1 : maxWi;
            double dsmaxWi = degree_steps[maxWi];
            double step = wxMax(fabsf(dsmaxWi - degree_steps[Wi1]),
                                fabsf(dsmaxWi - degree_steps[Wi2])) / 4;

            while(step > 2e-3) {
                double W1 = wxMax(maxW-step, limits[i][0]), W2 = wxMin(maxW+step, limits[i][1]);
                double VB1 = upwind*cos(deg2rad(W1))*Speed(W1, ws.VW, true);
                double VB2 = upwind*cos(deg2rad(W2))*Speed(W2, ws.VW, true);

                if(VB1 > VB2)
                    maxW = (W1 + maxW) / 2;
                if(VB1 < VB2)
                    maxW = (W2 + maxW) / 2;

                step /= 2;
            }
        }

        ws.VMG.values[i] = maxW;
    }

    // for symmetric polars
    if(degree_steps[degree_steps.size()-1] <= 180) {
        ws.VMG.values[SailingVMG::PORT_UPWIND] = 360 - ws.VMG.values[SailingVMG::STARBOARD_UPWIND];
        ws.VMG.values[SailingVMG::PORT_DOWNWIND] = 360 - ws.VMG.values[SailingVMG::STARBOARD_DOWNWIND];
    }
}

void Polar::AddDegreeStep(double twa)
{
    unsigned int Wi;
    for(Wi = 0; Wi < degree_steps.size(); Wi++)
        if(twa < degree_steps[Wi])
            break;

    degree_steps.insert(degree_steps.begin()+Wi, twa);
    for(unsigned int VWi = 0; VWi<wind_speeds.size(); VWi++)
        wind_speeds[VWi].orig_speeds.insert(wind_speeds[VWi].orig_speeds.begin()+Wi, NAN);

    UpdateSpeeds();
    UpdateDegreeStepLookup();
}

void Polar::RemoveDegreeStep(int index)
{
    degree_steps.erase(degree_steps.begin()+index);
    for(unsigned int VWi = 0; VWi<wind_speeds.size(); VWi++)
        wind_speeds[VWi].orig_speeds.erase(wind_speeds[VWi].orig_speeds.begin()+index);

    UpdateSpeeds();
    UpdateDegreeStepLookup();
}

void Polar::AddWindSpeed(double tws)
{
    unsigned int VWi;
    for(VWi = 0; VWi<wind_speeds.size(); VWi++)
        if(tws < wind_speeds[VWi].VW)
            break;

    wind_speeds.insert(wind_speeds.begin()+VWi, SailingWindSpeed(tws));
    for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++)
        wind_speeds[VWi].orig_speeds.push_back(NAN);
    UpdateSpeeds();
}

void Polar::RemoveWindSpeed(int index)
{
    wind_speeds.erase(wind_speeds.begin()+index);
    UpdateSpeeds();
}
