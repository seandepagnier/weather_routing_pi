:: Install build dependencies. Requires a working choco installation,
:: see https://docs.chocolatey.org/en-us/choco/setup.
::
:: Usage:
::      win_deps.bat [wx32]
::
:: Arguments:
::     wx32:
::          If present install dependencies for building against
::          wxWidgets 3.2. If non-existing or anything but wx32
::          build using 3.1
:: Output:
::     cache\wx-config.bat:
::          Script which set wxWidgets_LIB_DIR and wxWidgets_ROOT_DIR
::
:: Initial run will do choco installs requiring administrative
:: privileges.
::
:: NOTE: at the very end, this script runs refreshenv. This clears the
:: process's PATH and replaces it with a fresh copy from the
:: registry. This means that any "set PATH=..." is lost for caller.

:: Install the pathman tool: https://github.com/therootcompany/pathman
:: Fix PATH so it can be used in this script
::

@echo off
echo In win_deps

setlocal enabledelayedexpansion

if not exist %SCRIPTDIR%\..\cache ( mkdir %SCRIPTDIR%\..\cache )
set "CONFIG_FILE=%SCRIPTDIR%\..\cache\wx-config.bat"
set EXTRA_PATH=

git --version > nul 2>&1
if errorlevel 1 (
   set "GIT_HOME=C:\Program Files\Git"
   if not exist "!GIT_HOME!" choco install -y git
)
echo done git
:: Install choco cmake and add it's persistent user path element
::
cmake --version > nul 2>&1
if errorlevel 1 (
  set "CMAKE_HOME=C:\Program Files\CMake"
  choco install -y --no-progress cmake
  set "EXTRA_PATH=%CMAKE_HOME%\bin;%EXTRA_PATH%"
)

:: Install choco poedit and add it's persistent user path element
::
set "POEDIT_HOME=C:\Program Files (x86)\Poedit\GettextTools"
if not exist "%POEDIT_HOME%" (choco install -y poedit)
dir "%POEDIT_HOME%"
set "EXTRA_PATH=%POEDIT_HOME%\bin;%EXTRA_PATH%"

:: Update required python stuff
::
echo doing python
python --version > nul 2>&1 && python -m ensurepip > nul 2>&1
if errorlevel 1 choco install -y python
python --version
python -m ensurepip
python -m pip install --upgrade pip
python -m pip install -q setuptools wheel
python -m pip install -q cloudsmith-cli
python -m pip install -q cryptography

:: Install pre-compiled wxWidgets and other DLL; add required paths.
::
set SCRIPTDIR=%~dp0
if "%~1"=="wx32" (
  set "WXWIN=%SCRIPTDIR%..\cache\wxWidgets-3.2.1"
  set "wxWidgets_ROOT_DIR=!WXWIN!"
  set "wxWidgets_LIB_DIR=!WXWIN!\lib\vc14x_dll"
  set "TARGET_TUPLE=msvc-wx32"
) else (
  set "WXWIN=%SCRIPTDIR%..\cache\wxWidgets-3.1.2"
  set "wxWidgets_ROOT_DIR=!WXWIN!"
  set "wxWidgets_LIB_DIR=!WXWIN!\lib\vc_dll"
  set "TARGET_TUPLE=msvc"
)

:: Add settings to CONFIG_FILE to allow them to be set in the calling batch file
echo set "EXTRA_PATH=%EXTRA_PATH%" > %CONFIG_FILE%
echo set "wxWidgets_ROOT_DIR=%wxWidgets_ROOT_DIR%" >> %CONFIG_FILE%
echo set "wxWidgets_LIB_DIR=%wxWidgets_LIB_DIR%" >> %CONFIG_FILE%
echo set "TARGET_TUPLE=%TARGET_TUPLE%" >> %CONFIG_FILE%

if not exist "%WXWIN%" (
  wget --version > nul 2>&1 || choco install -y wget
  if  "%~1"=="wx32" (
      echo Downloading 3.2.1
      if not exist  %SCRIPTDIR%..\cache\wxWidgets-3.2.1 (
          mkdir %SCRIPTDIR%..\cache\wxWidgets-3.2.1
      )
      set "GITHUB_DL=https://github.com/wxWidgets/wxWidgets/releases/download"
      wget -nv !GITHUB_DL!/v3.2.1/wxMSW-3.2.1_vc14x_Dev.7z
      7z x -o%SCRIPTDIR%..\cache\wxWidgets-3.2.1 wxMSW-3.2.1_vc14x_Dev.7z
      wget -nv !GITHUB_DL!/v3.2.1/wxWidgets-3.2.1-headers.7z
      7z x -o%SCRIPTDIR%..\cache\wxWidgets-3.2.1 wxWidgets-3.2.1-headers.7z
  ) else (
      echo Downloading 3.1.2
      wget -O wxWidgets-3.1.2.7z -nv ^
        https://download.opencpn.org/s/E2p4nLDzeqx4SdX/download
      7z i > nul 2>&1 || choco install -y 7zip
      7z x wxWidgets-3.1.2.7z -o%WXWIN%
  )
)
dir cache
type "%CONFIG_FILE%"
refreshenv
