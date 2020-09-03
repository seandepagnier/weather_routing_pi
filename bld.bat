cd build
cmake -T v141_xp ..
cmake --build . --target package --config release 
bash ./cloudsmith-upload.sh
