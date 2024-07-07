/***************************************************************************
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

#include <vector>
#include "PolygonRegion.h"

struct SailingVMG
{
    enum { STARBOARD_UPWIND, PORT_UPWIND, STARBOARD_DOWNWIND, PORT_DOWNWIND};
    float values[4];
};

struct PolarMeasurement
{
    PolarMeasurement(double v, double d, double vb, bool apparent=true);
    double VW() const;
    double W() const;
    double VA, A, VB, eta;
};

class Boat;

#define DEGREES 360

// Error codes when Polar::Speed returns NaN.
enum class PolarErrorCode {
    None,                  // No error has occurred.
    NegativeWindSpeed,     // The input true wind speed is negative.
    EmptyPolarData,        // The polar file contains no data.
    WindAngleOutOfRange,   // The input heading is out of the polar range, either too much upwind or too much downwind.
    WindSpeedOutOfBounds,  // The input wind speed is either below the minimum polar wind or above the maximum polar wind.
    NegativeBoatSpeed,     // The calculated boat speed is negative.
    BoatSpeedNaNValue      // The polar data contains NaN boat speed value.
};

class Polar
{
public:
    static double VelocityApparentWind(double VB, double W, double VW);
    static double DirectionApparentWind(double VA, double VB, double W, double VW);
    static double DirectionApparentWind(double VB, double W, double VW);
    static double VelocityTrueWind(double VA, double VB, double W);
    static double VelocityTrueWind2(double VA, double VB, double A);

    Polar();

    bool Open(const wxString &filename, wxString &message);
    bool Save(const wxString &filename);

    wxString FileName;

    void OptimizeTackingSpeeds();
    void ClosestVWi(double VW, int &VW1i, int &VW2i);

    double Speed(double W, double VW, bool bound=false, bool optimize_tacking=false, PolarErrorCode* error_code=NULL);
    double SpeedAtApparentWindDirection(double A, double VW, double *pW=0);
    double SpeedAtApparentWindSpeed(double W, double VA);
    double SpeedAtApparentWind(double A, double VA, double *pW=0);

    double MinDegreeStep() { return degree_steps[0]; }

    SailingVMG GetVMGTrueWind(double VW);
    SailingVMG GetVMGApparentWind(double VA);

    double TrueWindSpeed(double VB, double W, double maxVW);
    bool InterpolateSpeeds();
    void UpdateSpeeds();
    void UpdateDegreeStepLookup();

    bool InsideCrossOverContour(float H, float VW, bool optimize_tacking);
    PolygonRegion CrossOverRegion;

    void Generate(const std::list<PolarMeasurement> &measurements);
    void AddDegreeStep(double twa);
    void RemoveDegreeStep(int index);
    void AddWindSpeed(double tws);
    void RemoveWindSpeed(int index);

    double m_crossoverpercentage;

private:
    friend class EditPolarDialog;
    friend class BoatDialog;

    void CalculateVMG(int speed);

    double AngleofAttackBoat(double A, double VA);
    double VelocityBoat(double A, double VA);

    struct SailingWindSpeed {
        SailingWindSpeed(double nVW) : VW(nVW) {}

        float VW;
        std::vector<float> speeds;      // by degree_count
        std::vector<float> orig_speeds; // by degree_count, from polar file.
        SailingVMG VMG;
    }; // num_wind_speeds
    bool VMGAngle(SailingWindSpeed &ws1, SailingWindSpeed &ws2, float VW, float &W);

    // The true wind speed for each wind angle in 'degree_steps'.
    std::vector<SailingWindSpeed> wind_speeds;
    // The values of the true wind angle for which true wind speed is reported.
    std::vector<double> degree_steps;
    unsigned int degree_step_index[DEGREES];
};
