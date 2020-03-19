# ---------------------------------------------------------------------------
# Author:      Pavel Kalian (Based on the work of Sean D'Epagnier) Copyright:   2014 License:     GPLv3+
# ---------------------------------------------------------------------------

if(OCPN_FLATPAK_CONFIG)
  return()
endif(OCPN_FLATPAK_CONFIG)

if(NOT APPLE)
  target_link_libraries(${PACKAGE_NAME} ${wxWidgets_LIBRARIES} ${EXTRA_LIBS})
endif(NOT APPLE)

if(WIN32)
  if(MSVC)
    # TARGET_LINK_LIBRARIES(${PACKAGE_NAME} gdiplus.lib glu32.lib)
    target_link_libraries(${PACKAGE_NAME} ${OPENGL_LIBRARIES})

    set(OPENCPN_IMPORT_LIB "${CMAKE_SOURCE_DIR}/api-16/opencpn.lib")
  endif(MSVC)

  if(MINGW)
    # assuming wxwidgets is compiled with unicode, this is needed for mingw headers
    add_definitions(" -DUNICODE")
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${OPENGL_LIBRARIES})
#    SET(OPENCPN_IMPORT_LIB "${PARENT}.dll")
    SET( CMAKE_SHARED_LINKER_FLAGS "-L../buildwin" )
#    target_link_libraries(${PACKAGE_NAME} ${OPENGL_LIBRARIES})
    set(OPENCPN_IMPORT_LIB "${CMAKE_SOURCE_DIR}/api-16/libopencpn.dll.a")
  endif(MINGW)

  target_link_libraries(${PACKAGE_NAME} ${OPENCPN_IMPORT_LIB})
endif(WIN32)

if(UNIX)
  if(PROFILING)
    find_library(
      GCOV_LIBRARY
      NAMES gcov
      PATHS /usr/lib/gcc/i686-pc-linux-gnu/4.7)

    set(EXTRA_LIBS ${EXTRA_LIBS} ${GCOV_LIBRARY})
  endif(PROFILING)
endif(UNIX)

if(APPLE)
  install(
    TARGETS ${PACKAGE_NAME}
    RUNTIME
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/OpenCPN.app/Contents/SharedSupport/plugins)
  install(
    TARGETS ${PACKAGE_NAME}
    RUNTIME
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/OpenCPN.app/Contents/PlugIns)
  if(EXISTS ${PROJECT_SOURCE_DIR}/data)
    install(DIRECTORY data DESTINATION ${CMAKE_BINARY_DIR}/OpenCPN.app/Contents/SharedSupport/plugins/${PACKAGE_NAME})
  endif()

  find_package(ZLIB REQUIRED)
  target_link_libraries(${PACKAGE_NAME} ${ZLIB_LIBRARIES})

endif(APPLE)

if(UNIX AND NOT APPLE)
  find_package(BZip2 REQUIRED)
  include_directories(${BZIP2_INCLUDE_DIR})
  find_package(ZLIB REQUIRED)
  include_directories(${ZLIB_INCLUDE_DIR})
  target_link_libraries(${PACKAGE_NAME} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARY})
endif(UNIX AND NOT APPLE)

set(PARENT opencpn)

# Based on code from nohal
if(NOT WIN32)
  # default
  set(ARCH "i386")
  set(LIB_INSTALL_DIR "lib")
  if(EXISTS /etc/debian_version)
    set(PACKAGE_FORMAT "DEB")
    set(PACKAGE_DEPS "libc6, libwxgtk3.0-0, wx3.0-i18n, libglu1-mesa (>= 7.0.0), libgl1-mesa-glx (>= 7.0.0), zlib1g, bzip2, libportaudio2")
    set(PACKAGE_RECS "xcalib,xdg-utils")
    set(LIB_INSTALL_DIR "lib")
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(ARCH "amd64")
      # SET (LIB_INSTALL_DIR "lib64")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(ARCH "i386")
    endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
    # SET(TENTATIVE_PREFIX "/usr/local")
    set(TENTATIVE_PREFIX "/usr")
  endif(EXISTS /etc/debian_version)
  if(EXISTS /etc/redhat-release)
    set(PACKAGE_FORMAT "RPM")
    # SET (PACKAGE_DEPS  "wxGTK mesa-libGLU mesa-libGL gettext zlib bzip2 portaudio")
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(ARCH "x86_64")
      set(LIB_INSTALL_DIR "lib64")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(ARCH "i386")
      set(LIB_INSTALL_DIR "lib")
    endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
  endif(EXISTS /etc/redhat-release)
  if(EXISTS /etc/suse-release OR EXISTS /etc/SuSE-release)
    set(PACKAGE_FORMAT "RPM")
    # SET (PACKAGE_DEPS  "libwx_baseu-2_8-0-wxcontainer MesaGLw libbz2-1 portaudio")
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(ARCH "x86_64")
      set(LIB_INSTALL_DIR "lib64")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(ARCH "i386")
      set(LIB_INSTALL_DIR "lib")
    endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
  endif(EXISTS /etc/suse-release OR EXISTS /etc/SuSE-release)
  if(EXISTS /etc/gentoo-release)
    set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}")
  endif(EXISTS /etc/gentoo-release)
  if(APPLE)
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      # IF (CMAKE_OSX_64)
      set(ARCH "x86_64")
      # ENDIF (CMAKE_OSX_64)
    endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
  endif()
