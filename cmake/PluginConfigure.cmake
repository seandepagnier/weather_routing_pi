# ---------------------------------------------------------------------------
# Author:      Jon Gough (Based on the work of Sean D'Epagnier and Pavel Kalian) Copyright:   2019 License:     GPLv3+
# ---------------------------------------------------------------------------

message(STATUS "*** Staging to build ${PACKAGE_NAME} ***")

# Do the version.h & wxWTranslateCatalog configuration into the build output directory, thereby allowing building from a read-only source tree.
if(NOT SKIP_VERSION_CONFIG)
  set(BUILD_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
  configure_file(cmake/in-files/version.h.in ${BUILD_INCLUDE_PATH}/include/version.h)
  configure_file(cmake/in-files/wxWTranslateCatalog.h.in ${BUILD_INCLUDE_PATH}/include/wxWTranslateCatalog.h)
  include_directories(${BUILD_INCLUDE_PATH}/include)
endif(NOT SKIP_VERSION_CONFIG)

# configure xml file for circleci
message(STATUS "OCPN_TARGET: $ENV{OCPN_TARGET}")
if(NOT DEFINED ENV{OCPN_TARGET})
    set($ENV{OCPN_TARGET} ${PKG_TARGET})
    message(STATUS "Setting OCPN_TARGET")
endif(NOT DEFINED ENV{OCPN_TARGET})
if($ENV{OCPN_TARGET} MATCHES "(.*)gtk3")
    set(PKG_TARGET_FULL "${PKG_TARGET}-gtk3")
    message(STATUS "Found gtk3")
else($ENV{OCPN_TARGET} MATCHES "(.*)gtk3")
    set(PKG_TARGET_FULL "${PKG_TARGET}")
endif($ENV{OCPN_TARGET} MATCHES "(.*)gtk3")

message(STATUS "PKG_TARGET_FULL: ${PKG_TARGET_FULL}")
message(STATUS "*.in files generated in ${CMAKE_CURRENT_BINARY_DIR}")
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/plugin.xml.in ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGING_NAME}.xml)
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/pkg_version.sh.in ${CMAKE_CURRENT_BINARY_DIR}/pkg_version.sh)
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/cloudsmith-upload.sh.in ${CMAKE_CURRENT_BINARY_DIR}/cloudsmith-upload.sh @ONLY)

message(STATUS "Checking OCPN_FLATPAK: ${OCPN_FLATPAK}")
if(OCPN_FLATPAK_CONFIG)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/org.opencpn.OpenCPN.Plugin.yaml.in ${CMAKE_CURRENT_BINARY_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.${PACKAGE}.yaml)

  message(STATUS "Done OCPN_FLATPAK CONFIG")
  message(STATUS "Directory used: ${CMAKE_CURRENT_BINARY_DIR}/flatpak")
  message(STATUS "Git Branch: ${GIT_REPOSITORY_BRANCH}")
  return()
endif(OCPN_FLATPAK_CONFIG)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# SET(PROFILING 1)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_definitions("-DDEBUG_BUILD")
  set(CMAKE_INSTALL_DO_STRIP FALSE)
  set(CPACK_DEBIAN_DEBUGINFO_PACKAGE YES)
  message(STATUS "DEBUG available")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")

if(NOT WIN32 AND NOT APPLE)
  add_definitions("-Wall -Wno-unused -fexceptions -rdynamic -fvisibility=hidden")
  add_definitions(" -fno-strict-aliasing")
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(" -O0 -g")
    message(STATUS "Optimisation: -O0 -g")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(" -O2 -march=native")
    message(STATUS "Optimisation: -O2 -march=native")
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(" -O2 -march=native -g")
    message(STATUS "Optimisation: -O2 -march=native -g")
  else(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(" -O2")
    message(STATUS "Optimisation: -O2")
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

  add_definitions(" -DPREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\"")
  # profiling with gprof
  # ADD_DEFINITIONS( -pg ) SET(CMAKE_EXE_LINKER_FLAGS -pg)
  # profiling with gcov
  # ADD_DEFINITIONS( "-fprofile-arcs -ftest-coverage" ) SET(EXTRA_LIBS ${EXTRA_LIBS} "gcov")
endif(NOT WIN32 AND NOT APPLE)

if(MINGW)
  add_definitions("-Wall -Wno-unused -Wno-cpp -fexceptions")
  add_definitions(" -g -fno-strict-aliasing")
endif(MINGW)

if(APPLE)
  add_definitions("-Wall -Wno-unused -fexceptions -Wno-overloaded-virtual")
  add_definitions(" -g -fno-strict-aliasing")
  add_definitions(" -Wno-deprecated -Wno-deprecated-declarations -Wno-unknown-pragmas")
  add_definitions(" -D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_")
endif(APPLE)

# Add some definitions to satisfy MS
if(MSVC)
  add_definitions(-D__MSVC__)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D HAVE_SNPRINTF)
  message(STATUS "Set SNPRINTF")
