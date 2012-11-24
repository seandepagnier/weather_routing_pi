/******************************************************************************
 *
 * Project:  OpenCPN Advanced Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2012 by Sean D'Epagnier                                 *
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

#include <stdlib.h>
#include <math.h>

#include "utilities.h"
#include "boatspeed.h"

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
but we can simplifty and have it go against forward movement

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

double sail_forward_force(double eta, double A, double VA)
{
  return eta * sin(A/2) * sqrt(VA);
}

double sail_slip_force(double eta, double A, double VA)
{
  return eta * cos(A/2) * sqrt(VA);
}

double boat_slip_speed(double eta, double A, double VA, double keel_pressure)
{
  return sail_slip_force(eta, A, VA) / keel_pressure;
}
double boat_forward_speed(double eta, double A, double VA, double keel_pressure, double keel_lift, double P)
{
  return (sail_forward_force(eta, A, VA) + P) / (1 + keel_lift * boat_slip_speed(eta, A, VA, keel_pressure));
}

/* difference between direction we face and direction moving thru water */
double AngleofAttackBoat(double eta, double A, double VA, double keel_pressure, double keel_lift, double P)
{
  double ss = boat_slip_speed(eta, A, VA, keel_pressure);
  double fs = boat_forward_speed(eta, A, VA, keel_pressure, keel_lift, P);
  return atan2(ss,
               fs);
}

/* actual speed over water of boat */
double VelocityBoat(double eta, double A, double VA, double keel_pressure, double keel_lift, double P)
{
  double ss = boat_slip_speed(eta, A, VA, keel_pressure);
  double fs = boat_forward_speed(eta, A, VA, keel_pressure, keel_lift, P);
  return hypot(ss,
               fs);
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

Since at maximum velocity drag force exactly cancels boat force:
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
A(

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

double VelocityApparentWind(double VB, double W, double VW)
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
double DirectionApparentWind(double VA, double VB, double W, double VW)
{
  if(VA == 0) /* apparent wind direction is anything */
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
void BoatSteadyState(double W, double VW, double eta, double keel_pressure, double keel_lift, double P,
                     double &BA, double &VB, double &A, double &VA)
{
  /* starting out not moving */
  VB = 0, A = W, VA = VW;
  for(;;) {
    double v = VelocityBoat(eta, A, VA, keel_pressure, keel_lift, P);
    if(v - VB < 1e-10) {
      BA = AngleofAttackBoat(eta, A, VA, keel_pressure, keel_lift, P);
      return; /* reached steady state */
    }

    VB = (VB + v) / 2; /* average to avoid overshoot */
    VA = VelocityApparentWind(VB, W, VW);
    A =  DirectionApparentWind(VA, VB, W, VW);
  }
}

void BoatSpeed::ComputeBoatSpeeds(double eta, double keel_pressure, double keel_lift)
{
  for(int P = 0; P <= 1/*MAX_POWER*/; P++)
    for(int VW = 0; VW < MAX_KNOTS; VW++)
      for(int W = 0; W <= DEGREES/2; W+=DEGREE_STEP) {
        double BA, VB, A, VA;
        BoatSteadyState(deg2rad(W), VW, eta, keel_pressure, keel_lift, P, BA, VB, A, VA);
        Set(P, W, VW, W+rad2deg(BA), VB);
        if(W != 0)
          Set(P, DEGREES-W, VW, DEGREES-W-rad2deg(BA), VB);
    }
  computed = true;
}

/* instead of traveling in the direction given, allow traveling at angles
   closer to +- 90.  Basically we are trying every two tack angles and combining
   to determine if alternating between these two courses is actually faster than
   the current strategy for the given course, and if so, use it.
   has implications
*/
void BoatSpeed::OptimizeTackingSpeed()
{
#if 0
  for(int P = 0; P <= MAX_POWER; P++)
    for(int VW = 0; VW < MAX_KNOTS; VW++)
      for(int at = 0; at <= DEGREES/2; at+=DEGREE_STEP)
        for(int bt = DEGREE_STEP; bt < DEGREES/2; bt+=DEGREE_STEP)
          for(int ct = -DEGREES/2+DEGREE_STEP; ct < 0; ct+=DEGREE_STEP) {
            /* don't use speeds which already optimize with tacking */
            if(speed[P][VW][b]->w != 1 || speed[P][VW][c]->w != 1)
              continue;
            /*
              w is the weight between b and c tacks (.5 for equal time on each
              t is the improvement factor]
              sa * sin(a) = w*sb*sin(b) + (1-w)*sc*sin(c)
              sa * cos(a) = w*sb*cos(b) + (1-w)*sc*cos(c)
            */
            double a = Direction(P, VW, at), Direction(P, VW, bt), c = Direction(P, VW, ct);
            double sb = Speed(P, W, VW, b), sc = Speed(P, VW, c + 360);
            double ar = deg2rad(a), br = deg2rad(b), cr = deg2rad(c);
            double sa = (sb * sc * (-cos(br)*sin(cr) +  cos(cr)*sin(br)))
              / ((cos(ar)*sb*sin(br) - cos(ar)*sc*sin(cr)
                  - sin(ar)*sb*cos(br) + sin(ar)*sc*cos(cr)));
            double w = (sc * (sin(ar) * cos(cr) - cos(ar) * sin(cr)))
              / (cos(ar)*sb*sin(br) - cos(ar)*sc*sin(cr)
                 - sin(ar)*sb*cos(br) + sin(ar)*sc*cos(cr));

            if(sa > speed[P][VW][a] && w > 0 && w < 1) {
              speed[P][VW][a]->knots = sa;
              speed[P][VW][a]->b = b;
              speed[P][VW][a]->c = c;
              speed[P][VW][a]->w = 1;
              if(a > 0)
                speed[P][VW][360-a] = speed[P][VW][a];
          }
        }
#endif
}

void BoatSpeed::Speed(int P, int W, int VW, double &BA, double &VB)
{
  if(P < 0 || P > MAX_POWER
     || VW < 0 || VW > MAX_KNOTS
     || W < 0 || W >= DEGREES)
    exit(-233);

  VB = speed[P][VW][W].knots;
  BA = speed[P][VW][W].direction;
}

void BoatSpeed::Set(int P, int W, int VW, double direction, double knots)
{
  speed[P][VW][W].knots = knots;
  speed[P][VW][W].direction = direction;
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
