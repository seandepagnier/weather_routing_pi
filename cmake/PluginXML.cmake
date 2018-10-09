##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3
##---------------------------------------------------------------------------


IF(WIN32)
SET(SRC_TINYXML
            src/tinyxml/tinyxml.cpp
            src/tinyxml/tinyxmlparser.cpp
            src/tinyxml/tinyxmlerror.cpp
)
ADD_LIBRARY(LIB_PLUGINXML STATIC ${SRC_TINYXML})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_PLUGINXML )
ENDIF(WIN32)

ADD_DEFINITIONS(-DTIXML_USE_STL)
