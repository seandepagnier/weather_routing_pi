# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/weatherrouting-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/weatherrouting-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/weatherrouting-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME weather_routing_pi)
set(PKG_VERSION  1.15.18.6)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME WeatherRouting)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME WeatherRouting) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Compute optimal routes based on weather and constraints.")
set(PKG_DESCRIPTION [=[
WR features include: optimal routing subject to various constraints based on weather data,
automatic boat polar computation.
]=])

set(PKG_AUTHOR "Sean d'Epagnier")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/seandepagnier/weather_routing_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/weatherroute.html)

SET(SRC
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

set(PKG_API_LIB api-18)  #  A dir in opencpn-libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugin_dc")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugin-dc)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

  # The wxsvg library enables SVG overall in the plugin
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxsvg")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxsvg)
endmacro ()