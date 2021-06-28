#!/bin/sh  -xe
cd $(dirname $(readlink -fn $0))

#
# Actually build the Travis flatpak artifacts inside the Fedora container
#
set -xe

df -h
cd $TOPDIR
su -c "dnf install -y sudo cmake gcc-c++ flatpak-builder flatpak make tar"

flatpak remote-add --user --if-not-exists flathub \
    https://flathub.org/repo/flathub.flatpakrepo

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

#rm -rf flatpak/.flatpak-builder && rm -rf build && mkdir build && cd build
rm -rf build && mkdir build && cd build
if [ "$FLATPAK_BRANCH" = 'beta' ]; then
  cmake -DOCPN_FLATPAK_CONFIG=ON -DSDK_VER=20.08 ..
else
  cmake -DOCPN_FLATPAK_CONFIG=ON -DSDK_VER=18.08 ..
fi

make flatpak-build
make flatpak-pkg
