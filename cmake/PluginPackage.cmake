# build a CPack driven installer package
#include (InstallRequiredSystemLibraries)

SET (LIB_INSTALL_DIR "lib")
SET (PACKAGE_FORMAT "DEB")
SET (PACKAGE_DEPS "libc6, libwxgtk2.8-0 (>= 2.8.7.1), libglu1-mesa (>= 7.0.0), libgl1-mesa-glx (>= 7.0.0), zlib1g, bzip2")

SET(CPACK_PACKAGE_NAME "S63_PI")
SET(CPACK_PACKAGE_VENDOR "opencpn.org")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_NAME} ${PACKAGE_VERSION})
SET(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};${PACKAGE_NAME};ALL;/")
SET(CPACK_PACKAGE_EXECUTABLES ${PACKAGE_NAME} "S63_pi")



IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.

  SET(CPACK_NSIS_INSTALLED_ICON_NAME "s63_pi.dll")
  SET(CPACK_NSIS_PACKAGE_NAME_LC "s63_pi")

#  These lines set the name of the Windows Start Menu shortcut and the icon that goes with it
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME} ${PACKAGE_VERSION}")
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\opencpn.ico")

  SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}_setup" )

  SET(CPACK_NSIS_DIR "${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode")  #Gunther
  SET(CPACK_BUILDWIN_DIR "${CMAKE_SOURCE_DIR}/buildwin")  #Gunther

ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${PACKAGE_NAME})
ENDIF(WIN32 AND NOT UNIX)

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/license.txt")
IF (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README")
#    MESSAGE(STATUS "Using generic cpack package description file.")
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
ENDIF ()
SET(CPACK_SOURCE_GENERATOR "TGZ")

# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
set(CPACK_SOURCE_IGNORE_FILES
"\\\\.cvsignore$"
"^${CMAKE_CURRENT_SOURCE_DIR}.*/CVS/"
"^${CMAKE_CURRENT_SOURCE_DIR}/build*"
"^${CPACK_PACKAGE_INSTALL_DIRECTORY}/*"
)

IF(APPLE)
#  SET(CPACK_GENERATOR "Bundle")
#  SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
#  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/buildosx/opencpn.icns")
#  SET(CPACK_BUNDLE_NAME "${PACKAGE_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
#  SET(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/buildosx/opencpn.icns")
#  SET(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/buildosx/Info.plist")

#  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/buildosx/README.txt")
#  SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/buildosx/README.txt")

#  SET(CPACK_PACKAGING_INSTALL_PREFIX "/")

ENDIF(APPLE)


IF(UNIX AND NOT APPLE)
#    INCLUDE(UseRPMTools)
#    IF(RPMTools_FOUND)
#        RPMTools_ADD_RPM_TARGETS(packagename ${CMAKE_SOURCE_DIR}/package.spec)
#    ENDIF(RPMTools_FOUND)


    SET(CPACK_GENERATOR ${PACKAGE_FORMAT})
    SET(CPACK_PACKAGE_CONTACT "David S. Register ")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS ${PACKAGE_DEPS})
    SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS ${PACKAGE_RECS})
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
    SET(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
#    SET(CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
#    SET(CPACK_RPM_PACKAGE_ARCHITECTURE  ${ARCH})
#    SET(CPACK_RPM_PACKAGE_REQUIRES  ${PACKAGE_DEPS})
#    SET(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_SOURCE_DIR}/opencpn.spec.in")
#    SET(CPACK_OPENCPN_RPM_BINDIR "${CMAKE_INSTALL_PREFIX}/${PREFIX_BIN}")
#    SET(CPACK_OPENCPN_RPM_LIBDIR "${PREFIX_LIB}")
#    SET(CPACK_OPENCPN_RPM_DATADIR "${CMAKE_INSTALL_PREFIX}/${PREFIX_DATA}")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PACKAGE_NAME} PlugIn for OpenCPN")
    SET(CPACK_PACKAGE_DESCRIPTION "${PACKAGE_NAME} PlugIn for OpenCPN")
    SET(CPACK_SET_DESTDIR ON)
    SET(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}_${PACKAGE_VERSION}-${PACKAGE_RELEASE}_${ARCH}" )


ENDIF(UNIX AND NOT APPLE)

IF(TWIN32 AND NOT UNIX)
# configure_file(${CMAKE_SOURCE_DIR}/src/opencpn.rc.in ${CMAKE_SOURCE_DIR}/src/opencpn.rc)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_GERMAN.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_GERMAN.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_FRENCH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_FRENCH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_CZECH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_CZECH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_DANISH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_DANISH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_SPANISH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_SPANISH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_ITALIAN.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_ITALIAN.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_DUTCH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_DUTCH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_POLISH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_POLISH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_PORTUGUESEBR.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_PORTUGUESEBR.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_PORTUGUESE.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_PORTUGUESE.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_RUSSIAN.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_RUSSIAN.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_SWEDISH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_SWEDISH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_FINNISH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_FINNISH.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_NORWEGIAN.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_NORWEGIAN.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_CHINESETW.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_CHINESETW.nsh" @ONLY)
 configure_file("${CMAKE_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_TURKISH.nsh.in" "${CMAKE_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_TURKISH.nsh" @ONLY)
ENDIF(TWIN32 AND NOT UNIX)


# this dummy target is necessary to make sure the ADDITIONAL_MAKE_CLEAN_FILES directive is executed.
# apparently, the base CMakeLists.txt file must have "some" target to activate all the clean steps.
#ADD_CUSTOM_TARGET(dummy COMMENT "dummy: Done." DEPENDS ${PACKAGE_NAME})


INCLUDE(CPack)

IF(APPLE)

 ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.dmg
   COMMAND ${CMAKE_SOURCE_DIR}/buildosx/create-dmg ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.dmg ${CMAKE_INSTALL_PREFIX}/s63_pi/
   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
   COMMENT "create-dmg [${PACKAGE_NAME}]: Generated dmg file."
)

 ADD_CUSTOM_TARGET(create-dmg COMMENT "create-dmg: Done."
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.dmg)



ENDIF(APPLE)
