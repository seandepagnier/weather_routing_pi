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

docker run --privileged -d -ti -e "container=docker"  -v $(pwd):/ci-source:rw -v ~/source_top:/source_top $DOCKER_IMAGE /bin/bash

DOCKER_CONTAINER_ID=$(docker ps | grep $DOCKER_IMAGE | awk '{print $1}')

echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    -e "CIRCLECI=$CIRCLECI" \
    -e "CIRCLE_BRANCH=$CIRCLE_BRANCH" \
    -e "CIRCLE_TAG=$CIRCLE_TAG" \
    -e "CIRCLE_PROJECT_USERNAME=$CIRCLE_PROJECT_USERNAME" \
    -e "CIRCLE_PROJECT_REPONAME=$CIRCLE_PROJECT_REPONAME" \
    -e "GIT_REPOSITORY_SERVER=$GIT_REPOSITORY_SERVER" \
    -e "OCPN_TARGET=$OCPN_TARGET" \
    -e "BUILD_GTK3=$BUILD_GTK3"

# Run build script
rm -f build.sh
if [ "$BUILD_ENV" = "raspbian" ]; then
    if [ "$OCPN_TARGET" = "buster-armhf" ]; then
        cat > build.sh << "EOF"
        # cmake 3.16 has a bug that stops the build to use an older version
        install_packages cmake=3.13.4-1 cmake-data=3.13.4-1
EOF
    else
        cat > build.sh << "EOF1"
        install_packages cmake cmake-data
EOF1
    fi
    cat >> build.sh << "EOF2"
    install_packages git build-essential devscripts equivs gettext wx-common libgtk2.0-dev libwxbase3.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release
EOF2
else
    cat > build.sh << "EOF3"
    apt-get -qq update
    apt-get -y install --no-install-recommends \
    git cmake build-essential gettext wx-common libgtk2.0-dev libwxbase3.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release
EOF3
fi

cat build.sh

if type nproc &> /dev/null
then
    BUILD_FLAGS="-j"$(nproc)
fi

docker exec -ti \
    $DOCKER_CONTAINER_ID /bin/bash -xec "bash -xe /ci-source/build.sh; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

