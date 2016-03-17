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
            const char *str = e->Attribute("CrossOverContours");
            if(str) {
                wxFile contours(str);
                if(contours.IsOpened()) {
                    int size = contours.Length();
                    char *data = new char[size+1];
                    if(contours.Read(data, size) == size) {
                        data[size] = 0; // null terminate
                        polar.CrossOverRegion = PolygonRegion(data);
                    }
                    delete [] data;
                }
            }
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
        if(!polar.CrossOverRegion.Empty()) {
            wxString ContoursFileName = polar.FileName + _T(".contours");
            wxFile file;
            if(file.Open(ContoursFileName, wxFile::write)) {
                e->SetAttribute("CrossOverContours", ContoursFileName.mb_str());
                file.Write(polar.CrossOverRegion.toString());
            }
        }

//        e->SetAttribute("optimize_tacking", polar.optimize_tacking);

        root->LinkEndChild(e);
    }

    if(!doc.SaveFile(filename.mb_str()))
        return _("Failed saving file: ") + filename;

    return wxString();
}

int Boat::TrySwitchPolar(int curpolar, double VW, double H, double Swell)
{
    // are we still valid?
    if(Polars[curpolar].InsideCrossOverContour(H, VW))
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
    const float maxVW = 40;
    if(VW == 0 || VW == maxVW)
        return -1;

    int maxi = -1;
    float max = 0;
    for(unsigned int i=0; i<Polars.size(); i++) {
        float speed = Polars[i].Speed(H, VW);
        if(speed > max) {
            max = speed;
            maxi = i;
        }
    }
    return maxi;
}

