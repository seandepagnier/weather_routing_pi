/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2023 by David S. Register                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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
 **************************************************************************/

//https://www.cruisersforum.com/forums/f134/weather-routing-100060-476.html#post3806197

#include "navobj_util.h"
#include <iostream>

// RFC4122 version 4 compliant random UUIDs generator.
wxString GetUUID(void) {
  wxString str;
  struct {
    int time_low;
    int time_mid;
    int time_hi_and_version;
    int clock_seq_hi_and_rsv;
    int clock_seq_low;
    int node_hi;
    int node_low;
  } uuid;

  uuid.time_low = GetRandomNumber(
      0, 2147483647);  // FIXME: the max should be set to something like
                       // MAXINT32, but it doesn't compile un gcc...
  uuid.time_mid = GetRandomNumber(0, 65535);
  uuid.time_hi_and_version = GetRandomNumber(0, 65535);
  uuid.clock_seq_hi_and_rsv = GetRandomNumber(0, 255);
  uuid.clock_seq_low = GetRandomNumber(0, 255);
  uuid.node_hi = GetRandomNumber(0, 65535);
  uuid.node_low = GetRandomNumber(0, 2147483647);

  /* Set the two most significant bits (bits 6 and 7) of the
   * clock_seq_hi_and_rsv to zero and one, respectively. */
  uuid.clock_seq_hi_and_rsv = (uuid.clock_seq_hi_and_rsv & 0x3F) | 0x80;

  /* Set the four most significant bits (bits 12 through 15) of the
   * time_hi_and_version field to 4 */
  uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0fff) | 0x4000;

  str.Printf(_T("%08x-%04x-%04x-%02x%02x-%04x%08x"), uuid.time_low,
             uuid.time_mid, uuid.time_hi_and_version, uuid.clock_seq_hi_and_rsv,
             uuid.clock_seq_low, uuid.node_hi, uuid.node_low);

  return str;
}

int GetRandomNumber(int range_min, int range_max) {
  long u = (long)wxRound(
      ((double)rand() / ((double)(RAND_MAX) + 1) * (range_max - range_min)) +
      range_min);
  return (int)u;
}


