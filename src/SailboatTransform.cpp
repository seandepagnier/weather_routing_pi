
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

      sin A    sin W      sin W-A
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

pF(t) = BF(A(t), VA(t)) - DF(t)
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
        if(p.MaxWindSpeed >= VW)     continue;
        if(p.MinWindSpeed <= VW)     continue;
        if(p.MaxWindDirection >= H)  continue;
        if(p.MinWindDirection <= H)  continue;
        if(p.MaxWaveHeight >= Swell) continue;
        if(p.MinWaveHeight <= Swell) continue;
        if(!p.DayTime) {
            if(ComputeDayTime(gribtime, lat, lon, daytime)) continue;
        } else if(!p.NightTime)
            if(!ComputeDayTime(gribtime, lat, lon, daytime)) continue;
        return i;
    }
    return -1;
}


double Boat::Hulls()
{
    switch(hulltype) {
    case CATAMARAN: return 2;
    case TRIMARAN: return 3;
    case PROA: return 2;
    default: return 1;
    }
}

/* values greater than 2 are a danger, less than 2 is "good" */
double Boat::CapsizeRisk()
{
    return beam_ft/pow(1.1*DisplacementPounds()/64, 1.0/3);
}

double Boat::SailAreaDisplacementRatio()
{
    return sail_area_ft2 / pow(DisplacementPounds() / 64, 2.0/3.0);
}

/* values of 30-40 for cruisers, 20 or less for racers, higher for
   heavy boats */
double Boat::ComfortFactor()
{
    return DisplacementPounds() / (.65 * (.7*lwl_ft + .3*loa_ft) * pow(beam_ft, 1.33));
}

double Boat::DisplacementLengthRatio()
{
    return DisplacementLongTons() / cube(.01 * lwl_ft) / Hulls();
}

double Boat::DisplacementLongTons()
{
    return DisplacementPounds() / 2240.0;
}

double Boat::DisplacementPounds()
{
    return displacement_tons * 2000.0;
}

/* to calculate power required to move the boat:

   SL = 8.26 / (Disp / (.01 lwl)^3 )^.311
   lwl - length at water line
   Ship Horse Power = Disp.lwl / ((2.3-SL)*8.11)^3

1.34 = knots/sqrt(lwl)
   Ship Horse Power = Disp.lwl / ((2.3-SL)*8.11)^3

*/

double Boat::HullSLRatio()
{
    double SL = 8.26 / pow(DisplacementLengthRatio(), .311);
    if(SL < 1.34)
        SL = 1.34;
    return SL;
}

double Boat::HullSpeed()
{
    return HullSLRatio() * sqrt(lwl_ft);
}

/* assume frictional drag is related to speed squared */
double Boat::FrictionDrag(double VB)
{
    return 10*frictional_drag*VB*VB;
}

/* wave drag in terms of froude number
                                       2
           /      sin(Pi - F^-2)      \
   Drag = |  -----------------------   |
           \ (Pi - F^-2) (1 + Pi F^2) /

              ___
    V = F * \/g l

    F = sqrt(g * l) / V;

    g = 9.8 (gravity constant)
    l is length of vessel in meters
    V is meters per second

    Reaches a peak at F=Pi^-.5 which is about .56

    The huge increase starts at F = .4  (normal hull speed setting)
    
    Also interesting to note that at 80% of normal hull speed, the wave
    drag is zero as well, but the first hump occurs from .32 to .4

    This works for the full range from displacement to planing mode.
 */
double Boat::WakeDrag(double VB)
{
#if 0 // this may be theoretically correct in flat water, but is complex and not practical
    if(VB == 0) return 0;

    const double G = 9.8;
    double L = ft2m(lwl_ft);
    double F = knots2m_s(VB) / sqrt(G * L);

    double F2 = square(F), invF2 = 1/F2;
    double D = square(sin(M_PI - invF2) / (M_PI - invF2) / (1 + M_PI * F2));

    return wake_drag * wake_drag * D * VB * VB; /* D is max of .25 (at F=.56) so normalize to 1 */

#else
    // classic hull speed exponential without planing possible
//    double hull_speed = 1.34*sqrt(lwl_ft);
//    double our_wave_drag = 2 * wake_drag * (pow(8, VB / sqrt(lwl_ft)) - 1);

    double coeff = VB / 1.34 / sqrt(lwl_ft);
    if(coeff < 1)
        return 0;

    double drag = (pow(32, coeff - 1) - 1) * wake_drag * 10;

    if(drag > VB)
        drag = VB;

    return drag;
#endif
}

void Boat::RecomputeDrag()
{
    /* for frictional drag, assume related to wetted surface area,
       I pulled this out of my ass so please fix it. */
    frictional_drag = .001 * (pow(DisplacementLengthRatio(), .5));

    if(hulltype == SUBMARINE) {
        frictional_drag *= 2.5; /* subs have high surface area */
        wake_drag = 0; /* assuming submerged to depth of more than sqrt(2) length */
    } else {
        /* for wake drag..this really depends on hull type, but we will relate
           displacement here  */
        double froude = HullSLRatio() * .4 / 1.34;
        double maxdrag = 1.2;//pow(M_PI, -.5);
        wake_drag = (maxdrag-froude) / (maxdrag - .4);
        wake_drag = square(wake_drag);
        if(wake_drag < 0) wake_drag = 0; else if(wake_drag > 1) wake_drag = 1;
    }
}

/*
          

           2          2
     VA sin (A/2) = VB eta

eta = VA*sin^2 (A/2)/VB^2


        2        1 - cos(A)
     sin (A/2) = ----------
                     2

eta = VA/VB^2*(1-cos(A))/2

                 __
           A    /VA
VB =  sin (-)  /---
           2 \/ eta

*/
