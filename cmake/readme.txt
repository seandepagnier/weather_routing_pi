The *.cmake files in this directory provide resources to build the make environment. However, the contents of these
files is mainly static, so this helps reduce the size and complexity of the main CMakeLists.txt file.

The following files are manadatory for the cmake process to work:
pluginConfigure.cmake
PluginInstall.cmake
PluginLocalization.cmake
PluginPackage.cmake
PluginSetup.cmake

These file should only be changed with great care as they use generalised processes to handle the generation of the
build environments. If you need to add functionality that you need for your plugin then add extra cmake files here
for your purpose. It is strongly suggested that you do not change these files as if updates to the process are
required it is much easier to do a file replace than identify what has changed.

An example of an 'extra' cmake file can be seen with:
FindTinyXML.cmake

Extra files are added to the CMakeLists.txt file by using an include statement such as:
include("cmake/FindTinyXML.cmake")

Please be case sensitive as it may work on windows builds without this but Linux builds will not.
