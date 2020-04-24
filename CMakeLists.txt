
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
# CMake-CI Number: 0.0.003  
## ---------------------------------------------------------------------------
# ----- Modify section below to include all the details for your plugin ----- ##

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

set(PACKAGE_NAME "weather_routing_pi")
set(VERBOSE_NAME "WeatherRouting") 
set(TITLE_NAME "weatherrouting")
set(PACKAGE_CONTACT "Sean d'Epagnier")
set(COMMON_NAME "WeatherRouting")
# Search "getcommonname"in weather_routing_pi.cpp 
# for the "CommonName" to avoid unmanaged plugins duplication.

set(VERSION_MAJOR "1")
set(VERSION_MINOR "13")
set(VERSION_PATCH "5")
set(VERSION_TWEAK "3")
set(VERSION_DATE "02/29/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")
set(PACKAGE "weather_routing")  #used for Package name
#set(GIT_USER "seandepagnier")  #for Sean
set(GIT_USER "rgleason")

set(GIT_REPOSITORY_NAME "weather_routing_pi")
message(STATUS "CIRCLECI: ${CIRCLECLI}, Env CIRCLECI: $ENV{CIRCLECI}")

# Allows setup of a local webserver with git for testing
if($ENV{CIRCLECI})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "master")
# else()
#  set(GIT_REPOSITORY_HOST "git.eclipse.com.au")
#  set(GIT_REPOSITORY_DIR "")
#  set(GIT_REPOSITORY_BRANCH "cmake_flatpak_test")
endif()
message(STATUS "Git Branch: ${GIT_REPOSITORY_BRANCH}")

# set(CLOUDSMITH_USER "sean-depagnier")
set(CLOUDSMITH_USER "rick-gleason")

#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins") #Rick's customization

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherroute.html")  
set(XML_SUMMARY "Plugin to complete optimal routing with weather files")
set(XML_DESCRIPTION "Weather_Routing Plugin optimizes weather routing")

set(APPVEYOR_TEST_DEPLOY_TO_CLOUDSMITH "true")
option(PLUGIN_USE_SVG "Use SVG graphics" ON)

##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")

set(PACKAGE_NAME ${PACKAGE}_pi)
set(PACKAGE_FILE_NAME "${PARENT}-plugin-${PACKAGE}")
string(TOUPPER "${PACKAGE}" TITLE_NAME)

project(${PACKAGE_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})

set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(STATUS "${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")
message(STATUS "OPCN_FLATPAK: ${OCPN_FLATPAK}")

# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build" FORCE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build")
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION}-${OCPN_MIN_VERSION}-${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR})

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
    set(PKG_URL
 "https://dl.cloudsmith.io/public/@pkg_repo@/raw/names/@name@/versions/@version@/@filename@")

 else(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
 # This version works for the other environments
    set(PKG_URL
 "https://dl.cloudsmith.io/public/--pkg_repo--/raw/names/--name--/versions/--version--/--filename--")

 endif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")

#=================================================================================
INCLUDE("cmake/PluginSetup.cmake")
#=================================================================================
#========Below is specific to the plugin and is from testplugin  =================

set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "CMAKE version: ${CMAKE_VERSION}")
if(CMAKE_VERSION VERSION_GREATER 3.4)
  set(ENABLE_CLANG_TIDY
      OFF
      CACHE BOOL "Add clang-tidy automatically to builds")
  if(ENABLE_CLANG_TIDY)
    find_program(
      CLANG_TIDY_EXE
      NAMES "clang-tidy"
      PATHS /usr/local/opt/llvm/bin)
    if(CLANG_TIDY_EXE)
      message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
      # For more, see http://clang.llvm.org/extra/clang-tidy/ set(CLANG_TIDY_CHECKS "-*,modernize-*")
      set(CLANG_TIDY_CHECKS "-*,performance-*")
      set(CMAKE_CXX_CLANG_TIDY
          "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/*'"
          CACHE STRING "" FORCE)
    else()
      message(AUTHOR_WARNING "clang-tidy not found!")
      set(CMAKE_CXX_CLANG_TIDY
          ""
          CACHE STRING "" FORCE) # delete it
    endif()
  endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.9)
  set(ENABLE_CPPCHECK
      OFF
      CACHE BOOL "Add cppcheck automatically to builds")
  if(ENABLE_CPPCHECK)
    find_program(CPPCHECK_EXECUTABLE NAMES "cppcheck")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
  endif()
endif()

# SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/include/version.h)

#
# ----- Modify section above for special plugin requirements----- ##
#=================================================================================
INCLUDE("cmake/PluginConfigure.cmake")
#=================================================================================
# ----- Change below to match project requirements ----- ##
#

add_definitions(-DUSE_S57)  #???  What is this for? Needed?

IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)

#======== Below from cmakelists.txt  weather_routing_pi

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

 SET (SRC_TINYXML
             src/tinyxml/tinystr.h
             src/tinyxml/tinyxml.h
)

# SET  (SRC_ZLIB
#             src/zlib-1.2.3/zconf.h
#             src/zlib-1.2.3/zlib.h
#)

# SET  (SRC_bzip2
#             src/bzip2/blocksort.c
#			 src/bzip2/bzlib.c
#			 src/bzip2/bzlib_private.h
#			 src/bzip2/compress.c
#			 src/bzip2/crctable.c
#			 src/bzip2/decompress.c
#			 src/bzip2/huffman.c
#			 src/bzip2/randtable.c
#)

# Added and tried by Rick
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
#   src/jsoncpp/json_tool.h
#)

SET(SRC ${SRCS} ${SRC_LIBTESS2} ${SRC_PLUGINGL} ${SRC_TINYXML} )

##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  is used in the conditionals below.
##================================================================================
## EG:   add_library(${PACKAGE_NAME} SHARED ${SRCS}  ${SRC_NMEA0183})
##================================================================================

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC})
#Add_Library (${PACKAGE_NAME} is used below. Cannot use same name.

IF(NOT UNIX)
    SET(SRC_BZIP	
            src/bzip2/bzlib.c
            src/bzip2/blocksort.c
            src/bzip2/compress.c
            src/bzip2/crctable.c
            src/bzip2/decompress.c
            src/bzip2/huffman.c
            src/bzip2/randtable.c
    )
    ADD_LIBRARY(LIB_BZIP_WR STATIC ${SRC_BZIP})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP_WR )
    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)
			
IF(NOT UNIX)
    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
    INCLUDE_DIRECTORIES(src/bzip2)
ENDIF(NOT UNIX)

IF(WIN32)
  IF(MSVC)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE()
    # MINGW
#    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lwxzlib-2.8")
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
