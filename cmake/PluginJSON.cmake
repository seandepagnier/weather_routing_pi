##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------

SET(SRC_JSON
	    src/jsoncpp/json_reader.cpp
	    src/jsoncpp/json_value.cpp
	    src/jsoncpp/json_writer.cpp
            )

IF(QT_ANDROID)
  ADD_DEFINITIONS(-DJSONCPP_NO_LOCALE_SUPPORT)
ENDIF(QT_ANDROID)


INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/jsoncpp)

ADD_LIBRARY(LIB_PLUGINJSON STATIC ${SRC_JSON})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_PLUGINJSON )
