# FRONTEND 2  v1.0.124

GOAL: Assist plugin developers to convert their plugins to the Plugin Manager system which:
1. Uses tarballs and metadata.xml files to store the necessary files and provide information.
1. Deploys directly to Cloudsmith repositories (or another location)
1. Files are accessed by OpenCPN Plugin Manager
1. Installed via options > Plugins > Plugin Manager Catalogs "master" and "beta" url in the metadata files
1. During installation. 

This "Frontend2" configuration is found in jongough/testplugin_pi and is easier to configure all from CMakeLists.txt. 
"Frontend1" can be found in the current versions of bdbcat/Oesenc_pi and rgleason/Vdr_pi. 


## KEEP EXISTING DIRECTORIES AND FILES
_________________________________

#### Important:
1. Make these changes on a new branch "frontend2" or "ci" (if possible).
1. Keep your currently working "master" branch intact.
1. Create a new branch EG: 'git checkout -b frontend2 master'
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
- Any other specific plugin directories essential to your plugin.


## LIST of FOLDERS & FILES copied from TESTPLUGIN_PI
_________________________________

#### Add these Directories + Sub-directories + Files

Copy the following directories and files from testplugin_pi to the same location in the plugin directory you are working on:
- testplugin_pi/.circleci
- testplugin_pi/api-16
- testplugin_pi/ci
- testplugin_pi/cmake
- testplugin_pi/buildosx
- testplugin_pi/mingw
- testplugin_pi/extinclude  (JSON Validation - only Ocpn_draw, watchdog, weather_routing)
- testplugin_pi/extsrc  (JSON Validation - only Ocpn_draw, watchdog, weather_routing)

#### Files
Review and Compare these files, deployment to Cloudsmith is required.
- testplugin_pi/appveyor.yml  (compare with your file)
- testplugin_pi/.travis.yml   (compare with your file)
- testplugin_pi/CMakeLists.txt  (see in line notes, major editing comparing to old file)
- See notes below for more detail

#### Directories NOT needed
The following directories and files are not needed from testplugin_pi
- testplugin_pi/data
- testplugin_pi/forms
- testplugin_pi/include
- testplugin_pi/lib
- testplugin_pi/ocpninclude
- testplugin_pi/ocpnsrc
- testplugin_pi/src


## CHANGES REQUIRED
_________________________________

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
     - Make sure all your necessary libraries are found.
     - Add/Modify a statement like this to join all of your project's set(
       - EG: add_library(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${NMEA0183} ${LIBSSRC})
1. API Number must be at least 1.16 for the new Plugin Manager, due to a change in how directories are found and location.
1. API Names have been changed from MY_API_VERSION_MAJOR and MY_API_VERSION_MINOR, to OCPN_API_VERSION_MAJOR/MINOR
   - OCPN_API_VERSION_MAJOR/MINOR are now used in cmake/in-files/version.h.in
   - In the file (pluginname_pi).cpp the are several lines which need to be changed from MY_API_VERSION_MAJOR/MINOR to 
     - EG: "return OCPN_API_VERSION_MAJOR;"
     - EG: "return OCPN_API_VERSION_MINOR;"
   - Also in file (pluginname_pi).cpp find   wxString (pluginname_pi)::GetCommonName() and change 
	 - return _("(pluginname-pi)");  to
     - return _T(PLUGIN_COMMON_NAME);		
   - In the file (plugin_pi).h there are several lines which need to be commented out.
     - // #define     MY_API_VERSION_MAJOR    1
     - // #define     MY_API_VERSION_MINOR    16
	 - or removed.
     - Because the new values definitions are defined in cmake/in-files/version.h.in	   
1. Cmake Files are somewhat generic, but often can be plugin specific, depending on the plugin.
   - Review the cmake.save files one by one with the new ones and make necessary adjustments.
   - Configuring this is not simple and requires knowledge about the plugin operation.
