#!/usr/bin/env bash

if [[ "$#" -eq "1" && ( "$1" -eq "-h " ||  "$1" -eq "-help" ) ]]; then
    echo "Will update opencpn-libs current branch to latest version."
fi

git submodule update --remote --merge opencpn-libs
git add opencpn-libs
git commit -m "Updating current opencpn-libs to latest version"

echo "This will now need to be pushed to make it available to circleci builds in the cloud"