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
    if("${GIT_REPOSITORY_EXISTS}" STREQUAL "0")
        # Get the current working branch
        execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_REPOSITORY_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if("${GIT_REPOSITORY_BRANCH}" STREQUAL "")
            message(STATUS "${CMLOC}Setting default GIT repository branch - master")
            set(GIT_REPOSITORY_BRANCH "master")
        endif()
        execute_process(
            COMMAND git tag --contains
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_REPOSITORY_TAG OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(
            COMMAND git status --porcelain -b
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
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
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
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
    else()
        message(STATUS "${CMLOC}This directory does not contain git or git is not available")
        set(GIT_REPOSITORY "")
        set(GIT_REPOSITORY_BRANCH "")
        set(GIT_REPOSITORY_TAG "")
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

# Process files in in-files sub directory into the build output directory, thereby allowing building from a read-only source tree.
if(NOT SKIP_VERSION_CONFIG)
    if(MINGW)
        message(STATUS "${CMLOC}Temporarily allowing different search path for MINGW")
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE_SAVE ${CMAKE_FIND_ROOT_PATH_MODE_INCLUDE})
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    endif()
    set(BUILD_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
    unset(PLUGIN_EXTRA_VERSION_VARS CACHE)
    find_file(PLUGIN_EXTRA_VERSION_VARS NAMES version.h.extra PATHS cmake/in-files NO_DEFAULT_PATH )
    if(${PLUGIN_EXTRA_VERSION_VARS} STREQUAL "PLUGIN_EXTRA_VERSION_VARS-NOTFOUND")
        message(STATUS "${CMLOC}PLUGIN_EXTRA_VERSION_VARS: Not Found")
        set(EXTRA_VERSION_INFO "")
    else()
        message(STATUS "${CMLOC}PLUGIN_EXTRA_VERSION_VARS: Found")
        configure_file(${PLUGIN_EXTRA_VERSION_VARS} ${BUILD_INCLUDE_PATH}/include/version_extra.h)
        set(EXTRA_VERSION_INFO "#include \"version_extra.h\"")
    endif()
    find_file(PLUGIN_EXTRA_FORMBUILDER_HEADERS NAMES extra_formbuilder_headers.h.in PATHS cmake/in-files NO_DEFAULT_PATH)
    message(STATUS "${CMLOC}PLUGIN_EXTRA_FORMBUILDER_HEADERS: ${PLUGIN_EXTRA_FORMBUILDER_HEADERS}")
    if(${PLUGIN_EXTRA_FORMBUILDER_HEADERS} STREQUAL "PLUGIN_EXTRA_FORMBUILDER_HEADERS-NOTFOUND")
        message(STATUS "${CMLOC}PLUGIN_EXTRA_FORMBUILDER_HEADERS: Not found")
    else()
        message(STATUS "${CMLOC}PLUGIN_EXTRA_FORMBUILDER_HEADERS: Found")
        configure_file(${PLUGIN_EXTRA_FORMBUILDER_HEADERS} ${BUILD_INCLUDE_PATH}/include/extra_formbuilder_headers.h)
    endif()
    configure_file(cmake/in-files/version.h.in ${BUILD_INCLUDE_PATH}/include/version.h)
    configure_file(cmake/in-files/wxWTranslateCatalog.h.in ${BUILD_INCLUDE_PATH}/include/wxWTranslateCatalog.h)
    include_directories(${BUILD_INCLUDE_PATH}/include)
    if(MINGW)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ${CMAKE_FIND_ROOT_PATH_MODE_INCLUDE_SAVE})
    endif()
endif(NOT SKIP_VERSION_CONFIG)

# configure xml file for circleci

message(STATUS "${CMLOC}ENV BUILD_GTK3: $ENV{BUILD_GTK3}")
string(TOUPPER "$ENV{BUILD_GTK3}" BUILD_GTK3_TEMP)
set(ENV{BUILD_GTK3} ${BUILD_GTK3_TEMP})
unset(BUILD_GTK3_TEMP)
string(TOUPPER "$ENV{BUILD_ENV}" BUILD_ENV_TEMP)
message(STATUS "${CMLOC}BUILD_ENV: $ENV{BUILD_ENV}, BUILD_ENV_TEMP ${BUILD_ENV_TEMP}, OCPN_TARGET: $ENV{OCPN_TARGET}")
message(STATUS "${CMLOC}Doing build_gtk3: $ENV{BUILD_GTK3}")
if("$ENV{BUILD_GTK3}" STREQUAL "TRUE" AND ( ("${BUILD_ENV_TEMP}" STREQUAL "UBUNTU" AND NOT "$ENV{OCPN_TARGET}" STREQUAL "jammy") OR "$ENV{OCPN_TARGET}" STREQUAL "buster-armhf"))
    set(PKG_TARGET_GTK "gtk3")
    message(STATUS "${CMLOC}Found gtk3")
else()
    unset(PKG_TARGET_GTK)
endif()
message(STATUS "${CMLOC}PKG_TARGET_GTK: ${PKG_TARGET_GTK}")

#message(STATUS "${CMLOC}ENV WX_VER: $ENV{WX_VER}")
#if(NOT "$ENV{WX_VER}" STREQUAL "")
#    set(PKG_TARGET_WX_VER "-$ENV{WX_VER}")
#else()
#    set(PKG_TARGET_WX_VER "")
#endif()

if(UNIX AND NOT APPLE)
    # Handle gtk3 build variant
    string(STRIP "${PKG_TARGET}" PKG_TARGET)
    string(TOLOWER "${PKG_TARGET}" PKG_TARGET)

    # Generate architecturally uniques names for linux output packages
    if(ARCH MATCHES "aarch64")
        set(PKG_TARGET_ARCH "-aarch64")
    elseif(ARCH MATCHES "arm64")
        set(PKG_TARGET_ARCH "-arm64")
    elseif(ARCH MATCHES "armhf")
        set(PKG_TARGET_ARCH "-armhf")
    elseif(ARCH MATCHES "i386")
        set(PKG_TARGET_ARCH "-i386")
    elseif(ARCH MATCHES "amd64")
        set(PKG_TARGET_ARCH "-amd64")
    else()
        set(PKG_TARGET_ARCH "-x86_64")
    endif()
else()
    if(MINGW)
        set(PKG_TARGET_ARCH "-x86_64")
    else(MINGW)
        set(PKG_TARGET_ARCH "")
    endif(MINGW)
endif()

message(STATUS "${CMLOC}ARCH: ${ARCH}")

if("${PKG_BUILD_TARGET}" STREQUAL "")
    set(PKG_BUILD_TARGET "${PKG_TARGET}")
    set(PKG_TARGET_BUILD "-${PKG_BUILD_TARGET}")
endif()

if(NOT WIN32 AND NOT APPLE AND NOT QT_ANDROID)
    find_package(GTK2)

    if(GTK2_FOUND AND NOT "$ENV{BUILD_GTK3}" STREQUAL "TRUE")
        set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk2)
        include_directories(${GTK2_INCLUDE_DIRS})
        set(GTK_LIBRARIES ${GTK2_LIBRARIES})
        set(PKG_BUILT_WITH_GTK "gtk2")
        message(STATUS "${CMLOC}Building against GTK2...")
    else(GTK2_FOUND AND NOT "$ENV{BUILD_GTK3}" STREQUAL "TRUE")
        find_package(GTK3)
        if(GTK3_FOUND)
            include_directories(${GTK3_INCLUDE_DIRS})
            set(GTK_LIBRARIES ${GTK3_LIBRARIES})
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXGTK3__")
            set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
            set(PKG_BUILT_WITH_GTK "gtk3")
            message(STATUS "${CMLOC}Building against GTK3...")
        else(GTK3_FOUND)
            message(STATUS "${CMLOC} Unix: Neither FATAL_ERROR GTK2 nor GTK3")
            set(PKG_BUILT_WITH_GTK "")
        endif(GTK3_FOUND)
    endif(GTK2_FOUND AND NOT "$ENV{BUILD_GTK3}" STREQUAL "TRUE")
    set(EXTRA_LIBS ${EXTRA_LIBS} ${GTK_LIBRARIES})
