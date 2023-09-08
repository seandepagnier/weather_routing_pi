#!/usr/bin/env bash

#
# Build the  MacOS artifacts
#

set -xe

set -o pipefail
# Check if the cache is with us. If not, re-install brew.
brew list --versions libexif || brew update-reset

for pkg in cairo cmake gettext libarchive libexif python wget; do
    brew list --versions $pkg || brew install $pkg || brew install $pkg || :
    brew link --overwrite $pkg || brew install $pkg
done

if [ -n "$WX_VER" ] && [ "$WX_VER" -eq "32" ]; then
    echo "Building for WXVERSION 32";
    WX_URL=https://download.opencpn.org/s/Djqm4SXzYjF8nBw/download
    WX_DOWNLOAD=/tmp/wx321_opencpn50_macos1010.tar.xz
    WX_EXECUTABLE=/tmp/wx321_opencpn50_macos1010/bin/wx-config
    WX_CONFIG="--prefix=/tmp/wx321_opencpn50_macos1010"
    MACOSX_DEPLOYMENT_TARGET=10.10
else
    echo "Building for WXVERSION 315";
    WX_URL=https://download.opencpn.org/s/MCiRiq4fJcKD56r/download
    WX_DOWNLOAD=/tmp/wx315_opencpn50_macos1010.tar.xz
    WX_EXECUTABLE=/tmp/wx315_opencpn50_macos1010/bin/wx-config
    WX_CONFIG="--prefix=/tmp/wx315_opencpn50_macos1010"
    MACOSX_DEPLOYMENT_TARGET=10.10
fi

# Download required binaries using wget, since curl causes an issue with Xcode 13.1 and some specific certificates.
# Inspect the response code to see if the file is downloaded properly.
# If the download failed or file does not exist, then exit with an error.
# For local purposes: only download if it has not been downloaded already. That does not harm building on CircleCI.
if [ ! -f "$WX_DOWNLOAD" ]; then
  echo "Downloading $WX_DOWNLOAD";
  SERVER_RESPONSE=$(wget --server-response  -O $WX_DOWNLOAD $WX_URL 2>&1 | grep "HTTP"/ | awk '{print $2}')
  if [ $SERVER_RESPONSE -ne 200 ]; then
    echo "Fatal error: could not download $WX_DOWNLOAD. Server response: $SERVER_RESPONSE."
    exit 0
  fi
fi
if [ -f "$WX_DOWNLOAD" ]; then
  echo "$WX_DOWNLOAD exists"
else
  echo "Fatal error: $WX_DOWNLOAD does not exist";
  exit 0
fi

# Unpack the binaries to /tmp
tar xJf $WX_DOWNLOAD -C /tmp

# Extend PATH, only when necesary
INCLUDE_DIR_GETTEXT="/usr/local/opt/gettext/bin:"

if [[ ":$PATH:" != *$INCLUDE_DIR_GETTEXT* ]]; then
  echo "Your path is missing $INCLUDE_DIR_GETTEXT. Trying to add it automatically:"
  export PATH=$INCLUDE_DIR_GETTEXT$PATH
  echo 'export PATH="'$INCLUDE_DIR_GETTEXT'$PATH"' >> ~/.bash_profile
else
    echo "Path includes $INCLUDE_DIR_GETTEXT"
fi

export MACOSX_DEPLOYMENT_TARGET=$MACOSX_DEPLOYMENT_TARGET

# use brew to get Packages.pkg
if brew list --cask --versions packages; then
    version=$(brew list --cask --versions packages)
    version="${version/"packages "/}"
    sudo installer \
        -pkg /usr/local/Caskroom/packages/$version/packages/Packages.pkg \
        -target /
else
    brew install --cask packages
fi

rm -rf build && mkdir build && cd build
cmake \
  -DwxWidgets_CONFIG_EXECUTABLE=$WX_EXECUTABLE \
  -DwxWidgets_CONFIG_OPTIONS=$WX_CONFIG \
  -DCMAKE_INSTALL_PREFIX= \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=$MACOSX_DEPLOYMENT_TARGET \
  "/" \
  ..
make -sj2
make package

