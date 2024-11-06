# ---------------------------------------------------------------------------
# Author:      Pavel Kalian (Based on the work of Sean D'Epagnier) Copyright:
# 2014 License:     GPLv3+
# ---------------------------------------------------------------------------

set(SAVE_CMLOC ${CMLOC})
set(CMLOC "PluginPackage: ")

# build a FLATPAK installer package
message(STATUS "${CMLOC}Package: OCPN_FLATPAK_CONFIG: ${OCPN_FLATPAK_CONFIG}")

if (OCPN_FLATPAK_CONFIG)
  # On a flatpak build lib libraries such as LibGL and wxWidgets are only
  # available in the flatpak sandbox. Thus, building flatpak must be done before
  # attempts to locate these non-existing libraries in the host i. e., before
  # any FindLibrary(), FindWxWidgets(), etc.
  find_program(TAR NAMES gtar tar)
  if (NOT TAR)
    message(FATAL_ERROR "${CMLOC}tar not found, required for OCPN_FLATPAK")
  endif ()
  add_custom_target(
    flatpak-build ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/flatpak
    COMMAND
      cat
      ${CMAKE_CURRENT_BINARY_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.${PACKAGE}.yaml
    COMMAND
      /usr/bin/flatpak-builder --force-clean -v ${CMAKE_CURRENT_BINARY_DIR}/app
      ${CMAKE_CURRENT_BINARY_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.${PACKAGE}.yaml
  )
  add_custom_target("flatpak-pkg")
  add_custom_command(
    TARGET flatpak-pkg
    COMMAND
      ${TAR} -czf
      ${PKG_NVR}-${ARCH}${PKG_TARGET_WX_VER}_${PKG_TARGET_NVR}.tar.gz --verbose
      --transform 's|.*/files/|${PACKAGE}-flatpak-${PACKAGE_VERSION}/|'
      ${CMAKE_CURRENT_BINARY_DIR}/app/files
    COMMAND chmod -R a+wr ../build
  )

  message(
    STATUS
      "${CMLOC}Zip file name: ${PKG_NVR}-${ARCH}${PKG_TARGET_WX_VER}_${PKG_TARGET_NVR}.tar.gz"
  )
  set(CMLOC ${SAVE_CMLOC})
  return()
endif (OCPN_FLATPAK_CONFIG)

# build a CPack driven installer package SET(CPACK_PACKAGE_NAME
# "${PACKAGE_NAME}")
set(CPACK_PACKAGE_CONTACT ${PACKAGE_CONTACT})
set(CPACK_PACKAGE_VENDOR "opencpn.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_NAME} ${PACKAGE_VERSION})
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}-${OCPN_MIN_VERSION}")
set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_CURRENT_BINARY_DIR};${PACKAGE_NAME};ALL;/"
)
message(STATUS "${CMLOC}CPACK_PACKAGE_NAME: ${CPACK_PACKAGE_NAME}")

if (WIN32)
  # The TGZ (tar.gz) is used by experimental plugin manager,
  set(CPACK_GENERATOR "NSIS;TGZ")

  # override install directory to put package files in the opencpn directory
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenCPN")

  set(CPACK_NSIS_PACKAGE_NAME "${PACKAGE_NAME}")

  # Let cmake find NSIS.template.in
  list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/buildwin")

  # These lines set the name of the Windows Start Menu shortcut and the icon
  # that goes with it
  set(CPACK_NSIS_DISPLAY_NAME "OpenCPN ${PACKAGE_NAME}")

  set(CPACK_PACKAGE_FILE_NAME
      "${PACKAGE_FILE_NAME}_${CPACK_PACKAGE_VERSION}-${OCPN_MIN_VERSION}_win32"
  )
  message(STATUS "${CMLOC}CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}")

  set(CPACK_NSIS_DIR "${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode") # Gunther
  set(CPACK_BUILDWIN_DIR "${PROJECT_SOURCE_DIR}/buildwin") # Gunther

  message(STATUS "${CMLOC}FILE: ${CPACK_PACKAGE_FILE_NAME}")
else (WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PACKAGE_NAME})
endif (WIN32)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CPACK_STRIP_FILES FALSE)
  message(STATUS "${CMLOC}Not stripping debug information from module")
else (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  set(CPACK_STRIP_FILES TRUE)
  message(STATUS "${CMLOC}Stripping debug information from module")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gpl.txt")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README")
  message(STATUS "${CMLOC}Using generic cpack package description file.")
  set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
endif ()

# SET(CPACK_SOURCE_GENERATOR "TGZ")

# The following components are regex's to match anywhere (unless anchored) in
# absolute path + filename to find files or directories to be excluded from
# source tarball.
set(CPACK_SOURCE_IGNORE_FILES
    "^${CMAKE_CURRENT_SOURCE_DIR}/.git/*" "^${CMAKE_CURRENT_SOURCE_DIR}/build*"
    "^${CPACK_PACKAGE_INSTALL_DIRECTORY}/*"
)

