cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::bzip2)
    return ()
endif ()

set(SAVE_CMLOC_BZIP2 ${CMLOC})
set(CMLOC "CMakeLists_BZIP2: ")

message(STATUS "${CMLOC}Building BZIP2")
SET(SRC_BZIP
    src/bzlib.c
    src/blocksort.c
    src/compress.c
    src/crctable.c
    src/decompress.c
    src/huffman.c
    src/randtable.c
  )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(LIB_BZIP_WR STATIC ${SRC_BZIP})
target_include_directories(LIB_BZIP_WR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ocpn::bzip2 ALIAS LIB_BZIP_WR)

set(CMLOC ${SAVE_CMLOC_BZIP2})
