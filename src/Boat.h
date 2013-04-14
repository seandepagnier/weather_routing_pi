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

#include "BoatPlan.h"

class Boat
{
public:
    Boat();
    ~Boat();

    wxString OpenXML(wxString filename);
    wxString SaveXML(wxString filename);

    BoatPlan &Plan(int plan) { return *Plans[plan]; }

    std::vector<BoatPlan*> Plans;

    int TrySwitchBoatPlan(int curplan, double VW, double H, double Swell,
                          wxDateTime &gribtime, double lat, double lon, int &daytime);
    double Hulls();
    double CapsizeRisk();
    double ComfortFactor();
    double DisplacementLengthRatio();
    double SailAreaDisplacementRatio();
    double DisplacementLongTons();
    double DisplacementPounds();
    double HullSLRatio();
    double HullSpeed();
    double FrictionDrag(double VB);
    double WakeDrag(double VB);
    void RecomputeDrag();

    enum HullType {MONO, CATAMARAN, TRIMARAN, PROA, SUBMARINE};
    HullType hulltype;

    double displacement_tons, lwl_ft, loa_ft, beam_ft;
    double frictional_drag, wake_drag;
};
