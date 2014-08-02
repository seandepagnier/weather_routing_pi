/******************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier                                 *
 *   sean@depagnier.com                                                    *
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

static const int computed_wind_speeds[] = {0, 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 36, 40, 45, 50, 55, 60};
static const unsigned int num_computed_wind_speeds = (sizeof computed_wind_speeds) / (sizeof *computed_wind_speeds);

static const double computed_max_knots = computed_wind_speeds[num_computed_wind_speeds - 1];
static const unsigned int computed_degree_step = 3;
static const unsigned int computed_degree_count = (DEGREES / computed_degree_step);

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

/* used to convert from one table to another */
#if 0
double BoatSpeedTable::InterpolateSpeed(double VW, double W)
{
    BoatSpeedTableEntry *minW = NULL, *maxW = NULL;
    for(std::vector<BoatSpeedTableEntry>::iterator it = table.begin(); it != table.end(); ++it) {
        if((*it).W >= W && (!maxW || maxW->W > (*it).W))
            maxW = &(*it);

        if((*it).W <= W && (!minW || minW->W < (*it).W))
            minW = &(*it);
    }

    int minVWind = -1, maxVWind = -1;
    for(unsigned int i=0; i<windspeeds.size(); i++) {
        if(windspeeds[i] >= VW && (maxVWind == -1 || windspeeds[maxVWind] > windspeeds[i]))
            maxVWind = i;

        if(windspeeds[i] <= VW && (minVWind == -1 || windspeeds[minVWind] < windspeeds[i]))
            minVWind = i;
    }

    if(!minW || !maxW || minVWind == -1 || maxVWind == -1)
        return 0;

    double x1, x2, y1, y2;
    x1 = windspeeds[minVWind];
    x2 = windspeeds[maxVWind];
    y1 = minW->boatspeed[minVWind];
    y2 = minW->boatspeed[maxVWind];
    double minWBoatspeed = interp_value(VW, x1, x2, y1, y2);

    y1 = maxW->boatspeed[minVWind];
    y2 = maxW->boatspeed[maxVWind];
    double maxWBoatspeed = interp_value(VW, x1, x2, y1, y2);

    x1 = minW->W;
    x2 = maxW->W;
    y1 = minWBoatspeed;
    y2 = maxWBoatspeed;

    return interp_value(W, x1, x2, y1, y2);
}
#endif

/* to calculate power required to get the boat on plane

   horsepower = Disp * lwl / ((2.3-(8.26 / (Disp / (.01 lwl)^3 )^.311))*8.3)^3

   The boat has forces:

apparent
 wind      ^ powered
     \     | thrust
      \    |       /|
       \|         / sail
           /\    / power
          |  |
          |  |  ---->
          |  |  leeway
          |__|
         
           |
           |  drag
          \|/

technically drag is going against any movement,
but we can simplify and have it go against forward movement

Since we know BV is related to square root
of power input, the coefficient k computes speed

f - forward velocity
s - slip velocity
j - keel lateral force at 1 knot
k - drag at 1 knot
l - keel lift factor

k = wetted_sq_ft drag_coefficient
Drag is related to velocity times wetted surface area squared
       
D = k f

a(t) = (P - D)/m
f(infinity) = integral(a(t), t, 0..infinity)

P = k V

P = V P

       2
P = k V
                 A    __
FSx = k eta sin (-) \/VA
                 2

                 A    __
FSy = k eta cos (-) \/VA
                 2

FMx = sqrt(P k)
FMy = 0

FKx = f (l s - k)
FKy = - s j

f = (FSx + FKx + FMx)dt = (k eta sin(A/2)*sqrt(VA) + f*(l*s-k)) dt

s = (FSy + FKy + FMy)dt

s(inf) = k eta cos(A/2) sqrt(VA) / j
f(inf) = eta sin(A/2) sqrt(VA) / (1 - l / j s(inf))

         eta sin(A/2) sqrt(VA) + P
         ---------------------------
f(inf) = 1 - l s(inf)
             -
             k

(define (s eta A VA k j) (* k eta (cos (/ (deg2rad A) 2)) (sqrt VA) (/ j)))
(define (f eta A VA l k j P) (/ (+ (* eta (sin (/ (deg2rad A) 2)) (sqrt VA)) P) (- 1 (* (/ l k) (s eta A VA k j)))))
(define (aoa eta A VA l k j P) (rad2deg (atan (s eta A VA k j) (v eta A VA l k j P))))
(define (BV eta A VA l k j P) (sqrt (+ (square (s eta A VA k j)) (square (f eta A VA l k j P)))))
aoa - angle of attack, direction keel is moving thru water
aoa = atan2(s, f)
BV
        _______
BV =   / 2    2
     \/ f  + s

*/

