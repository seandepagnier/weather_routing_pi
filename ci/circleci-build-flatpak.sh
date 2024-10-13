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

if [ "${CIRCLECI_LOCAL,,}" = "true" ]; then
    if [[ -d ~/circleci-cache ]]; then
        if [[ -f ~/circleci-cache/apt-proxy ]]; then
            cat ~/circleci-cache/apt-proxy | sudo tee -a /etc/apt/apt.conf.d/00aptproxy
            cat /etc/apt/apt.conf.d/00aptproxy
        fi
    fi
fi

sudo apt-get -q -y --allow-unauthenticated --allow-downgrades --allow-remove-essential --allow-change-held-packages update

#PLUGIN=bsb4

sudo apt --allow-unauthenticated --allow-downgrades --allow-remove-essential --allow-change-held-packages install flatpak flatpak-builder

# Install extra build libs
ME=$(echo ${0##*/} | sed 's/\.sh//g')
EXTRA_LIBS=./ci/extras/extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    sudo apt update
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi
EXTRA_LIBS=./ci/extras/${ME}_extra_libs.txt
if test -f "$EXTRA_LIBS"; then
    sudo apt update
    while read line; do
        sudo apt-get install $line
    done < $EXTRA_LIBS
fi

git config --global protocol.file.allow always
git submodule update --init opencpn-libs

if [ -n "$CI" ]; then
    sudo apt update

    # Avoid using outdated TLS certificates, see #210.
    sudo apt install --reinstall  ca-certificates

    # Use updated flatpak workaround
#    sudo add-apt-repository -y ppa:alexlarsson/flatpak
#    sudo apt update

    # Handle possible outdated key for google packages, see #486
    wget -q -O - https://cli-assets.heroku.com/apt/release.key \
        | sudo apt-key add -
    wget -q -O - https://dl.google.com/linux/linux_signing_key.pub \
        | sudo apt-key add -


    # Install flatpak and flatpak-builder - obsoleted by flathub
    sudo apt install flatpak flatpak-builder

fi

flatpak remote-add --user --if-not-exists \
    flathub https://dl.flathub.org/repo/flathub.flatpakrepo


if [ "$FLATPAK_BRANCH" = "beta" ]; then
    flatpak install --user -y flathub org.freedesktop.Sdk//$SDK_VER >/dev/null
    flatpak remote-add --user --if-not-exists flathub-beta \
        https://dl.flathub.org/beta-repo/flathub-beta.flatpakrepo
    flatpak install --user -y flathub-beta \
        org.opencpn.OpenCPN >/dev/null
else
    flatpak install --user -y flathub org.freedesktop.Sdk//$SDK_VER >/dev/null
    flatpak remote-add --user --if-not-exists flathub \
        https://dl.flathub.org/repo/flathub.flatpakrepo
    flatpak install --user -y flathub \
        org.opencpn.OpenCPN >/dev/null
    FLATPAK_BRANCH='stable'
fi

rm -rf build && mkdir build && cd build
if [ -n "$WX_VER" ]; then
    SET_WX_VER="-DWX_VER=$WX_VER"
else
    SET_WX_VER=""
fi

if [ "$FLATPAK_BRANCH" = '' ]; then
    cmake -DOCPN_TARGET=$OCPN_TARGET -DOCPN_FLATPAK_CONFIG=ON -DSDK_VER=$SDK_VER -DFLATPAK_BRANCH='beta' $SET_WX_VER ..
else
    cmake -DOCPN_TARGET=$OCPN_TARGET -DOCPN_FLATPAK_CONFIG=ON -DSDK_VER=$SDK_VER -DFLATPAK_BRANCH=$FLATPAK_BRANCH $SET_WX_VER ..
fi

make flatpak-build
make flatpak-pkg
