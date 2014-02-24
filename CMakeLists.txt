##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier 
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(weather_routing_pi)
SET(PACKAGE_NAME weather_routing_pi)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "10")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_WEATHER_ROUTING
            src/weather_routing_pi.cpp
            src/WeatherRouting.cpp
            src/ConfigurationDialog.cpp
            src/SettingsDialog.cpp
            src/PlotDialog.cpp
            src/BoatDialog.cpp
            src/SwitchPlanDialog.cpp
            src/StatisticsDialog.cpp
            src/FilterRoutesDialog.cpp
            src/ConfigurationBatchDialog.cpp
            src/AboutDialog.cpp
            src/BoatPlan.cpp
            src/Boat.cpp
            src/RouteMap.cpp
            src/RouteMapOverlay.cpp
            src/Utilities.cpp
            src/WeatherRoutingUI.cpp

            src/icons.cpp

            src/georef.c

            src/GribRecord.cpp  
            src/zuFile.cpp 

            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonwriter.cpp

            src/tinyxml/tinyxml.cpp
            src/tinyxml/tinystr.cpp
            src/tinyxml/tinyxmlparser.cpp
            src/tinyxml/tinyxmlerror.cpp
)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WEATHER_ROUTING})

IF(NOT UNIX)
    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
    INCLUDE_DIRECTORIES(src/bzip2)
ENDIF(NOT UNIX)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
