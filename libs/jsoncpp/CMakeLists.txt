##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------

if (TARGET ${PACKAGE_NAME}_LIB_PLUGINJSON)
    return ()
endif ()

set(SAVE_CMLOC_JSONCPP ${CMLOC})
set(CMLOC "CMakeLists_JSONCPP: ")

SET(SRC_JSON
	    src/json_reader.cpp
	    src/json_value.cpp
	    src/json_writer.cpp
)

IF(QT_ANDROID)
  ADD_DEFINITIONS(-DJSONCPP_NO_LOCALE_SUPPORT)
ENDIF(QT_ANDROID)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message(STATUS "${CMLOC}PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)

ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINJSON STATIC ${SRC_JSON})

set(CMLOC ${SAVE_CMLOC_JSONCPP})
