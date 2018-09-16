##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------

IF(WIN32)
SET(SRC_JSON
	    src/wxJSON/jsonreader.cpp
	    src/wxJSON/jsonval.cpp
	    src/wxJSON/jsonwriter.cpp
        )
ENDIF(WIN32)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/wxJSON)