/* difference between direction we face and direction moving thru water */
double BoatPlan::AngleofAttackBoat(double A, double VA)
{
    return 0;
}

/* speed of boat give apparent wind based on sailboat transform

 */
double BoatPlan::VelocityBoat(double A, double VA)
{
    /* are we luffing? */
    if(A < deg2rad(luff_angle))
        return 0;

    if(eta <= 0) /* not ideal but prevent nans */
        return 0;

    double val = sin(A/2) * sqrt(VA / eta);

    /* for wing on wing, increase speed when wind is behind
       reaching 50% speed increase when dead downwind */
    if(wing_wing_running && A > deg2rad(90))
        val += val*sin(A - deg2rad(90))/2;

    return val;
}

/* Now that we can convert the wind speed in gribs correctly
   still need more coefficients.  We can run calculations
   relative to motion thru water rather  than over ground
   now.  Also the wind angle is the true bearing of the wind
   across the water.  The final boat direction is therefore
   shifted from this:

   B  - boat direction thru water relative to north
   W  - true wind direction (relative to vessel)

   optionally could include slippage here

   Solve for the augmented sailboat transform for apparent wind

   W  - true wind direction (relative to vessel)
   A  - apparent wind direction
   VW - true wind velocity
   VA - apparent wind velocity
   eta - constant for boat efficiency (drag ratio)
   VB - boat velocity

                           Angle is W - A
                          /|
                        / /
        Wind (VW)     /  |
                    /   /
  Angle is Pi-W   /    |
sin(Pi-W)=sin(W) |    /  Apparent Wind (VA)
cos(Pi-W)=-cos(W)|   |
     Boat (VB)   |  /
                 | |
                 |/
              Angle is A

            Law of sines:

      sin A   sin Pi-W   sin W-A
      ----- = -------- = -------
       VW        VA        VB

            Law of cosines;
           ________________________
          /  2    2
   VW =  / VA + VB - 2 VA VB cos(A)
       \/
           ________________________
          /  2    2
   VA =  / VW + VB  + 2 VW VB cos(W)
       \/
           ___________________________
          /  2    2
   VB =  / VW + VA  - 2 VW VA cos(W-A)
       \/

Half angle formula:

        2        1 - cos(A)
     sin (A/2) = ----------
                     2

   The original sailboat transform relating W and A to VW and eta:
                                2
                  /  sin(A/2)  \
   sin(W) sin(A) |  ----------  | = VW eta
                  \ sin(W - A) /

   Reformulation in terms of VB instead of VW:
             2      
   sin(W) sin (A/2)
   ---------------- = VB eta
      sin(W - A)

                      VB
    sin(W-A) = sin(W) --    Law of sines
                      VA

           2          2
     VA sin (A/2) = VB eta

          ____________
         /      2     
        / VA sin (A/2)
VB =   /  -----------
     \/        eta
                 __
           A    /VA
VB =  sin (-)  /---
           2 \/ eta

To decompose further to forces
BF - Boat Force

Since at speed, drag force exactly cancels boat force (no acceleration):
F = m a
V = integral(F / m)

    inf              __
  /       
 /  F   m
 \     =
  \ 
  /
 /  0
                      __
                A    /VA
VB(inf) =  sin (-)  /---
                2 \/ eta

DF(inf) = BF(inf)
DF(0) = 0
BV(0) = 0
DF(t) = DC VB(t)

        int(BF, 0..t) - DC int(VB, 0..t) = VB(t)

F(t) = BF(A(t), VA(t)) - DF(t)
F(t) = BF(A(t), VA(t)) - DC VB(t)

                      /                 /
                     |                 |
                     |  BF(t) dt - DC  |  VB(t) dt = m VB(t)
                     |                 |
                    /                 /

                   __
             A    /VA
BF = DC sin (-)  /---
             2 \/ eta

                      /                 /
                     |                 |
                     |  BF(t) dt - DC  |  VB(t) dt = m VB(t)
                     |                 |
                    /                 /

Finally we have simplified to this equation to have boat speed in
apparent wind components only.  The above formula states that boat speed
from sail power is directly related to sin(A/2).  The further off the wind,
the more effective for converting to power (obviously true), but in this case
the actual velocity of the apparent wind greatly decreases as well which
suggests the course for maximum speed must be more than 90 and less than 180.

Because the use of eta in the original sailboat transform is
ill-placed for computation, and confusing to utilize,
I have redefined eta to be SC which is the sailing coefficient
A boat with SC of 1 sails the speed of the true wind in ideal conditions.  An eta of 2
would sail twice the speed of the wind.

The term P which indicates additional thrust
or power (motor) enough to move the boat that many knots
in a dead calm

Since the steady state power required to move most boats is basically
related to the square of the speed:

       2
SP = VB

SC - Sail Coeffcient
SP - Sail Power
SF - Sail Force
MP - Motor Power
 D - Drag

power = force * velocity
power = work / time
work  = force * distance
velocity = distance / time

              2 A
SP = SC VA sin (-)
                2
           2 A
SF = SC sin (-)
             2

We can express A as a function:

The steady state speed (time is infinity) for the boat's mass
is irrelevant.  It is convenient to calculate but we are
really after the position of the vessel via double integration.

velocity of the air molecules is VA so the amount of wind
we can catch is related to the cross sectional area
of the sail and direction of flow:

      2 A
SC sin (-)
        2

This is, to say, this the the c
  2
VB  = BP - 
BP = SP
               2
BP = 64 eta VA sin (A/2)   

If P is negative then it is drag, or slows the boat by this amount.
If the overall result is 

; scheme code to compute boat speed steady state
(define (square x) (* x x))
(define (rad2deg x) (* 180 (/ 3.14) x))
(define (deg2rad x) (* 3.14 (/ 180) x))

(define (compute-VB A VA eta P)
  (let ((d (+ (* 64 (square eta) (square (sin (/ A 2))) VA) (* P (abs P)))))
    ((if (negative? d) - +) (sqrt (abs d)))))

(define (compute-VA VW VB W) ; law of cosines
   (let ((d (+ (square VW) (square VB) (* 2 VW VB (cos W)))))
     (if (negative? d) (error "d < 0 : " d) (sqrt d))))
      
(define (compute-A VA VB VW W) ; law of sines
   (if (zero? VA) 0
     (if (zero? VB) W
       (let ((d (/ (+ (square VA) (square VB) (- (square VW)))
                   (* 2 VA VB))))
            (cond ((> d 1) 0)
                  ((< d -1) 3.14159)
                  (else (acos d)))))))

(define (boat-speed VW dW eta P)
   (let ((W (deg2rad dW)))
     (let each-vb ((A W) (VA VW) (VB 0))
       (let ((nVB (compute-VB A VA eta P)))
       (if (<= (abs (- nVB VB)) 1e-8)
           (print "A: " (rad2deg A) " VA: " VA " VB: "  VB)
           (let* ((nVB (/ (+ nVB VB) 2))
                  (nVA (compute-VA VW nVB W))
                  (nA (compute-A nVA nVB VW W)))
               (each-vb nA nVA nVB)))))))
*/

