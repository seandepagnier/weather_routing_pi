##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------

message(STATUS "Processing PluginJSON.cmake")
SET(SRC_JSON
	    src/jsoncpp/json_reader.cpp
	    src/jsoncpp/json_value.cpp
	    src/jsoncpp/json_writer.cpp
            )

IF(QT_ANDROID)
  ADD_DEFINITIONS(-DJSONCPP_NO_LOCALE_SUPPORT)
ENDIF(QT_ANDROID)


message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)

ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINJSON STATIC ${SRC_JSON})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${PACKAGE_NAME}_LIB_PLUGINJSON )
message(STATUS "Add Library ${PACKAGE_NAME}_LIB_PLUGINJSON")