static bool GPXCreateWpt(pugi::xml_node node, SimpleRoutePoint *pr) {
    wxString s;
    pugi::xml_node child;
    pugi::xml_attribute attr;

    s.Printf(_T("%.9f"), pr->m_lat);
    node.append_attribute("lat") = s.mb_str();
    s.Printf(_T("%.9f"), pr->m_lon);
    node.append_attribute("lon") = s.mb_str();

    if (!pr->m_MarkName.IsEmpty() ) {
        wxCharBuffer buffer = pr->m_MarkName.ToUTF8();
        if (buffer.data()) {
            child = node.append_child("name");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }

    if (!pr->m_MarkDescription.IsEmpty() ) {
        wxCharBuffer buffer = pr->m_MarkDescription.ToUTF8();
        if (buffer.data()) {
            child = node.append_child("desc");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }


    pugi::xml_node child_ext = node.append_child("extensions");

    child = child_ext.append_child("opencpn:guid");
    child.append_child(pugi::node_pcdata).set_value(pr->m_GUID.mb_str());

    child = child_ext.append_child("opencpn:rte_properties");
    wxString sps;
    sps.Printf("%.6f", pr->m_seg_vmg);
    child.append_attribute("planned_speed") = sps.mbc_str();

    if (pr->etd.IsValid()) {
        pugi::xml_attribute use = child.append_attribute("etd");
        use.set_value(pr->etd.FormatISOCombined().mb_str());
    }

    //<opencpn:rte_properties planned_speed="44.0" etd="2023-08-18T00:00:00" />

/*
    if (ext_name == _T ( "opencpn:rte_properties" )) {
        for (pugi::xml_attribute attr = ext_child.first_attribute(); attr;
             attr = attr.next_attribute()) {
            if (!strcmp(attr.name(), "planned_speed"))
              plan_speed = attr.as_double();
            else if (!strcmp(attr.name(), "etd"))
              etd = attr.as_string();
*/
            //   if (ext_name == _T ( "opencpn:rte_properties" )) {
 //       for (pugi::xml_attribute attr = ext_child.first_attribute(); attr;
 //            attr = attr.next_attribute()) {
 //           if (!strcmp(attr.name(), "planned_speed"))

              /*
    // Hyperlinks
    if (flags & OUT_HYPERLINKS) {
        HyperlinkList *linklist = pr->m_HyperlinkList;
        if (linklist && linklist->GetCount()) {
            wxHyperlinkListNode *linknode = linklist->GetFirst();
            while (linknode) {
                Hyperlink *link = linknode->GetData();

                pugi::xml_node child_link = node.append_child("link");
                ;
                wxCharBuffer buffer = link->Link.ToUTF8();
                if (buffer.data()) child_link.append_attribute("href") = buffer.data();

                buffer = link->DescrText.ToUTF8();
                if (buffer.data()) {
                    child = child_link.append_child("text");
                    child.append_child(pugi::node_pcdata).set_value(buffer.data());
                }

                buffer = link->LType.ToUTF8();
                if (buffer.data() && strlen(buffer.data()) > 0) {
                    child = child_link.append_child("type");
                    child.append_child(pugi::node_pcdata).set_value(buffer.data());
                }

                linknode = linknode->GetNext();
            }
        }
    }
*/

/*
    if (flags & OUT_SYM_FORCE) {
        child = node.append_child("sym");
        if (!pr->GetIconName().IsEmpty()) {
            child.append_child(pugi::node_pcdata)
                    .set_value(pr->GetIconName().mb_str());
        } else {
            child.append_child("empty");
        }
    }

    if (flags & OUT_TYPE) {
        child = node.append_child("type");
        child.append_child(pugi::node_pcdata).set_value("WPT");
    }
*/

/*
    if ((flags & OUT_GUID) || (flags & OUT_VIZ) || (flags & OUT_VIZ_NAME) ||
        (flags & OUT_SHARED) || (flags & OUT_AUTO_NAME) ||
        (flags & OUT_EXTENSION) || (flags & OUT_TIDE_STATION) ||
        (flags & OUT_RTE_PROPERTIES)) {
        pugi::xml_node child_ext = node.append_child("extensions");

        if (!pr->m_GUID.IsEmpty() && (flags & OUT_GUID)) {
            child = child_ext.append_child("opencpn:guid");
            child.append_child(pugi::node_pcdata).set_value(pr->m_GUID.mb_str());
        }

        if ((flags & OUT_VIZ) && !pr->m_bIsVisible) {
            child = child_ext.append_child("opencpn:viz");
            child.append_child(pugi::node_pcdata).set_value("0");
        }

        if ((flags & OUT_VIZ_NAME) && pr->m_bShowName) {
            child = child_ext.append_child("opencpn:viz_name");
            child.append_child(pugi::node_pcdata).set_value("1");
        }

        if ((flags & OUT_AUTO_NAME) && pr->m_bDynamicName) {
            child = child_ext.append_child("opencpn:auto_name");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
        if ((flags & OUT_SHARED) && pr->IsShared()) {
            child = child_ext.append_child("opencpn:shared");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
        if (flags & OUT_ARRIVAL_RADIUS) {
            child = child_ext.append_child("opencpn:arrival_radius");
            s.Printf(_T("%.3f"), pr->GetWaypointArrivalRadius());
            child.append_child(pugi::node_pcdata).set_value(s.mbc_str());
        }
        if (flags & OUT_WAYPOINT_RANGE_RINGS) {
            child = child_ext.append_child("opencpn:waypoint_range_rings");
            pugi::xml_attribute viz = child.append_attribute("visible");
            viz.set_value(pr->m_bShowWaypointRangeRings);
            pugi::xml_attribute number = child.append_attribute("number");
            number.set_value(pr->m_iWaypointRangeRingsNumber);
            pugi::xml_attribute step = child.append_attribute("step");
            step.set_value(pr->m_fWaypointRangeRingsStep);
            pugi::xml_attribute units = child.append_attribute("units");
            units.set_value(pr->m_iWaypointRangeRingsStepUnits);
            pugi::xml_attribute colour = child.append_attribute("colour");
            colour.set_value(
                    pr->m_wxcWaypointRangeRingsColour.GetAsString(wxC2S_HTML_SYNTAX)
                            .utf8_str());
        }
        if (flags & OUT_WAYPOINT_SCALE) {
            child = child_ext.append_child("opencpn:scale_min_max");
            pugi::xml_attribute use = child.append_attribute("UseScale");
            use.set_value(pr->GetUseSca());
            pugi::xml_attribute sca = child.append_attribute("ScaleMin");
            sca.set_value(pr->GetScaMin());
            pugi::xml_attribute max = child.append_attribute("ScaleMax");
            max.set_value(pr->GetScaMax());
        }
        if ((flags & OUT_TIDE_STATION) && !pr->m_TideStation.IsEmpty()) {
            child = child_ext.append_child("opencpn:tidestation");
            child.append_child(pugi::node_pcdata)
                    .set_value(pr->m_TideStation.mb_str());
        }
        if ((flags & OUT_RTE_PROPERTIES) &&
            (pr->GetPlannedSpeed() > 0.0001 || pr->m_manual_etd)) {
            child = child_ext.append_child("opencpn:rte_properties");
            if (pr->GetPlannedSpeed() > 0.0001) {
                pugi::xml_attribute use = child.append_attribute("planned_speed");
                use.set_value(
                        wxString::Format(_T("%.1lf"), pr->GetPlannedSpeed()).mb_str());
            }
            if (pr->m_manual_etd) {
                pugi::xml_attribute use = child.append_attribute("etd");
                use.set_value(pr->GetManualETD().FormatISOCombined().mb_str());
            }
        }
    }
*/
    return true;
}

static bool GPXCreateRoute(pugi::xml_node node, const SimpleRoute &Route) {
    pugi::xml_node child;

    if (Route.m_RouteNameString.Len()) {
        wxCharBuffer buffer = Route.m_RouteNameString.ToUTF8();
        if (buffer.data()) {
            child = node.append_child("name");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }

    if (Route.m_RouteDescription.Len()) {
        wxCharBuffer buffer = Route.m_RouteDescription.ToUTF8();
        if (buffer.data()) {
            child = node.append_child("desc");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }

#if 0
    // Hyperlinks
    HyperlinkList *linklist = pRoute->m_HyperlinkList;
    if (linklist && linklist->GetCount()) {
        wxHyperlinkListNode *linknode = linklist->GetFirst();
        while (linknode) {
            Hyperlink *link = linknode->GetData();

            pugi::xml_node child_link = node.append_child("link");
            wxCharBuffer buffer = link->Link.ToUTF8();
            if (buffer.data()) child_link.append_attribute("href") = buffer.data();

            buffer = link->DescrText.ToUTF8();
            if (buffer.data()) {
                child = child_link.append_child("text");
                child.append_child(pugi::node_pcdata).set_value(buffer.data());
            }

            buffer = link->LType.ToUTF8();
            if (buffer.data() && strlen(buffer.data()) > 0) {
                child = child_link.append_child("type");
                child.append_child(pugi::node_pcdata).set_value(buffer.data());
            }

            linknode = linknode->GetNext();
        }
    }
#endif
    pugi::xml_node child_ext = node.append_child("extensions");

    child = child_ext.append_child("opencpn:guid");
    child.append_child(pugi::node_pcdata).set_value(Route.m_GUID.mb_str());

    if (Route.m_RouteStartString.Len()) {
        wxCharBuffer buffer = Route.m_RouteStartString.ToUTF8();
        if (buffer.data()) {
            child = child_ext.append_child("opencpn:start");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }

    if (Route.m_RouteEndString.Len()) {
        wxCharBuffer buffer = Route.m_RouteEndString.ToUTF8();
        if (buffer.data()) {
            child = child_ext.append_child("opencpn:end");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }


    if (0/*Route.m_PlannedSpeed != ROUTE_DEFAULT_SPEED*/) {
        child = child_ext.append_child("opencpn:planned_speed");
        wxString s;
        s.Printf(_T("%.8f"), Route.m_PlannedSpeed);
        child.append_child(pugi::node_pcdata).set_value(s.mb_str());
    }

    if (Route.m_PlannedDeparture.IsValid()) {
        child = child_ext.append_child("opencpn:planned_departure");
        wxString t = Route.m_PlannedDeparture.FormatISODate()
                .Append(_T("T"))
                .Append(Route.m_PlannedDeparture.FormatISOTime())
                .Append(_T("Z"));
        child.append_child(pugi::node_pcdata).set_value(t.mb_str());
    }

//    child = child_ext.append_child("opencpn:time_display");
//    child.append_child(pugi::node_pcdata)
//            .set_value(pRoute->m_TimeDisplayFormat.mb_str());

/*
    if (pRoute->m_width != WIDTH_UNDEFINED ||
        pRoute->m_style != wxPENSTYLE_INVALID) {
        child = child_ext.append_child("opencpn:style");

        if (pRoute->m_width != WIDTH_UNDEFINED)
            child.append_attribute("width") = pRoute->m_width;
        if (pRoute->m_style != wxPENSTYLE_INVALID)
            child.append_attribute("style") = pRoute->m_style;
    }
*/
/*
 *  pugi::xml_node gpxx_ext = child_ext.append_child("gpxx:RouteExtension");
    child = gpxx_ext.append_child("gpxx:IsAutoNamed");
    child.append_child(pugi::node_pcdata).set_value("false");
*/

/*
    if (pRoute->m_Colour != wxEmptyString) {
        child = gpxx_ext.append_child("gpxx:DisplayColor");
        child.append_child(pugi::node_pcdata).set_value(pRoute->m_Colour.mb_str());
    }
*/
    for (size_t i=0 ; i < Route.m_vec_points.size() ; i++) {
        GPXCreateWpt(node.append_child("rtept"),
                     Route.m_vec_points[i]);
    }
/*
    RoutePointList *pRoutePointList = pRoute->pRoutePointList;
    wxRoutePointListNode *node2 = pRoutePointList->GetFirst();
    RoutePoint *prp;

    while (node2) {
        prp = node2->GetData();

        GPXCreateWpt(node.append_child("rtept"), prp, OPT_ROUTEPT);

        node2 = node2->GetNext();
    }
*/
    return true;
}


SimpleRoutePoint::SimpleRoutePoint(double lat, double lon, const wxString &icon_ident,
                 const wxString &name, const wxString &sGUID) {
    m_lat = lat;
    m_lon = lon;
    m_MarkName = name;
    m_GUID = sGUID;
}


SimpleNavObjectXML::SimpleNavObjectXML()
        : pugi::xml_document() {
   if (!strlen(first_child().name())) {
            pugi::xml_node gpx_root = append_child("gpx");
            gpx_root.append_attribute("version") = "1.1";
            gpx_root.append_attribute("creator") = "OpenCPN";
            gpx_root.append_attribute("xmlns:xsi") =
                "http://www.w3.org/2001/XMLSchema-instance";
            gpx_root.append_attribute("xmlns") = "http://www.topografix.com/GPX/1/1";
            gpx_root.append_attribute("xmlns:gpxx") =
                "http://www.garmin.com/xmlschemas/GpxExtensions/v3";
            gpx_root.append_attribute("xsi:schemaLocation") =
                "http://www.topografix.com/GPX/1/1 "
                "http://www.topografix.com/GPX/1/1/gpx.xsd "
                "http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
                "http://www8.garmin.com/xmlschemas/GpxExtensionsv3.xsd";
            gpx_root.append_attribute("xmlns:opencpn") = "http://www.opencpn.org";
    }
}

SimpleNavObjectXML::~SimpleNavObjectXML() {}

bool SimpleNavObjectXML::CreateNavObjGPXRoute(const SimpleRoute &Route) {
    // Route

    pugi::xml_node doc = root();
    pugi::xml_node gpx = doc.first_child();
    pugi::xml_node new_node = gpx.append_child("rte");

    GPXCreateRoute(new_node, Route);

    //save(std::cout);
    return true;
}
