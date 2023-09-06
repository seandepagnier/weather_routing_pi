##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::nmea0183)
    return ()
endif ()

set(SAVE_CMLOC_NMEA0183 ${CMLOC})
set(CMLOC "CMakeLists_NMEA0183: ")

if (NOT CMAKE_MODULE_PATH)
  set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
endif ()

SET(SRC_NMEA0183
    src/latlong.cpp
    src/long.cpp
    src/nmea0183.cpp
    src/response.cpp
    src/rmb.cpp
    src/sentence.cpp
    src/talkerid.cpp
    src/rmc.cpp
    src/hexvalue.cpp
    src/lat.cpp
    src/expid.cpp
    src/wpl.cpp
    src/rte.cpp
    src/hdt.cpp
    src/hdg.cpp
    src/hdm.cpp
    src/gll.cpp
    src/vtg.cpp
    src/gga.cpp
    src/gsv.cpp
    src/dbt.cpp
    src/dpt.cpp
    src/mtw.cpp
    src/mwv.cpp
    src/vhw.cpp
    src/vwr.cpp
    src/zda.cpp
    src/rsa.cpp
    src/mwd.cpp
    src/vwt.cpp
    src/mta.cpp
    src/vlw.cpp
    src/mda.cpp
    src/xdr.cpp
)

#message(STATUS "${CMLOC}About to look for widgets, wxWidgets_INCLUDE_DIRS: ${wxWidgets_INCLUDE_DIRS}")
#if (NOT wxWidgets_INCLUDE_DIRS)
#    message(STATUS "${CMLOC}Not found so using SimpleWxConfig")
#    include(SimpleWxConfig)
#    SimpleWxConfig()
#endif ()

#IF(DEFINED _wx_selected_config)
#    MESSAGE(STATUS "${CMLOC}_wx_select_config defined as $ENV{_wx_select_config}")
#    IF(_wx_selected_config MATCHES "androideabi-qt")
#        MESSAGE (STATUS "${CMLOC}Qt_Base: " ${Qt_Base})
#        MESSAGE (STATUS "${CMLOC}wxQt_Base/Build: " ${wxQt_Base} "/" ${wxQt_Build})
#        ADD_DEFINITIONS(-DocpnUSE_GLES)
#        ADD_DEFINITIONS(-DocpnUSE_GL)

#        SET(OPENGLES_FOUND "YES")
#        SET(OPENGL_FOUND "YES")

#        ADD_DEFINITIONS(-DUSE_GLU_TESS)
#        SET(USE_GLES2 ON )
#        MESSAGE (STATUS "${CMLOC}Using GLESv2 for Android")
#        ADD_DEFINITIONS(-DUSE_ANDROID_GLES2)
#        ADD_DEFINITIONS(-DUSE_GLSL)
#        include_directories( ${CMAKE_SOURCE_DIR}/libs/glshim/include/GLES )
#        set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} ${CMAKE_SOURCE_DIR}/libs/glshim/include/GLES)
#        set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} libs/glshim/include)

#    ENDIF(_wx_selected_config MATCHES "androideabi-qt")
#ENDIF(DEFINED _wx_selected_config)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(NMEA0183 STATIC ${SRC_NMEA0183})
add_library(ocpn::nmea0183 ALIAS NMEA0183)

set_property(TARGET NMEA0183 PROPERTY COMPILE_FLAGS "${OBJ_VISIBILITY}")
target_include_directories(NMEA0183 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(NMEA0183 PRIVATE ${wxWidgets_INCLUDE_DIRS})


set(CMLOC ${SAVE_CMLOC_NMEA0183})
