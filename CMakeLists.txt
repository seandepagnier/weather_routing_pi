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
# CMake-CI Number: 0.0.006  
#    - 2/19/2020 10 environments needed for beta testing.
#    - 2/19/2020 fix debian build
#    - 2/19/2020 Add raspian build
#    - 2/20/2020 fix execute ci/*.sh files  and increment version patch
#    - 3/1/2020  fix raspian xml target-url with if-then
#    - 3/30/2020 MacOS build fixed 12 env
#    - Reverse dates here
#    - 2/05/2020 MacOS final fix- JG
# ----------------------------------------------------------------------------
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

set(VERBOSE_NAME "WeatherRouting")
# Search "getcommonname"in weather_routing_pi.cpp
set(COMMON_NAME "WeatherRouting")  
set(TITLE_NAME "weatherrouting")
set(PACKAGE_CONTACT "Sean d'Epagnier")
set(PACKAGE "weather_routing_pi")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "13")
set(VERSION_PATCH "7")
set(VERSION_TWEAK "0")
set(VERSION_DATE "05/04/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")
#set(GIT_USER "jongough")
#set(GIT_USER "seandepagnier")  #for Sean
set(GIT_USER "rgleason")

set(GIT_REPOSITORY_NAME "weather_routing_pi")
message(STATUS "CIRCLECI: ${CIRCLECLI}, Env CIRCLECI: $ENV{CIRCLECI}")
message(STATUS "TRAVIS: ${TRAVIS}, Env TRAVIS: $ENV{TRAVIS}")

# Permits setup of a local webserver with git for testing
if($ENV{CIRCLECI})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "$ENV{CIRCLE_BRANCH}")
  set(GIT_REPOSITORY_TAG "$ENV{CIRCLE_TAG}")
elseif($ENV{TRAVIS})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "$ENV{TRAVIS_BRANCH}")
  set(GIT_REPOSITORY_TAG "$ENV{TRAVIS_TAG}")
elseif($ENV{APPVEYOR})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "$ENV{APPVEYOR_REPO_BRANCH}")
  set(GIT_REPOSITORY_TAG "$ENV{APPVEYOR_REPO_TAG_NAME}")
else()
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  # Get the current working branch
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REPOSITORY_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(${GIT_REPOSITORY_BRANCH} EQUAL "")
    message(STATUS "Setting default GIT repository branch - master")
    set(GIT_REPOSITORY_BRANCH "master")
  endif()
  execute_process(
    COMMAND git tag --contains
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REPOSITORY_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
message(STATUS "Git Branch: ${GIT_REPOSITORY_BRANCH}")
message(STATUS "Git Tag: ${GIT_REPOSITORY_TAG}")
if("${GIT_REPOSITORY_BRANCH}" EQUAL "")
  set(GIT_BRANCH_OR_TAG "TAG")
else()
  set(GIT_BRANCH_OR_TAG "BRANCH")
endif()

#set(CLOUDSMITH_USER "sean-depagnier") 
#set(CLOUDSMITH_USER "jon-gough")
set(CLOUDSMITH_USER "rick-gleason")

#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
#set(CLOUDSMITH_BASE_REPOSITORY "weather_routing")  #Jon Gough's customization
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Sean's customization
set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Rick's customization

# Set Cloudsmith upload repository suffix for each catalog
set(PROD "prod")  #Standard Repos
set(BETA "beta")  #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherroute.html")  
set(XML_SUMMARY "Compute optimal routes based on weather and constraints.")
set(XML_DESCRIPTION "WR features include: optimal routing subject to various constraints based on weather data, automatic boat polar computation.")

##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

# set(APPVEYOR_TEST_DEPLOY_TO_CLOUDSMITH "true")
option(PLUGIN_USE_SVG "Use SVG graphics" ON)

##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

set(CMAKE_CXX_STANDARD 11)


if(NOT ${PACKAGE} MATCHES "(.*)_pi")
  set(PACKAGE_NAME ${PACKAGE}_pi)
  set(PACKAGE_FILE_NAME "${PACKAGE}_pi")
else(NOT ${PACKAGE} MATCHES "(.*)_pi")
  set(PACKAGE_NAME ${PACKAGE})
  set(PACKAGE_FILE_NAME "${PACKAGE}")
endif(NOT ${PACKAGE} MATCHES "(.*)_pi")
string(TOUPPER "${PACKAGE}" TITLE_NAME)

project(${PACKAGE_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")

set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(STATUS "${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")
message(STATUS "OPCN_FLATPAK: ${OCPN_FLATPAK}")

#------Settings
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

# Not found in Master branch. Question still needed? on or off?
# Testplugin has OFF
option(USE_GL "Enable OpenGL support" ON)
message(STATUS "USE_GL: ${USE_GL}")
#------End Settings

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION})
set(PKG_URL "https://dl.cloudsmith.io/public/--pkg_repo--/raw/names/--name--/versions/--version--/--filename--")

#========================================================
INCLUDE("cmake/PluginSetup.cmake")
#========================================================

if(DEFINED ENV{OCPN_TARGET})
    set(PACKAGING_NAME "${PKG_NVR}_${PKG_TARGET}-${PKG_TARGET_VERSION}-$ENV{OCPN_TARGET}")
else(DEFINED ENV{OCPN_TARGET})
    set(PACKAGING_NAME "${PKG_NVR}_${PKG_TARGET}-${PKG_TARGET_VERSION}")
endif(DEFINED ENV{OCPN_TARGET})
message(STATUS "PACKAGING_NAME: ${PACKAGING_NAME}")

set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

# Settings and Options
set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF) #Wass commented out

