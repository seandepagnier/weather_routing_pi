##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier 
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(weather_routing_pi)
SET(PACKAGE_NAME weather_routing_pi)
SET(PLUGIN_SOURCE_DIR .)
MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "10")

INCLUDE("cmake/PluginConfigure.cmake")

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

            src/GribRecord.cpp  
            src/zuFile.cpp 

            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonwriter.cpp

	    src/tinyxml/tinyxml.h
            src/tinyxml/tinyxml.cpp
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
