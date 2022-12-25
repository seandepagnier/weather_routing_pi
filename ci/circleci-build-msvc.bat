::
:: Build the msvc artifacts
::
set "SCRIPTDIR=%~dp0"
set "GIT_HOME=C:\Program Files\Git"
if "%CONFIGURATION%" == "" set "CONFIGURATION=RelWithDebInfo"

set wx_vers="wx%WXVERSION%"
echo Building %wx_vers%

PATH %SCRIPTDIR%.local\bin;%PATH%
dir %SCRIPTDIR%..
dir %SCRIPTDIR%..\msvc
call "%SCRIPTDIR%..\msvc\win_deps" %wx_vers%
path %SCRIPTDIR%.local\bin;%PATH%;"C:\Program Files\CMake\bin"
call "%SCRIPTDIR%..\cache\wx-config.bat"
echo USING wxWidgets_LIB_DIR: %wxWidgets_LIB_DIR%
echo USING wxWidgets_ROOT_DIR: %wxWidgets_ROOT_DIR%
echo USING OCPN_TARGET_TUPLE: %TARGET_TUPLE%

nmake /?  >nul 2>&1
if errorlevel 1 (
  set "VS_HOME=C:\Program Files\Microsoft Visual Studio\2022"
  call "%VS_HOME%\Community\VC\Auxiliary\Build\vcvars32.bat"
)

if exist build (rmdir /s /q build)
mkdir build && cd build

cmake -A Win32 -G "Visual Studio 17 2022" ^
    -DCMAKE_GENERATOR_PLATFORM=Win32 ^
    -DCMAKE_BUILD_TYPE=%CONFIGURATION% ^
    -DwxWidgets_LIB_DIR=%wxWidgets_LIB_DIR% ^
    -DwxWidgets_ROOT_DIR=%wxWidgets_ROOT_DIR% ^
    -DOCPN_TARGET_TUPLE=%TARGET_TUPLE! ^
    ..
cmake --build . --target tarball --config %CONFIGURATION%
