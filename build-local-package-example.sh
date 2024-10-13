#!/bin/bash
# This file should be run from the local build directory you want to use
# This file is a modification of the 'circleci local' build process, so it will complain the circleci-output directory is not found.
# If you want to do a 'circleci local' build use the 'run-circleci-local.txt' file as the example to follow.
#
# Change the exports to meet the build requirements, i.e. the values in the config.yml file relevant to the particular job
export OCPN_TARGET=bookworm
export BUILD_GTK3=true
export WX_VER=32
export LOCAL_DEPLOY=true
# this removes old xml files from the build directory
rm *.xml
# the actual configuration, build and installable package creation
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j2
make package
chmod a+x cloudsmith-upload.sh
./cloudsmith-upload.sh

