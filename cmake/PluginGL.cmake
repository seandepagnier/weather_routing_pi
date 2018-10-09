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

ADD_LIBRARY(LIB_PLUGINGL STATIC ${SRC_PLUGINGL})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_PLUGINGL )
