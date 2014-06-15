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

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "12")

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
            src/ReportDialog.cpp
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
)

IF(WIN32)
SET(SRC_JSON
            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonwriter.cpp)
SET(SRC_TINYXML
            src/tinyxml/tinyxml.cpp
            src/tinyxml/tinyxmlparser.cpp
            src/tinyxml/tinyxmlerror.cpp
)
ENDIF(WIN32)
ADD_DEFINITIONS(-DTIXML_USE_STL)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WEATHER_ROUTING} ${SRC_JSON} ${SRC_TINYXML})

#IF(NOT UNIX)
#    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
#    INCLUDE_DIRECTORIES(src/bzip2)
#ENDIF(NOT UNIX)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
