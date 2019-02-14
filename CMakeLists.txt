##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:
## License:     GPLv3+
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(weather_routing_pi)
SET(PACKAGE_NAME weather_routing_pi)
SET(VERBOSE_NAME Weather_routing)
SET(TITLE_NAME WEATHER_ROUTING)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

INCLUDE("cmake/PluginConfigure.cmake")

IF(WEATHER_ROUTING_USE_SVG)
  ADD_DEFINITIONS(-DWEATHER_ROUTING_USE_SVG)
ENDIF(WEATHER_ROUTING_USE_SVG)

SET(SRC_WEATHER_ROUTING
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


SET(SRC_LIBTESS2
  src/libtess2/bucketalloc.c
  src/libtess2/dict.c
  src/libtess2/geom.c
  src/libtess2/mesh.c
  src/libtess2/priorityq.c
  src/libtess2/sweep.c
  src/libtess2/tess.c)


SET(SRC ${SRC_WEATHER_ROUTING} ${SRC_LIBTESS2})
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC})

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

#IF(NOT UNIX)
#    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
#    INCLUDE_DIRECTORIES(src/bzip2)
#ENDIF(NOT UNIX)


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

INCLUDE("cmake/PluginJSON.cmake")
INCLUDE("cmake/PluginXML.cmake")
INCLUDE("cmake/PluginGL.cmake")

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
