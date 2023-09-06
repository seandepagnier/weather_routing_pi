##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3
##---------------------------------------------------------------------------

if (TARGET ocpn::pluginDC)
    return ()
endif ()

set(SAVE_CMLOC_PLUGINDCL ${CMLOC})
set(CMLOC "CMakeLists_PLUGINDC: ")

message(STATUS "${CMLOC}Building PluginDC")
SET(SRC_PLUGINDC
          src/pidc.cpp
          src/pi_shaders.cpp
          src/TexFont.cpp
          src/qtstylesheet.cpp
          )

if(UNIX)
    add_definitions("-fpic")
endif(UNIX)

# Needed for android builds
if  (QT_ANDROID)

#    include_directories(BEFORE ${qt_android_include})
    message(STATUS "${CMLOC}Building PluginDC: qt_android_include:  ${qt_android_include}")
#	include_directories(${qt_android_include})
#  There is probably a much simpler way to do this...
    IF(_wx_selected_config MATCHES "androideabi-qt-arm64")
 # from bdbcat changes
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtCore")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtWidgets")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtGui")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtOpenGL")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtTest")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/wxWidgets/libarm64/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/wxWidgets/include")
	
# suggested by JG
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtCore")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtWidgets")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtGui")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtOpenGL")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/include/QtTest")

        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/libarm64/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/include")

        SET(wxWidgets_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5Core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5OpenGL.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5Widgets.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5Gui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm64_O3/qtbase/lib/libQt5AndroidExtras.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/opencpn/API-117/libarm64/libgorp.so

        -lc++_shared
        -lz
        libGLESv2.so
        libEGL.so
          )

    ELSE(_wx_selected_config MATCHES "androideabi-qt-arm64")
# from bdbcat changes	
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtCore")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtWidgets")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtGui")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtOpenGL")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtTest")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/wxWidgets/libarmhf/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
#        include_directories( "${CMAKE_SOURCE_DIR}/${OCPN_Android_Common}/wxWidgets/include")

        ADD_DEFINITIONS( -DOCPN_ARMHF )

        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtCore")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtWidgets")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtGui")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtOpenGL")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/include/QtTest")

        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/libarmhf/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
        set(qt_android_include ${qt_android_include} "${OCPN_Android_Common}/wxWidgets/include")

        ADD_DEFINITIONS( -DOCPN_ARMHF )

        SET(wxWidgets_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5Core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5OpenGL.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5Widgets.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5Gui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/qt5/build_arm32_19_O3/qtbase/lib/libQt5AndroidExtras.so
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCPN_Android_Common}/opencpn/API-117/libarmhf/libgorp.so

        -lc++_shared
        -lz
        libGLESv2.so
        libEGL.so
          )

    ENDIF(_wx_selected_config MATCHES "androideabi-qt-arm64")
	
	    
    include_directories(${qt_android_include})
endif(QT_ANDROID)


ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINDC STATIC ${SRC_PLUGINDC})
target_include_directories(${PACKAGE_NAME}_LIB_PLUGINDC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ocpn::pluginDC ALIAS ${PACKAGE_NAME}_LIB_PLUGINDC)

set(CMLOC ${SAVE_CMLOC_PLUGINDC})
