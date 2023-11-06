#!/bin/sh  -xe

#
# Build the Android artifacts inside the circleci linux container
#

set -xe

pwd

git submodule update --init opencpn-libs

ls -la

#sudo apt-get -q update
#sudo apt-get -y install git cmake gettext unzip

# Get the OCPN Android build support package.
# FOR LOCAL BUILD - have a local version to avoid big download each run - need to stage it but not commit it. DO NOT COMMIT AND PUSH master.zip
if [ "${CIRCLECI_LOCAL,,}" = "true" ]; then
    if [[ -d ~/circleci-cache ]]; then
        if [[ -f ~/circleci-cache/apt-proxy ]]; then
            cat ~/circleci-cache/apt-proxy | sudo tee -a /etc/apt/apt.conf.d/00aptproxy
            cat /etc/apt/apt.conf.d/00aptproxy
        fi
        if [[ ! -f ~/circleci-cache/master.zip ]]; then
            sudo wget https://github.com/bdbcat/OCPNAndroidCommon/archive/master.zip -O ~/circleci-cache/master.zip
        fi
        MASTER_LOC=~/circleci-cache
        #unzip -qq -o /home/circleci/circleci-cache/master.zip
    fi
else
    MASTER_LOC=$(pwd)
    wget https://github.com/bdbcat/OCPNAndroidCommon/archive/master.zip
    #unzip -qq -o master.zip
fi
echo "unzipping $MASTER_LOC/master.zip"

unzip -qq -o $MASTER_LOC/master.zip

sudo apt-get -q update
sudo apt-get -y install git cmake gettext unzip
# Install extra build libs
ME=$(echo ${0##*/} | sed 's/\.sh//g')
EXTRA_LIBS=./ci/extras/extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi
EXTRA_LIBS=./ci/extras/${ME}_extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi

# Install extra build libs
ME=$(echo ${0##*/} | sed 's/\.sh//g')
EXTRA_LIBS=./ci/extras/extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi
EXTRA_LIBS=./ci/extras/${ME}_extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi

pwd
ls -la

mkdir -p build
cd build

rm -f CMakeCache.txt

# Install python to get a recent version of cmake
sudo apt install python3-pip
python3 -m pip install --user --force-reinstall -q pip setuptools
sudo apt remove python3-six python3-colorama python3-urllib3
export LC_ALL=C.UTF-8  LANG=C.UTF-8
python3 -m pip install --user -q cmake -vv

last_ndk=$(ls -d /home/circleci/android-sdk/ndk/* | tail -1)
test -d /opt/android || sudo mkdir -p /opt/android
sudo ln -sf $last_ndk /opt/android/ndk

cmake -DCMAKE_TOOLCHAIN_FILE=cmake/android-armhf-toolchain.cmake \
  -D_wx_selected_config=androideabi-qt-armhf \
  -DwxQt_Build=build_android_release_19_static_O3 \
  -DQt_Build=build_arm32_19_O3/qtbase \
  -DOCPN_Android_Common=OCPNAndroidCommon-master \
  ..

make VERBOSE=1

make package