# Master version of cmakelists.txt has the below commented out
# Testplugin has this on!!  Trying to turn this back on this time.
# Check on compiler support
if(Plugin_CXX11)
  message(STATUS "Attempting to use c++11")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
  check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(Plugin_USE_CXX11 ON)
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    set(Plugin_USE_CXX11 ON)
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    set(Plugin_USE_CXX11 OFF)
  endif()
else()
  set(Plugin_USE_CXX11 OFF)
endif()

if(Plugin_USE_CXX11)
  message(STATUS "Allowing use of c++11")
endif()

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

# -------Section above is directly from Testplugin --------------#
# ----- Modify section above for special plugin requirements----- ##
#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
# ----- Change below to match project requirements ----- ##

add_definitions(-DUSE_S57)

# From master branch - is this workiing? necessary?
IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "SVG support enabled...")
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
## EG  (from Testplugin) also in cmake/Plugin_____.cmake files
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC})
#Add_Library (${PACKAGE_NAME} is used below. Cannot use same name.

# Used in textplugin. Just added.
add_definitions(-DTIXML_USE_STL)

#JSON Validator: Every plugin that uses JSON for passing messages should use a validator.  A JSON validator is much like an XML validator, the validator allows the system to ignore non-conformant messages.  The JSON validator changes the OSABI flavor of the Linux executable so there is code to change it back.

#IF(UNIX AND NOT APPLE)
#  IF(TP_JSON_SCHEMA_VALIDATOR)
#    # Change OS/ABI to be UNIX - System V to make it run with OCPN
#    SET(lib_name lib${PACKAGE_NAME}.so)
#    MESSAGE(STATUS "lib-name: ${lib_name}")
#    FIND_FILE(elfedit elfedit)
#    IF(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#      MESSAGE(STATUS "Will ensure OS/ABI is set to 0 to match OCPN")
#      ADD_CUSTOM_COMMAND(
#        TARGET ${PACKAGE_NAME}
#        POST_BUILD
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        DEPENDS ${PACKAGE_NAME}
#        COMMENT " Running post build action on ${lib_name}."
#        COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) '
#      )
#    ELSE(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#      MESSAGE(STATUS "Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
#    ENDIF(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#  ENDIF(TP_JSON_SCHEMA_VALIDATOR)
#ENDIF(UNIX AND NOT APPLE)

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
message(STATUS "Libraries: ${OUT}")
