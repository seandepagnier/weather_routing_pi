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

#wget -q http://opencpn.navnux.org/build_deps/wx312_opencpn50_macos109.tar.xz - unsupported link, replaced with below
curl -o wx312B_opencpn50_macos109.tar.xz https://download.opencpn.org/s/rwoCNGzx6G34tbC/download
tar xJf wx312B_opencpn50_macos109.tar.xz -C /tmp
export PATH="/usr/local/opt/gettext/bin:$PATH"
echo 'export PATH="/usr/local/opt/gettext/bin:$PATH"' >> ~/.bash_profile

export MACOSX_DEPLOYMENT_TARGET=10.9

#wget -q http://opencpn.navnux.org/build_deps/Packages.dmg - unsupported link, replaced with below
#curl -o Packages.dmg https://download.opencpn.org/s/SneCR3z9XM3aRc6/download
#hdiutil attach Packages.dmg
#sudo installer -pkg "/Volumes/Packages 1.2.5/Install Packages.pkg" -target "/"
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
  -DwxWidgets_CONFIG_EXECUTABLE=/tmp/wx312B_opencpn50_macos109/bin/wx-config \
  -DwxWidgets_CONFIG_OPTIONS="--prefix=/tmp/wx312B_opencpn50_macos109" \
  -DCMAKE_INSTALL_PREFIX= \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9 \
  "/" \
  ..
make -sj2
make package

# removed as fails to build
# make create-pkg

