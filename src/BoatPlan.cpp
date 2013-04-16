/******************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
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

BoatSpeedTable::BoatSpeedTable()
{
}

BoatSpeedTable::~BoatSpeedTable()
{
}

bool BoatSpeedTable::Open(const char *filename, int &wind_speed_step, int &wind_degree_step)
{
    FILE *f = fopen(filename, "r");
    numwindspeeds=0;

    if(!f)
        return false;

    char line[1024];
    
    char *token, *saveptr;
    if(!fgets(line, sizeof line, f))
        goto failed; /* error here too */
    token = strtok_r(line, ";", &saveptr);
    if(strcasecmp(token, "twa/tws") && strcasecmp(token, "twa\\tws"))
        goto failed; /* unrecognized format */
    
    while((token = strtok_r(NULL, ";", &saveptr))) {
        windspeeds[numwindspeeds++] = strtod(token, 0);
        if(numwindspeeds > MAX_WINDSPEEDS_IN_TABLE)
            goto failed;
    }
    wind_speed_step = windspeeds[numwindspeeds-1] / numwindspeeds;

    wind_degree_step = 0;
    while(fgets(line, sizeof line, f)) {
        token = strtok_r(line, ";", &saveptr);
        BoatSpeedTableEntry entry;
        entry.W = strtod(token, 0);
        int i=0;
        while((token = strtok_r(NULL, ";", &saveptr)))
            entry.boatspeed[i++] = strtod(token, 0);
        if(i > MAX_WINDSPEEDS_IN_TABLE)
            goto failed;
        table.push_back(entry);
        wind_degree_step++;
    }

    if(wind_degree_step < 2)
        wind_degree_step = 2;
    wind_degree_step = 180 / (wind_degree_step - 1);

    fclose(f);
    return true;
    
failed:
    fclose(f);
    return false;
}

bool BoatSpeedTable::Save(const char *filename)
{
    FILE *f = fopen(filename, "w");
    int numwindspeeds=0;

    if(!f)
        return false;

    fputs("twa/tws", f);
    for(int i = 0; i<=numwindspeeds; i++)
        fprintf(f, ";%d", (int)windspeeds[i]);

    for(std::list<BoatSpeedTableEntry>::iterator it = table.begin();
        it != table.end(); it++) {
        fprintf(f, "%d", (int)(*it).W);
        for(int i = 0; i<=numwindspeeds; i++) {
            fprintf(f, ";%.2f", (*it).boatspeed[i]);
        }
        fputs("\n", f);
    }
    fclose(f);
    return true;
}