else()
    set(PKG_BUILT_WITH_GTK "${PKG_TARGET_GTK}")
endif(NOT WIN32 AND NOT APPLE AND NOT QT_ANDROID)

if(NOT "${PKG_TARGET_GTK}"  STREQUAL "")
    set(PKG_BUILD_GTK "-${PKG_TARGET_GTK}")
    set(PKG_TARGET_GTK "-${PKG_TARGET_GTK}")
endif()

message(STATUS "${CMLOC}PKG_NVR: ${PKG_NVR}, PKG_TARGET: ${PKG_TARGET}, ARCH: ${ARCH}, PKG_TARGET_WX_VER: ${PKG_TARGET_WX_VER}, PKG_BUILD_GTK: ${PKG_BUILD_GTK}, PKG_TARGET_VERSION: ${PKG_TARGET_VERSION}")
if(DEFINED ENV{OCPN_TARGET})
    message(STATUS "${CMLOC}OCPN_TARGET defined: $ENV{OCPN_TARGET}")
    if(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD OR MINGW)
        set(PACKAGING_NAME "${PKG_NVR}-${PKG_TARGET}-${ARCH}${PKG_TARGET_WX_VER}${PKG_BUILD_GTK}-${PKG_TARGET_VERSION}-$ENV{OCPN_TARGET}")
        set(PACKAGING_NAME_XML "${PKG_NVR}-${PKG_TARGET}-${ARCH}${PKG_TARGET_WX_VER}${PKG_BUILD_GTK}-${PKG_TARGET_VERSION}-$ENV{OCPN_TARGET}")
    else(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD OR MINGW)
        set(PACKAGING_NAME "${PKG_NVR}-${PKG_TARGET}-${PKG_TARGET_VERSION}${PKG_BUILD_GTK}-$ENV{OCPN_TARGET}")
        set(PACKAGING_NAME_XML "${PKG_NVR}-${PKG_TARGET}-${ARCH}-${PKG_TARGET_VERSION}${PKG_BUILD_GTK}-$ENV{OCPN_TARGET}")
    endif(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD OR MINGW)
