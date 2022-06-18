#!/bin/sh  -xe

#
# Build the Android artifacts inside the circleci linux container
#


set -xe

pwd

ls -la

# FOR LOCAL BUILD - have a local version to avoid big download each run - need to stage it but not commit it. DO NOT COMMIT AND PUSH master.zip
if [ "${CIRCLECI_LOCAL,,}" = "true" ]; then
    if [[ -d ~/circleci-cache ]]; then
        if [[ -f ~/circleci-cache/apt-proxy ]]; then
            cat ~/circleci-cache/apt-proxy | sudo tee -a /etc/apt/apt.conf.d/00aptproxy
            cat /etc/apt/apt.conf.d/00aptproxy
        fi
        if [[ ! -f ~/circleci-cache/master.zip ]]; then
            sudo wget https://github.com/bdbcat/OCPNAndroidCommon/archive/master.zip -O ~/circleci-cache/master.zip
        fi
        MASTER_LOC=~/circleci-cache
        #unzip -qq -o /home/circleci/circleci-cache/master.zip
    fi
else
    MASTER_LOC=$(pwd)
    wget https://github.com/bdbcat/OCPNAndroidCommon/archive/master.zip
    #unzip -qq -o master.zip
fi
echo "unzipping $MASTER_LOC/master.zip"

unzip -qq -o $MASTER_LOC/master.zip

sudo apt-get -q update
sudo apt-get -y install git cmake gettext unzip

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

pwd
ls -la

mkdir -p build
cd build

rm -f CMakeCache.txt
COMPDIR=$(find ~/. -regex ".*/ndk/22.[0-9].[0-9]*")

cmake  \
  -D_wx_selected_config=androideabi-qt-arm64 \
  -DwxQt_Build=build_android_release_64_static_O3 \
  -DQt_Build=build_arm64/qtbase \
  -DCMAKE_AR=$COMPDIR/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android-ar \
  -DCMAKE_CXX_COMPILER=$COMPDIR/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang++ \
  -DCMAKE_C_COMPILER=$COMPDIR/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang \
  -DOCPN_Android_Common=OCPNAndroidCommon-master \
  -DCMAKE_INSTALL_PREFIX=/ \
  ..

# Get number of processors and use this on make to speed up build
procs=$(awk -F- '{print $2}' /sys/fs/cgroup/cpuset/cpuset.cpus)
procs=$((procs + 1))
make_cmd="make -j"$procs
eval $make_cmd
make package

#  All below for local docker build
#ls -l

#xml=$(ls *.xml)
#tarball=$(ls *.tar.gz)
#tarball_basename=${tarball##*/}

#echo $xml
#echo $tarball
#echo $tarball_basename
#sudo sed -i -e "s|@filename@|$tarball_basename|" $xml


#tmpdir=repack.$$
#sudo rm -rf $tmpdir && sudo mkdir $tmpdir
#sudo tar -C $tmpdir -xf $tarball_basename
#sudo cp oesenc-plugin-android-arm64-16.xml metadata.xml
#sudo cp metadata.xml $tmpdir
#sudo tar -C $tmpdir -czf $tarball_basename .
#sudo rm -rf $tmpdir