void Boat::GenerateCrossOverChart()
{
    const int maxVW = 40;
    const int stepi = 8;
    float step = 1.0f/stepi;

    int buffer[2][maxVW*stepi+1];
    int bi = 0;
    std::list<Segment> segments[Polars.size()];
    for(float H = 0; H <= 180; H+=step) {
        for(int VWi = 0; VWi <= maxVW*stepi; VWi++) {
            float VW = VWi * step;
            buffer[bi][VWi] = FastestPolar(H, VW);

            if(VWi > 0 && H > 0) {
                int q[4] = {buffer[!bi][VWi-1], buffer[bi][VWi-1],
                            buffer[!bi][VWi],   buffer[bi][VWi]};
                float VW = VWi * step;
                GenerateSegments(H, VW, step, q, segments);
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
        Polars[p].CrossOverRegion.Simplify(1e-1);
    }
}

Point Boat::Interp(const Point &p0, const Point &p1,
                   int q, int q0, int q1)
{
    Point p01((p0.x + p1.x)/2, (p0.y + p1.y)/2);
    if(fabsf(p0.x - p1.x) < 1e-2 && fabsf(p0.y - p1.y) < 1e-2)
        return p01;
    
    if((q == q0 || q == q1) && (q0 != q1)) {
        int q01 = FastestPolar(p01.x, p01.y);
        if((q == q01) == (q == q0))
            return Interp(p01, p1, q, q01, q1);
        else
            return Interp(p0, p01, q, q0, q01);
    }

    return p0;
}

void Boat::NewSegment(int q, Point &p0, Point &p1,
                      std::list<Segment> segments[])
{
//    if(fabsf(p0.x - p1.x) < 1e-1 && fabsf(p0.y - p1.y) < 1e-1)
        segments[q].push_back(Segment(p0, p1));
#if 0
    else {
        Point p01((p0.x + p1.x)/2, (p0.y + p1.y)/2);
        float dx = p1.x - p0.x, dy = p1.y - p0.y;
        Point pa(p01.x + dy / 2, p01.y - dx / 2);
        Point pb(p01.x - dy / 2, p01.y + dx / 2);
        int qa = FastestPolar(pa.x, pa.y);
        int qb = FastestPolar(pb.x, pb.y);
        Point p = Interp(pa, pb, q, qa, qb);
        if(p == pa)
            segments[q].push_back(Segment(p0, p1));
        else {
            NewSegment(q, p0, p, segments);
            NewSegment(q, p, p1, segments);
        }
    }
#endif
}

Point Intersection(Point a0, Point a1, Point b0, Point b1)
{
    double ax = a1.x - a0.x, ay = a1.y - a0.y;
    double bx = b0.x - b1.x, by = b0.y - b1.y;
    double cx = a0.x - b0.x, cy = a0.y - b0.y;

    double denom = ay * bx - ax * by;
    double recip = 1 / denom;
    double na = (by * cx - bx * cy) * recip;

    return Point(a0.x + ax*na, a0.y + ay * na);
}
#define SIMPLE
void Boat::GenerateSegments(float H, float VW, float step, int q[4],
                            std::list<Segment> segments[])
{
    float H0 = H-step;
    float VW0 = VW-step;

    Point p0(H0, VW0), p1(H, VW0), p2(H0, VW), p3(H, VW);

    /*        2 --- e --- 3
              |           |
              b     c     d
              |           |
              0 --- a --- 1         */

  
//    Point c((H0+H)/2, (VW0+VW)/2);

    Point a[Polars.size()][4];
    for(int p = 0; p < (int)Polars.size(); p++) {
        a[p][0] = Interp(p0, p1, p, q[0], q[1]);
        a[p][1] = Interp(p0, p2, p, q[0], q[2]);
        a[p][2] = Interp(p1, p3, p, q[1], q[3]);
        a[p][3] = Interp(p2, p3, p, q[2], q[3]);
        //Point c = Intersection(a[p][0], a[p][3], a[p][1], a[p][2]);        
        if(p == q[0]) {
            if(p == q[1]) {
                if(p == q[2]) {
                    if(p != q[3]) {
#ifdef SIMPLE
                           NewSegment(p, a[p][2], a[p][3], segments);
#else
                           NewSegment(p, a[p][2], c, segments);
                           NewSegment(p, c, a[p][3], segments);
#endif
                    }
                } else {
                    if(p == q[3]) {
#ifdef SIMPLE
                        NewSegment(p, a[p][3], a[p][1], segments);
#else
                        NewSegment(p, a[p][3], c, segments);
                        NewSegment(p, c, a[p][1], segments);
#endif
                    } else
                        NewSegment(p, a[p][2], a[p][1], segments);
                }
            } else {
                if(p == q[2]) {
                    if(p == q[3]) {
#ifdef SIMPLE
                        NewSegment(p, a[p][0], a[p][2], segments);
#else
                        NewSegment(p, a[p][0], c, segments);
                        NewSegment(p, c, a[p][2], segments);
#endif
                    } else
                        NewSegment(p, a[p][0], a[p][3], segments);
                } else {
#ifdef SIMPLE
                    NewSegment(p, a[p][0], a[p][1], segments);
#else
                    NewSegment(p, a[p][0], c, segments);
                    NewSegment(p, c, a[p][1], segments);
#endif
                    if(p == q[3]) {
#ifdef SIMPLE
                         NewSegment(p, a[p][3], a[p][2], segments);
#else
                        NewSegment(p, a[p][3], c, segments);
                        NewSegment(p, c, a[p][2], segments);
#endif
                    }
                }
            }
        } else {
            if(p == q[1]) {
                if(p == q[2]) {
#ifdef SIMPLE
                    NewSegment(p, a[p][1], a[p][0], segments);
#else
                    NewSegment(p, a[p][1], c, segments);
                    NewSegment(p, c, a[p][0], segments);
#endif
                    if(p != q[3]) {
#ifdef SIMPLE
                        NewSegment(p, a[p][2], a[p][3], segments);
#else
                        NewSegment(p, a[p][2], c, segments);
                        NewSegment(p, c, a[p][3], segments);
#endif
                    }
                } else {
                    if(p == q[3])
                        NewSegment(p, a[p][3], a[p][0], segments);
                    else {
#ifdef SIMPLE
                        NewSegment(p, a[p][2], a[p][0], segments);
#else
                        NewSegment(p, a[p][2], c, segments);
                        NewSegment(p, c, a[p][0], segments);
#endif
                    }
                }
            } else {
                if(p == q[2]) {
                    if(p == q[3])
                        NewSegment(p, a[p][1], a[p][2], segments);
                    else {
#ifdef SIMPLE
                         NewSegment(p, a[p][1], a[p][3], segments);
#else
                        NewSegment(p, a[p][1], c, segments);
                        NewSegment(p, c, a[p][3], segments);
#endif
                    }
                } else {
                    if(p == q[3]) {
#ifdef SIMPLE
                         NewSegment(p, a[p][3], a[p][2], segments);
#else
                        NewSegment(p, a[p][3], c, segments);
                        NewSegment(p, c, a[p][2], segments);
#endif                        
                    }
                }
            }
        }
    }
}
