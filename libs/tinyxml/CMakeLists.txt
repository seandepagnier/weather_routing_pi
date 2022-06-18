cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::tinyxml)
    return ()
endif ()

set(SAVE_CMLOC_TINYXML ${CMLOC})
set(CMLOC "CMakeLists_TINYXML: ")

message(STATUS "${CMLOC}Building TinyXML")
set (SRC
src/tinyxml.cpp
src/tinyxmlerror.cpp
src/tinyxmlparser.cpp
  )
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(TINYXML STATIC ${SRC})
target_compile_definitions(TINYXML PUBLIC -DTIXML_USE_STL)
set_property(TARGET TINYXML PROPERTY COMPILE_FLAGS "${OBJ_VISIBILITY}")
target_include_directories(TINYXML PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ocpn::tinyxml ALIAS TINYXML)

set(CMLOC ${SAVE_CMLOC_TINYXML})
