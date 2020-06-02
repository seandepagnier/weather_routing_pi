#!/usr/bin/env bash

#
# Run created cloudsmith upload script
#

CURRENT_DIR=$(pwd)
bash $CURRENT_DIR/build/cloudsmith-upload.sh
