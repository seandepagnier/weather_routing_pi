::
:: Build the msvc artifacts
::

set "SCRIPTDIR=%~dp0"
set "GIT_HOME=C:\Program Files\Git"
if "%CONFIGURATION%" == "" set "CONFIGURATION=RelWithDebInfo"

set wx_vers="wx%WX_VER%"
echo Building %wx_vers%
echo Building with %MSVC_VERSION%

PATH %SCRIPTDIR%.local\bin;%PATH%
echo SCRIPTDIR: %SCRIPTDIR%
dir %SCRIPTDIR%
dir %SCRIPTDIR%..
dir %SCRIPTDIR%..\msvc
call %SCRIPTDIR%..\msvc\win_deps.bat %wx_vers%
path %SCRIPTDIR%.local\bin;%PATH%;"C:\Program Files\CMake\bin"
call "%SCRIPTDIR%..\cache\wx-config.bat"
set PATH=%EXTRA_PATH%;%PATH%
echo EXTRA_PATH: %EXTRA_PATH%
echo USING wxWidgets_LIB_DIR: %wxWidgets_LIB_DIR%
echo USING wxWidgets_ROOT_DIR: %wxWidgets_ROOT_DIR%
echo USING OCPN_TARGET_TUPLE: %TARGET_TUPLE%

nmake /?  >nul 2>&1
if errorlevel 1 (
  set "VS_HOME=C:\Program Files\Microsoft Visual Studio\2022"
  call "%VS_HOME%\Community\VC\Auxiliary\Build\vcvars32.bat"
)

git submodule update --init opencpn-libs

dir

if exist build (rmdir /s /q build)
mkdir build && cd build
dir

wget https://sourceforge.net/projects/opencpnplugins/files/opencpn.lib
wget https://download.opencpn.org/s/oibxM3kzfzKcSc3/download/OpenCPN_buildwin-4.99a.7z
7z x -y OpenCPN_buildwin-4.99a.7z -o..\buildwin
wget https://download.opencpn.org/s/54HsBDLNzRZLL6i/download/nsis-3.04-setup.exe
nsis-3.04-setup.exe /S

echo Check if poedit has been installed
poedit -version
echo Done check

echo Create build environment

if "%MSVC_VERSION%" == "2019" (
cmake -T v141_xp -G "Visual Studio 16 2019" ^
    -DCMAKE_GENERATOR_PLATFORM=Win32 ^
    -DCMAKE_BUILD_TYPE=%CONFIGURATION% ^
    -DwxWidgets_LIB_DIR=%wxWidgets_LIB_DIR% ^
    -DwxWidgets_ROOT_DIR=%wxWidgets_ROOT_DIR% ^
    ..
) else (
cmake -A Win32 -G "Visual Studio 17 2022" ^
    -DCMAKE_GENERATOR_PLATFORM=Win32 ^
    -DCMAKE_BUILD_TYPE=%CONFIGURATION% ^
    -DwxWidgets_LIB_DIR=%wxWidgets_LIB_DIR% ^
    -DwxWidgets_ROOT_DIR=%wxWidgets_ROOT_DIR% ^
    ..
)


cd
dir

echo Build for windows

:cmake --build . --target tarball --config %CONFIGURATION%
cmake --build . --target package --config %CONFIGURATION%
