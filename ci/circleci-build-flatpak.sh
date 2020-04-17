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
##sudo apt-get -qq update

#PLUGIN=bsb4

DOCKER_SOCK="unix:///var/run/docker.sock"
if [ -n "$TRAVIS" ]; then
    TOPDIR=/opencpn-ci
elif [ -n "$CIRCLECI" ]; then
   TOPDIR=/root/project
else
   TOPDIR=/opencpn-ci
fi

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" \
    | sudo tee /etc/default/docker > /dev/null

if [ -n "$CIRCLECI" ]; then
    sudo service docker restart;
    sleep 5;
    sudo docker pull fedora:28;
    sleep 2
fi

DOCKER_CONTAINER_ID=$(docker ps | grep fedora | awk '{print $1}')
if [ "" = "$DOCKER_CONTAINER_ID" ]; then
    docker run --privileged -d -ti -e "container=docker"  \
        -e TOPDIR=$TOPDIR \
        -v /sys/fs/cgroup:/sys/fs/cgroup \
        -v $(pwd):$TOPDIR:rw \
        fedora:28   /usr/sbin/init
    DOCKER_CONTAINER_ID=$(docker ps | grep fedora | awk '{print $1}')
fi

echo $CIRCLE_BRANCH
docker logs $DOCKER_CONTAINER_ID
if [ -n "$CIRCLECI" ]; then
  docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "export CIRCLECI=$CIRCLECI;
    export CIRCLE_BRANCH=$CIRCLE_BRANCH;
    export OCPN_TARGET=$OCPN_TARGET;
    bash -xe $TOPDIR/ci/docker-build-flatpak.sh 28;
         echo -ne \"------\nEND OPENCPN-CI BUILD\n\";"
else
  docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "bash -xe $TOPDIR/ci/docker-build-flatpak.sh 28;
         echo -ne \"------\nEND OPENCPN-CI BUILD\n\";"
fi

docker ps -a

if [ -n "$CIRCLECI" ]; then
    docker stop $DOCKER_CONTAINER_ID
    docker rm -v $DOCKER_CONTAINER_ID
fi
