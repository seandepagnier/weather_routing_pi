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

if [ -n "$WXVERSION" ] && [ "$WXVERSION" eq "315" ]; then
    curl -o wx315_opencpn50_macos1010.tar.xz https://download.opencpn.org/s/MCiRiq4fJcKD56r/download
    tar xJf wx315_opencpn50_macos1010.tar.xz -C /tmp
    WX_EXECUTABLE=/tmp/wx315_opencpn50_macos1010/bin/wx-config
    WX_CONFIG="--prefix=/tmp/wx315_opencpn50_macos1010"
else
    curl -o wx312B_opencpn50_macos109.tar.xz https://download.opencpn.org/s/rwoCNGzx6G34tbC/download
    tar xJf wx312B_opencpn50_macos109.tar.xz -C /tmp
    WX_EXECUTABLE=/tmp/wx312B_opencpn50_macos109/bin/wx-config
    WX_CONFIG="--prefix=/tmp/wx312B_opencpn50_macos109"
fi

export PATH="/usr/local/opt/gettext/bin:$PATH"
echo 'export PATH="/usr/local/opt/gettext/bin:$PATH"' >> ~/.bash_profile

export MACOSX_DEPLOYMENT_TARGET=10.9

# use brew to get Packages.pkg
if brew list --cask --versions packages; then
    version=$(pkg_version packages '--cask')
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
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9 \
  "/" \
  ..
make -sj2
make package

