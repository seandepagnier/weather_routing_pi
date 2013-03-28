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

#include "Utilities.h"
#include "Boat.h"

Boat::Boat()
{
    Plans.push_back(new BoatPlan(_("Initial Plan")));
    displacement_lbs = 8000, lwl_ft = 24, loa_ft = 27, beam_ft = 8;
    frictional_drag = 0, wake_drag = 0;
}

Boat::~Boat()
{
}

bool Boat::OpenBinary(const char *filename)
{
    return false;

    FILE *f = fopen(filename, "r");
    if(!f)
        return false;

    int s = sizeof *this;
    bool ret = true;
    if(fread(this, s, 1, f) != 1)
        ret = false;

    fclose(f);
    return ret;
}

bool Boat::SaveBinary(const char *filename)
{
    return false;

    FILE *f = fopen(filename, "w");
    if(!f)
        return false;

    int s = sizeof *this;
    bool ret = true;
    if(fwrite(this, s, 1, f) != 1)
        ret = false;

    fclose(f);
    return ret;
}

int Boat::TrySwitchBoatPlan(int curplan, double VW, double H, double Swell)
{
    BoatPlan &boatplan = *Plans[curplan];

again:
    wxString Name = boatplan.TrySwitchBoatPlan(VW, H, Swell);
    if(Name.empty())
        return curplan;
    for(unsigned int i=0; i<Plans.size(); i++)
        if(Name == Plans[i]->Name) {
            curplan = i;
            goto again;
        }

    printf("error, failed to find plan: %s\n", (const char*)Name.ToAscii());
    exit(1);
    return curplan;
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
    return beam_ft/pow(1.1*displacement_lbs/64, 1.0/3);
}

/* values of 30-40 for cruisers, 20 or less for racers, higher for
   heavy boats */
double Boat::ComfortFactor()
{
    return displacement_lbs / (.65 * (.7*lwl_ft + .3*loa_ft) * pow(beam_ft, 1.33));
}

double Boat::DisplacementLengthRatio()
{
    return DisplacementLongTons() / cube(.01 * lwl_ft) / Hulls();
}

double Boat::DisplacementLongTons()
{
    return displacement_lbs / 2240.0;
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
    return frictional_drag*VB*VB;
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
    if(VB == 0) return 0;

    const double G = 9.8;
    double L = ft2m(lwl_ft);
    double F = knots2m_s(VB) / sqrt(G * L);

    double F2 = square(F), invF2 = 1/F2;
    double D = square(sin(M_PI - invF2) / (M_PI - invF2) / (1 + M_PI * F2));

    return wake_drag * wake_drag * D * VB * VB * 4; /* D is max of .25 (at F=.56) so normalize to 1 */
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