/* compute apparent wind speed from boat speed and true wind */
double BoatPlan::VelocityApparentWind(double VB, double W, double VW)
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
double BoatPlan::DirectionApparentWind(double VA, double VB, double W, double VW)
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

double BoatPlan::DirectionApparentWind(double VB, double W, double VW)
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
double BoatPlan::VelocityTrueWind(double VA, double VB, double W)
{
    double a = 1, b = 2 * VB * cos(W), c = VB*VB - VA*VA;
    double det = b*b - 4*a*c;

    if(-b - sqrt(det) > 0)
        printf("ambiguous true wind.\n");

    return (-b + sqrt(det)) / (2 * a);
}

#define MAX_WINDSPEEDS_IN_TABLE 200
#define MESSAGE(S) (S + wxString(_T("\n")) + wxString::FromUTF8(filename) \
                    + _(" line ") + wxString::Format(_T("%d"), linenum))
#define PARSE_WARNING(S) do { if(message.empty()) message = MESSAGE(S); } while (0)
#define PARSE_ERROR(S) if(message) do { message = _("Boat polar failed") + wxString(_T("\n")) \
                                  + MESSAGE(S); goto failed; } while (0)
bool BoatPlan::Open(const char *filename, wxString &message)
{
    wind_speeds.clear();
    degree_steps.clear();
    
    int linenum = 0;
    ZUFILE *f = zu_open(filename, "r");
    char line[1024];
    double lastentryW = -1;
    char *token, *saveptr;

    if(!f)
        PARSE_ERROR(_("Failed to open."));

    if(!zu_gets(f, line, sizeof line))
        PARSE_ERROR(_("Failed to read."));

    token = strtok_r(line, ";", &saveptr);
    linenum++;

    /* chomp invisible bytes */
    while(*token < 0) token++;

    if(strcasecmp(token, "twa/tws") && strcasecmp(token, "twa\\tws"))
        PARSE_ERROR(_("Unrecognized format."));
    
    while((token = strtok_r(NULL, ";", &saveptr))) {
        wind_speeds.push_back(SailingWindSpeed(strtod(token, 0)));
        if(wind_speeds.size() > MAX_WINDSPEEDS_IN_TABLE)
            PARSE_ERROR(_("Too many wind speeds."));
    }

    wind_speed_step = (int)round(wind_speeds.back().VW / wind_speeds.size());

    while(zu_gets(f, line, sizeof line)) {
        linenum++;

        token = strtok_r(line, ";", &saveptr);

        double W = strtod(token, 0);

        if(W < 0 || W > 180) {
            PARSE_WARNING(_("Wind direction out of range."));
            continue;
        }

        if(W <= lastentryW) {
            PARSE_WARNING(_("Wind direction out of order."));
            continue;
        }

        degree_steps.push_back(W);
        lastentryW = W;

        {
            std::vector<double>boatspeed;
            for(int VWi = 0; VWi < (int)wind_speeds.size(); VWi++) {
                double s = 0;
                if((token = strtok_r(NULL, ";", &saveptr)))
                    s = strtod(token, 0);
                else
                    PARSE_WARNING(_("Too few tokens."));

                wind_speeds[VWi].speeds.push_back
                    (SailingWindSpeed::SailingSpeed(s, W));
            }

            if(strtok_r(NULL, ";", &saveptr))
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
    wind_degree_step = degree_steps.size() ? 360 / degree_steps.size() : 0;

    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
        CalculateVMG(VWi);

    csvFileName = wxString::FromUTF8(filename);
    polarmethod = CSV;
    return true;
    
failed:
    wind_speeds.clear();
    degree_steps.clear();

    zu_close(f);
    return false;
}

bool BoatPlan::Save(const char *filename)
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
            fprintf(f, ";%.5g", wind_speeds[VWi].speeds[Wi].VB);
        fputs("\n", f);
    }
    fclose(f);
    return true;
}

