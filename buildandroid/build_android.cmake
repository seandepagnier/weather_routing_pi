

#Toolchain and options definition file for OPenCPN Android build


#  Locations of the cross-compiler tools

# this one is important for bundled builds, but must be commented away for standalone builds
# otherwise you get:
# "ADD_LIBRARY for library oesenc_pi is used with the SHARED option, but the target platform supports only STATIC libraries"

#SET(CMAKE_SYSTEM_NAME Generic)


#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /home/sean/android-ndk/android-ndk-r10d/ndk-toolchain/bin/arm-linux-androideabi-gcc)
SET(CMAKE_CXX_COMPILER   /home/sean/android-ndk/android-ndk-r10d/ndk-toolchain/bin/arm-linux-androideabi-g++)


SET(TARGET_SUPPORTS_SHARED_LIBS TRUE)

#Location of the root of the Android NDK
SET(NDK_Base /home/sean/android-ndk/android-ndk-r10d)
#  /home/dsr/Projects/android-ndk/android-ndk-r10 )

SET(_wx_selected_config androideabi-qt)
SET(wxQt_Build build_android_55_intl)
SET(wxQt_Base /home/sean/wxqt/wxWidgets)
SET(Qt_Base /home/sean/qt5bare)
SET(Qt_Build build_release/qtbase)
SET(CMAKE_BUILD_TYPE Debug)
