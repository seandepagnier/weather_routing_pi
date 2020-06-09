##---------------------------------------------------------------------------
## Author:      Sean d'Epagnier
##***************************************************************************
## *   Copyright (C) 2010 by Sean d'Epagnier                              *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,                                       *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                       *
#-----------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# ---------------------------------------------------------------------------
# CMake-CI Number: 0.0.009
#    - 28/05/2020 Work on Local Build
#    - 28/05/2020 Move CmakeLists code into PluginSetup.cmake
#    - 30/05/2020 Minor Update for Arch
#    - 09/06/2020 Fix gtk3 and new build naming  
# ---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##

##
##----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
set(VERBOSE_NAME "WeatherRouting")
set(COMMON_NAME "WeatherRouting")  # Search "getcommonname"in <plugin_pi>.cpp
set(TITLE_NAME "weatherrouting")
set(PACKAGE_CONTACT "Sean d'Epagnier")
set(PACKAGE "weather_routing_pi")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "13")
set(VERSION_PATCH "7")
set(VERSION_TWEAK "3")
set(VERSION_DATE "06/09/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
set(GIT_USER "rgleason")
set(GIT_REPOSITORY_NAME "weather_routing_pi")
## Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")  #Standard Repos
set(BETA "beta")  #Standard Repos
set(ALPHA "alpha") #Standard Repos

#set(CLOUDSMITH_USER "sean-depagnier")
#set(CLOUDSMITH_USER "jon-gough")
#set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
#set(CLOUDSMITH_BASE_REPOSITORY "weather_routing")  #Jon Gough's customization
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Ricks's customization
set(CLOUDSMITH_BASE_REPOSITORY "weather-routing")  #without the _pi

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherroute.html")  
set(XML_SUMMARY "Compute optimal routes based on weather and constraints.")
set(XML_DESCRIPTION "WR features include:\n\ optimal routing subject to various constraints based on weather data,\n\ automatic boat polar computation.")

##
## ----- Modify section above to include all the details for your plugin ----- ##
##

##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)
option(USE_GL "Enable OpenGL support" ON)  #WxRoute requires on
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(${PACKAGE})

#========================================================
INCLUDE("cmake/PluginSetup.cmake")
#========================================================

##
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF) #Wass commented out
##
## ----- Modify section above if there are special requirements for the plugin ----- ##
##

##
## ----- do not change next section - needed to configure build process ----- ##
##

#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
# ----- Change below to match project requirements for source, headers, etc. ----- ##
#

add_definitions(-DUSE_S57)

# From master branch - is this workiing? necessary?
IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "${CMLOC}SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)

#======== Below from cmakelists.txt  weather_routing_pi
# ----Remember to Add your project Set(SRCS and set(HDRS  etc..

SET(SRCS
            src/weather_routing_pi.cpp
            src/WeatherRouting.cpp
            src/ConfigurationDialog.cpp
            src/SettingsDialog.cpp
            src/PlotDialog.cpp
            src/BoatDialog.cpp
            src/StatisticsDialog.cpp
            src/ReportDialog.cpp
            src/FilterRoutesDialog.cpp
            src/ConfigurationBatchDialog.cpp
            src/EditPolarDialog.cpp
            src/AboutDialog.cpp
            src/Polar.cpp
            src/Boat.cpp
            src/RouteMap.cpp
            src/RouteMapOverlay.cpp
            src/Utilities.cpp
            src/LineBufferOverlay.cpp
            src/PolygonRegion.cpp
            src/WeatherRoutingUI.cpp
            src/icons.cpp
            src/zuFile.cpp
            src/georef.c
            src/GribRecord.cpp
)

# Added and tried by Rick
SET (HDRS
            src/weather_routing_pi.h
            src/WeatherRouting.h
            src/ConfigurationDialog.h
            src/SettingsDialog.h
            src/PlotDialog.h
            src/BoatDialog.h
            src/StatisticsDialog.h
            src/ReportDialog.h
            src/FilterRoutesDialog.h
            src/ConfigurationBatchDialog.h
            src/EditPolarDialog.h
            src/AboutDialog.h
            src/Polar.h
            src/Boat.h
            src/RouteMap.h
            src/RouteMapOverlay.h
            src/Utilities.h
            src/LineBufferOverlay.h
            src/PolygonRegion.h
            src/WeatherRoutingUI.h
            src/icons.h
            src/zuFile.h
            src/georef.h
            src/GribRecord.h
            src/tinyxml/tinyxml.h
            src/tinyxml/tinystr.h
            src/jsoncpp/json/json.h
)

SET(SRC_LIBTESS2
             src/libtess2/bucketalloc.c
             src/libtess2/dict.c
             src/libtess2/geom.c
             src/libtess2/mesh.c
             src/libtess2/priorityq.c
             src/libtess2/sweep.c
             src/libtess2/tess.c
)
  
 SET(SRC_PLUGINGL
             src/plugingl/linmath.h
             src/plugingl/pidc.h
             src/plugingl/qtstylesheet.h
             src/plugingl/TexFont.h
)

