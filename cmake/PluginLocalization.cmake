# ---------------------------------------------------------------------------
# Author:      Pavel Kalian / Sean D'Epagnier Copyright: License:     GPLv3+
# ---------------------------------------------------------------------------

set(SAVE_CMLOC ${CMLOC})
set(CMLOC "PluginLocalization: ")

if (OCPN_FLATPAK_CONFIG)
  set(CMLOC ${SAVE_CMLOC})
  return()
endif (OCPN_FLATPAK_CONFIG)

message(STATUS "${CMLOC}Starting POTFILE generation")

set(POTFILE ${CMAKE_CURRENT_SOURCE_DIR}/po/POTFILES.in)
file(REMOVE ${POTFILE}.test)
file(WRITE ${POTFILE}.test "")
message(
  STATUS
    "${CMLOC}Checking file: ${CMAKE_CURRENT_SOURCE_DIR}/po/${PACKAGE_NAME}.pot"
)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/po/${PACKAGE_NAME}.pot)
  message(
    STATUS "${CMLOC}Found: ${CMAKE_CURRENT_SOURCE_DIR}/po/${PACKAGE_NAME}.pot"
  )
else ()
  file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/po/${PACKAGE_NAME}.pot "")
  message(
    STATUS
      "${CMLOC}Creating empty ${CMAKE_CURRENT_SOURCE_DIR}/po/${PACKAGE_NAME}.pot"
  )
endif ()
foreach (POTLINE IN ITEMS ${SRCS})
  file(APPEND ${POTFILE}.test "${POTLINE}\n")
endforeach (POTLINE)
foreach (POTLINE IN ITEMS ${HDRS})
  file(APPEND ${POTFILE}.test "${POTLINE}\n")
endforeach (POTLINE)
# convert crlf to lf for consistency and make copy_if_different work correctly
configure_file(${POTFILE}.test ${POTFILE}.test NEWLINE_STYLE UNIX)
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${POTFILE}.test ${POTFILE}
  OUTPUT_QUIET ERROR_QUIET
)

find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
string(REPLACE "_pi" "" I18N_NAME ${PACKAGE_NAME})
if (GETTEXT_XGETTEXT_EXECUTABLE)
  add_custom_command(
    OUTPUT po/${PACKAGE_NAME}.pot.dummy
    COMMAND
      ${GETTEXT_XGETTEXT_EXECUTABLE} --force-po -F
      --package-name=${PACKAGE_NAME} --package-version="${PACKAGE_VERSION}"
      --output=po/${PACKAGE_NAME}.pot --keyword=_ --width=80
      --files-from=${CMAKE_CURRENT_SOURCE_DIR}/po/POTFILES.in
    DEPENDS po/POTFILES.in po/${PACKAGE_NAME}.pot
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${I18N_NAME}-pot-update [${PACKAGE_NAME}]: Generated pot file."
  )
  add_custom_target(
    ${I18N_NAME}-pot-update
    COMMENT "[${PACKAGE_NAME}]-pot-update: Done."
    DEPENDS po/${PACKAGE_NAME}.pot.dummy
  )

endif (GETTEXT_XGETTEXT_EXECUTABLE)

macro (GETTEXT_UPDATE_PO _potFile)
  set(_poFiles ${_potFile})
  get_filename_component(_absPotFile ${_potFile} ABSOLUTE)

  foreach (_currentPoFile ${ARGN})
    get_filename_component(_absFile ${_currentPoFile} ABSOLUTE)
    get_filename_component(_poBasename ${_absFile} NAME_WE)

    add_custom_command(
      OUTPUT ${_absFile}.dummy
      COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --width=80 --strict --quiet
              --update --backup=none --no-location -s ${_absFile} ${_absPotFile}
      DEPENDS ${_absPotFile} ${_absFile}
      COMMENT "${I18N_NAME}-po-update [${_poBasename}]: Updated po file."
    )

    set(_poFiles ${_poFiles} ${_absFile}.dummy)

  endforeach (_currentPoFile)

  add_custom_target(
    ${I18N_NAME}-po-update
    COMMENT "[${PACKAGE_NAME}]-po-update: Done."
    DEPENDS ${_poFiles}
  )
endmacro (GETTEXT_UPDATE_PO)

if (GETTEXT_MSGMERGE_EXECUTABLE)
  file(GLOB PACKAGE_PO_FILES po/*.po)
  gettext_update_po(po/${PACKAGE_NAME}.pot ${PACKAGE_PO_FILES})
endif (GETTEXT_MSGMERGE_EXECUTABLE)

set(_gmoFiles)
macro (GETTEXT_BUILD_MO)
  file(MAKE_DIRECTORY "Resources")
  message(STATUS "${CMLOC}Creating Resources directory")
  add_custom_target(
    create_resources_dir ALL COMMAND ${CMAKE_COMMAND} -E make_directory
                                     "./Resources"
  )
  foreach (_poFile ${ARGN})
    get_filename_component(_absFile ${_poFile} ABSOLUTE)
    get_filename_component(_poBasename ${_absFile} NAME_WE)
    set(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_poBasename}.mo)

    add_custom_command(
      OUTPUT ${_gmoFile}
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_absFile}
      COMMAND ${CMAKE_COMMAND} -E copy ${_gmoFile}
              "Resources/${_poBasename}.lproj/opencpn-${PACKAGE_NAME}.mo"
      DEPENDS ${_absFile}
      COMMENT "${I18N_NAME}-i18n [${_poBasename}]: Created mo file."
    )

    if (APPLE)
      install(
        FILES ${_gmoFile}
        DESTINATION OpenCPN.app/Contents/Resources/${_poBasename}.lproj
        RENAME opencpn-${PACKAGE_NAME}.mo
      )
      message(
        STATUS
          "${CMLOC}Install language files to: OpenCPN.app/Contents/Resources/${_poBasename}.lproj renamed to: opencpn-${PACKAGE_NAME}.mo"
      )
    else (APPLE)
      install(
        FILES ${_gmoFile}
        DESTINATION ${PREFIX_DATA}/locale/${_poBasename}/LC_MESSAGES
        RENAME opencpn-${PACKAGE_NAME}.mo
      )
      message(
        STATUS
          "${CMLOC}Install language files to: ${PREFIX_DATA}/locale/${_poBasename}/LC_MESSAGES renamed to: opencpn-${PACKAGE_NAME}.mo"
      )
    endif (APPLE)

    set(_gmoFiles ${_gmoFiles} ${_gmoFile})
  endforeach (_poFile)
endmacro (GETTEXT_BUILD_MO)

if (GETTEXT_MSGFMT_EXECUTABLE)
  file(GLOB PACKAGE_PO_FILES po/*.po)
  gettext_build_mo(${PACKAGE_PO_FILES})
  add_custom_target(
    ${I18N_NAME}-i18n
    COMMENT "${PACKAGE_NAME}-i18n: Done."
    DEPENDS ${_gmoFiles}
  )
  add_dependencies(${PACKAGE_NAME} ${I18N_NAME}-i18n)
endif (GETTEXT_MSGFMT_EXECUTABLE)

set(CMLOC ${SAVE_CMLOC})
