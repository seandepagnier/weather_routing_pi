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
ocpnfound=$(flatpak list | grep org.opencpn.OpenCPN | awk '{print $1}')
if [ "" = "$ocpnfound" ]; then
   flatpak install --user  -y \
       http://opencpn.duckdns.org/opencpn/opencpn.flatpakref
fi
flatpak install --user -y  flathub org.freedesktop.Sdk//18.08
#rm -rf flatpak/.flatpak-builder && rm -rf build && mkdir build && cd build
rm -rf build && mkdir build && cd build
cmake -DOCPN_FLATPAK_CONFIG=ON ..
make flatpak-build
make flatpak-pkg
