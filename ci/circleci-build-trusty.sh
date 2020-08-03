#!/usr/bin/env bash

#
# Build the Trusty Ubuntu artifacts
#
set -xe
sudo apt-get -qq update
sudo apt-get install devscripts equivs gdebi libglu1-mesa-dev

# Install extra libs
ME=$(echo ${0##*/} | sed 's/\.sh//g')
EXTRA_LIBS=extras/extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi
EXTRA_LIBS=extras/${ME}_extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi

rm -rf build && mkdir build && cd build
mk-build-deps ../ci/control
sudo gdebi -n ./*all.deb  || :
sudo apt-get --allow-unauthenticated install -f
rm -f ./*all.deb

tag=$(git tag --contains HEAD)

if [ -n "$BUILD_GTK3" ]; then
  sudo update-alternatives --set wx-config /usr/lib/*-linux-*/wx/config/gtk3-unicode-3.0
fi

if [ -n "$tag" ]; then
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
else
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr/local ..
fi

make -j2
make package
ls -l
