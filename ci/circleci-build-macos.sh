#!/usr/bin/env bash

#
# Build the  MacOS artifacts
#

# Fix broken ruby on the CircleCI image:
if [ -n "$CI" ]; then
    curl -fsSL \
        https://raw.githubusercontent.com/Homebrew/install/master/uninstall \
        > uninstall
    chmod 755 uninstall
    ./uninstall -f
    inst="https://raw.githubusercontent.com/Homebrew/install/master/install"
    /usr/bin/ruby -e "$(curl -fsSL $inst)"
fi


set -xe

set -o pipefail
for pkg in cairo libexif xz libarchive python3 wget cmake; do
    brew list $pkg 2>/dev/null | head -10 || brew install $pkg
done

wget -q http://opencpn.navnux.org/build_deps/wx312_opencpn50_macos109.tar.xz
tar xJf wx312_opencpn50_macos109.tar.xz -C /tmp
export PATH="/usr/local/opt/gettext/bin:$PATH"
echo 'export PATH="/usr/local/opt/gettext/bin:$PATH"' >> ~/.bash_profile

rm -rf build && mkdir build && cd build
test -z "$TRAVIS_TAG" && CI_BUILD=OFF || CI_BUILD=ON
cmake \
  -DwxWidgets_CONFIG_EXECUTABLE=/tmp/wx312_opencpn50_macos109/bin/wx-config \
  -DwxWidgets_CONFIG_OPTIONS="--prefix=/tmp/wx312_opencpn50_macos109" \
  -DCMAKE_INSTALL_PREFIX=/tmp/opencpn \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9 \
  ..
make -sj2
make package

wget -q http://opencpn.navnux.org/build_deps/Packages.dmg
hdiutil attach Packages.dmg
sudo installer -pkg "/Volumes/Packages 1.2.5/Install Packages.pkg" -target "/"
make install
make create-pkg

