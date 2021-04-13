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
# Frontend2: v1.0.132					 
# ---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##


##
##----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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
set(SHORT_DESCRIPTION "Compute optimal routes based on weather and constraints.")
set(LONG_DESCRIPTION "WR features include: optimal routing subject to various constraints based on weather data, automatic boat polar computation.")				  

set(VERSION_MAJOR "1")
set(VERSION_MINOR "13")
set(VERSION_PATCH "19")
set(VERSION_TWEAK "0")
set(VERSION_DATE "09/03/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
#Now set in plugin configure, needed to build flatpak, also locally.
#set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
# set(GIT_USER "rgleason")
# set(GIT_REPOSITORY_NAME "weather_routing_pi")

## Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

#set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Ricks's customization
set(CLOUDSMITH_BASE_REPOSITORY "weather-routing")  #without the _pi

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherroute.html")  
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

##
## ----- Modify section above to include all the details for your plugin ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
#set(OpenGL_GL_PREFERENCE "LEGACY")   #not operational
# Don't use local version of GLU library
#set(USE_LOCAL_GLU FALSE)             #not operational
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
include("PluginSetup")
#========================================================

##
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF) 

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
##

#===========================================================
include("PluginConfigure")
#===========================================================
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##
#

add_definitions(-DUSE_S57)

# From master branch - is this working? necessary?
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
)
# Seems to work without this -Rick
#            src/jsoncpp/json/json.h

# Tried removing this, got many Tesselate Linker errors. Put back in.
SET(SRC_LIBTESS2
             src/libtess2/bucketalloc.c
             src/libtess2/dict.c
             src/libtess2/geom.c
             src/libtess2/mesh.c
             src/libtess2/priorityq.c
             src/libtess2/sweep.c
             src/libtess2/tess.c
)

# added by rick - removed by rick pluginJSON.cmake   
# SET(SRC_PLUGINGL
#             src/plugingl/linmath.h
#             src/plugingl/pidc.h
#             src/plugingl/qtstylesheet.h
#             src/plugingl/TexFont.h
#)

# added by rick - removed by rick pluginTINYXML.cmake does this
# SET (SRC_TINYXML
#    src/tinyxml/tinyxml.cpp
#	src/tinyxml/tinyxmlerror.cpp
#	src/tinyxml/tinyxmlparser.cpp
#    src/tinyxml/tinyxml.h
#)

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
# SET(SRC ${SRCS} ${SRC_LIBTESS2}  ${SRC_PLUGINGL} ${SRC_TINYXML})
SET(SRC ${SRCS} ${SRC_LIBTESS2} )

##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ocpn-api)

##

## Statement below is required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##
##    SET(EXTSRC  ${EXTSRC}   etc   Can be in an if statement as below
##

target_sources(${PACKAGE_NAME} PUBLIC ${SRC})
#Add_Library (${PACKAGE_NAME} is used below. Cannot use same name.

# Used in textplugin. Just added.
add_definitions(-DTIXML_USE_STL)

# Alec from watchdog
add_subdirectory(libs/tinyxml)
target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

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
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs/jsoncpp)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE()
    # MINGW
#    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lwxzlib-2.8")
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs/jsoncpp)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lz")
  ENDIF()
ENDIF()

#--------Above from cmakelists.txt  weather_routing_pi------------
#--------REMOVE LN 194 to LN 296 from Testplugin_pi---------------
#------- Change below to match project requirements --------------

# Jon identified these were missing causing json/json.h and other problems.
#----INCLUDE CMAKE #3 & #4 -----
#include("PluginJSON")
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/jsoncpp)
add_subdirectory(libs/jsoncpp)
target_link_libraries(${PACKAGE_NAME} ${PACKAGE_NAME}_LIB_PLUGINJSON)


# include("PluginXML") #Alec -added cmake_module and sub-directory. same as watchdog
include("PluginGL")

# Added for android builds
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

#
# ----- Do not change - needed to build app ----- ##
#
#INCLUDE CMAKE #5,6 & 7 -----
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
get_target_property(OUT ${PACKAGE_NAME} LINK_LIBRARIES)
message(STATUS "${CMLOC}Libraries: ${OUT}")

## ----- do not change section above - needed to configure build process ----- ##
##
get_property(
    dirs
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}include dir='${dir}'")
endforeach()
