##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3
##---------------------------------------------------------------------------

SET(SRC_PLUGINGL
          src/plugingl/pidc.cpp
          src/plugingl/pi_shaders.cpp
          src/plugingl/TexFont.cpp
          src/plugingl/qtstylesheet.cpp
          )

ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINGL STATIC ${SRC_PLUGINGL})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${PACKAGE_NAME}_LIB_PLUGINGL )
