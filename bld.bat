REM FE2  Testplugin

REM  For Opencpn 5.6.2 and wxWidgets-3.1.2
REM cd build
REM cmake -T v141_xp -G "Visual Studio 15 2017"
REM       -DCMAKE_BUILD_TYPE=RelWithDebInfo  .. 
REM cmake --build . --target tarball --config RelWithDebInfo >output.txt
REM bash ./cloudsmith-upload.sh

REM  For Opencpn 5.6.2 and wxWidgets-3.1.2
REM cd build
REM cmake -T v141_xp ..
REM cmake --build . --target package --config release >output.txt
REM bash ./cloudsmith-upload.sh

REM  For Opencpn 5.7.1 and wxWidgets-3.2.1
REM .\buildwin\win_deps.bat
REM cd build
REM cmake -T v143 -A Win32 "Visual Studio 17 2022" ^
REM      -DCMAKE_BUILD_TYPE=RelWithDebInfo  ..
REM cmake --build . --target tarball --config RelWithDebInfo >output.txt

REM  For Opencpn 5.7.1 and wxWidgets-3.2.1
cd build
cmake -T v143 -A Win32 ..
cmake --build . --target package --config relwithdebinfo >output.txt
bash ./cloudsmith-upload.sh
