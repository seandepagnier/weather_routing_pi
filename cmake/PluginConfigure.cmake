# ---------------------------------------------------------------------------
# Author:      Jon Gough (Based on the work of Sean D'Epagnier and Pavel Kalian) Copyright:   2019 License:     GPLv3+
# ---------------------------------------------------------------------------

set(SAVE_CMLOC ${CMLOC})
set(CMLOC "PluginConfigure: ")

message(STATUS "${CMLOC}*** Staging to build ${PACKAGE_NAME} ***")

message(STATUS "${CMLOC}CIRCLECI: ${CIRCLECLI}, Env CIRCLECI: $ENV{CIRCLECI}")
message(STATUS "${CMLOC}TRAVIS: ${TRAVIS}, Env TRAVIS: $ENV{TRAVIS}")

set(GIT_REPOSITORY "")

if($ENV{CIRCLECI})
    set(GIT_REPOSITORY "$ENV{CIRCLE_PROJECT_USERNAME}/$ENV{CIRCLE_PROJECT_REPONAME}")
    set(GIT_REPOSITORY_BRANCH "$ENV{CIRCLE_BRANCH}")
    set(GIT_REPOSITORY_TAG "$ENV{CIRCLE_TAG}")
elseif($ENV{TRAVIS})
    set(GIT_REPOSITORY "$ENV{TRAVIS_REPO_SLUG}")
    set(GIT_REPOSITORY_BRANCH "$ENV{TRAVIS_BRANCH}")
    set(GIT_REPOSITORY_TAG "$ENV{TRAVIS_TAG}")
    if("${GIT_REPOSITORY_BRANCH}" STREQUAL "${GIT_REPOSITORY_TAG}")
        # Travis sets TRAVIS_BRANCH to TRAVIS_TAG for tagged builds. Need to clear this setting
        set(GIT_REPOSITORY_BRANCH "")
    endif()
elseif($ENV{APPVEYOR})
    set(GIT_REPOSITORY "$ENV{APPVEYOR_REPO_NAME}")
    set(GIT_REPOSITORY_BRANCH "$ENV{APPVEYOR_REPO_BRANCH}")
    set(GIT_REPOSITORY_TAG "$ENV{APPVEYOR_REPO_TAG_NAME}")