else()
    if(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD OR MINGW)
        set(PACKAGING_NAME "${PKG_NVR}-${PKG_TARGET}-${ARCH}-${PKG_TARGET_VERSION}${PKG_BUILD_GTK}")
        set(PACKAGING_NAME_XML "${PKG_NVR}-${PKG_TARGET}-${ARCH}-${PKG_TARGET_VERSION}${PKG_BUILD_GTK}")
    else(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD OR MINGW)
        set(PACKAGING_NAME "${PKG_NVR}-${PKG_TARGET}-${PKG_TARGET_VERSION}${PKG_BUILD_GTK}")
        set(PACKAGING_NAME_XML "${PKG_NVR}-${PKG_TARGET}-${ARCH}-${PKG_TARGET_VERSION}${PKG_BUILD_GTK}")
    endif(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD OR MINGW)
endif(DEFINED ENV{OCPN_TARGET})
if(OCPN_FLATPAK_CONFIG)
    message(STATUS "${CMLOG}Checking for flatpak extensions")
    find_file(PLUGIN_FLATPAK_ARGS NAMES flatpak_args.in PATHS cmake/in-files NO_DEFAULT_PATH )
    if(${PLUGIN_FLATPAK_ARGS} STREQUAL "PLUGIN_FLATPAK_ARGS-NOTFOUND")
        message(STATUS "${CMLOC}PLUGIN_FLATPAK_ARGS: Not Found")
        set(finish_args "")
    else()
        message(STATUS "${CMLOC}finish_args: Found")
        message(STATUS "${CMLOC}PLUGIN_FLATPAK_ARGS: ${PLUGIN_FLATPAK_ARGS}")
        file(READ ${PLUGIN_FLATPAK_ARGS} finish_args)
    endif()
    find_file(PLUGIN_FLATPAK_OPTIONS NAMES flatpak_options.in PATHS cmake/in-files NO_DEFAULT_PATH )
    if(${PLUGIN_FLATPAK_ARGS} STREQUAL "PLUGIN_FLATPAK_OPTIONS-NOTFOUND")
        message(STATUS "${CMLOC}PLUGIN_FLATPAK_OPTIONS: Not Found")
        set(build_options "")
    else()
        message(STATUS "${CMLOC}build_options: Found")
        file(READ ${PLUGIN_FLATPAK_OPTIONS} build_options)
    endif()
    find_file(PLUGIN_FLATPAK_EXTRA_MODULES NAMES flatpak_extra_modules.in PATHS cmake/in-files NO_DEFAULT_PATH )
    if(${PLUGIN_FLATPAK_EXTRA_MODULES} STREQUAL "PLUGIN_FLATPAK_EXTRA_MODULES-NOTFOUND")
        message(STATUS "${CMLOC}PLUGIN_FLATPAK_EXTRA_MODULES: Not Found")
        set(flatpak_extra_modules "")
    else()
        message(STATUS "${CMLOC}flatpak_extra_modules: Found")
        file(READ ${PLUGIN_FLATPAK_EXTRA_MODULES} flatpak_extra_modules)
    endif()