/* start out with the boat stopped, and over time, iterate accelerating boat
   until it reaches steady state.  The speed of the boat is known already
   from apparent wind, this function finds it for true wind */
void BoatPlan::BoatSteadyState(double W, double VW, double &B, double &VB, double &A, double &VA,
                                Boat &boat)
{
    /* starting out not moving */
    VB = 0, A = W, VA = VW;
    double lp = .1;

    const int count = 128;
    double bucket = 0;
    int bcount = 0;

    for(;;) {
        double v = VelocityBoat(A, VA);

        if(v == 0) { // we cannot sail this way
            B = 0;
            VB = 0;
            return;
        }
        double a = v - VB;

        double drag = boat.FrictionDrag(VB) + boat.WakeDrag(VB);

        if(isnan(drag)) {
            VB = 0;
            return;
        }
        a -= drag;

        if(bcount == count) {
            VB = bucket / count;
            a = 0;
        }

        if(fabs(a) < 1e-2 || lp < 1e-2) {
            if(VB < 0) // not allowing backwards sailing
                VB = 0;
            B = AngleofAttackBoat(A, VA);
            return; /* reached steady state */
        }

        if(a < 0) {
            bucket += VB;
            bcount++;
//            lp *= .97;
        }

        VB = (1-lp)*VB + lp*(VB+a); /* lowpass to get a smooth update */
        VA = VelocityApparentWind(VB, W, VW);
        A =  DirectionApparentWind(VA, VB, W, VW);
    }
}