else()
    # Get the current working branch
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REPOSITORY_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if("${GIT_REPOSITORY_BRANCH}" STREQUAL "")
        message(STATUS "${CMLOC}Setting default GIT repository branch - master")
        set(GIT_REPOSITORY_BRANCH "master")
    endif()
    execute_process(
        COMMAND git tag --contains
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REPOSITORY_TAG OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(
        COMMAND git status --porcelain -b
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_STATUS OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(FIND ${GIT_STATUS} "..." START_TRACKED)
    if(NOT START_TRACKED EQUAL -1)
        string(FIND ${GIT_STATUS} "/" END_TRACKED)
        math(EXPR START_TRACKED "${START_TRACKED}+3")
        math(EXPR END_TRACKED "${END_TRACKED}-${START_TRACKED}")
        string(SUBSTRING ${GIT_STATUS} ${START_TRACKED} ${END_TRACKED} GIT_REPOSITORY_REMOTE)
        message(STATUS "${CMLOC}GIT_REPOSITORY_REMOTE: ${GIT_REPOSITORY_REMOTE}")
        execute_process(
            COMMAND git remote get-url ${GIT_REPOSITORY_REMOTE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_REPOSITORY_URL OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_VARIABLE GIT_REMOTE_ERROR)
        if(NOT GIT_REMOTE_ERROR STREQUAL "")
            message(STATUS "${CMLOC}Command error: ${GIT_REMOTE_ERROR}")
            message(STATUS "${CMLOC}Using default repository")
        else()
            string(FIND ${GIT_REPOSITORY_URL} ${GIT_REPOSITORY_SERVER} START_URL REVERSE)
            string(LENGTH ${GIT_REPOSITORY_SERVER} STRING_LENGTH)
            math(EXPR START_URL "${START_URL}+1+${STRING_LENGTH}")
            string(LENGTH ${GIT_REPOSITORY_URL} STRING_LENGTH)
            message(STATUS "${CMLOC}START_URL: ${START_URL}, STRING_LENGTH: ${STRING_LENGTH}")
            string(SUBSTRING ${GIT_REPOSITORY_URL} ${START_URL} ${STRING_LENGTH} GIT_REPOSITORY)
        endif()
    else()
        message(STATUS "${CMLOC}Branch is not tracking a remote branch")
    endif()
endif()
message(STATUS "${CMLOC}GIT_REPOSITORY: ${GIT_REPOSITORY}")
message(STATUS "${CMLOC}Git Branch: \"${GIT_REPOSITORY_BRANCH}\"")
message(STATUS "${CMLOC}Git Tag: \"${GIT_REPOSITORY_TAG}\"")
if("${GIT_REPOSITORY_BRANCH}" STREQUAL "")
    set(GIT_BRANCH_OR_TAG "tag")
    set(GIT_REPOSITORY_ITEM ${GIT_REPOSITORY_TAG})
else()
    set(GIT_BRANCH_OR_TAG "branch")
    set(GIT_REPOSITORY_ITEM ${GIT_REPOSITORY_BRANCH})
endif()
message(STATUS "${CMLOC}GIT_BRANCH_OR_TAG: ${GIT_BRANCH_OR_TAG}")
message(STATUS "${CMLOC}GIT_REPOSITORY_ITEM: ${GIT_REPOSITORY_ITEM}")

if(NOT DEFINED CLOUDSMITH_BASE_REPOSITORY AND NOT ${GIT_REPOSITORY} STREQUAL "")
    string(FIND ${GIT_REPOSITORY} "/" START_NAME REVERSE)
    math(EXPR START_NAME "${START_NAME}+1")
    string(LENGTH ${GIT_REPOSITORY} STRING_LENGTH)
    message(STATUS "${CMLOC}START_NAME: ${START_NAME}, STRING_LENGTH: ${STRING_LENGTH}")
    string(SUBSTRING ${GIT_REPOSITORY} ${START_NAME} ${STRING_LENGTH} CLOUDSMITH_BASE_REPOSITORY)
endif()
message(STATUS "${CMLOC}CLOUDSMITH_BASE_REPOSITORY: ${CLOUDSMITH_BASE_REPOSITORY}")

# Do the version.h & wxWTranslateCatalog configuration into the build output directory, thereby allowing building from a read-only source tree.
if(NOT SKIP_VERSION_CONFIG)
    set(BUILD_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
    find_file(PLUGIN_EXTRA_VERSION_VARS version.h.extra ${CMAKE_CURRENT_SOURCE_DIR}/cmake/in-files )
    message(STATUS "${CMLOC}PLUGIN_EXTRA_VERSION_VARS: ${PLUGIN_EXTRA_VERSION_VARS}")
    if("${PLUGIN_EXTRA_VERSION_VARS}" STREQUAL "PLUGIN_EXTRA_VERSION_VARS-NOTFOUND")
        set(EXTRA_VERSION_INFO "")
    else()
        configure_file(${PLUGIN_EXTRA_VERSION_VARS} ${BUILD_INCLUDE_PATH}/include/version_extra.h)
        set(EXTRA_VERSION_INFO "#include version_extra.h")
    endif()
    configure_file(cmake/in-files/version.h.in ${BUILD_INCLUDE_PATH}/include/version.h)
    configure_file(cmake/in-files/wxWTranslateCatalog.h.in ${BUILD_INCLUDE_PATH}/include/wxWTranslateCatalog.h)
    include_directories(${BUILD_INCLUDE_PATH}/include)
endif(NOT SKIP_VERSION_CONFIG)

# configure xml file for circleci
message(STATUS "${CMLOC}OCPN_TARGET: $ENV{OCPN_TARGET}")
if(NOT DEFINED $ENV{OCPN_TARGET})
    set($ENV{OCPN_TARGET} ${PKG_TARGET})
    message(STATUS "${CMLOC}Setting OCPN_TARGET")
endif()

if("$ENV{BUILD_GTK3}" STREQUAL "true")
    set(PKG_TARGET_GTK "-gtk3")
    message(STATUS "${CMLOC}Found gtk3")
else()
    set(PKG_TARGET_GTK "")
endif()
message(STATUS "${CMLOC}PKG_TARGET_GTK: ${PKG_TARGET_GTK}")

if(UNIX AND NOT APPLE)
    # Handle gtk3 build variant
    string(STRIP "${PKG_TARGET}" PKG_TARGET)
    string(TOLOWER "${PKG_TARGET}" PKG_TARGET)

    # Generate architecturally uniques names for linux output packages
    if(ARCH MATCHES "arm64")
        set(PKG_TARGET_ARCH "-arm64")
    elseif(ARCH MATCHES "armhf")
        set(PKG_TARGET_ARCH "-armhf")
    elseif(ARCH MATCHES "i386")
        set(PKG_TARGET_ARCH "-i386")
    else()
        set(PKG_TARGET_ARCH "-x86_64")
    endif()
else()
    set(PKG_TARGET_ARCH "")
endif()

set(PKG_TARGET_FULL "${PKG_TARGET}${PKG_TARGET_GTK}${PKG_TARGET_ARCH}")
message(STATUS "${CMLOC}PKG_TARGET_FULL: ${PKG_TARGET}${PKG_TARGET_GTK}${PKG_TARGET_ARCH}")
message(STATUS "${CMLOC}*.in files generated in ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "${CMLOC}PACKAGING_NAME_XML: ${PACKAGING_NAME_XML}")
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/plugin.xml.in ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGING_NAME_XML}.xml)
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/pkg_version.sh.in ${CMAKE_CURRENT_BINARY_DIR}/pkg_version.sh)
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/cloudsmith-upload.sh.in ${CMAKE_CURRENT_BINARY_DIR}/cloudsmith-upload.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/PluginCPackOptions.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PluginCPackOptions.cmake @ONLY)


message(STATUS "${CMLOC}Checking OCPN_FLATPAK_CONFIG: ${OCPN_FLATPAK_CONFIG}")
if(OCPN_FLATPAK_CONFIG)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/in-files/org.opencpn.OpenCPN.Plugin.yaml.in ${CMAKE_CURRENT_BINARY_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.${PACKAGE}.yaml)

    message(STATUS "${CMLOC}Done OCPN_FLATPAK CONFIG")
    message(STATUS "${CMLOC}Directory used: ${CMAKE_CURRENT_BINARY_DIR}/flatpak")
    message(STATUS "${CMLOC}Git Branch: ${GIT_REPOSITORY_BRANCH}")
    set(CMLOC ${SAVE_CMLOC})
    return()
endif(OCPN_FLATPAK_CONFIG)


set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# SET(PROFILING 1)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions("-DDEBUG_BUILD")
    set(CMAKE_INSTALL_DO_STRIP FALSE)
    set(CPACK_DEBIAN_DEBUGINFO_PACKAGE YES)
    message(STATUS "${CMLOC}DEBUG available")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")

if(NOT WIN32 AND NOT APPLE)
    add_definitions("-Wall -Wno-unused -fexceptions -rdynamic -fvisibility=hidden")
    add_definitions(" -fno-strict-aliasing")
    message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(" -O0 -g")
        message(STATUS "${CMLOC}Optimisation: -O0 -g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_definitions(" -O2 ")
        message(STATUS "${CMLOC}Optimisation: -O2")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_definitions(" -O2 -g")
        message(STATUS "${CMLOC}Optimisation: -O2 -g")
    else(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_definitions(" -O2")
        message(STATUS "${CMLOC}Optimisation: -O2")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

    add_definitions(" -DPREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\"")
    # profiling with gprof ADD_DEFINITIONS( -pg ) SET(CMAKE_EXE_LINKER_FLAGS -pg) profiling with gcov ADD_DEFINITIONS( "-fprofile-arcs -ftest-coverage" ) SET(EXTRA_LIBS ${EXTRA_LIBS} "gcov")
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
    message(STATUS "${CMLOC}Set SNPRINTF")
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

# Search for opengles, short of running a program to test the speed of acceleration, I simply use gles on "native linux" arm systems
if(ARCH MATCHES "arm*"
   AND (NOT QT_ANDROID)
   AND USE_GL MATCHES "ON")
    find_path(OPENGLESv1_INCLUDE_DIR GLES/gl.h)
    if(OPENGLESv1_INCLUDE_DIR)
        message(STATUS "${CMLOC}Found OpenGLESv1")
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
    message(STATUS "${CMLOC}Using GLESv1 for Android")
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
        message(STATUS "${CMLOC}Finding package OpenGL")
        find_package(OpenGL)
    else(USE_GL MATCHES "ON")
        message(STATUS "${CMLOC}OpenGL disabled by option...")
    endif(USE_GL MATCHES "ON")

    if(OPENGL_FOUND)

        set(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
        include_directories(${OPENGL_INCLUDE_DIR})

        message(STATUS "${CMLOC}Found OpenGL...")
        message(STATUS "${CMLOC}    Lib: " ${OPENGL_LIBRARIES})
        message(STATUS "${CMLOC}    Include: " ${OPENGL_INCLUDE_DIR})
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
        message(STATUS "${CMLOC}    Revised GL Lib: " ${OPENGL_LIBRARIES})

    else(OPENGL_FOUND)
        message(STATUS "${CMLOC}OpenGL not found...")
    endif(OPENGL_FOUND)
endif()

if(USE_LOCAL_GLU)
    message(STATUS "${CMLOC}    Adding local GLU")
    add_subdirectory(ocpnsrc/glu)
    set(OPENGL_LIBRARIES "GLU_static" ${OPENGL_LIBRARIES})
    message(STATUS "${CMLOC}    Revised GL Lib (with local): " ${OPENGL_LIBRARIES})
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

    message(STATUS "${CMLOC}wxWidgets components: ${wxWidgets_USE_LIBS}")
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

    message(STATUS "${CMLOC}Found wxWidgets...")
    message(STATUS "${CMLOC} wxWidgets Include: ${wxWidgets_INCLUDE_DIRS}")
    message(STATUS "${CMLOC} wxWidgets Libraries: ${wxWidgets_LIBRARIES}")

    # We need to remove GLU from the wxWidgets_LIBRARIES list It only appears to get on the list for MSW...
    foreach(_currentLibFile ${wxWidgets_LIBRARIES})
        set(UCNAME ${_currentLibFile})
        string(TOUPPER ${UCNAME} UCNAME)
        if(NOT ${UCNAME} MATCHES "(.*)GLU(.*)")
            set(REVISED_wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES} ${_currentLibFile})
        endif()
    endforeach(_currentLibFile)
    set(wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES})

    message(STATUS "${CMLOC} Revised wxWidgets Libraries: ${wxWidgets_LIBRARIES}")
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
        message(STATUS "${CMLOC}Building against GTK2...")
    else(GTK2_FOUND)
        find_package(GTK3)
        include_directories(${GTK3_INCLUDE_DIRS})
        set(GTK_LIBRARIES ${GTK3_LIBRARIES})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXGTK3__")
        set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
        message(STATUS "${CMLOC}Building against GTK3...")
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

set(CMLOC ${SAVE_CMLOC})
