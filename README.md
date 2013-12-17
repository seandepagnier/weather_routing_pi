weather_routing_pi
==================



Weather Routing plugin for Opencn
The plugin is authored by Sean Depagnier.

Implement weather_routing ability for opencpn
It uses Gribs and Climatology Data in the form of gribs, to optimize routes with performance information about the sail boat.

Compiling
You have to be able to compile OpenCPN itself - Get the info at http://opencpn.org/ocpn/developers_manual

go to the plugin subdirectory in your OpenCPN source tree
git clone git://github.com/seandepagnier/weather_routing_pi.git
Build as normally:

cd ..
cd build
cmake ..
cmake --build .
License
The plugin code is licensed under the terms of the GPL v2 or, at your will, later.



===
These changes have been made to permit the plugin to compile successfully by MSVC++ and be run in the Windows environment.
Illegal External File References and other bugs will be fixed.