1. Get the ci/environment scripts working on Circleci, Appveyor and .travis-ci
1. Then get the uploads to Cloudsmith working.
   - First create your Cloudsmith Account, then join the OpenCPN Organization as a member of the Plugins Team.
   - Once you are accepted as a member, you will be able to create three repositories for your plugin.
   - The management of Cloudsmith has advised that Organizations will in the future be restricted to having repositories in the Organization's Folder, so it is best to start out that way.  
   - Configuration of uploads to Cloudsmith destinations:
     - ci\cloudsmith-upload.sh points to  ..cmake\in-files\cloudsmith-upload.sh.in
     - See for standard repository directories.
       - @CLOUDSMITH_BASE_REPOSITORY@-prod
       - @CLOUDSMITH_BASE_REPOSITORY@-beta
       - @CLOUDSMITH_BASE_REPOSITORY@-alpha
     - The Frontend2 defaults to 'CLOUDSMITH_BASE_REPOSTORY' = 'yoour Github Repository', however you should set this value in CMakeLists.txt as "opencpn"
     - In the organization "OpenCPN" create (pluginname)-alpha, (pluginname)-beta and (pluginname)-alpha repositories.
	 - Make sure that you select "Open Source", not just "public". It must be "Open Source"
     - See the more detailed instructions in the Developer's Manual wiki.
   - For custom Cloudsmith repository destinations, modify if needed.


## DEPLOYMENT
_________________________________

The current setup for Frontend2 plugins does this:
- Non-Master branch - Push with no tag -> Alpha repository
- Non-Master branch - Push with tag -> Beta repository
- Master branch - Push with no tag -> Beta repository
- Master branch - Push with tag -> Prod repository

 There are several ways to issue a "push with tag" or "tagged push"
    - See https://opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:developer_manual:pi_installer_procedure#deploy_to_prod_repository
  $ git commit -am "my last changes")
  $ git tag v[new version number]
  $ git push <repo-name> <tag-name>  OR
  $ git push origin refs/tags/tag_a

Not preferred:
  $ git push --tags origin master:master  <--- Is not preferred due possibility of pushing all old tags..

Example:
1. Change version number and date in  CMakeLists.txt . Save  
1. git add CMakeLists.txt
1. git commit -am "v1.9.5.10"
1. git tag v1.9.5.10
1. git push origin refs/tags/v1.9.5.10
  - Enumerating objects: 5, done.
  - remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
  - To https://github.com/xxxx/weatherfax_pi.git  
  - * [new tag]           v1.9.5.10 -> v1.9.5.10
1. git push origin master 


### Weatherfax_pi specific differences from testplugin_pi "Frontend2"
_________________________________
   
Weatherfax_pi needs to have sound support for Windows and Mingw, additionally use with rtlsdr requires additional files. 

1. circleci/config.yml  -same
1. ci/ all scripts same except
   - ci/control  weatherfax has 'build librtlsdr-dev'
   - ci/circleci-build-mingw.sh downloads and installs portauduio & PVWcon32.exe
     - wget https://downloads.sourceforge.net/project/opencpnplugins/opencpn_packaging_data/PVW32Con.exe
     - wget https://downloads.sourceforge.net/project/opencpnplugins/opencpn_packaging_data/portaudio-vc12.7z
     - 7za e portaudio-vc12.7z -o../buildwin -y
     - mv PVW32Con.exe ../buildwin/.
1. cmake/  all the files in testplugin are used in weatherfax and are identical
1. cmake/  weatherfax has 3-4 of its own files too
1. cmake/in-files/ has all the same files
1. extinclude   all the same
1. extsrc   all the same
1. mingw  
   - opencpn-deps.spec file has
     - BuildRequires: p7zip
     - BuildRequires: wget
1. .travis.yml  same
1. appveyor.yml  (same except the path & name of plugin and the api key)
1. CMakeLists.txt  -similar, very different in places, 
   - personnal settings,
   - "USE_GL ON" for weatherfax,
   - plugin name is different,
   - libraries and includes all different.