#!/usr/bin/env bash

#
#

# bailout on errors and echo commands.
set -xe
sudo apt-get -qq update

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" \
    | sudo tee /etc/default/docker > /dev/null
sudo service docker restart;
sleep 5;

docker run --rm --privileged multiarch/qemu-user-static:register --reset

docker run --privileged -d -ti -e "container=docker" \
      -v ~/source_top:/source_top \
      -v $(pwd):/ci-source:rw \
      $DOCKER_IMAGE /bin/bash
      
DOCKER_CONTAINER_ID=$(sudo docker ps | grep raspbian | awk '{print $1}')


#echo $DOCKER_CONTAINER_ID 

docker exec -ti $DOCKER_CONTAINER_ID apt-get update
docker exec -ti $DOCKER_CONTAINER_ID echo "------\nEND apt-get update\n" 

docker exec -ti $DOCKER_CONTAINER_ID apt-get -y install git cmake build-essential cmake gettext wx-common libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release 


#docker exec -ti $DOCKER_CONTAINER_ID echo $OCPN_BRANCH

#docker exec -ti $DOCKER_CONTAINER_ID wget https://github.com/bdbcat/oernc_pi/tarball/$OCPN_BRANCH
#docker exec -ti $DOCKER_CONTAINER_ID tar -xzf $OCPN_BRANCH -C source_top --strip-components=1


#docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -c \
#    'mkdir source_top/build; cd source_top/build; cmake ..; make; make package;'
echo $TRAVIS_BRANCH
echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "export TRAVIS=$TRAVIS; export TRAVIS_BRANCH=$TRAVIS_BRANCH; export OCPN_TARGET=$OCPN_TARGET; mkdir ci-source/build; cd ci-source/build; cmake ..; make; make package;"
 
echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

