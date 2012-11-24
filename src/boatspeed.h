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

#define MAX_KNOTS 60

/* from 0 to 100% motor power 10% at a time */
#define MAX_POWER 10
#define DEGREES 360
#define DEGREE_STEP 5

class SailingSpeed
{
public:
  double knots, direction;

  int b, c; /* tacks to sail relative to true wind */
  double w; /* weight of time on each tack */
};

/* contain all data for boat under sail with various motor power levels */
class BoatSpeed
{

public:
  double eta; /* sailing constant */

  double powerspeed[MAX_POWER+1]; /* How fast do we power at each power level
                                      in dead calm. */

  SailingSpeed speed[MAX_POWER+1][MAX_KNOTS][DEGREES];

  BoatSpeed() { computed = false; }
  void ComputeBoatSpeeds(double eta, double keel_pressure, double keel_lift);
  void OptimizeTackingSpeed();

  void Speed(int P, int W, int VW, double &BA, double &VB);
  void Set(int P, int W, int VW, double direction, double knots);

  bool computed;
};

/* calculate power from solar charged battery system
   driving electric motors to power the boat.  The
   wind involved can be assumed negligable because the
*/
class ElectricBoatSpeed : public BoatSpeed
{
/* charge */
//  ChargeSources sources;

/* battery */
  double kwh;
  double drive_efficiency; /* factor to compute power */
  double drag_factor;

                      
  double Speed(double kw);
};

class SteamPoweredBoat : public BoatSpeed
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
class HumanPoweredBoatSpeed : BoatSpeed
{
  double humans;
  double JoulsPerPerson;
  double drive_efficiency;
};

class Boat
{
  BoatSpeed *DriveMethod;
  double hull_length;
  double drag;
};

extern Boat all_boat;