else(MSVC)
  if(NOT APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
  else(NOT APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
  endif(NOT APPLE)
endif(MSVC)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(BUILD_SHARED_LIBS TRUE)

# Allow multiprocess compile
if(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(MSVC)

if(WIN32)
  if(MSVC)
    set(CMAKE_C_FLAGS_DEBUG "/MP /MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1")
    set(CMAKE_C_FLAGS_MINSIZEREL "/MP /MD  /O1  /Ob1 /D NDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "/MP /MD  /O2  /Ob2 /D NDEBUG /Zi")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/MP /MD  /O2  /Ob1 /D NDEBUG /Zi")
    set(CMAKE_CXX_FLAGS_DEBUG "/MP /MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1 /EHa")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/MP /MD  /O1  /Ob1 /D NDEBUG /EHa")
    set(CMAKE_CXX_FLAGS_RELEASE "/MP /MD  /O2  /Ob2 /D NDEBUG /Zi /EHa")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MP /MD  /O2  /Ob1 /D NDEBUG /Zi /EHa")
    set(CMAKE_EXE_LINKER_FLAGS /DEBUG)
  endif(MSVC)
endif(WIN32)

set(wxWidgets_USE_LIBS
    base
    core
    net
    xml
    html
    adv
    aui)

option(USE_GL "Enable OpenGL support" ON)

# Search for opengles, short of running a program to test the speed of acceleration, I simply use gles on "native linux" arm systems
if(ARCH MATCHES "arm*" AND (NOT QT_ANDROID) AND USE_GL MATCHES "ON")
  find_path(OPENGLESv1_INCLUDE_DIR GLES/gl.h)
  if(OPENGLESv1_INCLUDE_DIR)
    message(STATUS "Found OpenGLESv1")
    add_definitions(-DocpnUSE_GLES)
    add_definitions(-DocpnUSE_GL)

    set(OPENGLES_FOUND "YES")
    set(OPENGL_FOUND "YES")

    set(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
    add_subdirectory(src/glshim)

    set(OPENGL_LIBRARIES "GL_static" "EGL" "X11" "drm")
  endif()
endif()

# Building for QT_ANDROID involves a cross-building environment, So the include directories, flags, etc must be stated explicitly without trying to locate them on the host build system.
if(QT_ANDROID AND USE_GL MATCHES "ON")
  message(STATUS "Using GLESv1 for Android")
  add_definitions(-DocpnUSE_GLES)
  add_definitions(-DocpnUSE_GL)
  add_definitions(-DARMHF)

  set(OPENGLES_FOUND "YES")
  set(OPENGL_FOUND "YES")

  set(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
  add_subdirectory(src/glshim)
endif(QT_ANDROID AND USE_GL MATCHES "ON")

if((NOT OPENGLES_FOUND) AND (NOT QT_ANDROID))

  if(USE_GL MATCHES "ON")
    message(STATUS "Finding package OpenGL")
    find_package(OpenGL)
  else(USE_GL MATCHES "ON")
    message(STATUS "OpenGL disabled by option...")
  endif(USE_GL MATCHES "ON")

  if(OPENGL_FOUND)

    set(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
    include_directories(${OPENGL_INCLUDE_DIR})

    message(STATUS "Found OpenGL...")
    message(STATUS "    Lib: " ${OPENGL_LIBRARIES})
    message(STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
    add_definitions(-DocpnUSE_GL)

    # We need to remove GLU from the OPENGL_LIBRARIES list
    foreach(_currentLibFile ${OPENGL_LIBRARIES})
      set(UCNAME ${_currentLibFile})
      string(TOUPPER ${UCNAME} UCNAME)
      if(NOT ${UCNAME} MATCHES "(.*)GLU(.*)")
        set(REVISED_OPENGL_LIBRARIES ${_currentLibFile} ${REVISED_OPENGL_LIBRARIES})
      endif()
    endforeach(_currentLibFile)

    set(OPENGL_LIBRARIES ${REVISED_OPENGL_LIBRARIES})
    message(STATUS "    Revised GL Lib: " ${OPENGL_LIBRARIES})

  else(OPENGL_FOUND)
    message(STATUS "OpenGL not found...")
  endif(OPENGL_FOUND)
endif()

if(USE_LOCAL_GLU)
  message(STATUS "    Adding local GLU")
  add_subdirectory(ocpnsrc/glu)
  set(OPENGL_LIBRARIES "GLU_static" ${OPENGL_LIBRARIES})
  message(STATUS "    Revised GL Lib (with local): " ${OPENGL_LIBRARIES})
endif(USE_LOCAL_GLU)

if(NOT QT_ANDROID)
  # Find wxWidgets here, and the setting get inherited by all plugins. These options can be used to set the linux widgets build type
  set(wxWidgets_USE_DEBUG OFF)
  set(wxWidgets_USE_UNICODE ON)
  set(wxWidgets_USE_UNIVERSAL OFF)
  set(wxWidgets_USE_STATIC OFF)

  set(WXWIDGETS_FORCE_VERSION CACHE STRING "Force usage of a specific wxWidgets version.")
  if(WXWIDGETS_FORCE_VERSION)
    set(wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
  endif()

  message(STATUS "wxWidgets components: ${wxWidgets_USE_LIBS}")
  find_package(wxWidgets REQUIRED COMPONENTS ${wxWidgets_USE_LIBS})

  if(MSVC)
    # Exclude wxexpat.lib, since we use our own version. Other things are excluded as well, but we don't need them
    set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
  endif(MSVC)

  if(WIN32
     OR APPLE
     OR QT_ANDROID)
    if(MSYS)
      # this is to convert msys to windows paths, and handle the missing /usr
      string(REGEX REPLACE "/usr/local" ";C:/MinGW/msys/1.0/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS})
    endif(MSYS)
  endif(
    WIN32
    OR APPLE
    OR QT_ANDROID)

  include(${wxWidgets_USE_FILE})

  message(STATUS "Found wxWidgets...")
  message(STATUS " wxWidgets Include: ${wxWidgets_INCLUDE_DIRS}")
  message(STATUS " wxWidgets Libraries: ${wxWidgets_LIBRARIES}")

  # We need to remove GLU from the wxWidgets_LIBRARIES list It only appears to get on the list for MSW...
  foreach(_currentLibFile ${wxWidgets_LIBRARIES})
    set(UCNAME ${_currentLibFile})
    string(TOUPPER ${UCNAME} UCNAME)
    if(NOT ${UCNAME} MATCHES "(.*)GLU(.*)")
      set(REVISED_wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES} ${_currentLibFile})
    endif()
  endforeach(_currentLibFile)
  set(wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES})

  message(STATUS " Revised wxWidgets Libraries: ${wxWidgets_LIBRARIES}")
endif(NOT QT_ANDROID)

if(NOT WIN32
   AND NOT APPLE
   AND NOT QT_ANDROID)
  option(OCPN_FORCE_GTK3 "Force the build to use GTK3" OFF)

  if(NOT OCPN_FORCE_GTK3)
    find_package(GTK2)
  endif(NOT OCPN_FORCE_GTK3)

  if(GTK2_FOUND)
    set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk2)
    include_directories(${GTK2_INCLUDE_DIRS})
    set(GTK_LIBRARIES ${GTK2_LIBRARIES})
    message(STATUS "Building against GTK2...")
  else(GTK2_FOUND)
    find_package(GTK3)
    include_directories(${GTK3_INCLUDE_DIRS})
    set(GTK_LIBRARIES ${GTK3_LIBRARIES})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXGTK3__")
    set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
    message(STATUS "Building against GTK3...")
  endif(GTK2_FOUND)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${GTK_LIBRARIES})
endif(
  NOT WIN32
  AND NOT APPLE
  AND NOT QT_ANDROID)

# On Android, PlugIns need a specific linkage set....
if(QT_ANDROID)
  # These libraries are needed to create PlugIns on Android.

  set(OCPN_Core_LIBRARIES
      # Presently, Android Plugins are built in the core tree, so the variables {wxQT_BASE}, etc.
      # flow to this module from above.  If we want to build Android plugins out-of-core, this will need improvement.
      # TODO This is pretty ugly, but there seems no way to avoid specifying a full path in a cross build....
      /home/dsr/Projects/opencpn_sf/opencpn/build-opencpn-Android_for_armeabi_v7a_GCC_4_8_Qt_5_5_0-Debug/libopencpn.so
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_core-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_html-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu_xml-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_qa-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_adv-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_aui-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu_net-3.1-arm-linux-androideabi.a
      ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_gl-3.1-arm-linux-androideabi.a
      ${Qt_Base}/android_armv7/lib/libQt5Core.so
      ${Qt_Base}/android_armv7/lib/libQt5OpenGL.so
      ${Qt_Base}/android_armv7/lib/libQt5Widgets.so
      ${Qt_Base}/android_armv7/lib/libQt5Gui.so
      ${Qt_Base}/android_armv7/lib/libQt5AndroidExtras.so
      # ${NDK_Base}/sources/cxx-stl/gnu-libstdc++/4.8/libs/armeabi-v7a/libgnustl_shared.so
  )

endif(QT_ANDROID)

find_package(Gettext REQUIRED)
