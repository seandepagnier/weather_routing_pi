#!/usr/bin/env bash

#
# Build for Raspbian in a docker container
#

# bailout on errors and echo commands.
set -xe

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" | sudo tee /etc/default/docker > /dev/null
sudo service docker restart
sleep 5;

if [ "$BUILD_ENV" = "raspbian" ]; then
    docker run --rm --privileged multiarch/qemu-user-static:register --reset
else
    docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
fi

docker run --privileged -d -ti -e "container=docker"  -v $(pwd):/ci-source:rw $DOCKER_IMAGE /bin/bash
DOCKER_CONTAINER_ID=$(docker ps | grep $BUILD_ENV | awk '{print $1}')

docker exec -ti $DOCKER_CONTAINER_ID apt-get update
docker exec -ti $DOCKER_CONTAINER_ID apt-get -y install git cmake build-essential cmake gettext wx-common libgtk2.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release


echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "export CIRCLECI=$CIRCLECI;
    export CIRCLE_BRANCH=\"$CIRCLE_BRANCH\";
    export CIRCLE_TAG=\"$CIRCLE_TAG\";
    export CIRCLE_PROJECT_USERNAME=\"$CIRCLE_PROJECT_USERNAME\";
    export CIRCLE_PROJECT_REPONAME=\"$CIRCLE_PROJECT_REPONAME\";
    export GIT_REPOSITORY_SERVER=\"$GIT_REPOSITORY_SERVER\";
    export OCPN_TARGET=$OCPN_TARGET;
    export TRAVIS=$TRAVIS;
    export TRAVIS_REPO_SLUG=\"$TRAVIS_REPO_SLUG\";
    export TRAVIS_BRANCH=\"$TRAVIS_BRANCH\";
    export TRAVIS_TAG=\"$TRAVIS_TAG\";
    rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

