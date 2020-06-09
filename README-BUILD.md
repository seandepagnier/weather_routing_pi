### Weather Fax Plugin for OpenCPN
=======================================
Implement weather fax ability for opencpn.  This includes retrieval via sound card (radio fax) as well as the internet, or local image files.
 
### Compiling
============
* git clone git://github.com/seandepagnier/weatherfax_pi.git

Build Normally (linux)
* mkdir build
* cd build
* cmake ..
* make
* sudo make install

Build Normally (linux) and manually deploy to cloudsmith for testing
* cmake -DCMAKE_BUILD_TYPE=Release ..
* make -j4
* make package
* ./cloudsmith-upload.sh
You will find xml and tarball files.

Compiling on windows (visual studio):
* cd ..
* cd build
* cmake .. # To produce a binary compatible with Windows XP, you must set the respective toolset - use 'cmake -T v120_xp ..'   (use cmake -T v141_xp .." for version 5)
* make
* make install

Another Windows using VS Command Prompt:
* cmake  -G "Visual Studio 15 2017" -T v141_xp ..
* cmake --build . --config release --target package 
It builds and creates ..tar.gz  files and xml.
The tarball does not have the xml file

Compile in Windows using the VS Command Prompt from the build directory:
* cmake -T v141_xp ..
* cmake --build . --process package --configure Release

Build tarball and metadate.xml locally to test.
Git clone the latest plugin, create a new build directory, go into that and issue the requisite commands.
For Windows, from Git-gui (install "git on windows") started from start menu Git/Git GUI:
* Download and Unzip http://opencpn.navnux.org/build_deps/OpenCPN_buildwin-4.99a.7z into buildwin directory.
* Start Git-Gui (install "git on windows") and navigate to the plugin build directory
* Change directory to the plugins build directory, make sure it is completely empty. 
* Execute the following at the Git-Gui prompt:
* cmake -T v141_xp ..
* cmake --build . --target package --config release
* ./cloudsmith-upload.sh
Note that the cloudsmith-upload.sh script process updates the 'xml' file target-url path
and adds that metadata.xml file to the tarball.

For Linux:
* Make sure you are in the correct, clean (empty) directory for a build
* cmake -DWXWIDGETS_FORCE_VERSION=3.0 -DCMAKE_BUILD_TYPE=Release ..
* make -j4
* make package
* ./cloudsmith-upload.sh
Under linux change the number of -j4 to how many processors you have, 
I use -j12 (6 real processors, 2 threads per processor AMD  Ryzen 5 1600). 
Under windows VS will pick the number of processor automatically.

### Deployment Repository
=============================

The script cloudsmith-uploads.sh which is created at runtime from cmake/in-files/cloudsmith-upload.sh.in
uploads the .tar.gz and .xml artifacts to cloudsmith
1. Builds are uploaded to one of PROD, BETA or ALPHA repositories
1. The repository depends on the branch and use of tag to determine which repo.
   1. ALPHA: Non-master branch no tag
   1. BETA: Non-master branch with annotated tag
   1. BETA: Master branch no tag
   1. PROD: Master branch with annotated tag
If this script is used on CIRCLECI, TRAVIS or APPVEYOR it will upload the correct files to the chosen repository
If this script is used locally it will build the correct artifacts but will not do the upload


### Windows Specific Libraries
=============================
The Windows compilation of weatherfax_pi is dependent on these files 

Under windows, you must find the file "opencpn.lib" (Visual Studio) or "libopencpn.dll.a" (mingw) which is built in the build directory after compiling opencpn.  This file must be copied to the plugin directory.

There are also some libraries and an external program needed:
http://sourceforge.net/projects/opencpnplugins/files/opencpn_packaging_data/portaudio-vc12.7z/download
http://sourceforge.net/projects/opencpnplugins/files/opencpn_packaging_data/PVW32Con.exe/download (not an archive, this one just copy to buildwin)
and unpack all these files into the buildwin directory as the compile is dependent on these.

### OSX Specific Libraries
==========================
The OS X compilation of weatherfax_pi is dependent on these files 

Under OS X the port audio library is needed. Install it using ```brew install portaudio```
There is a zip file containing header and library files:
http://sourceforge.net/projects/opencpnplugins/files/weatherfax_pi/OSXportaudiofiles.zip/download
Download this file and unzip it in the /usr/local directory as the compile is dependent on these.
The make create-pkg command will include the library in the package.

### License
============
The plugin code is licensed under the terms of the GPL v3+
