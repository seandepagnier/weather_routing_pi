##---------------------------------------------------------------------------
## Author:      Sean d'Epagnier
##***************************************************************************
## *   Copyright (C) 2010 by Sean d'Epagnier                              *
## *   License:     GPLv3+
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
# Testplugin(TP) v1.0.260 07/10/2023 DD/MM/YYYY format
# buildCI:yes useODAPI:no validateJSON:no
# ---------------------------------------------------------------------------
## -- When changing this file do NOT change the order of the sections.    -- ##
## -- Make changes only between the section blocks identified             -- ##
## -- The whole configuration process relies on this sequence for success.-- ##
##
## -- Modify section below to include all the details for your plugin.    -- ##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.5.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
# Need cmake 3.13 to implement this, but raspbian builds are at 3.7.2 and bionic at 3.10 and xenial builds at 3.5.1
#    cmake_policy(SET CMP0076 NEW)
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
set(VERSION_MINOR "15")
set(VERSION_PATCH "17")
set(VERSION_TWEAK "0")
set(VERSION_DATE  "12/10/2023")#DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov58")

set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "17")
set(TP_COMMENT "  * Release using CI")

set(PARENT "opencpn")

#Use to set up a local webserver with git installed.
#Default is github.com. Now set in pluginconfigure.cmake
#set(GIT_REPOSITORY_SERVER "github.com")

## Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Ricks's customization
set(CLOUDSMITH_BASE_REPOSITORY "weather-routing")  #without the _pi

# Set the following to use OpenCPN cloudsmith repositories instead of own repositories
set(CLOUDSMITH_USER "opencpn")
# set(CLOUDSMITH_USER "rick-gleason")

set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherroute.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

## ----- Modify section above to include all the details for your plugin ------- ##
## ----- Modify section below if there are special requirements for the plugin --##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Use local version of GLU library
# requires libs/glu directory
set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")   #not operational
# For Weatherfax this must be set ON or it fails to compile
option(USE_GL "Enable OpenGL support" ON)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

## ----- Statements below used to setup standard variables required by the CMAKE process -- Do not remove - ##

project(${PACKAGE})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

##========================================================

include("PluginSetup")

##========================================================
## ----- Above sets up standard variables required by CMAKE - Do not remove -----##
## ----- Modify section below if there are special requirements for the plugin --##

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin --##
## ----- Do not change next section - needed to configure build process ---------##
##===========================================================

include("PluginConfigure")

##===========================================================
## ----- Do not change section above - needed to configure build process --------##
## ----- Change below to match project requirements for source, headers, etc. ---##

add_definitions(-DUSE_S57)

## ---- Section Below - Add your project SET(SRCS and SET(HDRS  etc..

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
            src/navobj_util.cpp
)

SET (HDRS
            include/weather_routing_pi.h
            include/WeatherRouting.h
            include/ConfigurationDialog.h
            include/SettingsDialog.h
            include/PlotDialog.h
            include/BoatDialog.h
            include/StatisticsDialog.h
            include/ReportDialog.h
            include/FilterRoutesDialog.h
            include/ConfigurationBatchDialog.h
            include/EditPolarDialog.h
            include/AboutDialog.h
            include/Polar.h
            include/Boat.h
            include/RouteMap.h
            include/RouteMapOverlay.h
            include/Utilities.h
            include/LineBufferOverlay.h
            include/PolygonRegion.h
            include/WeatherRoutingUI.h
            include/icons.h
            include/zuFile.h
            include/georef.h
            include/GribRecord.h
            include/navobj_util.h
)


add_definitions(-DPLUGIN_USE_SVG)

## ----- Section above - Add your project SET(SRCS and SET(HDRS  etc.-------##
## ----- If using JSON validation in plugin section below is needed --------##

# Do not need Json Schema Validation

## ----- If using JSON validation in plugin section above is needed --------##


##=============================================================
## Change as required to collect all the set( headers and SRCS
## This add_library  can be used in conditionals below.
##=============================================================

SET(SRC ${SRCS} ${HDRS} )

##=============================================================
## Modify/configure 'Include' Directories below for the Plugin
##=============================================================

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/GL)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/glu)   #added with plugDC
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ocpn-api)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ODAPI)

#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wxJSON/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/pugixml)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/pluginDC)


## Change target_sources to match your project needs.
##==============================================================

target_sources(${PACKAGE_NAME} PUBLIC ${SRC} )

##==============================================================

if(NOT OCPN_FLATPAK_CONFIG)
    # Build environment not available when flatpak is being configured 
	# so following statements will not work
    message(STATUS "${CMLOC}Adding target link libraries to ${PACKAGE_NAME}")

#    add_subdirectory(opencpn-libs/WindowsHeaders)
#    target_link_libraries(${PACKAGE_NAME} windows::headers)

    add_subdirectory(opencpn-libs/api-17)
    target_link_libraries(${PACKAGE_NAME} ocpn::api)

    add_subdirectory(opencpn-libs/tinyxml)
    target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

    add_subdirectory(opencpn-libs/odapi)
    target_link_libraries(${PACKAGE_NAME} ocpn::odapi)

    add_subdirectory(opencpn-libs/jsonlib)
    target_link_libraries(${PACKAGE_NAME} ocpn::jsonlib)

    add_subdirectory(opencpn-libs/plugingl)
    target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

    add_subdirectory(opencpn-libs/pugixml)
    target_link_libraries(${PACKAGE_NAME} ocpn::pugixml)

    IF(NOT UNIX)
        add_subdirectory(opencpn-libs/bzip2)
        target_link_libraries(${PACKAGE_NAME} ocpn::bzip2)
    ENDIF(NOT UNIX)

    add_subdirectory(opencpn-libs/libtess2)
    target_link_libraries(${PACKAGE_NAME} ocpn::libtess2)

endif(NOT OCPN_FLATPAK_CONFIG)

add_definitions(-DTIXML_USE_STL)

##
## ----- If using JSON validation section below is needed ---------------##
##

# Do not need Json Schema Validation


##------- Change below to match project requirements --------------------##

IF(WIN32)
    IF(MSVC)
        #include_directories(${PROJECT_SOURCE_DIR}/buildwin/include)
        target_link_libraries(${PACKAGE_NAME} "${PROJECT_SOURCE_DIR}/buildwin/zlib1.lib")
    ELSE()
        # MINGW
        target_link_libraries(${PACKAGE_NAME} "-lz")
    ENDIF()
ENDIF()

##------- Change below to match project requirements --------------

# Needed for android builds
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

## Needed for all builds Do not change - needed to build app -------------##
##===============================================================

include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")

##================================================================
## ----- Do not change section above - needed to configure build process --##

message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
