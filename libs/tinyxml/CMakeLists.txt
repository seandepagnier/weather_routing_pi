#
#  Provide a TinyXML 3.0-style linkable targetocpn::tinyxml which
#  provides all necessary libraries, headers and defines required to
#  use tinyxml
#
#  Probes for an available, system implementation and uses it if 
#  is available. If not, builds the library from bundled sources.

cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::tinyxml)
  return ()
endif ()

INCLUDE( "FindPackageHandleStandardArgs" )

set (SRC
  src/tinyxml.cpp
  src/tinyxmlerror.cpp
  src/tinyxmlparser.cpp
  include/tinyxml.h
)

add_library(_TINYXML INTERFACE)
add_library(ocpn::tinyxml ALIAS _TINYXML)

find_package(TinyXML)
if (TINYXML_FOUND)
  message(${CMLOC} "Using system tinyxml")
  target_include_directories(_TINYXML INTERFACE ${TINYXML_INCLUDE_DIR})
  target_link_libraries(_TINYXML INTERFACE ${TINYXML_LIBRARIES})
else ()
  message(${CMLOC} "Using bundled tinyxml")
  add_library(TINYXML STATIC ${SRC})
  target_link_libraries(_TINYXML INTERFACE TINYXML)
  target_include_directories(
    _TINYXML INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_compile_definitions(TINYXML PUBLIC "-DTIXML_USE_STL")
  target_include_directories(
    TINYXML PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
endif ()
