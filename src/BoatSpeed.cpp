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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Utilities.h"
#include "BoatSpeed.h"

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

bool BoatSpeedTable::Open(const char *filename)
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
    }
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

bool BoatSpeedTable::OpenBinary(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if(!f)
        return false;

    int n, s;
    if(fscanf(f, "obs %d %d\n", &n, &s) != 2) goto failed;
    numwindspeeds = n;

    float windspeeds32[MAX_WINDSPEEDS_IN_TABLE];
    if((int)fread(windspeeds32, sizeof *windspeeds32, n, f) != n) goto failed;
    for(int j = 0; j<n; j++)
        windspeeds[j] = windspeeds32[j];

    for(int i=0; i<s; i++) {
        BoatSpeedTableEntry entry;
        float W32;
        if((int)fread(&W32, sizeof W32, 1, f) != 1) goto failed;
        entry.W = W32;

        float boatspeed32[MAX_WINDSPEEDS_IN_TABLE];
        if((int)fread(boatspeed32, sizeof *boatspeed32, n, f) != n) goto failed;

        for(int j = 0; j<n; j++)
            entry.boatspeed[j] = boatspeed32[j];

        table.push_back(entry);
    }

    fclose(f);
    return true;

failed:
    fclose(f);
    return false;
}

bool BoatSpeedTable::SaveBinary(const char *filename)
{
    FILE *f = fopen(filename, "w");
    if(!f)
        return false;

    int n = numwindspeeds;

    fprintf(f, "obs %d %d\n", n, table.size());

    float windspeeds32[MAX_WINDSPEEDS_IN_TABLE];
    for(int j = 0; j<n; j++)
        windspeeds32[j] = windspeeds[j];
    if((int)fwrite(windspeeds32, sizeof *windspeeds32, n, f) != n) goto failed;

    for(BoatSpeedTableEntryList::iterator it = table.begin(); it != table.end(); ++it) {
        float W32 = (*it).W;
        if(fwrite(&W32, sizeof W32, 1, f) != 1) goto failed;

        float boatspeed32[MAX_WINDSPEEDS_IN_TABLE];
        for(int j = 0; j<n; j++)
            boatspeed32[j] = (*it).boatspeed[j];

        if((int)fwrite(boatspeed32, sizeof *boatspeed32, n, f) != n) goto failed;
    }

    fclose(f);
    return true;

failed:
    fclose(f);
    return false;
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
P - thrust in units of amount required to push us 1 knot

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

double BoatSpeed::sail_forward_force(double A, double VA)
{
  return eta * sin(A/2) * sqrt(VA);
}

double BoatSpeed::sail_slip_force(double A, double VA)
{
  return eta * cos(A/2) * sqrt(VA);
}

double BoatSpeed::boat_slip_speed(double A, double VA)
{
  return sail_slip_force(A, VA) / keel_pressure;
}

double BoatSpeed::boat_forward_speed(double A, double VA, double P)
{
  return sail_forward_force(A, VA) + P + keel_lift * boat_slip_speed(A, VA);
}

/* difference between direction we face and direction moving thru water */
double BoatSpeed::AngleofAttackBoat(double A, double VA, double P)
{
  double ss = boat_slip_speed(A, VA);
  double fs = boat_forward_speed(A, VA, P);
  return atan2(ss, fs);
}

/* actual speed over water of boat */
double BoatSpeed::VelocityBoat(double A, double VA, double P)
{
#if 0
  double ss = boat_slip_speed(eta, A, VA);
  double fs = boat_forward_speed(eta, A, VA, P);
  return hypot(ss, fs);
#else
  double v = sin(A/2) * sqrt(VA / eta);
  v -= v*v*hull_drag;
  if(v < 0 || isnan(v)) v = 0;
  return v;
#endif
}

/*
   Now that we can convert the wind speed in gribs correctly
   still need more coefficients.  We can run calculations
   relative to motion thru water rather  than over ground
   now.  Also the wind angle is the true bearing of the wind
   across the water.  The final boat direction is therefore
   shifted from this:

   B  - boat direction thru water relative to north
   W  - true wind direction (relative to vessel)

   optionally could include slippage here
*/

/* Solve for the augmented sailboat transform for apparent wind

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

double BoatSpeed::VelocityApparentWind(double VB, double W, double VW)
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
*/
double BoatSpeed::DirectionApparentWind(double VA, double VB, double W, double VW)
{
  if(VA == 0) /* apparent wind direction is not defined */
    return 0;
  if(VB == 0) /* trig identity breaks down, but if we aren't */
    return W; /*   moving, apparent wind is true wind */
  double cosA = (VA*VA + VB*VB - VW*VW) / (2*VA*VB);
  if(cosA > 1) /* slight arithematic errors */
    cosA = 1;
  if(cosA < -1)
    cosA = -1;
  return acos(cosA);
}

/* start out with the boat stopped, and over time, iterate accelerating boat
   until it reaches steady state.  The speed of the boat is known already
   from apparent wind, this function finds it for true wind */
void BoatSpeed::BoatSteadyState(double W, double VW, double P,
                                double &B, double &VB, double &A, double &VA)
{
  /* starting out not moving */
  VB = 0, A = W, VA = VW;
  for(;;) {
    double v = VelocityBoat(A, VA, P);
    if(v - VB < 1e-10) {
      B = AngleofAttackBoat(A, VA, P);
      return; /* reached steady state */
    }

    VB = (VB + v) / 2; /* average to avoid overshoot */
    VA = VelocityApparentWind(VB, W, VW);
    A =  DirectionApparentWind(VA, VB, W, VW);
  }
}

BoatSpeed::BoatSpeed()
{
    Open("/home/sean/boat.obs", true);
}

BoatSpeed::~BoatSpeed()
{
    Save("/home/sean/boat.obs", true);
}

bool BoatSpeed::Open(const char *filename, bool binary)
{
    BoatSpeedTable table;
    if(binary) {
        if(!table.OpenBinary(filename))
            return false;
    } else
        if(!table.Open(filename))
            return false;

    SetSpeedsFromTable(table);
    return true;
}

bool BoatSpeed::Save(const char *filename, bool binary)
{
    BoatSpeedTable table = CreateTable();
    if(binary)
        return table.SaveBinary(filename);
    return table.Save(filename);
}

/* eta is a measure of efficiency of the boat, from .01 for racing boats to .5 for
   heavy cruisers */
void BoatSpeed::ComputeBoatSpeeds()
{
    for(int P = 0; P <= 1/*MAX_POWER*/; P++)
        for(int VW = 0; VW < MAX_KNOTS; VW++)
            for(int W = 0; W <= DEGREES/2; W+=DEGREE_STEP) {
                double B, VB, A, VA;
                BoatSteadyState(deg2rad(W), VW, P, B, VB, A, VA);
                Set(P, W, VW, VB);
                if(W != 0)
                    Set(P, DEGREES-W, VW, VB);
            }
}

/* instead of traveling in the direction given, allow traveling at angles
   closer to +- 90.  Basically we are trying every two tack angles and combining
   to determine if alternating between these two courses is actually faster than
   the current strategy for the given course, and if so, use it.
*/
void BoatSpeed::OptimizeTackingSpeed()
{
    for(int P = 0; P <= 0/*MAX_POWER*/; P++)
        for(int VW = 0; VW < MAX_KNOTS; VW++)
            for(int at = 0; at <= DEGREES; at+=DEGREE_STEP) {
                SailingSpeed &a = speed[P][VW][at];
                for(int bt = DEGREE_STEP; bt < DEGREES/2; bt+=DEGREE_STEP) {
                    SailingSpeed &b = speed[P][VW][bt];
                    for(int ct = DEGREES/2+DEGREE_STEP; ct < DEGREES; ct+=DEGREE_STEP) {
                        SailingSpeed &c = speed[P][VW][ct];
                        /* don't use speeds which already optimized with tacking */
                        if(b.w != 1 || c.w != 1)
                            continue;

                        /* w is the weight between b and c tacks (.5 for equal time on each
                           t is the improvement factor]
                           bcVB * sin(a) = w*b.VB*sin(b) + (1-w)*c.VB*sin(c)
                           bcVB * cos(a) = w*b.VB*cos(b) + (1-w)*c.VB*cos(c) */
                        double ar = deg2rad(at), br = deg2rad(bt), cr = deg2rad(ct);
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
                                if(at < 180)
                                    a.b = bt, a.c = ct, a.w = w;
                                else
                                    a.b = ct, a.c = bt, a.w = (1-w);
                            }
                        }
                    }
                }
            }
}

