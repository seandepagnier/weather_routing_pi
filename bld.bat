cd build
cmake -T v141_xp ..
cmake --build . --target package --config release >output.txt
bash ./cloudsmith-upload.sh
