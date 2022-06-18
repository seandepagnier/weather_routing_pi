#!/usr/bin/env bash

#
# Build for Raspbian in a docker container
#

# bailout on errors and echo commands.
set -xe
sudo apt-get -qq update

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" | sudo tee /etc/default/docker > /dev/null
sudo service docker restart
sleep 5;

if [ "$BUILD_ENV" = "raspbian" ]; then
    docker run --rm --privileged multiarch/qemu-user-static:register --reset
else
    docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
fi

docker run --privileged -d -ti -e "container=docker"  \
    -e "CIRCLECI=$CIRCLECI" \
    -e "CIRCLE_BRANCH=$CIRCLE_BRANCH" \
    -e "CIRCLE_TAG=$CIRCLE_TAG" \
    -e "CIRCLE_PROJECT_USERNAME=$CIRCLE_PROJECT_USERNAME" \
    -e "CIRCLE_PROJECT_REPONAME=$CIRCLE_PROJECT_REPONAME" \
    -e "GIT_REPOSITORY_SERVER=$GIT_REPOSITORY_SERVER" \
    -e "OCPN_TARGET=$OCPN_TARGET" \
    -e "BUILD_GTK3=$BUILD_GTK3" \
    -e "BUILD_ENV=$BUILD_ENV" \
    -e "TZ=$TZ" \
    -e "DEBIAN_FRONTEND=$DEBIAN_FRONTEND" \
    -v $(pwd):/ci-source:rw -v ~/source_top:/source_top $DOCKER_IMAGE /bin/bash

DOCKER_CONTAINER_ID=$(docker ps | grep $DOCKER_IMAGE | awk '{print $1}')

echo $OCPN_TARGET
# Construct and run build script
rm -f build.sh

if [ "$BUILD_ENV" = "raspbian" ]; then
    if [ "$OCPN_TARGET" = "buster-armhf" ]; then
        cat >> build.sh << "EOF1"
        # cmake 3.16 has a bug that stops the build to use an older version
        install_packages cmake=3.13.4-1 cmake-data=3.13.4-1
EOF1
    else
        cat >> build.sh << "EOF2"
        install_packages cmake cmake-data
EOF2
    fi
    if [ "$OCPN_TARGET" = "bullseye-armhf" ]; then
        cat >> build.sh << "EOF3"
        curl http://mirrordirector.raspbian.org/raspbian.public.key  | apt-key add -
        curl http://archive.raspbian.org/raspbian.public.key  | apt-key add -
        sudo apt -q update
        sudo apt install devscripts equivs wget git lsb-release
        sudo mk-build-deps -ir ci-source/ci/control
        sudo apt-get --allow-unauthenticated install -f
EOF3
    else
            cat >> build.sh << "EOF4"
            install_packages git build-essential devscripts equivs gettext wx-common libgtk2.0-dev libwxbase3.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release
EOF4
    fi
else
    if [ "$OCPN_TARGET" = "focal-arm64" ] ||
       [ "$OCPN_TARGET" = "focal-armhf" ] ||
       [ "$OCPN_TARGET" = "bullseye-armhf" ] ||
       [ "$OCPN_TARGET" = "bullseye-arm64" ] ||
       [ "$OCPN_TARGET" = "buster-armhf" ]; then
        cat >> build.sh << "EOF5"
        echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
        apt-get -qq update && DEBIAN_FRONTEND='noninteractive' TZ='America/New_York' apt-get -y --no-install-recommends install tzdata
        apt-get -y --no-install-recommends --fix-missing install \
        software-properties-common devscripts equivs wget git build-essential gettext wx-common libgtk2.0-dev libwxbase3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release openssl libssl-dev
EOF5
        if [ "$OCPN_TARGET" = "buster-armhf" ] ||
           [ "$OCPN_TARGET" = "bullseye-arm64" ]; then
            echo "BUILD_GTK3: $BUILD_GTK3"
            if [ ! -n "$BUILD_GTK3" ] || [ "$BUILD_GTK3" = "false" ]; then
                echo "Building for GTK2"
                cat >> build.sh << "EOF6"
                apt-get -y --no-install-recommends --fix-missing install libwxgtk3.0-dev
EOF6
            else
                echo "Building for GTK3"
                cat >> build.sh << "EOF7"
                apt-get -y --no-install-recommends --fix-missing install libwxgtk3.0-gtk3-dev
EOF7
            fi
        fi
        if [ "$OCPN_TARGET" = "focal-armhf" ]; then
            cat >> build.sh << "EOF8"
            CMAKE_VERSION=3.20.5-0kitware1ubuntu20.04.1
            wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc --no-check-certificate 2>/dev/null | apt-key add -
            apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
            apt-get update
            apt install cmake=$CMAKE_VERSION cmake-data=$CMAKE_VERSION
EOF8
        else
            cat >> build.sh << "EOF9"
            apt install -y cmake
EOF9
        fi
    else
        cat > build.sh << "EOF10"
        apt-get -qq update
        apt-get -y --no-install-recommends install \
        git cmake build-essential gettext wx-common libgtk2.0-dev libwxbase3.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release
EOF10
    fi
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

cat build.sh

if type nproc &> /dev/null
then
    BUILD_FLAGS="-j"$(nproc)
fi

docker exec -ti \
    $DOCKER_CONTAINER_ID /bin/bash -xec "bash -xe ci-source/build.sh; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

