/***************************************************************************
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

static const int wind_speeds[] = {0, 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 36, 40, 45, 50, 55, 60};
static const int num_wind_speeds = (sizeof wind_speeds) / (sizeof *wind_speeds);

#define MAX_KNOTS wind_speeds[num_wind_speeds - 1]

#define DEGREES 360
#define DEGREE_STEP 3
#define DEGREE_COUNT (DEGREES / DEGREE_STEP)

struct SailingSpeed
{
    float VB; /* speed in knots */
    float origVB; /* speed before VMG optimization */
    float slipangle; /* angle of boat relative to its movement thru water */

    float w; /* weight of time on each tack */
    unsigned char b, c; /* tacks to sail relative to true wind */
};

struct SailingVMG
{
    int PortTackUpWind, StarboardTackUpWind;
    int PortTackDownWind, StarboardTackDownWind;
};

#define MAX_WINDSPEEDS_IN_TABLE 100

class BoatSpeedTableEntry
{
public:
  double W;
  double boatspeed[MAX_WINDSPEEDS_IN_TABLE];
};

#include <list>
#include <vector>
typedef std::list<BoatSpeedTableEntry> BoatSpeedTableEntryList;

class BoatSpeedTable
{
public:
    BoatSpeedTable();
    ~BoatSpeedTable();

    bool Open(const char *filename, int &wind_speed_step, int &wind_degree_step);
    bool Save(const char *filename);

    double InterpolateSpeed(double VW, double W);
    
    int numwindspeeds;
    double windspeeds[MAX_WINDSPEEDS_IN_TABLE];
    
    BoatSpeedTableEntryList table;
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

class BoatPlan
{
public:
    static double VelocityApparentWind(double VB, double W, double VW);
    static double DirectionApparentWind(double VA, double VB, double W, double VW);
    void BoatSteadyState(double W, double VW, double &B, double &VB, double &A, double &VA,
                         Boat &boat);
    wxString TrySwitchBoatPlan(double VW, double H, double Swell,
                               wxDateTime &gribtime, double lat, double lon, int &daytime);

    BoatPlan(wxString PlanName, Boat &boat);
    ~BoatPlan();

    std::vector<SwitchPlan> SwitchPlans;

    wxString Name;
    bool computed;

    /* if computed */
    double eta, luff_angle;

    /* if not computed (csv table) */
    wxString csvFileName;
    int wind_speed_step, wind_degree_step;

    void ComputeBoatSpeeds(Boat &boat);
    void OptimizeTackingSpeed();
    void ResetOptimalTackingSpeed();
    void SetSpeedsFromTable(BoatSpeedTable &table);
    BoatSpeedTable CreateTable(int wind_speed_step, int wind_degree_step);

    static int ClosestVWi(int VW);

    double Speed(double W, double VW);
    SailingVMG GetVMG(double VW);
    double TrueWindSpeed(double VB, double W, double maxVW);
    void Set(int Wi, int VWi, double VB);

private:
    SailingSpeed speed[num_wind_speeds][DEGREE_COUNT];
    SailingVMG VMG[num_wind_speeds];

    int BestVMG(int VW, int startW, int endW, int upwind);
    void CalculateVMG();

    double AngleofAttackBoat(double A, double VA);
    double VelocityBoat(double A, double VA);

    double m_MouseW;
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