/* this is not actually all that accurate (no oblate earth, refraction etc...
   but it's at least simple, could fix and optimize to only compute if
   there are possible plans which need this */
static bool ComputeDayTime(const wxDateTime &gribtime, double lat, double lon, int &daytime)

{
    if(daytime != -1)
        return daytime != 0;

    double yearpos = 2*M_PI*(gribtime.GetDay()-186)/365.24;
    double gha = 2*M_PI*(lon/15 - gribtime.GetHour() - gribtime.GetMinute()/60)/24;
    double suninc = 90*cos(deg2rad(lat))*sin(gha) + 23.45*cos(yearpos);    

    return (daytime = (suninc > 0)) != 0; /* sun above horizon */
}

int BoatPlan::TrySwitchBoatPlan(double VW, double H, double Swell,
                                     const wxDateTime &gribtime, double lat, double lon, int &daytime)

{
    H = abs(heading_resolve(H)); /* make this work for both tacks */

    for(unsigned int i=0; i<SwitchPlans.size(); i++) {
        SwitchPlan &p = SwitchPlans[i];
        if(!isnan(p.MaxWindSpeed)     && p.MaxWindSpeed >= VW)     continue;
        if(!isnan(p.MaxWindSpeed)     && p.MaxWindSpeed <= VW)     continue;
        if(!isnan(p.MaxWindDirection) && p.MaxWindDirection >= H)  continue;
        if(!isnan(p.MinWindDirection) && p.MinWindDirection <= H)  continue;
        if(!isnan(p.MaxWaveHeight)    && p.MaxWaveHeight >= Swell) continue;
        if(!isnan(p.MinWaveHeight)    && p.MinWaveHeight <= Swell) continue;
        if(!p.DayTime) {
            if(ComputeDayTime(gribtime, lat, lon, daytime)) continue;
        } else if(!p.NightTime)
            if(!ComputeDayTime(gribtime, lat, lon, daytime)) continue;
        return i;
    }
    return -1;
}

BoatPlan::BoatPlan(wxString PlanName, Boat &boat)
    : Name(PlanName), eta(.25), luff_angle(15),
      optimize_tacking(true), wing_wing_running(false)
{
}

/* eta is a measure of efficiency of the boat, from .01 for racing boats to .5 for
   heavy cruisers */
