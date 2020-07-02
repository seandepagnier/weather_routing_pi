#!/usr/bin/env bash

#
# Travis build for raspbian part 2
#

# bailout on errors and echo commands.
set -xe

DOCKER_CONTAINER_ID=$(sudo docker ps | grep $BUILD_ENV | awk '{print $1}')

echo $TRAVIS_BRANCH
echo $OCPN_TARGET

docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "export TRAVIS=$TRAVIS; export TRAVIS_REPO_SLUG=$TRAVIS_REPO_SLUG; export TRAVIS_BRANCH=$TRAVIS_BRANCH; export TRAVIS_TAG=$TRAVIS_TAG; export OCPN_TARGET=$OCPN_TARGET; export BUILD_FLAGS=$BUILD_FLAGS; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"
 
echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