else(NOT WIN32)
  # On WIN32 probably CMAKE_SIZEOF_VOID_P EQUAL 8, but we don't use it at all now...
  set(ARCH "i386")
endif(NOT WIN32)

if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX ${TENTATIVE_PREFIX})
endif(NOT CMAKE_INSTALL_PREFIX)

message(STATUS "*** Will install to ${CMAKE_INSTALL_PREFIX}  ***")
set(PREFIX_DATA share)
set(PREFIX_PKGDATA ${PREFIX_DATA}/${PACKAGE_NAME})
#set(PREFIX_LIB "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
SET(PREFIX_LIB lib)

if(WIN32)
  message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
  set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/../OpenCPN)
  if(CMAKE_CROSSCOMPILING)
    install(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")
    set(INSTALL_DIRECTORY "plugins/${PACKAGE_NAME}")
  else(CMAKE_CROSSCOMPILING)
    install(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")
    set(INSTALL_DIRECTORY "plugins\\\\${PACKAGE_NAME}")
  endif(CMAKE_CROSSCOMPILING)

  if(EXISTS ${PROJECT_SOURCE_DIR}/data)
    install(DIRECTORY data DESTINATION "${INSTALL_DIRECTORY}")
    message(STATUS "Install Data: ${INSTALL_DIRECTORY}")
  endif(EXISTS ${PROJECT_SOURCE_DIR}/data)

  # fix for missing dll's FILE(GLOB gtkdll_files "${CMAKE_CURRENT_SOURCE_DIR}/buildwin/gtk/*.dll") INSTALL(FILES ${gtkdll_files} DESTINATION ".") FILE(GLOB expatdll_files
  # "${CMAKE_CURRENT_SOURCE_DIR}/buildwin/expat-2.1.0/*.dll") INSTALL(FILES ${expatdll_files} DESTINATION ".")

endif(WIN32)

if(UNIX AND NOT APPLE)
  set(PREFIX_PARENTDATA ${PREFIX_DATA}/${PARENT})
  set(PREFIX_PARENTLIB ${PREFIX_LIB}/${PARENT})
  message(STATUS "PREFIX_PARENTLIB: ${PREFIX_PARENTLIB}")
  install(
    TARGETS ${PACKAGE_NAME}
    RUNTIME
    LIBRARY DESTINATION ${PREFIX_PARENTLIB})

  if(EXISTS ${PROJECT_SOURCE_DIR}/data)
    install(DIRECTORY data DESTINATION ${PREFIX_PARENTDATA}/plugins/${PACKAGE_NAME})
    message(STATUS "Install data: ${PREFIX_PARENTDATA}/plugins/${PACKAGE_NAME}")
  endif()
  if(EXISTS ${PROJECT_SOURCE_DIR}/UserIcons)
    install(DIRECTORY UserIcons DESTINATION ${PREFIX_PARENTDATA}/plugins/${PACKAGE_NAME})
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROJECT_SOURCE_DIR}/script/postinst")
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${PROJECT_SOURCE_DIR}/script/postinst")
    message(STATUS "Install UserIcons: ${PREFIX_PARENTDATA}/plugins/${PACKAGE_NAME}")
  endif()
endif(UNIX AND NOT APPLE)

if(APPLE)
  # For Apple build, we need to copy the "data" directory contents to the build directory, so that the packager can pick them up.
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/data/")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data/")
    message("Generating data directory")
  endif()

  file(GLOB_RECURSE PACKAGE_DATA_FILES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/data/*)

  foreach(_currentDataFile ${PACKAGE_DATA_FILES})
    message(STATUS "copying: ${_currentDataFile}")
    file(COPY ${_currentDataFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
  endforeach(_currentDataFile)

  install(
    TARGETS ${PACKAGE_NAME}
    RUNTIME
    LIBRARY DESTINATION ${PACKAGE_NAME}/${PACKAGE_NAME})
  message(STATUS "Install Target: ${PACKAGE_NAME}/${PACKAGE_NAME}")

endif(APPLE)
