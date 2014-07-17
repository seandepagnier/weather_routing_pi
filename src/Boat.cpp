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

#include <wx/wx.h>
#include <wx/filename.h>

#include "tinyxml/tinyxml.h"

#include "weather_routing_pi.h"
#include "Utilities.h"
#include "Boat.h"

Boat::Boat()
    : displacement_tons(4), lwl_ft(24), loa_ft(27), beam_ft(8),
      frictional_drag(0), wake_drag(0)
{
}

Boat::~Boat()
{
}

wxString Boat::OpenXML(wxString filename)
{
    /* shortcut if already loaded, and boat wasn't modified */
    wxDateTime last_filetime = wxFileName(filename).GetModificationTime();
    if(m_last_filename == filename && m_last_filetime.IsValid() && m_last_filetime == last_filetime)
        return _T("");

    bool cleared = false;
    Plans.clear();
    
    TiXmlDocument doc;
    if(!doc.LoadFile( filename.mb_str() ))
        return _("Failed to load file: ") + filename;

    TiXmlHandle root( doc.RootElement() );
    if(strcmp(doc.RootElement()->Value(), "OpenCPNWeatherRoutingBoat"))
        return _("Invalid xml file (no OCPWeatherRoutingBoat node): " + filename);

    for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement()) {
        if(!strcmp(e->Value(), "BoatCharacteristics")) {
            displacement_tons = AttributeDouble(e, "displacement_tons", 4);
            sail_area_ft2 = AttributeDouble(e, "sail_area_ft2", 400);
            lwl_ft = AttributeDouble(e, "lwl_ft", 24);
            loa_ft = AttributeDouble(e, "loa_ft", 27);
            beam_ft = AttributeDouble(e, "beam_ft", 8);
        } else if(!strcmp(e->Value(), "BoatDrag")) {
            frictional_drag = AttributeDouble(e, "frictional_drag", 0);
            wake_drag = AttributeDouble(e, "wake_drag", 0);
         } else if(!strcmp(e->Value(), "Plan")) {
             if(!cleared) {
                 Plans.clear();
                 cleared = true;
             }

             BoatPlan plan(wxString::FromUTF8(e->Attribute("Name")), *this);

             plan.polarmethod = (BoatPlan::PolarMethod)AttributeInt(e, "polarmethod", BoatPlan::TRANSFORM);

             switch(plan.polarmethod) {
             case BoatPlan::CSV:
                 plan.csvFileName = wxString::FromUTF8(e->Attribute("csvFileName"));
                 if(!plan.Open(plan.csvFileName.mb_str()))
                     return _("Failed to open file: ") + plan.csvFileName;
                 break;
             case BoatPlan::TRANSFORM:
                 plan.eta = AttributeDouble(e, "eta", .5);
                 plan.luff_angle = AttributeDouble(e, "luff_angle", 15);
                 plan.wing_wing_running = AttributeBool(e, "wing_wing_running", false);
                 plan.ComputeBoatSpeeds(*this, plan.polarmethod);
                 break;
             case BoatPlan::IMF:
                 plan.ComputeBoatSpeeds(*this, plan.polarmethod);
                 break;
             }

             plan.optimize_tacking = AttributeBool(e, "optimize_tacking", false);
             if(plan.optimize_tacking)
                 plan.OptimizeTackingSpeed();

             for(TiXmlElement* f = e->FirstChildElement(); f; f = f->NextSiblingElement()) {
                 if(!strcmp(f->Value(), "SwitchPlan")) {
                     SwitchPlan switchplan;
                     switchplan.MaxWindSpeed = AttributeDouble(f, "MaxWindSpeed", NAN);
                     switchplan.MinWindSpeed = AttributeDouble(f, "MinWindSpeed", NAN);
                     switchplan.MaxWindDirection = AttributeDouble(f, "MaxWindDirection", NAN);
                     switchplan.MinWindDirection = AttributeDouble(f, "MinWindDirection", NAN);
                     switchplan.MaxWaveHeight = AttributeDouble(f, "MaxWaveHeight", NAN);
                     switchplan.MinWaveHeight = AttributeDouble(f, "MinWaveHeight", NAN);
                     switchplan.DayTime = AttributeBool(f, "DayTime", true);
                     switchplan.NightTime = AttributeBool(f, "NightTime", true);
                     switchplan.Name = wxString::FromUTF8(f->Attribute("Name"));
                     plan.SwitchPlans.push_back(switchplan);
                 }
             }

             Plans.push_back(plan);
         }
     }

     m_last_filename = filename;
     m_last_filetime = last_filetime;
     return _T("");
 }

 wxString Boat::SaveXML(wxString filename)
 {
     TiXmlDocument doc;
     TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
     doc.LinkEndChild( decl );
     TiXmlElement * root = new TiXmlElement( "OpenCPNWeatherRoutingBoat" );

     doc.LinkEndChild( root );
     char version[24];
     sprintf(version, "%d.%d", PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR);
     root->SetAttribute("version", version);
     root->SetAttribute("creator", "Opencpn Weather Routing plugin");

     TiXmlElement *boatcharacteristics = new TiXmlElement( "BoatCharacteristics" );
     boatcharacteristics->SetAttribute("displacement_tons", displacement_tons);
    boatcharacteristics->SetAttribute("sail_area_ft2", sail_area_ft2);
    boatcharacteristics->SetAttribute("lwl_ft", lwl_ft);
    boatcharacteristics->SetAttribute("loa_ft", loa_ft);
    boatcharacteristics->SetAttribute("beam_ft", beam_ft);
    root->LinkEndChild(boatcharacteristics);

    TiXmlElement *boatdrag = new TiXmlElement( "BoatDrag" );
    char str[24];

    sprintf(str, "%.4f", frictional_drag);
    boatdrag->SetAttribute("frictional_drag", str);

    sprintf(str, "%.4f", wake_drag);
    boatdrag->SetAttribute("wake_drag", str);

    root->LinkEndChild(boatdrag);

    for(unsigned int i=0; i<Plans.size(); i++) {
        TiXmlElement *e = new TiXmlElement( "Plan" );
        BoatPlan &plan = Plans[i];
        e->SetAttribute("Name", plan.Name.mb_str());

        e->SetAttribute("polarmethod", plan.polarmethod);
        switch(plan.polarmethod) {
        case BoatPlan::CSV:
            e->SetAttribute("csvFileName", plan.csvFileName.mb_str());
            break;
        case BoatPlan::TRANSFORM:
            sprintf(str, "%.4g", plan.eta);
            e->SetAttribute("eta", str);

            sprintf(str, "%.4f", plan.luff_angle);
            e->SetAttribute("luff_angle", str);

            e->SetAttribute("wing_wing_running", plan.wing_wing_running);
            break;
        case BoatPlan::IMF:
            break;
        }

        e->SetAttribute("optimize_tacking", plan.optimize_tacking);

        for(unsigned int j=0; j<plan.SwitchPlans.size(); j++) {
            TiXmlElement *f = new TiXmlElement( "SwitchPlan" );
            SwitchPlan &switchplan = plan.SwitchPlans[j];
#define SetAttr(NAME) if(!isnan(switchplan.NAME)) f->SetDoubleAttribute(#NAME, switchplan.NAME)
            SetAttr(MinWindSpeed);
            SetAttr(MaxWindSpeed);
            SetAttr(MinWindDirection);
            SetAttr(MaxWindDirection);
            SetAttr(MinWaveHeight);
            SetAttr(MaxWaveHeight);
            if(switchplan.DayTime)   f->SetAttribute("DayTime", switchplan.DayTime);
            if(switchplan.NightTime) f->SetAttribute("NightTime", switchplan.NightTime);
            f->SetAttribute("Name", switchplan.Name.mb_str());
            e->LinkEndChild(f);
        }

        root->LinkEndChild(e);
    }

    if(!doc.SaveFile(filename.mb_str()))
        return _("Failed saving file: ") + filename;
    return wxString();
}

int Boat::TrySwitchBoatPlan(int curplan, double VW, double H, double Swell,
                            const wxDateTime &gribtime, double lat, double lon, int &daytime)
{
    for(int rounds = 0; rounds < 5; rounds++) {
        BoatPlan &boatplan = Plans[curplan];
        int index = boatplan.TrySwitchBoatPlan(VW, H, Swell, gribtime,
                                               lat, lon, daytime);
        if(index == -1)
            return curplan;

        wxString &Name = boatplan.SwitchPlans[index].Name;
        unsigned int i;
        for(i=0; i<Plans.size(); i++)
            if(Name == Plans[i].Name) {
                curplan = i;
                break;
            }

        if(i == Plans.size()) {
//            printf("error, failed to find plan: %s\n", (const char*)Name.mb_str());
            break;
        }
    }

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
