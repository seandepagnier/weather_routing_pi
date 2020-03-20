# ---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough Copyright: License:     wxWidgets License
# ---------------------------------------------------------------------------
# CMake-CI Number: 0.0.001  
#    - 2/19/2020 added Update build to create all 10 environments needed for beta testing.
#    - 2/19/2020 fixed debian build
#    - 2/19/2020 Add raspian build capability
#    - 2/20/2020 fix execute ci/*.sh files  and increment version patch
#    - 3/1/2020  fix raspian xml target-url with if-then
#
# define minimum cmake version
# cmake_minimum_required(VERSION 2.8.12.2)  #for trusty
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
if($ENV{CIRCLECI})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "master")
# else()
#  set(GIT_REPOSITORY_HOST "git.eclipse.com.au")
#  set(GIT_REPOSITORY_DIR "")
#  set(GIT_REPOSITORY_BRANCH "cmake_flatpak_test")
endif()
# set(CLOUDSMITH_USER "sean-depagnier")
set(CLOUDSMITH_USER "rick-gleason")
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherroute.html")  
set(XML_SUMMARY "Plugin to complete optimal routing with weather files")
set(XML_DESCRIPTION "Weather_Routing Plugin optimizes weather routing")
set(APPVEYOR_TEST_DEPLOY_TO_CLOUDSMITH "true")
option(PLUGIN_USE_SVG "Use SVG graphics" ON)

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

include("cmake/PluginSetup.cmake")

#========Below is specific to the plugin and is from testplugin  =================

set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

# option(OD_CXX11 "Use c++11" OFF)

# if(OD_CXX11)
#  message(STATUS "Attempting to use c++11")
#  include(CheckCXXCompilerFlag)
#  check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
#  check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#  if(COMPILER_SUPPORTS_CXX11)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#    set(Testplugin_USE_CXX11 ON)
#  elseif(COMPILER_SUPPORTS_CXX0X)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#    set(Testplugin_USE_CXX11 ON)
#  else()
#    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#    set(Testplugin_USE_CXX11 OFF)
#  endif()
#else()
#  set(Testplugin_USE_CXX11 OFF)
#endif()

#if(Testplugin_USE_CXX11)
#  message(STATUS "Allowing use of c++11")
#  add_definitions(-DTestplugin_USE_CXX11)
#endif()

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

include("cmake/PluginConfigure.cmake")

add_definitions(-DUSE_S57)  #???  What is this for? Needed?

IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)



#======== From cmakelists.txt  weather_routing_pi

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

SET(SRC_LIBTESS2
  src/libtess2/bucketalloc.c
  src/libtess2/dict.c
  src/libtess2/geom.c
  src/libtess2/mesh.c
  src/libtess2/priorityq.c
  src/libtess2/sweep.c
  src/libtess2/tess.c)

SET(SRC ${SRCS} ${SRC_LIBTESS2})
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



#======== Above from cmakelists.txt  weather_routing_pi=========
#=========Below from cmakelists.txt testplugin==================

#set(LIBHDRS
#    libs/GL/include/gl.h
#    libs/GL/include/gl_private.h
#    libs/GL/include/glext.h
#    libs/GL/include/glu.h
#    libs/wx/include/json_defs.h
#    libs/wx/include/jsonreader.h
#    libs/wx/include/jsonval.h
#    libs/wx/include/jsonwriter.h)
	
#option(TESTPLUGIN_USE_SVG "Use SVG graphics" ON)
#add_definitions(-DTESTPLUGIN_USE_SVG)

#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)
#add_library(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EX#TSRC} ${LIBSSRC})
#add_library(${PACKAGE_NAME} SHARED ${HDRS} )

#add_definitions(-DTIXML_USE_STL)

#==========Above is from Testplugin cmakelists.txt ========
#==========Below is from WxRoute cmakelists.txt========
INCLUDE("cmake/PluginJSON.cmake")
INCLUDE("cmake/PluginXML.cmake")
INCLUDE("cmake/PluginGL.cmake")

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
