/***************************************************************************
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

#include <vector>

struct SailingVMG
{
    enum { STARBOARD_UPWIND, PORT_UPWIND, STARBOARD_DOWNWIND, PORT_DOWNWIND};
    float values[4];
};

class SwitchPlan
{
public:
    SwitchPlan() :
        MaxWindSpeed(NAN), MinWindSpeed(NAN),
        MaxWindDirection(NAN), MinWindDirection(NAN),
        MaxWaveHeight(NAN), MinWaveHeight(NAN) {}

    double MaxWindSpeed, MinWindSpeed;
    double MaxWindDirection, MinWindDirection;
    double MaxWaveHeight, MinWaveHeight;

    bool DayTime, NightTime;

    wxString Name;
};

class Boat;

#define DEGREES 360

class BoatPlan
{
public:
    static double VelocityApparentWind(double VB, double W, double VW);
    static double DirectionApparentWind(double VA, double VB, double W, double VW);
    static double DirectionApparentWind(double VB, double W, double VW);
    static double VelocityTrueWind(double VA, double VB, double W);

    bool Open(const char *filename, wxString &message);
    bool Save(const char *filename);

    void BoatSteadyState(double W, double VW, double &B, double &VB,
                         double &A, double &VA, Boat &boat);
    int TrySwitchBoatPlan(double VW, double H, double S,
                          const wxDateTime &gribtime, double lat, double lon, int &daytime);

    BoatPlan(wxString PlanName);

    std::vector<SwitchPlan> SwitchPlans;

    wxString Name;
    enum PolarMethod { CSV, TRANSFORM, IMF };
    PolarMethod polarmethod;

    /* if computed */
    double eta, luff_angle;
    bool optimize_tacking;
    bool wing_wing_running;

    /* if not computed (csv table) */
    wxString csvFileName;
    int wind_speed_step, wind_degree_step;

    void ComputeBoatSpeeds(Boat &boat, PolarMethod method, int speed = -1);
    void OptimizeTackingSpeed();
    void ResetOptimalTackingSpeed();

    int ClosestVWi(double VW);

    double Speed(double W, double VW);
    double SpeedAtApparentWindDirection(double A, double VW, double *pW=0);
    double SpeedAtApparentWindSpeed(double W, double VA);
    double SpeedAtApparentWind(double A, double VA, double *pW=0);

    SailingVMG GetVMGTrueWind(double VW);
    SailingVMG GetVMGApparentWind(double VA);

    double TrueWindSpeed(double VB, double W, double maxVW);
    void Set(unsigned int Wi, unsigned int VWi, double VB, double W);

private:
    void UpdateDegreeStepLookup();
    void CalculateVMG(int speed);

    double AngleofAttackBoat(double A, double VA);
    double VelocityBoat(double A, double VA);

    double m_MouseW;

    double max_knots;

    struct SailingWindSpeed {
        struct SailingSpeed {
            SailingSpeed() {}
            SailingSpeed(double nVB, double W) : VB(nVB), origVB(nVB), slipangle(0), w(1), b(W) {}

            float VB; /* speed in knots */
            float origVB; /* speed before VMG optimization */
            float slipangle; /* angle of boat relative to its movement thru water */

            float w; /* weight of time on each tack */
            float b, c; /* tacks to sail relative to true wind */
        };

        SailingWindSpeed(double nVW) : VW(nVW) {}

        double VW;
        std::vector<SailingSpeed> speeds; // by degree_count
        SailingVMG VMG;
    }; // num_wind_speeds

    std::vector<SailingWindSpeed> wind_speeds;
    std::vector<double> degree_steps;

    unsigned int degree_step_index[DEGREES];
};

/* calculate power from solar charged battery system
   driving electric motors to power the boat.  The
   wind involved can be assumed negligable because the
*/
class ElectricBoatPlan : public BoatPlan
{
/* charge */
//  ChargeSources sources;

/* battery */
  double kwh;
  double drive_efficiency; /* factor to compute power */
  double drag_factor;

  double Plan(double kw);
};

class SteamPoweredBoat : public BoatPlan
{
  double efficiency;
  double power;
  double fuel_hours;

  double min_fire_speed; /* fire up engine below this speed */
  double max_fire_speed; /* quench fire at this speed */
};

/* given willingness of participants to participate,
   it is possible to power the boat using oar or sweep.
*/
class HumanPoweredBoatPlan : BoatPlan
{
  double humans;
  double JoulsPerPerson;
  double drive_efficiency;
};
