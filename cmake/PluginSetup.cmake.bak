# ---------------------------------------------------------------------------
# Author:      Jon Gough based on the work of Pavel Kalian (Based on the work of Sean D'Epagnier) Copyright:   2020 License:     GPLv3+
# ---------------------------------------------------------------------------

set(SAVE_CMLOC ${CMLOC})
set(CMLOC "PluginSetup: ")

# Make sure cmake sub directory files can be found by cmake
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(NOT DEFINED GIT_REPOSITORY_SERVER)
    set(GIT_REPOSITORY_SERVER "github.com")
    message(STATUS "${CMLOC}GIT_REPOSITORY_SERVER not found setting to: ${GIT_REPOSITORY_SERVER}")
endif()

# Export variables used in plugin setup: GIT_HASH, GIT_COMMIT, PKG_TARGET, PKG_TARGET_VERSION and PKG_NVR
if(NOT ${PACKAGE} MATCHES "(.*)_pi")
    set(PACKAGE_NAME ${PACKAGE}_pi)
    set(PACKAGE_FILE_NAME "${PACKAGE}_pi")
else(NOT ${PACKAGE} MATCHES "(.*)_pi")
    set(PACKAGE_NAME ${PACKAGE})
    set(PACKAGE_FILE_NAME "${PACKAGE}")
endif(NOT ${PACKAGE} MATCHES "(.*)_pi")
string(TOUPPER "${PACKAGE}" TITLE_NAME)

# add library for use later
add_library(${PACKAGE_NAME} SHARED)

project(${PACKAGE} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})
message(STATUS "${CMLOC}PROJECT_VERSION: ${PROJECT_VERSION}")

set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")

message(STATUS "${CMLOC}${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")
message(STATUS "${CMLOC}OPCN_FLATPAK: ${OCPN_FLATPAK}")

set(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION})
set(PKG_URL "https://dl.cloudsmith.io/public/--pkg_repo--/raw/names/--name--/versions/--version--/--filename--")

# check to make sure we have a git repository
execute_process(
    COMMAND git status $>/dev/null
    RESULT_VARIABLE GIT_REPOSITORY_EXISTS
    OUTPUT_QUIET
    ERROR_QUIET)
