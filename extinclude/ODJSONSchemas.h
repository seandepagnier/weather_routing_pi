/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   $EMAIL$   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

/* The header file contains the schema for the JSON messages that are being used. However it relies on Draf7 of the JSON schema 
 * which is not fully implemented yet in a C++ library. The code is defined out that will use this as the implementation changes the 
 * ABI code of the executable and is then not compatible with OpenCPN. There is basic checking for schema compliance but 
 * it is not robust and it is possible to crash OpenCPN if the values are not valid.
 * 
 */

static json jSchema = R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "OD Schemas",
    "definitions": {
        "schemahead": {
            "description": "Standalone for simple queries or required for complex internactions as a part of the message",
            "type": "object",
            "properties": {
                "Msg": {
                    "description": "Message Name",
                    "enum": ["Version", "GetAPIAddresses", "CreateBoundaryPoint", "CreateBoundary", "CreateTextPoint", "DeleteTextPoint", "DeleteBoundaryPoint", "DeleteBoundary"]
                },
                "Type": {
                    "description": "Type of Message",
                    "enum": ["Request","Response"]
                },
                "Source": {
                    "description": "Source of message",
                    "type": "string"
                },
                "MsgId": {
                    "description": "Message Name",
                    "type": "string"
                }
            },
            "required": ["Msg", "Type", "Source", "MsgId"],
            "addtionalProperties": false
        },
        "ODPoint": {
            "description": "Generic OD Point",
            "type": "object",
            "properties" : {
                "Name": {
                    "description": "Boundary Point Name",
                    "type": "string"
                },
                "Lat": {
                    "description": "Latitude of Boundary Point",
                    "type": "number",
                    "minimum": -90,
                    "maximum": 90
                },
                "Lon": {
                    "description": "Longitude of Boundary Point",
                    "type": "number",
                    "minimum": -180,
                    "maximum": 180
                },
                "IconName": {
                    "description": "Boundary Point Icon Name",
                    "type": "string"
                },
                "Visible": {
                    "description": "Visibility of Boundary Point",
                    "type": "boolean",
                    "default": false
                },
                "RingsVisible": {
                    "description": "Visibility of Boundary Point Range Rings",
                    "type": "boolean",
                    "default": false
                },
                "RingsNumber": {
                    "description": "Number of Boundary Point Rings",
                    "type": "number",
                    "minimum": 0,
                    "maximum": 10
                },
                "RingsSteps": {
                    "description": "Steps between Boundary Point Rings",
                    "type": "number",
                    "minimum": 0.0
                },
                "RingUnits": {
                    "description": "Boundary Point Range Ring Step Units",
                    "enum": [0,1,2,3,4,5,6,7,8,9,10]
                },
                "RingsColour": {
                    "description": "Boundary Point Range Rings Colour",
                    "type": "string"
                },
                "HyperLinks": {
                    "description": "Array of hyperlinks to local or internet documents",
                    "type": "array",
                    "Items": {
                        "allOf" : [
                            {
                                "LinkDescription": {
                                    "description": "Text description of hyper link",
                                    "type": "string"
                                }
                            },
                            {
                                "LinkURL": {
                                    "description": "Actual Hyperlink",
                                    "type": "string"
                                }
                            }
                        ]
                    }
                }
            },
            "required": [
                "Lat",
                "Lon"
            ]
        },
        "boundarypoint": {
            "description": "Used to create a boundary point",
            "type": "object",
            "properties": {
                "items": {
                    "$ref": "#/definitions/ODPoint"
                },
                "BoundaryPointType": {
                    "description": "Boundary Point Type",
                    "enum": ["Exclusion", "Inclusion", "Neither"]
                }
            },
            "required": [
                "BoundaryPointType"
            ]
        },
        "textpoint": {
            "description": "Used to create a text point",
            "type": "object",
            "properties": {
                "items": {
                    "$ref": "#/definitions/ODPoint"
                },
                "Temporary": {
                    "description": "Text point life, false - exists over restart, true - non persistent",
                    "type": "boolean",
                    "default": false
                },
                "GUID": {
                    "description": "GUID of Text Point",
                    "type": "string",
                    "optional": true
                },
                "TextToDisplay": {
                    "description": "Text Point Text",
                    "type": "string"
                },
                "TextPosition": {
                    "description": "Text Point Text Position",
                    "enum": [0,1,2,3,4,5,6]
                },
                "TextColour": {
                    "description": "Text Point Text Colour",
                    "type": "string"
                },
                "BackgroundColour": {
                    "description": "Text Point Text BackgroundColour",
                    "type": "string"
                },
                "BackgroundTransparancy": {
                    "description": "Text Point Text BackgroundTransparancy",
                    "type": "number",
                    "minimum": 0,
                    "maximum": 255
                },
                "TextFont": {
                    "description": "Text Point Font",
                    "type": "string"
                },
                "TextPointDisplayTextWhen": {
                    "description": "Text Point When To Display",
                    "type": "string"
                }
            }
        },
        "ODObject": {
            "description": "Used to delete an object",
            "type": "object",
            "properties": {
                "GUID": {
                    "description": "GUID of object",
                    "type": "string"
                }
            },
            "required": [
                "GUID"
            ]
        },
        "PointIcon": {
            "description": "Used to delete add an point icon",
            "type": "object",
            "properties": {
                "PointIcon": {
                    "description": "Point Icon in Base64",
                    "type": "string",
                    "optional": false
                },
                "PointIconName": {
                    "description": "Point Icon name",
                    "type": "string",
                    "optional": false
                },
                "PointIconDescription": {
                    "description": "Point Icon Description",
                    "type": "string",
                    "optional": false
                }
            },
            "required": [
                "PointIcon",
                "PointIconName",
                "PointIconDescription"
            ]
        },
        "boundary": {
            "description": "Used to create or modify a boundary",
            "type": "object",
            "properties": {
                "BoundaryName": {
                    "description": "Boundary Name",
                    "type": "string"
                },
                "BoundaryType": {
                    "description": "Boundary Type",
                    "enum": ["Exclusion","Inclusion","Neither"]
                },
                "Active": {
                    "description": "Is Boundary Active",
                    "type": "boolean",
                    "boolean": true
                },
                "Visible": {
                    "description": "Visibility of Boundary",
                    "type": "boolean",
                    "default": true
                },
                "Temporary": {
                    "description": "Boundary life, false - exists over restart, true - non persistent",
                    "type": "boolean",
                    "default": false
                },
                "BoundaryPoints": {
                    "description": "Array of boundary points needed to define a boundary",
                    "type": "array",
                    "items": {
                    "$ref": "#/definitions/boundarypoint"
                    },
                    "minItems": 3,
                    "additionalItems": false
                }
            },
            "required": [
                "BoundaryPoints", 
                "BoundaryType"
            ]
        }
    },
    "type": "object",
    "allOf": [
        {"$ref": "#/definitions/schemahead"},
        {
            "if": { "properties": {"Msg": {"const": "CreateBoundary"}}
            },
            "then": {
                "$ref": "#/definitions/boundary"
            } 
        },
        {
            "if": { "properties": {"Msg": {"const": "CreateBoundaryPoint"}}
            },
            "then": {
                "$ref": "#/definitions/boundarypoint"
            } 
        },
        {
            "if": { "properties": {"Msg": {"const": "CreateTextPoint"}}
            },
            "then": {
                "$ref": "#/definitions/textpoint"
            } 
        },
        {
            "if": { "properties": {"Msg": {"const": "DeleteTextPoint"}}
            },
            "then": {
                "$ref": "#/definitions/ODObject"
            } 
        },
        {
            "if": { "properties": {"Msg": {"const": "DeleteBoundaryPoint"}}
            },
            "then": {
                "$ref": "#/definitions/ODObject"
            } 
        },
        {
            "if": { "properties": {"Msg": {"const": "DeleteBoundary"}}
            },
            "then": {
                "$ref": "#/definitions/ODObject"
            } 
        }
        
    ]
}
)"_json;
    