void BoatSpeed::SetSpeedsFromTable(BoatSpeedTable &table)
{
    int P = 0;
    for(int VW = 0; VW < MAX_KNOTS; VW++)
        for(int W = 0; W <= DEGREES/2; W+=DEGREE_STEP) {
            double VB = table.InterpolateSpeed(VW, W);
            Set(P, W, VW, VB);
            if(W != 0)
                Set(P, DEGREES-W, VW, VB);
        }
}

BoatSpeedTable BoatSpeed::CreateTable()
{
    BoatSpeedTable boatspeedtable;
    int windsstep = 3;
    int winddstep = 5;

    for(;;) {
        boatspeedtable.numwindspeeds = MAX_KNOTS / windsstep;
        if(boatspeedtable.numwindspeeds < MAX_WINDSPEEDS_IN_TABLE)
            break;
        windsstep *= 2;
    }

    int VW, i;
    for(VW = 0, i=0; VW < MAX_KNOTS; VW+=windsstep, i++)
        boatspeedtable.windspeeds[i] = VW;

    for(int W = 0; W <= DEGREES/2; W+=winddstep) {
        BoatSpeedTableEntry entry;
        for(VW = 0, i=0; VW < MAX_KNOTS; VW+=windsstep, i++) {
            entry.W = W;
            entry.boatspeed[i] = Speed(0, W, VW);
        }
        boatspeedtable.table.push_back(entry);
    }
    return boatspeedtable;
}

