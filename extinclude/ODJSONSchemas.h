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
#ifndef ODJSONSCHEMAS_H
#define ODJSONSCHEMAS_H
#if 1
// split schema to enable MSVC to include large literal text
static json jSchema_defs =
#include "ODSchema_defs.json"
;
static json jSchema_scheme =
#include "ODSchema_scheme.json"
;
static json jSchema;
#else
// original large litteral text, but MSVC has issues with this
static json jSchema =
#include "ODSchema.json"
;
#endif

#endif // ODJSONSCHEMAS_H
