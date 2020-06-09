## FRONTEND 2

This document is intended to assist plugin developers intending to convert 
their plugins to the new Plugin Manager system of tarballs and metadata.xml files
deployed directly to Cloudsmith repositories that are accessed by OpenCPN Plugin Manager via Catalogs "master" and "beta"
during installation. We call this configuration "Frontend 2".  This version is easier to configure.
"Frontend 1" can be found in the current versions of Oesenc_pi and Vdr_pi. 

# KEEP EXISTING DIRECTORIES AND FILES
----------------------------------------------------
#### Important:
1. Make these changes on a new branch "frontend2" or "ci" (if possible).
1. Keep your currently working "master" branch intact.
1. Create a new branch EG: git checkout -b frontend2 master
1. Work in the new branch.

#### Preparation: 
1. Rename CMakeLists.txt ----> CMakeLists.save.txt for reference
1. Rename appveyor.yml ----> appveyor.save.yml for reference
1. Rename .travis.yml ----> .travis.save.yml for reference
1. Rename the cmake directory ---> "cmake.save" for reference
1. Keep any other specialized plugin directories

#### KEEP these directories, don't over-write them!:
- Include
- Data
- src
- po
- Any other specific plugin directories

## LIST of FOLDERS & FILES copied from TESTPLUGIN_PI
----------------------------------------------------
#### Add these Directories + Sub-directories + Files

Copy the following directories and files from testplugin_pi to the same location in the plugin directory you are working on:
- testplugin_pi/.circleci
- testplugin_pi/api-16
- testplugin_pi/ci
- testplugin_pi/cmake
- testplugin_pi/buildosx
- testplugin_pi/debian  (can be removed)
- testplugin_pi/mingw
- testplugin_pi/extinclude  Needed for json validation 
- testplugin_pi/extsrc  Needed for json validation

#### Files
- testplugin_pi/appveyor.yml
- testplugin_pi/.travis.yml
- testplugin_pi/CMakeLists.txt

#### Directories NOT needed
The following directories and files are not needed from testplugin_pi
- testplugin_pi/data
- testplugin_pi/forms
- testplugin_pi/include
- testplugin_pi/lib
- testplugin_pi/ocpninclude
- testplugin_pi/ocpnsrc
- testplugin_pi/src

# CHANGES REQUIRED 
----------------------------------------------------------------
1. Rename CMakeLists.txt, appveyor.yml, .travis.yml adding  .save for reference.
1. Modify CMakeLists.txt file, following the in-line notes
   - Modify Personal Specifics about Line 20 to 75.
   - Enter setup information:
     - Plugin name types
     - CommonName -search "plugin"_pi.cpp for "GetCommon"
     - Version numbers and date
     - Parent, Package and Git User. Git Repository
     - Cloudsmith User
     - Cloudsmith Base Repository (optional) 
     - Plugin XML Info_url, Summary and Description
   - In the section that replaces testplugin specific code:
     - Modify/configure the set(SRCS and HDRS and 'Include' Directories using CMakeLists.save.txt
     - Modify/configure 'Set(SRCS & HDRS' Directories using CMakeLists.save.txt
     - Modify/configure 'Add Library' listings for the plugin.
     - Make sure all your necesary libraries are found.
     - Add/Modify a statement like this to join all of your project's set(
       - EG: add_library(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${NMEA0183} ${LIBSSRC})
1. Cmake Files are somewhat generic, but often can be plugin specific, depending on the plugin.
   - Review the cmake.save files one by one with the new ones and make necessary adjustments.
   - Configuring this is not simple and requires knowledge about the plugin operation.
1. Next get the ci/environment scripts working on Circleci, Appveyor and .travis-ci
1. Then get the uploads to Cloudsmith working.
   - Configuration of uploads to Cloudsmith destinations:
     - ci\cloudsmith-upload.sh points to  ..cmake\in-files\cloudsmith-upload.sh.in
     - See for standard repository directories.
       - @CLOUDSMITH_BASE_REPOSITORY@-prod
       - @CLOUDSMITH_BASE_REPOSITORY@-beta
       - @CLOUDSMITH_BASE_REPOSITORY@-alpha
     - Default 'CLOUDSMITH_BASE_REPOSTORY' is your 'Github Repository', set in CMakeLists.txt
     - In your free Cloudsmith Account set up the prod, beta and alpha 'open source" repositories.
     - See the instrustions in the Developer's Manual wiki.
   - For custom Cloudsmith repository destinations, modify if needed.
     - Example custom Cloudsmith path uploads
     - Mauro's repositories https://cloudsmith.io/~mauro-calvi/repos/
     - OCPN_STABLE_REPO=mauro-calvi/squiddio-stable
     - OCPN_UNSTABLE_REPO=mauro-calvi/squiddio-pi
     - OCPN_PKG_REPO=mauro-calvi/squiddio-manual

# Deployment 

The current setup for Frontend2 plugins does this:
    - No tag -> Alpha repository
    - Non-Master branch tag -> Beta repository
    - Master branch no tag -> Beta repository
    - Master branch tag -> Prod repository

 There are several ways to issue a "tagged push"
    - See https://opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:developer_manual:pi_installer_procedure#deploy_to_prod_repository
  $ git commit -am "my last changes")
  $ git tag v[new version number]
  $ git push <repo-name> <tag-name>  OR  $ git push origin refs/tags/tag_a
Not preferred:
  $ git push --tags origin master:master  <--- Is not preferred due to pushing all old tags..

Example:
1. Change version number and date in  CMakeLists.txt . Save  
1. git add CMakeLists.txt
1. git commit -am "v1.9.5.10"
1. git tag v1.9.5.10
1. git push origin refs/tags/v1.9.5.10
  Enumerating objects: 5, done.
  remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
  To https://github.com/xxxx/weatherfax_pi.git  
    * [new tag]           v1.9.5.10 -> v1.9.5.10
1. git push origin master 
   
### Comparing Plugin Manager "Frontend2" in testplugin_pi to weatherfax_pi,
to highlight the differences

1. circleci/config.yml  -same
1. ci/ all scripts same except
1. ci/control  weatherfax has librtlsdr-dev
1. ci/circleci-build-mingw.sh     downloads and installs portauduio & PVWcon32.exe
    - wget https://downloads.sourceforge.net/project/opencpnplugins/opencpn_packaging_data/PVW32Con.exe
    - wget https://downloads.sourceforge.net/project/opencpnplugins/opencpn_packaging_data/portaudio-vc12.7z
    - 7za e portaudio-vc12.7z -o../buildwin -y
    - mv PVW32Con.exe ../buildwin/.
1. cmake/  all the files in testplugin are used in weatherfax and are identical
1. cmake/  weatherfax has 3-4 of its own files too
1. cmake/in-files/ has all the same files
1. extinclude   all the same
1. extsrc   all the same
1. mingw  all the same
1. .travis.yml  same
1. appveyor.yml  (same except the path & name of plugin and  the api key)
1. CMakeLists.txt  -similar, very different in places, 
    - personnal settings,
    - "USE_GL ON" for weatherfax,
    - plugin name is different,
    - libraries and includes all different.