/* compute boat speed from given power level, true wind angle and true wind speed */
double BoatSpeed::Speed(int P, double W, double VW)
{
    W = positive_degrees(W);
    if(P < 0 || P > MAX_POWER || VW < 0 || VW > MAX_KNOTS)
        return 0.0/0.0;

    double W1 = floor(W/DEGREE_STEP)*DEGREE_STEP;
    double W2 = ceil(W/DEGREE_STEP)*DEGREE_STEP;

    /* get headings nearby */
    while(W1 - W2 > 180)
        W2 += 360;
    while(W2 - W1 > 180)
        W1 += 360;

    int W1i = positive_degrees(W1);
    int W2i = positive_degrees(W2);

    int VW1 = floor(VW), VW2 = ceil(VW);

    double VB11 = speed[P][VW1][W1i].VB;
    double VB12 = speed[P][VW1][W2i].VB;
    double VB21 = speed[P][VW2][W1i].VB;
    double VB22 = speed[P][VW2][W2i].VB;

    double VB1 = interp_value(VW, VW1, VW2, VB11, VB21);
    double VB2 = interp_value(VW, VW1, VW2, VB12, VB22);

    double VB = interp_value(W, W1, W2, VB1, VB2);

    return VB;
}

/* find true wind speed from boat speed and true wind direction, because often at high
 winds the polar inverts and boat speed is lower, we canspecify the max VW to search */
double BoatSpeed::TrueWindSpeed(int P, double VB, double W, double maxVW)
{
    if(P < 0 || P > MAX_POWER)
        return 0.0/0.0;

    double W1 = floor(W/DEGREE_STEP)*DEGREE_STEP;
    double W2 = ceil(W/DEGREE_STEP)*DEGREE_STEP;

    /* get headings nearby */
    while(W1 - W2 > 180)
        W2 += 360;
    while(W2 - W1 > 180)
        W1 += 360;

    int W1i = positive_degrees(W1);
    int W2i = positive_degrees(W2);

    double VB1min = 1.0/0.0, VW1min = 0.0/0.0, VB1max = 0, VW1max = 0.0/0.0;
    double VB2min = 1.0/0.0, VW2min = 0.0/0.0, VB2max = 0, VW2max = 0.0/0.0;

    for(int VWi = 0; VWi < maxVW; VWi++) {
        double VB1 = speed[P][VWi][W1i].VB;
        if(VB1 > VB && VB1 < VB1min) VB1min = VB1, VW1min = VWi;
        if(VB1 < VB && VB1 > VB1max) VB1max = VB1, VW1max = VWi;

        double VB2 = speed[P][VWi][W2i].VB;
        if(VB2 > VB && VB2 < VB2min) VB2min = VB2, VW2min = VWi;
        if(VB2 < VB && VB2 > VB2max) VB2max = VB2, VW2max = VWi;
    }

    double VBmin = interp_value(W, W1, W2, VB1min, VB2min);
    double VWmin = interp_value(W, W1, W2, VW1min, VW2min);
    double VBmax = interp_value(W, W1, W2, VB1max, VB2max);
    double VWmax = interp_value(W, W1, W2, VW1max, VW2max);

    return interp_value(VB, VBmin, VBmax, VWmin, VWmax);
}

void BoatSpeed::Set(int P, int W, int VW, double VB)
{
    if(P < 0 || P > MAX_POWER || W < 0 || W >= DEGREES || VW < 0 || VW > MAX_KNOTS)
        return; /* maybe log warning here? */
    
    speed[P][VW][W].VB = VB;
    speed[P][VW][W].slipangle = 0;
    speed[P][VW][W].b = W;
    speed[P][VW][W].w = 1; /* weighted all on first course */
}

/* to calculate power required to move the boat:

   SL = 8.26 / (Disp / (.01 lwl)^3 )^.311
   lwl - length at water line
   SHP - Ship Horse Power
   SHP = Disp.lwl / ((2.3-SL)*8.3)^3
*/
double boatwake_drag(double lwl, double displacement, double planing_constant)
{
  return 0;
}

double hull_speed(double lwl_ft)
{
  return 1.34*sqrt(lwl_ft);
}

/* values greater than 2 are a danger, less than 2 is "good" */
double capsize_risk(double beam_ft, double displacement_pounds)
{
  return beam_ft/pow(1.1*displacement_pounds/64, 1.0/3);
}

/* values of 30-40 for cruisers, 20 or less for racers, higher for
   heavy boats */
double comfort_factor(double displacement_pounds, double lwl_ft, double loa_ft, double beam_ft)
{
  return displacement_pounds / (.65 * (.7*lwl_ft + .3*loa_ft) * pow(beam_ft, 1.33));
}

double displacement_length_ratio(double displacement_pounds, double lwl)
{
  return displacement_pounds/2240/(.0l*cube(lwl));
}

double sail_area_disp_ratio(double sail_area_sq_ft, double displacement_pounds)
{
  return sail_area_sq_ft/pow(displacement_pounds/64,.66);
}