void BoatPlan::ComputeBoatSpeeds(Boat &boat, PolarMethod method, int speed)
{
//    csvFileName = _T("");
    if(polarmethod == CSV ||
       wind_speeds.size() != num_computed_wind_speeds ||
       degree_steps.size() != computed_degree_count) {
        wind_speeds.clear();
        degree_steps.clear();

        for(unsigned int Wi = 0; Wi < computed_degree_count; Wi++)
            degree_steps.push_back(computed_degree_step*Wi);
        UpdateDegreeStepLookup();

        for(unsigned int VWi = 0; VWi < num_computed_wind_speeds; VWi++) {
            wind_speeds.push_back(SailingWindSpeed(computed_wind_speeds[VWi]));

            wind_speeds[VWi].speeds.clear();
            for(unsigned int Wi = 0; Wi < computed_degree_count; Wi++)
                wind_speeds[VWi].speeds.push_back(SailingWindSpeed::SailingSpeed(0, degree_steps[Wi]));
        }
    }

    // for IMF computation
    double SADR = boat.SailAreaDisplacementRatio();
    double lwl_ft = boat.lwl_ft;
    double hull_speed = boat.HullSpeed();

    int min, max;
    if(speed == -1) // all speeds
        min = 0, max = wind_speeds.size() - 1;
    else
        min = ClosestVWi(speed), max = min + 1;

    for(int VWi = min; VWi <= max; VWi++) {
        for(unsigned int Wi = 0; Wi <= computed_degree_count/2; Wi++) {
            double VW = wind_speeds[VWi].VW;
            double W = Wi * computed_degree_step;

            double B, VB, A, VA;
            switch(method) {
            case TRANSFORM:
                BoatSteadyState(deg2rad(W), VW, B, VB, A, VA, boat);
                break;
            case IMF:
            {
                if(fabsf(W) < 30)
                    VB = 0;
                else {
                    double base_speed  = 2.62 + .066*SADR + .051*lwl_ft;
                    double b = 1 / sqrt(VW + 3);
                    VB = base_speed*(sin(deg2rad(W)/2) + b*cos(deg2rad(W))) * sqrt(20*VW) / 8;
                    if(VB > hull_speed)
                        VB = hull_speed;
                }
            }
            default:
                break;
            }

            Set(Wi, VWi, VB, W);
            if(W != 0) // assume symmetric performance
                Set(computed_degree_count-Wi, VWi, VB, DEGREES-W);
        }

        CalculateVMG(VWi);
    }

    polarmethod = method;
}

/* instead of traveling in the direction given, allow traveling at angles
   closer to +- 90.  Basically we are trying every two tack angles and combining
   to determine if alternating between these two courses is actually faster than
   the current strategy for the given course, and if so, use it.

   TODO: Make this this work for asymmetrical polars correctly
*/
void BoatPlan::OptimizeTackingSpeed()
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
            
            SailingWindSpeed::SailingSpeed &a = wind_speeds[VWi].speeds[Wi];
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
                if(bcVB > a.VB) {
                    a.VB = bcVB;
                    if(at > 180)
                        a.b = bt, a.c = ct, a.w = w;
                    else
                        a.b = ct, a.c = bt, a.w = (1-w);
                }
            }           
        }
    }
}

void BoatPlan::ResetOptimalTackingSpeed()
{
    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
        for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
            SailingWindSpeed::SailingSpeed &a = wind_speeds[VWi].speeds[Wi];
            a.VB = a.origVB;
            a.b = Wi;
            a.w = 1;
        }
}

#if 0
BoatSpeedTable BoatPlan::CreateTable(int wind_speed_step, int wind_degree_step)
{
    BoatSpeedTable boatspeedtable;

    for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++)
        boatspeedtable.windspeeds.push_back(wind_speeds[VWi].VW);

    for(unsigned int Wi = 0; Wi < degree_steps.size(); Wi++) {
        if(degree_steps[Wi] > 180) // table is assuming symmetric performance 
            break;

        BoatSpeedTableEntry entry;
        for(unsigned int VWi = 0; VWi < wind_speeds.size(); VWi++) {
            entry.W = degree_steps[Wi];
            entry.boatspeed.push_back(wind_speeds[VWi].speeds[Wi].VB);
        }
        boatspeedtable.table.push_back(entry);
    }
    return boatspeedtable;
}
#endif