endif()
message(STATUS "${CMLOC}PACKAGING_NAME: ${PACKAGING_NAME}")
message(STATUS "${CMLOC}PACKAGING_NAME_XML: ${PACKAGING_NAME_XML}")

set(PKG_TARGET_FULL "${PKG_TARGET}${PKG_TARGET_GTK}${PKG_TARGET_ARCH}${PKG_TARGET_WX_VER}")
message(STATUS "${CMLOC}PKG_TARGET_FULL: ${PKG_TARGET_FULL}")
message(STATUS "${CMLOC}PKG_BUILD_TARGET: ${PKG_BUILD_TARGET}")
message(STATUS "${CMLOC}PKG_BUILD_GTK: ${PKG_TARGET_GTK}")
message(STATUS "${CMLOC}PKG_BUILT_WITH_GTK: ${PKG_BUILT_WITH_GTK}")
message(STATUS "${CMLOC}*.in files generated in ${CMAKE_CURRENT_BINARY_DIR}")
configure_file(${PROJECT_SOURCE_DIR}/cmake/in-files/plugin.xml.in ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGING_NAME_XML}.xml)
configure_file(${PROJECT_SOURCE_DIR}/cmake/in-files/pkg_version.sh.in ${CMAKE_CURRENT_BINARY_DIR}/pkg_version.sh)
configure_file(${PROJECT_SOURCE_DIR}/cmake/in-files/cloudsmith-upload.sh.in ${CMAKE_CURRENT_BINARY_DIR}/cloudsmith-upload.sh @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/cmake/in-files/PluginCPackOptions.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PluginCPackOptions.cmake @ONLY)

