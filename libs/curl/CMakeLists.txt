# OpenCPN wrapper for libcurl -- https://curl.se/libcurl/
#
# Exports: libcurl::libcurl transitive link object
#
# Copyright (C) 2022 Alec Leamas
#
# This program is free software; you can redistribute it and/or modify  *
# it under the terms of the GNU General Public License as published by  *
# the Free Software Foundation; either version 3 of the License, or     *
# (at your option) any later version.                                   *

cmake_minimum_required (VERSION 3.1.0)

set(curl_version 7.83.1)   # Actual version for built library

if (TARGET libcurl::libcurl)
  return ()
endif ()

if (QT_ANDROID)
  message(STATUS "NOTE: libcurl is not available on Android")
  return ()
endif ()

project(ocpn_curl)

add_library(curl_if INTERFACE)
add_library(libcurl::libcurl ALIAS curl_if)

if (UNIX AND NOT APPLE AND NOT OCPN_FLATPAK_CONFIG)
  #  On Linux and Flatpak: use the system library and be done with it.

  find_library(LIBCURL NAMES libcurl curl REQUIRED)
  message(STATUS "libs/curl/CMakeLists.txt LIBCURL:  ${LIBCURL}")
  if (${CMAKE_VERSION} VERSION_LESS 3.18)
    find_path(CURL_HEADERS NAMES curl.h PATH_SUFFIXES curl)
  else ()
    find_path(CURL_HEADERS NAMES curl.h PATH_SUFFIXES curl REQUIRED)
  endif ()
  message(STATUS "libs/curl/CMakeLists.txt CURL_HEADERS:  ${CURL_HEADERS}")
  target_link_libraries(curl_if INTERFACE ${LIBCURL})
  target_include_directories(curl_if INTERFACE ${CURL_HEADERS})
  return ()

endif ()

