# KEEP EXISTING DIRECTORIES AND FILES
-----------------------------------------------------
#### Important: 
1. Make these changes on a new branch "frontend1" or "ci" (if possible).
1. Keep your currently working "master" branch intact.
1. Create a new branch EG: git checkout -b master-frontend2 master
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

# LIST of FOLDERS & FILES copied from TESTPLUGIN_PI
----------------------------------------------------
#### Add these Directories + Sub-directories + Files

Copy the following directories and files from testplugin_pi to the same location in the plugin directory you are working on:
- testplugin_pi/.circleci
- testplugin_pi/api-16
- testplugin_pi/ci
- testplugin_pi/cmake
- testplugin_pi/buildosx
- testplugin_pi/debian
- testplugin_pi/mingw

#### Files
- testplugin_pi/appveyor.yml
- testplugin_pi/.travis.yml
- testplugin_pi/CMakeLists.txt

#### Directories NOT needed
The following directories and files are not needed from testplugin_pi
- testplugin_pi/data
- testplugin_pi/forms
- testplugin_pi/extinclude
- testplugin_pi/extsrc
- testplugin_pi/include
- testplugin_pi/lib
- testplugin_pi/ocpninclude
- testplugin_pi/ocpnsrc
- testplugin_pi/src

# CHANGES REQUIRED
----------------------------------------------------------------
1. Rename CMakeLists.txt, appveyor.yml, .travis.yml adding  .save for reference.
1. Modify CMakeLists.txt file, following the in-line notes
   - Modify Plugin Specifics about Line 20 to 75.
   - Enter the 5 name types
   - Determine CommonName by searching <plugin>_pi.cpp for "GetCommon" and enter that into set(CommonName)
   - Enter the current version numbers and data
   - Enter Parent, Package and Git User. Git Repository
   - Enter Cloudsmith User and (optional) Cloudsmith Base Repository 
   - Enter XML Info_url, Summary and Description
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
    
   