double BoatSpeedTable::InterpolateSpeed(double VW, double W)
{
    BoatSpeedTableEntry *minW = NULL, *maxW = NULL;
    for(BoatSpeedTableEntryList::iterator it = table.begin(); it != table.end(); ++it) {
        if((*it).W >= W && (!maxW || maxW->W > (*it).W))
            maxW = &(*it);

        if((*it).W <= W && (!minW || minW->W < (*it).W))
            minW = &(*it);
    }

    int minVWind = -1, maxVWind = -1;
    for(int i=0; i<numwindspeeds; i++) {
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

    return sin(A/2) * sqrt(VA / eta);
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

      sin A   sin W   sin W-A
      ----- = ----- = -------
       VW      VA       VB

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

double BoatPlan::VelocityApparentWind(double VB, double W, double VW)
{
  return sqrt(VW*VW + VB*VB + 2*VW*VB*cos(W)); /* law of cosines */
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
  if(cosA > 1) cosA = 1; else if(cosA < -1) cosA = -1; /* slight arithematic errors */
  double ac = acos(cosA);
  return W > 0 ? ac : -ac;
}

/* start out with the boat stopped, and over time, iterate accelerating boat
   until it reaches steady state.  The speed of the boat is known already
   from apparent wind, this function finds it for true wind */
void BoatPlan::BoatSteadyState(double W, double VW, double &B, double &VB, double &A, double &VA,
                                Boat &boat)
{
    /* starting out not moving */
    VB = 0, A = W, VA = VW;
    double lp = .03;
    for(;;) {
        double v = VelocityBoat(A, VA);
        double a = v - VB;            

        double drag = boat.FrictionDrag(VB) + boat.WakeDrag(VB);
        a -= drag;

        if(fabs(a) < 1e-2 || a < 0) {
            B = AngleofAttackBoat(A, VA);
            return; /* reached steady state */
        }

        VB = (1-lp)*VB + lp*(VB+a); /* lowpass to get a smooth update */
        VA = VelocityApparentWind(VB, W, VW);
        A =  DirectionApparentWind(VA, VB, W, VW);
    }
}

/* this is not actually all that accurate (no oblate earth, refraction etc...
   but it's at least simple, could fix and optimize to only compute if
   there are possible plans which need this */
static bool ComputeDayTime(wxDateTime &gribtime, double lat, double lon, int &daytime)

{
    if(daytime != -1)
        return daytime;

    double yearpos = 2*M_PI*(gribtime.GetDay()-186)/365.24;
    double gha = 2*M_PI*(lon/15 - gribtime.GetHour() - gribtime.GetMinute()/60)/24;
    double suninc = 90*cos(deg2rad(lat))*sin(gha) + 23.45*cos(yearpos);    

    return (daytime = (suninc > 0)); /* sun above horizon */
}

wxString BoatPlan::TrySwitchBoatPlan(double VW, double H, double Swell,
                                     wxDateTime &gribtime, double lat, double lon, int &daytime)

{
    H = abs(heading_resolve(H)); /* make this work for both tacks */

    for(unsigned int i=0; i<SwitchPlans.size(); i++) {
        SwitchPlan p = SwitchPlans[i];
        if(!isnan(p.MaxWindSpeed) && p.MaxWindSpeed <= VW) continue;
        if(!isnan(p.MinWindSpeed) && p.MinWindSpeed >= VW) continue;
        if(!isnan(p.MaxWindDirection) && p.MaxWindDirection <=  H) continue;
        if(!isnan(p.MinWindDirection) && p.MinWindDirection >=  H) continue;
        if(!isnan(p.MaxWaveHeight) && p.MaxWaveHeight <=  H) continue;
        if(!isnan(p.MinWaveHeight) && p.MinWaveHeight >=  H) continue;
        if(!p.DayTime) {
            if(ComputeDayTime(gribtime, lat, lon, daytime)) continue;
        } else
            if(!ComputeDayTime(gribtime, lat, lon, daytime)) continue;
        return p.Name;
    }
    return wxString();
}

BoatPlan::BoatPlan(wxString PlanName, Boat &boat)
    : Name(PlanName), eta(.25), luff_angle(15)
{
    memset(speed, 0, sizeof speed);
    memset(VMG, 0, sizeof VMG);
}

BoatPlan::~BoatPlan()
{
}

/* eta is a measure of efficiency of the boat, from .01 for racing boats to .5 for
   heavy cruisers */
void BoatPlan::ComputeBoatSpeeds(Boat &boat)
{
    csvFileName = _("<Computed>");

    for(int VWi = 0; VWi < num_wind_speeds; VWi++)
        for(int Wi = 0; Wi <= DEGREE_COUNT/2; Wi++) {
            double VW = wind_speeds[VWi];
            double W = Wi * DEGREE_STEP;
            double B, VB, A, VA;
            BoatSteadyState(deg2rad(W), VW, B, VB, A, VA, boat);
            Set(Wi, VWi, VB);
            if(W != 0)
                Set(DEGREE_COUNT-Wi, VWi, VB);
        }

    CalculateVMG();
    computed = true;
}

/* instead of traveling in the direction given, allow traveling at angles
   closer to +- 90.  Basically we are trying every two tack angles and combining
   to determine if alternating between these two courses is actually faster than
   the current strategy for the given course, and if so, use it.

   TODO: Make this this work for asymmetrical polars correctly
*/
void BoatPlan::OptimizeTackingSpeed()
{
    CalculateVMG();
    for(int VWi = 0; VWi < num_wind_speeds; VWi++) {
        for(int Wi = 0; Wi <= DEGREE_COUNT; Wi++) {
            int at = Wi, bt, ct;
            
            if(Wi >= DEGREE_COUNT*1/4 && Wi < DEGREE_COUNT*3/4) {
                bt = VMG[VWi].StarboardTackDownWind;
                ct = VMG[VWi].PortTackDownWind;
            } else {
                bt = VMG[VWi].StarboardTackUpWind;
                ct = VMG[VWi].PortTackUpWind;
            }
            
            SailingSpeed &a = speed[VWi][at];
            SailingSpeed &b = speed[VWi][bt];
            SailingSpeed &c = speed[VWi][ct];
            
            /* w is the weight between b and c tacks (.5 for equal time on each
               t is the improvement factor]
               bcVB * sin(a) = w*b.VB*sin(b) + (1-w)*c.VB*sin(c)
               bcVB * cos(a) = w*b.VB*cos(b) + (1-w)*c.VB*cos(c) */
            double ar = deg2rad(at*DEGREE_STEP);
            double br = deg2rad(bt*DEGREE_STEP);
            double cr = deg2rad(ct*DEGREE_STEP);
            double sa = sin(ar), ca = cos(ar);
            double sb = sin(br), cb = cos(br);
            double sc = sin(cr), cc = cos(cr);
            double X = ca*sc-sa*cc, Y = sa*cb-ca*sb;
            double d = (X*c.VB + Y*b.VB);
            double w = X*c.VB / d;
            
            if(w > 0 && w < 1) {
                double Z = cb*sc-sb*cc;
                double bcVB = Z*b.VB*c.VB / d;
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
    for(int VWi = 0; VWi < num_wind_speeds; VWi++)
        for(int Wi = 0; Wi <= DEGREE_COUNT; Wi++) {
            SailingSpeed &a = speed[VWi][Wi];
            a.VB = a.origVB;
            a.b = Wi;
            a.w = 1;
        }
}

void BoatPlan::SetSpeedsFromTable(BoatSpeedTable &table)
{
    for(int VWi = 0; VWi < num_wind_speeds; VWi++)
        for(int Wi = 0; Wi <= DEGREE_COUNT/2; Wi++) {
            double VB = table.InterpolateSpeed(wind_speeds[VWi], Wi*DEGREE_STEP);
            Set(Wi, VWi, VB);
            if(Wi != 0)
                Set(DEGREE_COUNT-Wi, VWi, VB);
        }
    CalculateVMG();
    computed = false;
}

BoatSpeedTable BoatPlan::CreateTable(int wind_speed_step, int wind_degree_step)
{
    BoatSpeedTable boatspeedtable;

    for(;;) {
        boatspeedtable.numwindspeeds = num_wind_speeds;
        if(boatspeedtable.numwindspeeds < MAX_WINDSPEEDS_IN_TABLE)
            break;
        wind_speed_step *= 2;
    }

    for(int VWi = 0; VWi < num_wind_speeds; VWi++)
        boatspeedtable.windspeeds[VWi] = wind_speeds[VWi];

    for(int Wi = 0; Wi <= DEGREE_COUNT/2; Wi+=wind_degree_step) {
        BoatSpeedTableEntry entry;
        for(int VWi = 0; VWi < num_wind_speeds; VWi++) {
            entry.W = Wi*DEGREE_STEP;
            entry.boatspeed[VWi] = speed[VWi][Wi].VB;
        }
        boatspeedtable.table.push_back(entry);
    }
    return boatspeedtable;
}

int BoatPlan::ClosestVWi(int VW)
{
    int VWi;
    for(VWi = 0; VWi < num_wind_speeds-1; VWi++)
        if(wind_speeds[VWi] >= VW)
            break;
    return VWi;
}

/* compute boat speed from given power level, true wind angle and true wind speed */
double BoatPlan::Speed(double W, double VW)
{
    W = positive_degrees(W);
    if(VW < 0 || VW > MAX_KNOTS)
        return NAN;

    double W1 = floor(W/DEGREE_STEP)*DEGREE_STEP;
    double W2 = ceil(W/DEGREE_STEP)*DEGREE_STEP;

    /* get headings nearby each other */
    while(W1 - W2 > 180)
        W2 += 360;
    while(W2 - W1 > 180)
        W1 += 360;

    int W1i = positive_degrees(W1)/DEGREE_STEP;
    int W2i = positive_degrees(W2)/DEGREE_STEP;

    int VW1i = ClosestVWi(VW), VW2i = VW1i+1;
    double VW1 = wind_speeds[VW1i], VW2 = wind_speeds[VW2i];

    double VB11 = speed[VW1i][W1i].VB;
    double VB12 = speed[VW1i][W2i].VB;
    double VB21 = speed[VW2i][W1i].VB;
    double VB22 = speed[VW2i][W2i].VB;

    double VB1 = interp_value(VW, VW1, VW2, VB11, VB21);
    double VB2 = interp_value(VW, VW1, VW2, VB12, VB22);

    double VB = interp_value(W, W1, W2, VB1, VB2);
    return VB;
}

SailingVMG BoatPlan::GetVMG(double VW)
{
    int VW2i = ClosestVWi(VW), VW1i = VW2i > 0 ? VW2i - 1 : 0;
    SailingVMG vmg, vmg1 = VMG[VW1i], vmg2 = VMG[VW2i];
    double VW1 = wind_speeds[VW1i], VW2 = wind_speeds[VW2i];

    vmg.PortTackUpWind = DEGREE_STEP * interp_value
        (VW, VW1, VW2, vmg1.PortTackUpWind, vmg2.PortTackUpWind);
    vmg.StarboardTackUpWind = DEGREE_STEP * interp_value
        (VW, VW1, VW2, vmg1.StarboardTackUpWind, vmg2.StarboardTackUpWind);
    vmg.PortTackDownWind = DEGREE_STEP * interp_value
        (VW, VW1, VW2, vmg1.PortTackDownWind, vmg2.PortTackDownWind);
    vmg.StarboardTackDownWind = DEGREE_STEP * interp_value
        (VW, VW1, VW2, vmg1.StarboardTackDownWind, vmg2.StarboardTackDownWind);
    return vmg;
}

/* find true wind speed from boat speed and true wind direction, because often at high
   winds the polar inverts and boat speed is lower, we can specify the max VW to search */
double BoatPlan::TrueWindSpeed(double VB, double W, double maxVW)
{
    int W1 = floor(W/DEGREE_STEP)*DEGREE_STEP;
    int W2 = ceil(W/DEGREE_STEP)*DEGREE_STEP;

    /* get headings nearby */
    while(W1 - W2 > 180)
        W2 += 360;
    while(W2 - W1 > 180)
        W1 += 360;

    W1 = positive_degrees(W1);
    W2 = positive_degrees(W2);

    int W1i = W1/DEGREE_STEP, W2i = W2/DEGREE_STEP;

    double VB1min = INFINITY, VW1min = NAN, VB1max = 0, VW1max = NAN;
    double VB2min = INFINITY, VW2min = NAN, VB2max = 0, VW2max = NAN;

    for(int VWi = 0; VWi < num_wind_speeds; VWi++) {
        if(wind_speeds[VWi] > maxVW)
            break;

        double VB1 = speed[VWi][W1i].VB;
        if(VB1 > VB && VB1 < VB1min) VB1min = VB1, VW1min = wind_speeds[VWi];
        if(VB1 < VB && VB1 > VB1max) VB1max = VB1, VW1max = wind_speeds[VWi];

        double VB2 = speed[VWi][W2i].VB;
        if(VB2 > VB && VB2 < VB2min) VB2min = VB2, VW2min = wind_speeds[VWi];
        if(VB2 < VB && VB2 > VB2max) VB2max = VB2, VW2max = wind_speeds[VWi];
    }

    double VBmin = interp_value(W, W1, W2, VB1min, VB2min);
    double VWmin = interp_value(W, W1, W2, VW1min, VW2min);
    double VBmax = interp_value(W, W1, W2, VB1max, VB2max);
    double VWmax = interp_value(W, W1, W2, VW1max, VW2max);

    return interp_value(VB, VBmin, VBmax, VWmin, VWmax);
}

void BoatPlan::Set(int Wi, int VWi, double VB)
{
    if(Wi < 0 || Wi >= DEGREE_COUNT ||
       VWi < 0 || VWi > num_wind_speeds)
        return; /* maybe log warning here? */
    
    speed[VWi][Wi].VB = VB;
    speed[VWi][Wi].origVB = VB;
    speed[VWi][Wi].slipangle = 0;
    speed[VWi][Wi].b = Wi;
    speed[VWi][Wi].w = 1; /* weighted all on first course */
}

int BoatPlan::BestVMG(int VWi, int startW, int endW, int upwind)
{
    double maxVB = 0;
    int maxWi = 0;
    for(int Wi = startW; Wi <= endW; Wi++) {
        double VB = upwind*cos(deg2rad(Wi*DEGREE_STEP))*speed[VWi][Wi].origVB;
        if(VB > maxVB) {
            maxVB = VB;
            maxWi = Wi;
        }
    }
    return maxWi;
}

void BoatPlan::CalculateVMG()
{
    for(int VWi = 0; VWi < num_wind_speeds; VWi++) {
        VMG[VWi].StarboardTackUpWind   = BestVMG(VWi, DEGREE_COUNT*3/4, DEGREE_COUNT*4/4,  1);
        VMG[VWi].PortTackUpWind        = BestVMG(VWi, DEGREE_COUNT*0/4, DEGREE_COUNT*1/4,  1);
        VMG[VWi].StarboardTackDownWind = BestVMG(VWi, DEGREE_COUNT*2/4, DEGREE_COUNT*3/4, -1);
        VMG[VWi].PortTackDownWind      = BestVMG(VWi, DEGREE_COUNT*1/4, DEGREE_COUNT*2/4, -1);
    }
}
