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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "zuFile.h"

#include "Utilities.h"
#include "Boat.h"

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
    return x2 - x1 ? (y2 - y1)*(x - x1)/(x2 - x1) + y1 : y1;
}

/* compute apparent wind speed from boat speed and true wind */
double Polar::VelocityApparentWind(double VB, double W, double VW)
{
  return sqrt(VW*VW + VB*VB + 2*VW*VB*cos(W)); /* law of cosines, W is flipped by 180 */
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
  while(W > M_PI) W-=2*M_PI;
  while(W < -M_PI) W+=2*M_PI;
  return W > 0 ? ac : -ac;
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

   a = 1
   b = 2 * VB * cos(W)
   c = VB^2 - VA^2
*/
double Polar::VelocityTrueWind(double VA, double VB, double W)
{
    double a = 1, b = 2 * VB * cos(W), c = VB*VB - VA*VA;
    double det = b*b - 4*a*c;

    if(-b - sqrt(det) > 0)
        printf("ambiguous true wind.\n");

    return (-b + sqrt(det)) / (2 * a);
}

Polar::Polar()
{
}

#define MAX_WINDSPEEDS_IN_TABLE 200
#define MESSAGE(S) (S + wxString(_T("\n")) + wxString::FromUTF8(filename) \
                    + (line > 0 ? (_(" line ") + wxString::Format(_T("%d"), linenum)) : _T("")))
#define PARSE_WARNING(S) do { if(message.empty()) message = MESSAGE(S); } while (0)
#define PARSE_ERROR(S) if(message.empty()) do { message = _("Boat polar failed") + wxString(_T("\n")) \
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
    const char delim[] = ";, \t\r\n";

    if(!f)
        PARSE_ERROR(_("Failed to open."));

    // polar file has optional first line which is description
    for(;;) {
        if(!zu_gets(f, line, sizeof line))
            PARSE_ERROR(_("Failed to read."));

        token = strtok_r(line, delim, &saveptr);
        linenum++;

        /* chomp invisible bytes */
        while(*token < 0) token++;

        if(!strcasecmp(token, "twa/tws") ||
           !strcasecmp(token, "twa\\tws") ||
           !strcasecmp(token, "twa"))
            break;

        if(linenum == 2)
            PARSE_ERROR(_("Unrecognized format."));
    }
    
    while((token = strtok_r(NULL, delim, &saveptr))) {
        wind_speeds.push_back(SailingWindSpeed(strtod(token, 0)));
        if(wind_speeds.size() > MAX_WINDSPEEDS_IN_TABLE)
            PARSE_ERROR(_("Too many wind speeds."));
    }

    while(zu_gets(f, line, sizeof line)) {
        linenum++;

#if 0
        /* strip newline/linefeed */
        for(unsigned int i=0; i<strlen(line); i++)
            if(line[i] == '\r' || line[i] == '\n')
                line[i] = '\0';
#endif

        if(!(token = strtok_r(line, delim, &saveptr)))
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
        if(degree_steps.empty() && W > 0)
        {
            degree_steps.push_back(0);

            for(int VWi = 0; VWi < (int)wind_speeds.size(); VWi++)
                wind_speeds[VWi].speeds.push_back(0);
        }

        degree_steps.push_back(W);
        lastentryW = W;

        {
            for(int VWi = 0; VWi < (int)wind_speeds.size(); VWi++) {
                double s = 0;
                if((token = strtok_r(NULL, delim, &saveptr)))
                    s = strtod(token, 0);
                else
                    PARSE_WARNING(_("Too few tokens."));

                wind_speeds[VWi].speeds.push_back(s);
            }

            if(strtok_r(NULL, delim, &saveptr))
                PARSE_WARNING(_("Too many tokens."));
        }
    }

    zu_close(f);

    /* fill in port tack assuming symmetric */
    {
        int Win = degree_steps.size()-1;
        if(degree_steps[Win] == 180) Win--;
        for(; Win >= 0; Win--) {
            if(degree_steps[Win] == 0)
                break;
            
            degree_steps.push_back(DEGREES - degree_steps[Win]);
            
            for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
                wind_speeds[VWi].speeds.push_back(wind_speeds[VWi].speeds[Win]);
        }
    }
        
    UpdateDegreeStepLookup();
    
    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
        CalculateVMG(VWi);

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

    fputs("twa/tws", f);
    for(unsigned int VWi = 0; VWi<wind_speeds.size(); VWi++)
        fprintf(f, ";%.4g", wind_speeds[VWi].VW);
    fputs("\n", f);

    for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
        if(degree_steps[Wi] > 180)
            break;
        fprintf(f, "%.5g", degree_steps[Wi]);
        for(unsigned int VWi = 0; VWi<wind_speeds.size(); VWi++)
            fprintf(f, ";%.5g", wind_speeds[VWi].speeds[Wi]);
        fputs("\n", f);
    }
    fclose(f);
    return true;
}

/* instead of traveling in the direction given, allow traveling at angles
   closer to +- 90.  Basically we are trying every two tack angles and combining
   to determine if alternating between these two courses is actually faster than
   the current strategy for the given course, and if so, use it.

   TODO: Make this this work for asymmetrical polars correctly
*/
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

            if(isnan(bt) || isnan(ct))
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


