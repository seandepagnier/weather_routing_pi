#!/usr/bin/env bash

#
# Build the travis flatpak artifacts. Uses docker to run Fedora on
# in the Travis ubuntu VM; the actual build is done in the Fedora
# container.
#

# bailout on errors and echo commands.
set -xe
if [ -n "$TRAVIS" ]; then
    sudo apt-get -qq update
fi
DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" \
    | sudo tee /etc/default/docker > /dev/null

if [ -n "$TRAVIS" ]; then

    sudo service docker restart;
    sleep 5;
    sudo docker pull fedora:28;
fi
DOCKER_CONTAINER_ID=$(docker ps | grep fedora | awk '{print $1}')
if [ "" = "$DOCKER_CONTAINER_ID" ]; then
    docker run --privileged -d -ti -e "container=docker"  \
        -e "TOPDIR=$TOPDIR" \
        -v /sys/fs/cgroup:/sys/fs/cgroup \
        -v $(pwd):$TOPDIR:rw \
        fedora:28   /usr/sbin/init
    DOCKER_CONTAINER_ID=$(docker ps | grep fedora | awk '{print $1}')
fi

docker logs $DOCKER_CONTAINER_ID
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "bash -xe /opencpn-ci/ci/docker-build-flatpak.sh 28;
         echo -ne \"------\nEND OPENCPN-CI BUILD\n\";"
docker ps -a
if [ -n "$TRAVIS" ]; then
    docker stop $DOCKER_CONTAINER_ID
    docker rm -v $DOCKER_CONTAINER_ID
    sudo apt-get install python3-pip python3-setuptools
fi