if (UNIX AND NOT APPLE)

  # need apt-get install rpm, for rpmbuild
  set(PACKAGE_DEPS "${PACKAGE_DEPS},opencpn, bzip2, gzip")
  message(STATUS "${CMLOC}PACKAGE_DEPS: ${PACKAGE_DEPS}")
  if (NOT QT_ANDROID)
    set(CPACK_GENERATOR "DEB;TGZ")
  else ()
    set(CPACK_GENERATOR "TGZ")
  endif ()

  set(CPACK_DEBIAN_PACKAGE_NAME ${PACKAGING_NAME})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS ${PACKAGE_DEPS})
  set(CPACK_DEBIAN_PACKAGE_RECOMMENDS ${PACKAGE_RECS})
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
  set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
  set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
  set(CPACK_DEBIAN_COMPRESSION_TYPE "xz") # requires my patches to cmake
  set(CPACK_CMAKE_GENERATOR Ninja)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PACKAGE_NAME} PlugIn for OpenCPN")
  set(CPACK_PACKAGE_DESCRIPTION "${PACKAGE_NAME} PlugIn for OpenCPN")
  set(CPACK_SET_DESTDIR ON)

endif (UNIX AND NOT APPLE)

if (NOT STANDALONE MATCHES "BUNDLED")
  # MacOS .pkg installer is deprecated in OCPN 5.6.2+ if(APPLE)
  if (FALSE)
    message(STATUS "${CMLOC}*** Staging to build PlugIn OSX Package ***")

    # Copy a bunch of files so the Packages installer builder can find them
    # relative to ${CMAKE_CURRENT_BINARY_DIR} This avoids absolute paths in the
    # chartdldr_pi.pkgproj file

    configure_file(
      ${PROJECT_SOURCE_DIR}/cmake/gpl.txt
      ${CMAKE_CURRENT_BINARY_DIR}/license.txt COPYONLY
    )

    configure_file(
      ${PROJECT_SOURCE_DIR}/buildosx/InstallOSX/pkg_background.jpg
      ${CMAKE_CURRENT_BINARY_DIR}/pkg_background.jpg COPYONLY
    )

    # Patch the pkgproj.in file to make the output package name conform to
    # Xxx-Plugin_x.x.pkg format Key is: <key>NAME</key>
    # <string>${VERBOSE_NAME}-Plugin_${VERSION_MAJOR}.${VERSION_MINOR}</string>

    configure_file(
      ${PROJECT_SOURCE_DIR}/buildosx/InstallOSX/plugin.pkgproj.in
      ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}.pkgproj
    )

    add_custom_command(
      OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}-Plugin_${PACKAGE_VERSION}_${OCPN_MIN_VERSION}.pkg
      COMMAND /usr/local/bin/packagesbuild -F ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}.pkgproj
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS ${PACKAGE_NAME}
      COMMENT "create-pkg [${PACKAGE_NAME}]: Generating pkg file."
    )

    add_custom_target(
      create-pkg
      COMMENT "create-pkg: Done."
      DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}-Plugin_${PACKAGE_VERSION}_${OCPN_MIN_VERSION}.pkg
    )

  endif (FALSE)

  set(CPACK_GENERATOR "TGZ")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PACKAGE_NAME} PlugIn for OpenCPN")
  set(CPACK_PACKAGE_DESCRIPTION "${PACKAGE_NAME} PlugIn for OpenCPN")
  set(CPACK_PACKAGE_FILE_NAME "${PACKAGING_NAME_XML}")
  message(STATUS "${CMLOC}CPACK_PACKAGE_FILE_NAME: ${CPACK_PACKAGE_FILE_NAME}")

  if (WIN32)
    set(CPACK_SET_DESTDIR OFF)
    message(STATUS "${CMLOC}FILE: ${CPACK_PACKAGE_FILE_NAME}")
    add_custom_command(
      OUTPUT ${CPACK_PACKAGE_FILE_NAME}
      COMMAND
        signtool sign /v /f \\cert\\OpenCPNSPC.pfx /d http://www.opencpn.org /t
        http://timestamp.verisign.com/scripts/timstamp.dll
        ${CPACK_PACKAGE_FILE_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS ${PACKAGE_NAME}
      COMMENT "Code-Signing: ${CPACK_PACKAGE_FILE_NAME}"
    )
    add_custom_target(
      codesign
      COMMENT "code signing: Done."
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}
    )
  endif (WIN32)

  message(
    STATUS
      "${CMLOC}CPACK_PACKAGE_VERSION: ${CPACK_PACKAGE_VERSION}, PACKAGE_VERSION ${PACKAGE_VERSION}, CPACK_PACKAGE_FILE_NAME: ${CPACK_PACKAGE_FILE_NAME}"
  )

  set(CPACK_PROJECT_CONFIG_FILE
      "${CMAKE_CURRENT_BINARY_DIR}/PluginCPackOptions.cmake"
  )
  message(
    STATUS
      "${CMLOC}PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}, CPACK_PROJECT_CONFIG_FILE: ${CPACK_PROJECT_CONFIG_FILE}"
  )

  include(CPack)

endif (NOT STANDALONE MATCHES "BUNDLED")

set(CMLOC ${SAVE_CMLOC})
