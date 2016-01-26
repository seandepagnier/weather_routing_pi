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

#include <wx/wx.h>
#include <wx/filename.h>

#include "tinyxml/tinyxml.h"

#include "weather_routing_pi.h"
#include "Utilities.h"
#include "Boat.h"

Boat::Boat()
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
    Polars.clear();

    if(!wxFileName::FileExists(filename))
        return _("Boat file does not exist.");
   
    TiXmlDocument doc;
    if(!doc.LoadFile( filename.mb_str() ))
        return _("Failed to load file: ") + filename;

    TiXmlHandle root( doc.RootElement() );
    if(strcmp(doc.RootElement()->Value(), "OpenCPNWeatherRoutingBoat"))
        return _("Invalid xml file (no OCPWeatherRoutingBoat node): " + filename);

    for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement()) {
        if(!strcmp(e->Value(), "Polar")) {
            if(!cleared) {
                Polars.clear();
                cleared = true;
            }

            Polar polar; //(wxString::FromUTF8(e->Attribute("Name")));

            polar.FileName = wxString::FromUTF8(e->Attribute("FileName"));
            wxString message;
            if(!polar.Open(polar.FileName, message))
                return message;

//            polar.optimize_tacking = AttributeBool(e, "optimize_tacking", false);
//            if(polar.optimize_tacking)
//                polar.OptimizeTackingSpeed();
             
            Polars.push_back(polar);
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

    for(unsigned int i=0; i<Polars.size(); i++) {
        TiXmlElement *e = new TiXmlElement( "Polar" );
        Polar &polar = Polars[i];
//        e->SetAttribute("Name", polar.Name.mb_str());

        e->SetAttribute("FileName", polar.FileName.mb_str());

//        e->SetAttribute("optimize_tacking", polar.optimize_tacking);

        root->LinkEndChild(e);
    }

    if(!doc.SaveFile(filename.mb_str()))
        return _("Failed saving file: ") + filename;

    return wxString();
}

int Boat::TrySwitchPolar(int curpolar, double VW, double H, double Swell)
{
    Polar &boatpolar = Polars[curpolar];

    if(boatpolar.InsideCrossOverContour(H, VW))
        return curpolar;

    // the current polar must change; select the first polar we can use
    for(int i=0; i<(int)Polars.size(); i++)
        if(i != curpolar && Polars[i].InsideCrossOverContour(H, VW))
            return i;

    return -1; // no valid polar
}

static Point
FindNextSegmentPoint(std::list<Segment> &segments, Point p, bool greater)
{
    Point best;
    bool nobest = true;
    for(std::list<Segment>::iterator it = segments.begin();
        it != segments.end(); it++)
        if(it->p[0].x == p.x)
            if((greater &&
                (it->p[0].y > p.y) && (nobest || it->p[0].y < best.y)) ||
               (!greater &&
                (it->p[0].y < p.y) && (nobest || it->p[0].y > best.y))) {
                best = it->p[0];
                nobest = false;
            }
    if(nobest)
        printf("warning, failed to find matching segment");
    return best;
}

int Boat::FastestPolar(float H, float VW)
{
    int maxi = -1;
    float max = 0;
    for(unsigned int i=0; i<Polars.size(); i++) {
        float speed = Polars[i].Speed(VW, H);
        if(speed > max) {
            max = speed;
            maxi = i;
        }
    }
    return maxi;
}

void Boat::GenerateCrossOverChart()
{
    int buffer[2][40];
    int bi = 0;
    std::list<Segment> segments[Polars.size()];
    for(double H = 0; H <= 180; H++) {
        for(int VW = 0; VW < 40; VW++) {
            if(VW == 0 || VW == 39)
                buffer[bi][VW] = -1;
            else
                buffer[bi][VW] = FastestPolar(H, VW);

            if(VW > 0 && H > 0) {
                int p0 = buffer[!bi][VW-1], p1 = buffer[bi][VW-1];
                int p2 = buffer[!bi][VW],   p3 = buffer[bi][VW];

                float H0 = H-1, H_5 = H - .5;
                float VW0 = VW-1, VW_5 = VW - .5;
                Point a(H_5, VW0), b(H0, VW_5), c(H, VW_5), d(H_5, VW);

                /*        2 --- d --- 3
                          |           |
                          b           c
                          |           |
                          0 --- a --- 1         */
                
                for(int p = 0; p < (int)Polars.size(); p++) {
                    if(p == p0) {
                        if(p == p1) {
                            if(p == p2) {
                                if(p != p3)
                                    segments[p].push_back(Segment(c, d));
                            } else {
                                if(p == p3)
                                    segments[p].push_back(Segment(d, b));
                                else
                                    segments[p].push_back(Segment(c, b));
                            }
                        } else {
                            if(p == p2) {
                                if(p == p3)
                                    segments[p].push_back(Segment(a, c));
                                else
                                    segments[p].push_back(Segment(a, d));
                            } else {
                                segments[p].push_back(Segment(a, b));
                                if(p == p3)
                                    segments[p].push_back(Segment(d, c));
                            }
                        }
                    } else {
                        if(p == p1) {
                            if(p == p2) {
                                segments[p].push_back(Segment(b, a));
                                if(p != p3)
                                    segments[p].push_back(Segment(c, d));
                            } else {
                                if(p == p3)
                                    segments[p].push_back(Segment(d, a));
                                else
                                    segments[p].push_back(Segment(c, a));
                            }
                        } else {
                            if(p == p2) {
                                if(p == p3)
                                    segments[p].push_back(Segment(b, c));
                                else
                                    segments[p].push_back(Segment(b, d));
                            } else {
                                if(p == p3)
                                    segments[p].push_back(Segment(d, c));
                            }
                        }
                    }
                }
            }
        }
        bi = !bi;
    }    
    
    for(unsigned int p = 0; p < Polars.size(); p++) {
        /* insert wrapping segments for 0 and 180 */
        std::list<Segment> wrapped_segments;
        for(std::list<Segment>::iterator it = segments[p].begin();
            it != segments[p].end(); it++) {
            if(it->p[1].x == 0) {
                // find next segment starting at 0 with greater y
                wrapped_segments.push_back
                    (Segment(it->p[1], FindNextSegmentPoint(segments[p], it->p[1], false)));
            } else if(it->p[1].x == 180) {
                // find next segment starting at 180 with lesser y
                wrapped_segments.push_back
                    (Segment(it->p[1], FindNextSegmentPoint(segments[p], it->p[1], true)));
            }
        }

        segments[p].splice(segments[p].end(), wrapped_segments);
        Polars[p].CrossOverRegion = PolygonRegion(segments[p]);
    }
}
