cd build
cmake -T v141_xp ..
cmake --build . --target package --config release >output.txt
cd build
bash ./cloudsmith-upload.sh