if(OCPN_FLATPAK_CONFIG)
    #set(SDK_VER $ENV{SDK_VER})
    #  Hack for temporary "beta" status of 20.08 runtime
    #  See new substitution variable in cmake/in-files/org.opencpn.OpenCPN.Plugin.yaml.in
    message(STATUS "${CMLOC}FLATPAK_BRANCH: ${FLATPAK_BRANCH}")
    set(RUNTIME_VERSION ${FLATPAK_BRANCH})

    message(STATUS "${CMLOC}Checking OCPN_FLATPAK_CONFIG: ${OCPN_FLATPAK_CONFIG}, SDK_VER: ${SDK_VER}, WX_VER: ${WX_VER}")
    configure_file(${PROJECT_SOURCE_DIR}/cmake/in-files/org.opencpn.OpenCPN.Plugin.yaml.in ${CMAKE_CURRENT_BINARY_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.${PACKAGE}.yaml)

    message(STATUS "${CMLOC}Done OCPN_FLATPAK CONFIG")
    message(STATUS "${CMLOC}Directory used: ${CMAKE_CURRENT_BINARY_DIR}/flatpak")
    message(STATUS "${CMLOC}Git Branch: ${GIT_REPOSITORY_BRANCH}")
    set(CMLOC ${SAVE_CMLOC})
    return()
endif(OCPN_FLATPAK_CONFIG)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

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
        add_definitions(" -O2 -s")
        message(STATUS "${CMLOC}Optimisation: -O2 -s")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_definitions(" -O2 -g")
        message(STATUS "${CMLOC}Optimisation: -O2 -g")
    else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        add_definitions(" -O2 -s")
        message(STATUS "${CMLOC}Optimisation: -O2 -s")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

    add_definitions(" -DPREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\"")
    # profiling with gprof ADD_DEFINITIONS( -pg ) SET(CMAKE_EXE_LINKER_FLAGS -pg) profiling with gcov ADD_DEFINITIONS( "-fprofile-arcs -ftest-coverage" ) SET(EXTRA_LIBS ${EXTRA_LIBS} "gcov")
endif(NOT WIN32 AND NOT APPLE)

if(MINGW)
    add_definitions("-Wall -Wno-unused -Wno-cpp -fexceptions")
    add_definitions(" -g -fno-strict-aliasing")
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        add_link_options(-Wl,--strip-all)
    endif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
endif(MINGW)

if(APPLE)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wno-unused -fexceptions -Wno-overloaded-virtual")
    string(APPEND CMAKE_CXX_FLAGS " -g -fno-strict-aliasing")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-deprecated -Wno-deprecated-declarations -Wno-unknown-pragmas")
    string(APPEND CMAKE_CXX_FLAGS " -D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_")
    string(APPEND CMAKE_CXX_FLAGS " -DAPPLE")
endif(APPLE)

# Add some definitions to satisfy MS
if(MSVC)
    add_definitions(-D__MSVC__)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D HAVE_SNPRINTF)
    message(STATUS "${CMLOC}Set SNPRINTF")
else(MSVC)
    if(NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-Bsymbolic")
    else(NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl -undefined dynamic_lookup")
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
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
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

        set(OPENGL_LIBRARIES "GL_static" "EGL" "X11" "drm")
    endif()
endif()

IF(DEFINED _wx_selected_config)
    MESSAGE(STATUS "${CMLOC}_wx_select_config defined as $ENV{_wx_select_config}")
    IF(_wx_selected_config MATCHES "androideabi-qt")
        MESSAGE (STATUS "${CMLOC}Qt_Base: " ${Qt_Base})
        MESSAGE (STATUS "${CMLOC}wxQt_Base/Build: " ${wxQt_Base} "/" ${wxQt_Build})
        ADD_DEFINITIONS(-DocpnUSE_GLES)
        ADD_DEFINITIONS(-DocpnUSE_GL)
        ADD_DEFINITIONS(-DARMHF)

        SET(OPENGLES_FOUND "YES")
        SET(OPENGL_FOUND "YES")

        ADD_DEFINITIONS(-DUSE_GLU_TESS)
        SET(USE_GLES2 ON )
        MESSAGE (STATUS "${CMLOC}Using GLESv2 for Android")
        ADD_DEFINITIONS(-DUSE_ANDROID_GLES2)
        ADD_DEFINITIONS(-DUSE_GLSL)
        include_directories( ${PROJECT_SOURCE_DIR}/libs/glshim/include/GLES )
        set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} ${PROJECT_SOURCE_DIR}/libs/glshim/include/GLES)

    ENDIF(_wx_selected_config MATCHES "androideabi-qt")
ENDIF(DEFINED _wx_selected_config)

