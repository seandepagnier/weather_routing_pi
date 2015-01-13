##---------------------------------------------------------------------------
## Author:      Pavel Kalian / Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------

FIND_PROGRAM(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
string(REPLACE "_pi" "" I18N_NAME ${PACKAGE_NAME})
IF (GETTEXT_XGETTEXT_EXECUTABLE)
  ADD_CUSTOM_COMMAND(
    OUTPUT po/${PACKAGE_NAME}.pot.dummy
    COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} --force-po --package-name=${PACKAGE_NAME} --package-version="${PACKAGE_VERSION}" --output=po/${PACKAGE_NAME}.pot  --keyword=_ --width=80 --files-from=${CMAKE_CURRENT_SOURCE_DIR}/po/POTFILES.in
    DEPENDS po/POTFILES.in po/${PACKAGE_NAME}.pot
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${I18N_NAME}-pot-update [${PACKAGE_NAME}]: Generated pot file."
    )
  ADD_CUSTOM_TARGET(${I18N_NAME}-pot-update COMMENT "[${PACKAGE_NAME}]-pot-update: Done." DEPENDS po/${PACKAGE_NAME}.pot.dummy)

ENDIF(GETTEXT_XGETTEXT_EXECUTABLE )

MACRO(GETTEXT_UPDATE_PO _potFile)
  SET(_poFiles ${_potFile})
  GET_FILENAME_COMPONENT(_absPotFile ${_potFile} ABSOLUTE)

  FOREACH (_currentPoFile ${ARGN})
    GET_FILENAME_COMPONENT(_absFile ${_currentPoFile} ABSOLUTE)
    GET_FILENAME_COMPONENT(_poBasename ${_absFile} NAME_WE)

    ADD_CUSTOM_COMMAND(
      OUTPUT ${_absFile}.dummy
      COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --width=80 --strict --quiet --update --backup=none --no-location -s ${_absFile} ${_absPotFile}
      DEPENDS ${_absPotFile} ${_absFile}
      COMMENT "${I18N_NAME}-po-update [${_poBasename}]: Updated po file."
      )

    SET(_poFiles ${_poFiles} ${_absFile}.dummy)

  ENDFOREACH (_currentPoFile )

  ADD_CUSTOM_TARGET(${I18N_NAME}-po-update COMMENT "[${PACKAGE_NAME}]-po-update: Done." DEPENDS ${_poFiles})
ENDMACRO(GETTEXT_UPDATE_PO)

IF (GETTEXT_MSGMERGE_EXECUTABLE)
  FILE(GLOB PACKAGE_PO_FILES po/*.po)
  GETTEXT_UPDATE_PO(po/${PACKAGE_NAME}.pot ${PACKAGE_PO_FILES})
ENDIF(GETTEXT_MSGMERGE_EXECUTABLE)

SET(_gmoFiles)
MACRO(GETTEXT_BUILD_MO)
  FOREACH (_poFile ${ARGN})
    GET_FILENAME_COMPONENT(_absFile ${_poFile} ABSOLUTE)
    GET_FILENAME_COMPONENT(_poBasename ${_absFile} NAME_WE)
    SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_poBasename}.mo)

IF(APPLE)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${_gmoFile}
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_absFile}
      COMMAND ${CMAKE_COMMAND} -E copy ${_gmoFile} "Resources/${_poBasename}.lproj/opencpn-${PACKAGE_NAME}.mo"
      DEPENDS ${_absFile}
      COMMENT "${I18N_NAME}-i18n [${_poBasename}]: Created mo file."
      )
ELSE(APPLE)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${_gmoFile}
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_absFile}
      COMMAND ${CMAKE_COMMAND} -E copy ${_gmoFile} "Resources/${_poBasename}/LC_MESSAGES/opencpn-${PACKAGE_NAME}.mo"
      DEPENDS ${_absFile}
      COMMENT "${I18N_NAME}-i18n [${_poBasename}]: Created mo file."
      )
ENDIF(APPLE)

## Why are we copying files to “/use/local/bin/OpenCPN.app” under OSX? Shouldn’t we copy to “/Applications/OpenCPN.app”?
## Now the languages files will never be used when we do make install.

    IF(APPLE)
      INSTALL(FILES ${_gmoFile} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/OpenCPN.app/Contents/Resources/${_poBasename}.lproj RENAME opencpn-${PACKAGE_NAME}.mo )
    ELSE(APPLE)
      INSTALL(FILES ${_gmoFile} DESTINATION ${PREFIX_DATA}/locale/${_poBasename}/LC_MESSAGES RENAME opencpn-${PACKAGE_NAME}.mo )
    ENDIF(APPLE)

    SET(_gmoFiles ${_gmoFiles} ${_gmoFile})
  ENDFOREACH (_poFile )
ENDMACRO(GETTEXT_BUILD_MO)

if(GETTEXT_MSGFMT_EXECUTABLE)
  FILE(GLOB PACKAGE_PO_FILES po/*.po)
  GETTEXT_BUILD_MO(${PACKAGE_PO_FILES})
  ADD_CUSTOM_TARGET(${I18N_NAME}-i18n COMMENT "${PACKAGE_NAME}-i18n: Done." DEPENDS ${_gmoFiles})
  ADD_DEPENDENCIES(${PACKAGE_NAME} ${I18N_NAME}-i18n)
ENDIF(GETTEXT_MSGFMT_EXECUTABLE)

IF(WIN32)
  FIND_PROGRAM(ZIP_EXECUTABLE wzzip PATHS "$ENV{ProgramFiles}/WinZip")
  IF(ZIP_EXECUTABLE)
    SET(ZIP_COMMAND "${ZIP_EXECUTABLE} -P")
  ENDIF(ZIP_EXECUTABLE)
ENDIF(WIN32)

IF(UNIX)
  FIND_PROGRAM(ZIP_EXECUTABLE zip)
  IF(ZIP_EXECUTABLE)
    SET(ZIP_COMMAND "${ZIP_EXECUTABLE} -r")
  ENDIF(ZIP_EXECUTABLE)
ENDIF(UNIX)

##
## This doesn't seem to work -- does everything, sets the variables, but doesn't
## actually build the zip file.  Does anyone have any idea how to do this?
##
MACRO(BUILD_LANGUAGES_ZIP zipFile)
  ADD_CUSTOM_COMMAND(
    OUTPUT "${zipFile}"
    COMMAND ${ZIP_COMMAND} "${zipFile}" "Resources/*"
    COMMENT "Created zip file ${zipFile} using ${ZIP_COMMAND}"
  )
ENDMACRO(BUILD_LANGUAGES_ZIP)

IF(ZIP_COMMAND)
  BUILD_LANGUAGES_ZIP(${PACKAGE_NAME}_Languages.zip)
  ADD_CUSTOM_TARGET(${PACKAGE_NAME}_Languages.zip COMMENT "${PACKAGE_NAME}_Languages.zip: Done." DEPENDS ${_gmoFiles})
  ADD_DEPENDENCIES(${PACKAGE_NAME} ${PACKAGE_NAME}_Languages.zip)
ENDIF(ZIP_COMMAND)