if("${GIT_REPOSITORY_EXISTS}" STREQUAL "0")
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(
        COMMAND git log -1 --format=%ci
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(GIT_HASH "")
    set(GIT_COMMIT_DATE "")
endif()

message(STATUS "${CMLOC}OCPN_FLATPAK_CONFIG: ${OCPN_FLATPAK_CONFIG}, OCPN_FLATPAK_BUILD: ${OCPN_FLATPAK_BUILD}, UNIX: ${UNIX}")
unset(PKG_TARGET_WX_VER)
if(OCPN_FLATPAK_CONFIG OR OCPN_FLATPAK_BUILD)
    set(PKG_TARGET "flatpak")
    set(PKG_TARGET_VERSION "${SDK_VER}") # As of flatpak/*yaml
    message(STATUS "${CMLOC}PKG_TARGET_VERSION: ${PKG_TARGET_VERSION}")
    if( NOT "${WX_VER}" STREQUAL "" )
        set(PKG_TARGET_WX_VER "-${WX_VER}")
    else()
        unset(PKG_TARGET_WX_VER)
    endif()
    message(STATUS "${CMLOC}PKG_TARGET_WX_VER: ${PKG_TARGET_WX_VER}")
elseif(MINGW)
    set(PKG_TARGET "mingw")
    if(CMAKE_SYSTEM_VERSION)
        set(PKG_TARGET_VERSION ${CMAKE_SYSTEM_VERSION})
    else()
        set(PKG_TARGET_VERSION 10)
    endif()
elseif(MSVC)
    set(PKG_TARGET "msvc")
    message(STATUS "${CMLOC}WX_VER: $ENV{WX_VER}")
    if( NOT "$ENV{WX_VER}" STREQUAL "" )
        set(PKG_TARGET_WX_VER "-wx$ENV{WX_VER}")
    else()
        unset(PKG_TARGET_WX_VER)
    endif()
    message(STATUS "${CMLOC}PKG_TARGET_WX_VER: ${PKG_TARGET_WX_VER}, WX_VER: $ENV{WX_VER}")
    if(CMAKE_SYSTEM_VERSION)
        set(PKG_TARGET_VERSION ${CMAKE_SYSTEM_VERSION})
    elseif(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
        set(PKG_TARGET_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
    else()
        set(PKG_TARGET_VERSION 10)
    endif()
elseif(APPLE)
    set(PKG_BUILD_TARGET "darwin")
    if("$ENV{WXVERSION}" STREQUAL "32")
        set(PKG_TARGET "darwin-wx$ENV{WXVERSION}")
    else()
        set(PKG_TARGET "${PKG_BUILD_TARGET}")
    endif()
    execute_process(COMMAND "sw_vers" "-productVersion" OUTPUT_VARIABLE PKG_TARGET_VERSION)
elseif(_wx_selected_config MATCHES "androideabi-qt-arm64")
    set(PKG_TARGET "Android")
    set(PKG_TARGET_VERSION 16)
    set(QT_ANDROID ON)
elseif(_wx_selected_config MATCHES "androideabi-qt-armhf")
    set(PKG_TARGET "Android")
    set(PKG_TARGET_VERSION 16)
    set(QT_ANDROID ON)
elseif(UNIX)
    # Some linux dist:
    execute_process(COMMAND "lsb_release" "-is" OUTPUT_VARIABLE PKG_TARGET)
    execute_process(COMMAND "lsb_release" "-rs" OUTPUT_VARIABLE PKG_TARGET_VERSION)
else()
    set(PKG_TARGET "unknown")
    set(PKG_TARGET_VERSION 1)
endif()

if(${BUILD_GTK3})
    message(STATUS "${CMLOC}set CMAKE defined BUILD_GTK3: ${BUILD_GTK3}")
    set(ENV{BUILD_GTK3} ${BUILD_GTK3})
endif()

if(NOT WIN32 AND NOT QT_ANDROID)
    # default
    set(ARCH "i386")
    if(UNIX AND NOT APPLE)

        message(STATUS "${CMLOC}*** Will install to ${CMAKE_INSTALL_PREFIX}  ***")

        if(EXISTS /etc/debian_version)
            message(STATUS "${CMLOC}*** Debian detected  ***")
            set(PACKAGE_FORMAT "DEB")
            set(PACKAGE_DEPS "libc6, libwxgtk3.0-0, wx3.0-i18n, libglu1-mesa (>= 7.0.0), libgl1-mesa-glx (>= 7.0.0), zlib1g, bzip2, libportaudio2")
            set(PACKAGE_RECS "xcalib,xdg-utils")
            set(LIB_INSTALL_DIR "lib")
            if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
                if(CMAKE_SIZEOF_VOID_P MATCHES "8")
                    if("$ENV{OCPN_TARGET}" MATCHES "flatpak")
                        message(STATUS "${CMLOC}*** Flatpak detected  ***")
                        set(ARCH "aarch64")
                    else("$ENV{OCPN_TARGET}" MATCHES "flatpak")
                        set(ARCH "arm64")
                    endif("$ENV{OCPN_TARGET}" MATCHES "flatpak")
                    add_definitions(-DOCPN_ARM64)
                else(CMAKE_SIZEOF_VOID_P MATCHES "8")
                    set(ARCH "armhf")
                    add_definitions(-DOCPN_ARMHF)
                endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
            else(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
                if(CMAKE_SIZEOF_VOID_P MATCHES "8")
                    set(ARCH "x86_64")
                else(CMAKE_SIZEOF_VOID_P MATCHES "8")
                    set(ARCH "i386")
                endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
            endif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
        endif(EXISTS /etc/debian_version)
        if(NOT DEFINED PACKAGE_FORMAT)
            if("$ENV{OCPN_TARGET}" MATCHES "flatpak")
                message(STATUS "*** Flatpak detected  ***")
                set(PACKAGE_FORMAT "TGZ")
                set(ARCH "aarch64")
                set(LIB_INSTALL_DIR "lib")
            endif("$ENV{OCPN_TARGET}" MATCHES "flatpak")
        endif(NOT DEFINED PACKAGE_FORMAT)

        if(NOT DEFINED PACKAGE_FORMAT)
            if(EXISTS /etc/redhat-release)
                message(STATUS "${CMLOC}*** Redhat detected  ***")
                set(PACKAGE_FORMAT "RPM")
                set(PACKAGE_DEPS "opencpn")

                if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
                    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
                        set(ARCH "aarch64")
                        add_definitions(-DOCPN_ARM64)
                    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
                        set(ARCH "armhf")
                        add_definitions(-DOCPN_ARMHF)
                    endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
                else(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
                    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
                        set(ARCH "x86_64")
                        set(LIB_INSTALL_DIR "lib64")
                    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
                        set(ARCH "i386")
                        set(LIB_INSTALL_DIR "lib")
                    endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
                endif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")

            endif(EXISTS /etc/redhat-release)
        endif(NOT DEFINED PACKAGE_FORMAT)

        if(NOT DEFINED PACKAGE_FORMAT)
            if(EXISTS /etc/os-release
               OR EXISTS /etc/sysconfig/SuSEfirewall2.d
               OR EXISTS /etc/suse-release
               OR EXISTS /etc/SuSE-release)
                message(STATUS "${CMLOC}*** OpenSUSE detected  ***")
                set(PACKAGE_FORMAT "RPM")
                set(PACKAGE_DEPS "opencpn")
                if(CMAKE_SIZEOF_VOID_P MATCHES "8")
                    set(ARCH "x86_64")
                    set(LIB_INSTALL_DIR "lib64")
                else(CMAKE_SIZEOF_VOID_P MATCHES "8")
                    set(ARCH "i386")
                    set(LIB_INSTALL_DIR "lib")
                endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
            endif(
                EXISTS /etc/os-release
                OR EXISTS /etc/sysconfig/SuSEfirewall2.d
                OR EXISTS /etc/suse-release
                OR EXISTS /etc/SuSE-release)
        endif(NOT DEFINED PACKAGE_FORMAT)

    endif(UNIX AND NOT APPLE)

    if(APPLE)
        set(ARCH "x86_64")
    endif(APPLE)

else(NOT WIN32 AND NOT QT_ANDROID)
    set(ARCH "x86_64")
    if(_wx_selected_config MATCHES "androideabi-qt-arm64")
        set(ARCH "arm64")
        # android cannot used graphics context is wxWidgets as it does not exist
        set(wxUSE_GRAPHICS_CONTEXT 0)
    endif(_wx_selected_config MATCHES "androideabi-qt-arm64")
    if(_wx_selected_config MATCHES "androideabi-qt-armhf")
        set(ARCH "armhf")
        # android cannot used graphics context is wxWidgets as it does not exist
        set(wxUSE_GRAPHICS_CONTEXT 0)
    endif(_wx_selected_config MATCHES "androideabi-qt-armhf")
endif(NOT WIN32 AND NOT QT_ANDROID)

message(STATUS "${CMLOC}ARCH: ${ARCH}")

string(STRIP ${PKG_TARGET} PKG_TARGET)
string(TOLOWER ${PKG_TARGET} PKG_TARGET)
string(STRIP ${PKG_TARGET_VERSION} PKG_TARGET_VERSION)
string(TOLOWER ${PKG_TARGET_VERSION} PKG_TARGET_VERSION)
set(PKG_TARGET_NVR ${PKG_TARGET}-${PKG_TARGET_VERSION})
message(STATUS "${CMLOC}PKG_TARGET: ${PKG_TARGET}, PKG_TARGET_VERSION: ${PKG_TARGET_VERSION}")

# Allow OCPN_TARGET to be used on the cmake command line
message(STATUS "${CMLOC}OCPN_TARGET: $ENV{OCPN_TARGET}")
if(NOT DEFINED ENV{OCPN_TARGET})
    if(DEFINED OCPN_TARGET)
        message(STATUS "${CMLOC}OCPN_TARGET: ${OCPN_TARGET}")
        set(ENV{OCPN_TARGET} ${OCPN_TARGET})
    else()
        message(STATUS "${CMLOC}PKG_TARGET: ${PKG_TARGET}")
    endif()
endif()


set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${ARCH}-${PKG_TARGET_VERSION})

if(Plugin_CXX11)
    message(STATUS "${CMLOC}Attempting to use c++11")
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        set(Plugin_USE_CXX11 ON)
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        set(Plugin_USE_CXX11 ON)
    else()
        message(STATUS "${CMLOC}The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
        set(Plugin_USE_CXX11 OFF)
    endif()
else()
    set(Plugin_USE_CXX11 OFF)
endif()

if(Plugin_USE_CXX11)
    message(STATUS "${CMLOC}Allowing use of c++11")
endif()

message(STATUS "${CMLOC}CMAKE version: ${CMAKE_VERSION}")
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
            message(STATUS "${CMLOC}clang-tidy found: ${CLANG_TIDY_EXE}")
            # For more, see http://clang.llvm.org/extra/clang-tidy/ set(CLANG_TIDY_CHECKS "-*,modernize-*")
            set(CLANG_TIDY_CHECKS "-*,performance-*")
            set(CMAKE_CXX_CLANG_TIDY
                "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS};-header-filter='${PROJECT_SOURCE_DIR}/*'"
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

find_program(HAVE_LD_SO
    PATHS /lib NO_DEFAULT_PATH
    NAMES ld.so ld-linux.so.1  ld-linux.so.2
)

set(CMAKE_SKIP_BUILD_RPATH true)
if (HAVE_LD_SO)   # linux.
    message(STATUS "${CMLOC}Setting RPATH: \$ORIGIN:\$ORIGIN/..")
    set(CMAKE_BUILD_RPATH "\$ORIGIN;\$ORIGIN/..")
    set(CMAKE_INSTALL_RPATH "\$ORIGIN;\$ORIGIN/..")
endif ()

set(CMLOC ${SAVE_CMLOC})