// return index of wind speed in table which less than our wind speed
void Polar::ClosestVWi(double VW, int &VW1i, int &VW2i)
{
    for(unsigned int VWi = 1; VWi < wind_speeds.size()-1; VWi++)
        if(wind_speeds[VWi].VW > VW) {
            VW1i = VWi - 1;
            VW2i = VWi;
            return;
        }

    VW1i = VW2i = wind_speeds.size()-1;
}

/* compute boat speed from true wind angle and true wind speed

   Maybe this should use rectangular coordinate interpolation when the speed
   is outside the VMG zone.
 */
double Polar::Speed(double W, double VW)
{
    if(VW < 0)
        return NAN;

    W = positive_degrees(W);
    unsigned int W1i = degree_step_index[(int)floor(W)], W2i;
    double W1 = degree_steps[W1i], W2;
    if(W1i == degree_steps.size()-1) {
        W2i = 0;
        W2 = 360 + degree_steps[0];
    } else {
        W2i = W1i+1;
        W2 = degree_steps[W2i];
    }

    int VW1i, VW2i;
    ClosestVWi(VW, VW1i, VW2i);
    if(VW1i == (int)wind_speeds.size() - 1)
        VW2i = VW1i;
    else
        VW2i = VW1i + 1;

    SailingWindSpeed &ws1 = wind_speeds[VW1i], &ws2 = wind_speeds[VW2i];
    double VW1 = ws1.VW, VW2 = ws2.VW;

    double VB11 = ws1.speeds[W1i], VB12 = ws1.speeds[W2i];
    double VB21 = ws2.speeds[W1i], VB22 = ws2.speeds[W2i];

    double VB1 = interp_value(VW, VW1, VW2, VB11, VB21);
    double VB2 = interp_value(VW, VW1, VW2, VB12, VB22);

    double VB  = interp_value(W, W1, W2, VB1, VB2);

    if(VB < 0) // with faulty polars, extrapolation, sometimes results in negative boat speed
        return 0;

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

        double VA = VelocityApparentWind(VB, deg2rad(W), VW);
        double cA = rad2posdeg(DirectionApparentWind(VA, VB, deg2rad(W), VW));

        if(isnan(cA) || iters++ > 256) {
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

        double cVA = VelocityApparentWind(VB, deg2rad(W), VW);
        if(isnan(cVA) || iters++ > 256)
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

        double cVA = VelocityApparentWind(VB, deg2rad(W), VW);
        double cA = rad2posdeg(DirectionApparentWind(cVA, VB, deg2rad(W), VW));

        if(isnan(cVA) || isnan(cA) || iters++ > 256) {
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

    for(int i=0; i<4; i++)
        vmg.values[i] = interp_value(VW, VW1, VW2, vmg1.values[i], vmg2.values[i]);

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
            if(isnan(W) || iters++ > 128) {
                avmg.values[i] = NAN;
                break;
            }

            double VB = Speed(W, VW);
            double cVA = VelocityApparentWind(VB, deg2rad(W), VW);            
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
    W = positive_degrees(W);
    unsigned int W1i = degree_step_index[(int)floor(W)], W2i;
    if(W1i == degree_steps.size()-1)
        W2i = 0;
    else
        W2i = W1i++;
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

void Polar::UpdateDegreeStepLookup()
{
    for(int d = 0; d < DEGREES; d++) {
        unsigned int Wi;
        for(Wi = 0; Wi < degree_steps.size(); Wi++)
            if(d < degree_steps[Wi])
                break;

        degree_step_index[d] = Wi - 1;
    }
}

// Determine if our current state is satisfied by the current cross over contour
bool Polar::InsideCrossOverContour(float H, float VW)
{
    return CrossOverRegion.Contains(H, VW);
}

void Polar::CalculateVMG(int VWi)
{
    SailingWindSpeed &ws = wind_speeds[VWi];
    // limits for port/starboard upwind/downwind
    const double limits[4][2] = {{270, 360}, {0, 90}, {180, 270}, {90, 180}};
    for(int i=0; i<4; i++) {
        double upwind = i < 2 ? 1 : -1;
        double maxVB = 0;
        float maxW = NAN;
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
        if(!isnan(maxW)) {
            unsigned int Wi1 = maxWi > 0 ? maxWi - 1 : degree_steps.size() - 1;
            unsigned int Wi2 = maxWi < degree_steps.size() - 1 ? maxWi + 1 : 0;
            double dsmaxWi = degree_steps[maxWi];
            double step = wxMax(fabsf(dsmaxWi - degree_steps[Wi1]),
                                fabsf(dsmaxWi - degree_steps[Wi2])) / 4;

            while(step > 2e-3) {
                double W1 = wxMax(maxW-step, limits[i][0]), W2 = wxMin(maxW+step, limits[i][1]);
                double VB1 = upwind*cos(deg2rad(W1))*Speed(W1, ws.VW);
                double VB2 = upwind*cos(deg2rad(W2))*Speed(W2, ws.VW);

                if(VB1 > VB2)
                    maxW = (W1 + maxW) / 2;
                else
                    maxW = (W2 + maxW) / 2;

                step /= 2;
            }
        }

        ws.VMG.values[i] = maxW;
    }
}
