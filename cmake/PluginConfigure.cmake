##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:
## License:     GPLv3+
##---------------------------------------------------------------------------

#  To build for android, use this cmake invocation..
#$cmake -DCMAKE_TOOLCHAIN_FILE=../buildandroid/build_android.cmake


SET(PLUGIN_SOURCE_DIR .)


MESSAGE (STATUS "*** Building ${PACKAGE_NAME} ***")

MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")
IF (PREFIX)
 SET(CMAKE_INSTALL_PREFIX ${PREFIX})
ENDIF (PREFIX)

SET(PREFIX_BIN bin)
SET(PREFIX_INCLUDE include)
SET(PREFIX_DATA share)
SET(PREFIX_PKGDATA ${PREFIX_DATA}/${PACKAGE_NAME})

IF (NOT WIN32)
  # default
  SET (ARCH "i386")
  IF (UNIX AND NOT APPLE)
    MESSAGE (STATUS "*** Will install to ${CMAKE_INSTALL_PREFIX}  ***")

    IF (EXISTS /etc/debian_version)
      SET (PACKAGE_FORMAT "DEB")
      #SET (PACKAGE_DEPS "libc6, libwxgtk3.0-0, libglu1-mesa (>= 7.0.0)")
      SET (LIB_INSTALL_DIR "lib")
      IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET (ARCH "amd64")
      ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET (ARCH "i386")
      ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    ENDIF (EXISTS /etc/debian_version)
    IF (EXISTS /etc/redhat-release)
      SET (PACKAGE_FORMAT "RPM")
      SET (PACKAGE_DEPS  "opencpn")
      IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET (ARCH "x86_64")
        SET (LIB_INSTALL_DIR "lib64")
      ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET (ARCH "i386")
        SET (LIB_INSTALL_DIR "lib")
      ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    ENDIF (EXISTS /etc/redhat-release)
  ENDIF (UNIX AND NOT APPLE)
endif(NOT WIN32)

IF (CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
  SET (ARCH "armhf")
  ADD_DEFINITIONS( -DARMHF )
ENDIF (CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")

MESSAGE (STATUS "*** Build Architecture is ${ARCH}")


IF(DEFINED _wx_selected_config)

#  May be overridden
IF(NOT DEFINED Qt_Build)
  SET(Qt_Build "android_armv7")
ENDIF(NOT DEFINED Qt_Build)

MESSAGE (STATUS "selected config ${_wx_selected_config}")

IF(_wx_selected_config MATCHES "androideabi-qt")
  MESSAGE (STATUS "Building for wxQt-Android")
  MESSAGE (STATUS "Qt_Base/Build: " ${Qt_Base} "/" ${Qt_Build})
  MESSAGE (STATUS "wxQt_Base/Build: " ${wxQt_Base} "/" ${wxQt_Build})
  SET(QT_ANDROID "ON")
  SET(QT_LINUX "OFF")
  SET(QT "ON")
ENDIF(_wx_selected_config MATCHES "androideabi-qt")

ENDIF(DEFINED _wx_selected_config)


MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

include  ("VERSION.cmake")

#  Do the version.h configuration into the build output directory,
#  thereby allowing building from a read-only source tree.
IF(NOT SKIP_VERSION_CONFIG)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/version.h.in ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/include/version.h)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/include)
ENDIF(NOT SKIP_VERSION_CONFIG)

SET(PLUGIN_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}" )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# SET(PROFILING 1)

#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT MSVC)
 ADD_DEFINITIONS( "-fvisibility=hidden" )
 IF(PROFILING)
  ADD_DEFINITIONS( "-Wall -g -fprofile-arcs -ftest-coverage -fexceptions -fPIC" )
 ELSE(PROFILING)
#  ADD_DEFINITIONS( "-Wall -g -fexceptions" )
 ADD_DEFINITIONS( "-Wall -Wno-unused-result -g -fexceptions -fPIC" )
 ENDIF(PROFILING)

 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
 ENDIF(NOT APPLE)

ENDIF(NOT MSVC)

# Add some definitions to satisfy MS
IF(MSVC)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(MSVC)

IF(NOT DEFINED wxWidgets_USE_FILE)
  SET(wxWidgets_USE_LIBS base core net xml html adv)
ENDIF(NOT DEFINED wxWidgets_USE_FILE)


