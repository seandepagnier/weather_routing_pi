## Weather Fax Plugin for OpenCPN

Implement weather fax ability for opencpn.  
This includes retrieval via sound card (radio fax) as well as the internet, or local image files.

 
### Compiling

* git clone git://github.com/seandepagnier/weatherfax_pi.git

Normal Linux Build
* mkdir build
* cd build
* cmake ..
* make
* sudo make install

Normal Linux Build and manually deploy to cloudsmith for testing
* cmake -DCMAKE_BUILD_TYPE=Release ..
* make -j4
* make package
* ./cloudsmith-upload.sh
You will find xml and tarball files.

Windows (visual studio) Build
* wxWidgets 3.1.2 required
* cmake -T v141_xp ..
* cmake --build . --target package --configure Release

Windows (Visual Studio) Build using VS Command Prompt:
* wxWidgets 3.1.2 required 
* cmake  -G "Visual Studio 15 2017" -T v141_xp ..
* cmake --build . --config release --target package 
It builds and creates ..tar.gz  files and xml.
The tarball does not have the xml file

Windows visual studio) Build for Opencpn v4 (old):
* wxWidgets 3.0.2 required 
* cd ..
* cd build
* To produce a binary compatible with Windows XP, you must set the respective toolset
* cmake -T v120_xp ..


###  Build tarball and metadate.xml locally to test.

Git clone the latest plugin, create a new build directory, go into that and issue the requisite commands. 

For Windows:
* Install "git on windows" to get a bash prompt. 
* Download and Unzip http://opencpn.navnux.org/build_deps/OpenCPN_buildwin-4.99a.7z into buildwin directory.
* Start Git-Gui (Bash prompt) and navigate to your local plugin build directory
* Make sure the plugin build directory is completely empty. (required for success) 
* From the build directory, Execute the following at the Git-Gui prompt:
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


### Add Plugin to the Plugin Manager Catalog

Add your new Metadata xml to Plugin Manager Catalog
Make Pull Request to github.com/OpenCPN/Plugiins 

After Circleci, Travis and Appveyor have built the environments and deployed to one of the Cloudsmith Repositories, the resultant metadata files (.xml) must be copied and pushed up too the plugins master branch github.com/OpenCPN/plugins to become part of the the master catalog. Jon Gough has provided some bash/python scripts that accomplish copy to your local branch to assist or you can simply do this part manually.
   
Generally try not to use raw git commands unless really needed, In Linux & Windows install the 'beta' testing version (for free opensource work) SmartGit (sometimes it hasn't had all the capabilities of git).


#### Use Smartgit  https://www.syntevo.com/smartgit/

Initially Fork opencpn/plugins to your github remote account, single time.
Start with a clean clone of the source (upstream) git, then keep using this until it becomes too much of a mess, then start over.

Do the following for every change that needs/has new xml:
1. Git pull upstream master
1. Git Create new local branch, i.e. rg_master or rg_beta or rg_alpha or rg_experimental
1. Checkout to new branch
1. Delete your old xml files you no longer want
1. Run 'download_xml_bash.sh' with the correct parameters for your Cloudsmith repository and build
1. Add new xml files to git in your local repository (this is called staging in SmartGit)
1. Commit these updates
1. Push your commit to your remote repository 'origin'
1. Goto your origin repository on the web
1. Create the pull request for the upstream repository
1. Check the outcome of its test action (it will run the validate_xml.sh script on what you have provided)

When you are notified your pull request has been accepted, then delete the following:
1. Your local branch you made the changes in
1. Your remote branch (can be done through SmartGit in one dialog). 
For the next change just start at the 'git pull upstream master' (just below the top).

When you create the pull request to opencpn/plugins master branch (which adds new and removes old xml files for your build), the remote git will now do validation checking (you can do this locally if you want, just use the validate_xml.sh script) and the ocpn-plugins.xml build. Unless there is more than one person working on a plugin at one time and updating the same xml files there should be no collisions.


#### Use a Manual process (not using Smartgit).

Initial Setup of Remote Fork and Local Repository
1. From the web browser httpls/github.com/username/ "Fork" the OpenCPN/plugins repository to create your remote repository.
1. From your local machine in the github folder, to create a local 'plugins' repository
1. Git clone https://github.com/OpenCPN/plugins.git (upstream)

Set remote upstream and origin	
1. Git remote add upstream https://github.com/OpenCPN/plugins.git   Set the 'upstream' remote
1. Git remote add origin https://github.com/username/plugins.git    Set the 'origin' remote

Create a new local branch, i.e. rg_master or rg_beta or rg_alpha or rg_experimental using upstream/master
1. Example:   git checkout -b 'rg-master' upstream/master
1. Now in the metadata folder remove/delete all of 'your' plugin files that are no longer wanted.
1. Leave the other xml files from other plugins intact and without change.	
1. For Windows, if you have a firewall/antivirus, you must permitting scripts to run in your settings, 

Use the Git-Gui (bash prompt) from your local github/plugins' folder, to run Jon's bash script.
1. Run: 'download_xml_bash.sh' with the correct parameters for your cloudsmith repository and build.
1. Examples
   1. ./download_xml_bash.sh <cloudsmith_repository> <plugin_version>  <cloudsmith_user>  <cloudsmith_level>	
   1. ./download_xml_bash.sh testplugin_pi 1.0.114.0 jon-gough prod
   1. ./download_xml_bash.sh weather-routing 1.13.8.0 opencpn prod

Then add the changed files, commit and push to remote origin rg-master branch
1. git add metadata/    To add the metadata files
1. git commit -am "[pluginname] v[version number]"         Commit the metadata files
1. git push -u origin rg-master    (Does the same as two commands 'git branch --set-upstream-to=origin/jg_master'  and 'git push')

Goto upstream/master in a web browser and create pull request to OpenCPN/plugins master  <---- from [gitusername]/plugins rg-master 
We can leave the local rg-master branch in place until the next job (see above) 	
		
When you are notified your PR has been accepted, we need to clean up our git plugins repository.
1. git checkout master                       Checkout the local master branch
1. git branch origin -d  [or -D] rg-master   Delete local rg-master
1. git push --delete origin rg-master        Delete remote origin rg-master 

Create an identical local copy of upstream/master as a new branch "rg-master"
1. git checkout -b rg-master upstream/master


### Windows Specific Libraries for Certain Plugins

The Windows compilation of weatherfax_pi is dependent on these files 
We can also use the "Extra" Folders for this - (need to augment this)

1. Under windows, you must find the file "opencpn.lib" (Visual Studio) or "libopencpn.dll.a" (mingw) which is built in the build directory after compiling opencpn.  This file must be copied to the plugin directory.
1. There are also some libraries and an external program needed:
   1. http://sourceforge.net/projects/opencpnplugins/files/opencpn_packaging_data/portaudio-vc12.7z/download
   1. http://sourceforge.net/projects/opencpnplugins/files/opencpn_packaging_data/PVW32Con.exe/download (not an archive, this one just copy to buildwin)
   1. Unpack all these files into the buildwin directory as the compile is dependent on these.


### OSX Specific Libraries

The OS X compilation of weatherfax_pi is dependent on these files 

1. Under OS X the port audio library is needed. Install it using ```brew install portaudio```
1. There is a zip file containing header and library files:
   1. http://sourceforge.net/projects/opencpnplugins/files/weatherfax_pi/OSXportaudiofiles.zip/download
   1. Download this file and unzip it in the /usr/local directory as the compile is dependent on these.
   1. Then make create-pkg command will include the library in the package.


### License

The plugin code is licensed under the terms of the GPL v3+
