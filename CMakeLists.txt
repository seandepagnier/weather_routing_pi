##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier 
## Copyright:   
## License:     wxWidgets License
##---------------------------------------------------------------------------
 

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
# This should be 2.8.0 to have FindGTK2 module
IF (COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 OLD)
  CMAKE_POLICY(SET CMP0005 OLD)
  CMAKE_POLICY(SET CMP0011 OLD)
ENDIF (COMMAND cmake_policy)

PROJECT(weather_routing_pi)

SET(PACKAGE_NAME weather_routing_pi)
SET(PLUGIN_SOURCE_DIR .)
SET(GRIB_PLUGIN_SOURCE_DIR ../grib_pi)
MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(PREFIX_PARENTDATA ${PREFIX_DATA}/${PARENT})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

# SET(PROFILING 1)

#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT WIN32)
 IF(PROFILING)
  ADD_DEFINITIONS( "-Wall -g -fprofile-arcs -ftest-coverage -fexceptions" )
 ELSE(PROFILING)
  ADD_DEFINITIONS( "-Wall -g -fexceptions" )
# ADD_DEFINITIONS( "-Wall -g -O2 -fexceptions" )
 ENDIF(PROFILING)

 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
 ENDIF(NOT APPLE)

ENDIF(NOT WIN32)

# Add some definitions to satisfy MS
IF(WIN32)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(WIN32)

IF(NOT DEFINED wxWidgets_USE_FILE)
  SET(wxWidgets_USE_LIBS base core net xml html adv)
  SET(BUILD_SHARED_LIBS TRUE)
  FIND_PACKAGE(wxWidgets REQUIRED)
ENDIF(NOT DEFINED wxWidgets_USE_FILE)

  INCLUDE(${wxWidgets_USE_FILE})

FIND_PACKAGE(Gettext REQUIRED)

# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out


SET(SRC_WEATHER_ROUTING
            src/weather_routing_pi.h
            src/weather_routing_pi.cpp
            src/WeatherRouting.h 
            src/WeatherRouting.cpp
            src/ConfigurationDialog.h 
            src/ConfigurationDialog.cpp
            src/SettingsDialog.h 
            src/SettingsDialog.cpp
            src/PlotDialog.h
            src/PlotDialog.cpp
            src/BoatDialog.h
            src/BoatDialog.cpp
            src/BoatPlanDialog.h
            src/BoatPlanDialog.cpp
            src/SwitchPlanDialog.h
            src/SwitchPlanDialog.cpp
            src/StatisticsDialog.h 
            src/StatisticsDialog.cpp
            src/FilterRoutesDialog.h
            src/FilterRoutesDialog.cpp
            src/ConfigurationBatchDialog.h
            src/ConfigurationBatchDialog.cpp
            src/AboutDialog.h
            src/AboutDialog.cpp
            src/BoatPlan.h 
            src/BoatPlan.cpp
            src/Boat.h 
            src/Boat.cpp
            src/RouteMap.h 
            src/RouteMap.cpp
            src/RouteMapOverlay.h 
            src/RouteMapOverlay.cpp
            src/Utilities.cpp
            src/Utilities.h
            src/WeatherRoutingUI.h
            src/WeatherRoutingUI.cpp
            src/icons.h 
            src/icons.cpp
            src/georef.h
            src/georef.c

            ../grib_pi/src/GribRecord.cpp  
            ../grib_pi/src/zuFile.cpp 

            ../../src/wxJSON/jsonval.cpp
            ../../src/wxJSON/jsonreader.cpp
            ../../src/wxJSON/jsonwriter.cpp

	    ../../../include/tinyxml.h
            ../../../src/tinyxml.cpp
            ../../../src/tinyxmlparser.cpp
            ../../../src/tinyxmlerror.cpp
)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WEATHER_ROUTING})

IF(NOT UNIX)
    INCLUDE_DIRECTORIES(${GRIB_PLUGIN_SOURCE_DIR}/src/zlib-1.2.3)
    INCLUDE_DIRECTORIES(${GRIB_PLUGIN_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)


IF(WIN32)
  SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
  TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP ${OPENCPN_IMPORT_LIB} )
  ADD_DEPENDENCIES(${PACKAGE_NAME} ${PARENT})
ENDIF(WIN32)

IF(UNIX)
 IF(PROFILING)
  find_library(GCOV_LIBRARY
    NAMES
    gcov
    PATHS
    /usr/local/lib/gcc/i686-pc-linux-gnu/4.7.0
    )

  MESSAGE (STATUS "gcov : " ${GCOV_LIBRARY})
  SET(EXTRA_LIBS ${EXTRA_LIBS} ${GCOV_LIBRARY})
 ENDIF(PROFILING)
ENDIF(UNIX)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} ${EXTRA_LIBS} )

IF(APPLE)
 FIND_PACKAGE(ZLIB REQUIRED)
 TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${ZLIB_LIBRARIES} )
ENDIF(APPLE)

IF(UNIX AND NOT APPLE)
    FIND_PACKAGE(BZip2 REQUIRED)
    INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
    FIND_PACKAGE(ZLIB REQUIRED)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARY} )
ENDIF(UNIX AND NOT APPLE)

IF(APPLE)
      INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/OpenCPN.app/Contents/PlugIns)
ENDIF(APPLE)

IF(UNIX AND NOT APPLE)
      IF(NOT DEFINED PREFIX_PLUGINS)
            SET(PREFIX_PLUGINS /usr/local/lib/opencpn)
      ENDIF(NOT DEFINED PREFIX_PLUGINS)

      INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME LIBRARY DESTINATION ${PREFIX_PLUGINS})
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")
ENDIF(WIN32)

IF(WIN32)
  INSTALL(DIRECTORY data DESTINATION "plugins\\\\weather_routing")
ELSE(WIN32)
  INSTALL(DIRECTORY data DESTINATION ${PREFIX_PARENTDATA}/plugins/weather_routing)
ENDIF(WIN32)

INCLUDE("../PluginLocalization.cmake")
PLUGIN_LOCALIZATION()