# Building for QT_ANDROID involves a cross-building environment, So the include directories, flags, etc must be stated explicitly without trying to locate them on the host build system.
IF(QT_ANDROID)
    MESSAGE(STATUS "${CMLOC}Processing QT_ANDROID")
    ADD_DEFINITIONS(-D__WXQT__)
    ADD_DEFINITIONS(-D__OCPN__ANDROID__)
    ADD_DEFINITIONS(-DOCPN_USE_WRAPPER)
    ADD_DEFINITIONS(-DANDROID)

    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-soname,libgorp.so ")

    #set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    SET(CMAKE_CXX_FLAGS "-pthread -fPIC")

    ## Compiler flags
    add_compile_options("-Wno-inconsistent-missing-override"
    "-Wno-potentially-evaluated-expression"
    "-Wno-overloaded-virtual"
    "-Wno-unused-command-line-argument"
    "-Wno-unknown-pragmas"
      )

    message(STATUS "${CMLOC}Adding libgorp.o shared library")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-soname,libgorp.so ")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")  ## Strip binary

    SET(QT_LINUX "OFF")
    SET(QT "ON")
    SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
    ADD_DEFINITIONS(-DQT_WIDGETS_LIB)
    if("$ENV{OCPN_TARGET}" STREQUAL "android-arm64")
        MESSAGE(STATUS "${CMLOC}Adding definition -DARM64")
        ADD_DEFINITIONS(-DARM64)
    else()
        MESSAGE(STATUS "${CMLOC}Adding definition -DARMHF")
        ADD_DEFINITIONS(-DARMHF)
    endif()

ENDIF(QT_ANDROID)

if((NOT OPENGLES_FOUND) AND (NOT QT_ANDROID))

    if(USE_GL MATCHES "ON")
        message(STATUS "${CMLOC}Finding package OpenGL")
        find_package(OpenGL)
    else(USE_GL MATCHES "ON")
        message(STATUS "${CMLOC}OpenGL disabled by option...")
    endif(USE_GL MATCHES "ON")

    if(USE_LOCAL_GLU)
        message(STATUS "${CMLOC}    Adding local GLU")
        add_subdirectory(libs/glu)
        message(STATUS "${CMLOC}PACKAGE_NAME: ${PACKAGE_NAME}")
        target_link_libraries(${PACKAGE_NAME} "GLU_static")
        #set(OPENGL_LIBRARIES "GLU_static" ${OPENGL_LIBRARIES})
        add_definitions(-DocpnUSE_GL)
        message(STATUS "${CMLOC}    Revised GL Lib (with local): " ${OPENGL_LIBRARIES})
    elseif(OPENGL_FOUND)

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

    else()
        message(STATUS "${CMLOC}OpenGL not found...")
    endif()
endif()

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

    if(WIN32 OR APPLE OR QT_ANDROID)
        if(MSYS)
            # this is to convert msys to windows paths, and handle the missing /usr
            string(REGEX REPLACE "/usr/local" ";C:/MinGW/msys/1.0/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS})
        endif(MSYS)
    endif(WIN32 OR APPLE OR QT_ANDROID)

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
else(NOT QT_ANDROID)
    IF(_wx_selected_config MATCHES "androideabi-qt-arm64")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtCore")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtWidgets")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtGui")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtOpenGL")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtTest")

        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/libarm64/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/include")

        SET(wxWidgets_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5Core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5OpenGL.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5Widgets.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5Gui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5AndroidExtras.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/opencpn/API-117/libarm64/libgorp.so

        -lc++_shared
        -lz
        libGLESv2.so
        libEGL.so
          )

    ELSE(_wx_selected_config MATCHES "androideabi-qt-arm64")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtCore")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtWidgets")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtGui")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtOpenGL")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtTest")

        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/libarmhf/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/include")

        ADD_DEFINITIONS( -DOCPN_ARMHF )

        SET(wxWidgets_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5Core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5OpenGL.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5Widgets.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5Gui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5AndroidExtras.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/opencpn/API-117/libarmhf/libgorp.so

        -lc++_shared
        -lz
        libGLESv2.so
        libEGL.so
          )

    ENDIF(_wx_selected_config MATCHES "androideabi-qt-arm64")
endif(NOT QT_ANDROID)

find_package(Gettext REQUIRED)

set(CMLOC ${SAVE_CMLOC})
