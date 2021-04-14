##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3
##---------------------------------------------------------------------------

if (TARGET ocpn::libtess2)
    return ()
endif ()

set(SAVE_CMLOC_PLUGINTESS2 ${CMLOC})
set(CMLOC "CMakeLists_PLUGINTESS2: ")

message(STATUS "${CMLOC}Building PluginTESS2")

SET(SRC_PLUGINTESS
          src/bucketalloc.c
          src/dict.c
          src/geom.c
          src/mesh.c
          src/priorityq.c
          src/sweep.c
          src/tess.c
          )

if(UNIX)
    add_definitions("-fpic")
endif(UNIX)

ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINTESS2 STATIC ${SRC_PLUGINTESS})
target_include_directories(${PACKAGE_NAME}_LIB_PLUGINTESS2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ocpn::libtess2 ALIAS ${PACKAGE_NAME}_LIB_PLUGINTESS2)

set(CMLOC ${SAVE_CMLOC_PLUGINTESS2})
