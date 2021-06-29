#!/usr/bin/env bash

#
# Build the flatpak artifacts. Uses docker to run Fedora on
# in full-fledged VM; the actual build is done in the Fedora
# container.
#
# flatpak-builder can be run in a docker image. However, this
# must then be run in privileged mode, which means it we need
# a full VM to run it.
#

# bailout on errors and echo commands.
set -xe
sudo apt-get -qq update

#PLUGIN=bsb4

sudo apt install flatpak flatpak-builder

flatpak remote-add --user --if-not-exists \
    flathub https://flathub.org/repo/flathub.flatpakrepo


if [ "$FLATPAK_BRANCH" = "beta" ]; then
        flatpak install --user -y flathub org.freedesktop.Sdk//20.08 >/dev/null
        flatpak remote-add --user --if-not-exists flathub-beta \
            https://flathub.org/beta-repo/flathub-beta.flatpakrepo
        flatpak install --user -y flathub-beta \
            org.opencpn.OpenCPN >/dev/null
else
        flatpak install --user -y flathub org.freedesktop.Sdk//18.08 >/dev/null
        flatpak remote-add --user --if-not-exists flathub \
            https://flathub.org/repo/flathub.flatpakrepo
        flatpak install --user -y flathub \
            org.opencpn.OpenCPN >/dev/null
        FLATPAK_BRANCH='stable'
fi

rm -rf build && mkdir build && cd build
if [ "$FLATPAK_BRANCH" = 'beta' ]; then
  cmake -DOCPN_FLATPAK_CONFIG=ON -DSDK_VER=20.08 ..
else
  cmake -DOCPN_FLATPAK_CONFIG=ON -DSDK_VER=18.08 ..
fi

make flatpak-build
make flatpak-pkg