# added by rick
 SET (SRC_TINYXML
             src/tinyxml/tinystr.h
			 src/tinyxml/tinyxml.h
)  

# SET  (SRC_ZLIB
#             src/zlib-1.2.3/zconf.h
#             src/zlib-1.2.3/zlib.h
#)

#SET  (SRC_BZIP2
#            src/bzip2/blocksort.c
#			 src/bzip2/bzlib.c
#			 src/bzip2/bzlib_private.h
#			 src/bzip2/compress.c
#			 src/bzip2/crctable.c
#			 src/bzip2/decompress.c
#			 src/bzip2/huffman.c
#			 src/bzip2/randtable.c
#)

# SET   (SRC_JSONCPP
#			 src/jsoncpp/json_reader.cpp
#			 src/jsoncpp/json_value.cpp
#			 src/jsoncpp/json_writer.cpp
#)

# Added and tried by Rick 
# SET (SRC_JSON
#   src/jsoncpp/json/json.h
#   src/jsoncpp/json/assertions.h
#   src/jsoncpp/json/autolink.h
#   src/jsoncpp/json/config.h
#   src/jsoncpp/json/forwards.h
#   src/jsoncpp/json/reader.h
#   src/jsoncpp/json/value.h
#   src/jsoncpp/json/writer.h
#   src/jsoncpp/json_batchallocator.h
#   src/jsoncpp/json_tool.h)  #rick added
         				  
# Testplugin has this line, so just added it
add_definitions(-DPLUGIN_USE_SVG)

##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##================================================================================
SET(SRC ${SRCS} ${SRC_LIBTESS2}  ${SRC_PLUGINGL} ${SRC_TINYXML})
##================================================================================

##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================

#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)

##
## Statement below is required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##
##    SET(EXTSRC  ${EXTSRC}   etc   Can be in an if statement as below
##

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC})
#Add_Library (${PACKAGE_NAME} is used below. Cannot use same name.

# Used in textplugin. Just added.
add_definitions(-DTIXML_USE_STL)

# Every plugin that uses JSON for passing messages should use a validator. The validator  allows the system to ignore non-conformant messages. 

#if(UNIX AND NOT APPLE)
#    if(PI_JSON_SCHEMA_VALIDATOR)
#        # Change OS/ABI to be UNIX - System V to make it run with OCPN
#        set(lib_name lib${PACKAGE_NAME}.so)
#        message(STATUS "${CMLOC}lib-name: ${lib_name}")
#        find_file(elfedit elfedit)
#        if(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#            message(STATUS "${CMLOC}Will ensure OS/ABI is set to 0 to match OCPN")
#            add_custom_command(
#                TARGET ${PACKAGE_NAME}
#                POST_BUILD
#                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                DEPENDS ${PACKAGE_NAME}
#                COMMENT " Running post build action on ${lib_name}."
#                COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) ')
#		
#        else(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#            message(STATUS "${CMLOC}Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
#        endif(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#    endif(PI_JSON_SCHEMA_VALIDATOR)
#endif(UNIX AND NOT APPLE)

IF(NOT UNIX)
    SET(SRC_BZIP	
            src/bzip2/bzlib.c
            src/bzip2/blocksort.c
            src/bzip2/compress.c
            src/bzip2/crctable.c
            src/bzip2/decompress.c
            src/bzip2/huffman.c
            src/bzip2/randtable.c
			src/bzip2/bzlib_private.h
    )
    ADD_LIBRARY(LIB_BZIP_WR STATIC ${SRC_BZIP})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP_WR )
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/bzip2)
#	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
ENDIF(NOT UNIX)

IF(NOT UNIX)
    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
    INCLUDE_DIRECTORIES(src/bzip2)
#	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
ENDIF(NOT UNIX)

IF(WIN32)
  IF(MSVC)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE()
    # MINGW
#    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lwxzlib-2.8")
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lz")
  ENDIF()
ENDIF()

#--------Above from cmakelists.txt  weather_routing_pi------------
#--------REMOVE LN 194 to LN 296 from Testplugin_pi---------------
#------- Change below to match project requirements --------------

# Jon identified these were missing causing json/json.h and other problems.
#----INCLUDE CMAKE #3 & #4 -----
INCLUDE("cmake/PluginJSON.cmake")
INCLUDE("cmake/PluginXML.cmake")
INCLUDE("cmake/PluginGL.cmake")
#
# ----- Do not change - needed to build app ----- ##
#
#INCLUDE CMAKE #5,6 & 7 -----
INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
get_target_property(OUT ${PACKAGE_NAME} LINK_LIBRARIES)
message(STATUS "${CMLOC}Libraries: ${OUT}")

## ----- do not change section above - needed to configure build process ----- ##
##
					