// return index of wind speed in table which less than our wind speed,
// or second from last (adding 1 to this value is a valid wind speed)
int BoatPlan::ClosestVWi(double VW)
{
    for(unsigned int VWi = 1; VWi < wind_speeds.size()-1; VWi++)
        if(wind_speeds[VWi].VW > VW)
            return VWi - 1;

    return wind_speeds.size()-2;
}

/* compute boat speed from true wind angle and true wind speed */
double BoatPlan::Speed(double W, double VW)
{
    if(VW < 0 || VW > wind_speeds.back().VW)
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

    int VW1i = ClosestVWi(VW), VW2i = VW1i + 1;
    SailingWindSpeed &ws1 = wind_speeds[VW1i], &ws2 = wind_speeds[VW2i];
    double VW1 = ws1.VW, VW2 = ws2.VW;

    double VB11 = ws1.speeds[W1i].VB, VB12 = ws1.speeds[W2i].VB;
    double VB21 = ws2.speeds[W1i].VB, VB22 = ws2.speeds[W2i].VB;

    double VB1 = interp_value(VW, VW1, VW2, VB11, VB21);
    double VB2 = interp_value(VW, VW1, VW2, VB12, VB22);

    return       interp_value(W, W1, W2, VB1, VB2);
}

double BoatPlan::SpeedAtApparentWindDirection(double A, double VW, double *pW)
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

double BoatPlan::SpeedAtApparentWindSpeed(double W, double VA)
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

double BoatPlan::SpeedAtApparentWind(double A, double VA, double *pW)
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

SailingVMG BoatPlan::GetVMGTrueWind(double VW)
{
    int VW1i = ClosestVWi(VW), VW2i = VW1i + 1;
    SailingWindSpeed &ws1 = wind_speeds[VW1i], &ws2 = wind_speeds[VW2i];
    double VW1 = ws1.VW, VW2 = ws2.VW;
    SailingVMG vmg, vmg1 = ws1.VMG, vmg2 = ws2.VMG;

    for(int i=0; i<4; i++)
        vmg.values[i] = interp_value(VW, VW1, VW2, vmg1.values[i], vmg2.values[i]);

    return vmg;
}

SailingVMG BoatPlan::GetVMGApparentWind(double VA)
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
double BoatPlan::TrueWindSpeed(double VB, double W, double maxVW)
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
        double VB1 = ws.speeds[W1i].VB;
        if(VB1 > VB && VB1 < VB1min) VB1min = VB1, VW1min = ws.VW;
        if(VB1 < VB && VB1 > VB1max) VB1max = VB1, VW1max = ws.VW;

        double VB2 = ws.speeds[W2i].VB;
        if(VB2 > VB && VB2 < VB2min) VB2min = VB2, VW2min = ws.VW;
        if(VB2 < VB && VB2 > VB2max) VB2max = VB2, VW2max = ws.VW;
    }

    double VBmin = interp_value(W, W1, W2, VB1min, VB2min);
    double VWmin = interp_value(W, W1, W2, VW1min, VW2min);
    double VBmax = interp_value(W, W1, W2, VB1max, VB2max);
    double VWmax = interp_value(W, W1, W2, VW1max, VW2max);

    return interp_value(VB, VBmin, VBmax, VWmin, VWmax);
}

void BoatPlan::Set(unsigned int Wi, unsigned int VWi, double VB, double W)
{
    if(Wi >= degree_steps.size() ||
       VWi > wind_speeds.size())
        return; /* maybe log warning here? */
    
    wind_speeds[VWi].speeds[Wi] = SailingWindSpeed::SailingSpeed(VB, W);
}

void BoatPlan::UpdateDegreeStepLookup()
{
    for(int d = 0; d < DEGREES; d++) {
        unsigned int Wi;
        for(Wi = 0; Wi < degree_steps.size(); Wi++)
            if(d < degree_steps[Wi])
                break;
        degree_step_index[d] = Wi - 1;
    }
}

void BoatPlan::CalculateVMG(int VWi)
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

            double VB = upwind*cos(deg2rad(W))*ws.speeds[Wi].origVB;
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