#  QT_ANDROID is a cross-build, so the native FIND_PACKAGE(wxWidgets...) and wxWidgets_USE_FILE is not useful.
#  We add the dependencies manually.
IF(QT_ANDROID)
  ADD_DEFINITIONS(-D__WXQT__)
  ADD_DEFINITIONS(-D__OCPN__ANDROID__)
  ADD_DEFINITIONS(-DOCPN_USE_WRAPPER)
  ADD_DEFINITIONS(-DANDROID)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/android)

  SET(CMAKE_CXX_FLAGS "-pthread -s")

  ## Compiler flags
 #   if(CMAKE_COMPILER_IS_GNUCXX)
 #       set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
        set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
 #   endif()

  INCLUDE_DIRECTORIES("${Qt_Base}/${Qt_Build}/include/QtCore")
  INCLUDE_DIRECTORIES("${Qt_Base}/${Qt_Build}/include")
  INCLUDE_DIRECTORIES("${Qt_Base}/${Qt_Build}/include/QtWidgets")
  INCLUDE_DIRECTORIES("${Qt_Base}/${Qt_Build}/include/QtGui")
  INCLUDE_DIRECTORIES("${Qt_Base}/${Qt_Build}/include/QtOpenGL")
  INCLUDE_DIRECTORIES("${Qt_Base}/${Qt_Build}/include/QtTest")

  INCLUDE_DIRECTORIES( "${wxQt_Base}/${wxQt_Build}/lib/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
  INCLUDE_DIRECTORIES("${wxQt_Base}/include")

  ADD_DEFINITIONS(-DQT_WIDGETS_LIB)

ENDIF(QT_ANDROID)


IF(MSYS)
# this is just a hack. I think the bug is in FindwxWidgets.cmake
STRING( REGEX REPLACE "/usr/local" "\\\\;C:/MinGW/msys/1.0/usr/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} )
ENDIF(MSYS)

#  QT_ANDROID is a cross-build, so the native FIND_PACKAGE(OpenGL) is not useful.
#
IF (NOT QT_ANDROID )

OPTION(OCPN_USE_SVG "Use SVG graphics" ON)

IF(OCPN_USE_SVG)
  ADD_DEFINITIONS(-DOCPN_USE_SVG)
ENDIF(OCPN_USE_SVG)

  
FIND_PACKAGE(OpenGL)
IF(OPENGL_GLU_FOUND)

    SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

    MESSAGE (STATUS "Found OpenGL..." )
    #MESSAGE (STATUS "    Lib: " ${OPENGL_LIBRARIES})
    #MESSAGE (STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
    ADD_DEFINITIONS(-DocpnUSE_GL)
ELSE(OPENGL_GLU_FOUND)
    MESSAGE (STATUS "OpenGL not found..." )
ENDIF(OPENGL_GLU_FOUND)
ENDIF(NOT QT_ANDROID)

#  Building for QT_ANDROID involves a cross-building environment,
#  So the OpenGL include directories, flags, etc must be stated explicitly
#  without trying to locate them on the host build system.
IF(QT_ANDROID)
  SET(CMAKE_VERBOSE_MAKEFILE ON)
    ADD_DEFINITIONS(-DocpnUSE_GLES)
    ADD_DEFINITIONS(-DocpnUSE_GL)
#    ADD_DEFINITIONS(-DUSE_GLU_TESS)
    ADD_DEFINITIONS(-DARMHF)

    SET(OPENGLES_FOUND "YES")
    SET(OPENGL_FOUND "YES")
    
      
  SET(USE_GLES2 ON )

  IF(USE_GLES2)
    MESSAGE (STATUS "Using GLESv2 for Android")
    ADD_DEFINITIONS(-DUSE_ANDROID_GLES2)
    ADD_DEFINITIONS(-DocpnUSE_GLES2)
    ADD_DEFINITIONS(-DUSE_GLSL)
  ENDIF(USE_GLES2)

ENDIF(QT_ANDROID)

IF (NOT QT_ANDROID )
    set (WXWIDGETS_FORCE_VERSION CACHE VERSION "Force usage of a specific wxWidgets version.")
    if(WXWIDGETS_FORCE_VERSION)
        set (wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
    endif()
    FIND_PACKAGE(wxWidgets REQUIRED)
    INCLUDE(${wxWidgets_USE_FILE})
ENDIF (NOT QT_ANDROID )

# On Android, PlugIns need a specific linkage set....
IF (QT_ANDROID )
  # These libraries are needed to create PlugIns on Android.

  SET(OCPN_Core_LIBRARIES
        # Presently, Android Plugins are built in the core tree, so the variables {wxQT_BASE}, etc.
        # flow to this module from above.  If we want to build Android plugins out-of-core, this will need improvement.

        # TODO This is pretty ugly, but there seems no way to avoid specifying a full path in a cross build....
        #/home/dsr/Projects/opencpn/build-opencpn-Production_build_Android_for_armeabi_v7a_GCC_4_8_Qt_5_5_0-Release/libopencpn.so
        /home/sean/build-opencpn-Production_Android_for_armeabi_v7a_Core_55_46_GCC_4_8_Qt_5_5_0-Release/libopencpn.so

        ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_core-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_html-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu_xml-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_qa-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_adv-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_aui-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu_net-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_gl-3.1-arm-linux-androideabi.a
        ${Qt_Base}/${Qt_Build}/lib/libQt5Core.so
        ${Qt_Base}/${Qt_Build}/lib/libQt5OpenGL.so
        ${Qt_Base}/${Qt_Build}/lib/libQt5Widgets.so
        ${Qt_Base}/${Qt_Build}/lib/libQt5Gui.so
        ${Qt_Base}/${Qt_Build}/lib/libQt5AndroidExtras.so
        libGLESv2.so
        libEGL.so
        )

ENDIF(QT_ANDROID)




SET(BUILD_SHARED_LIBS TRUE)

FIND_PACKAGE(Gettext REQUIRED)


IF(NOT WIN32)
 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
  ADD_DEFINITIONS( "-rdynamic" )
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
  ADD_DEFINITIONS( "-Wno-overloaded-virtual" )
 ENDIF(NOT APPLE)

ENDIF(NOT WIN32)


# Add some definitions to satisfy MS
IF(WIN32)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(WIN32)

# Let cmake find additional modules private
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})


ADD_DEFINITIONS(-DTIXML_USE_STL)

