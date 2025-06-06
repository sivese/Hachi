echo Initializing Emscripten SDK...
git submodule update --init --recursive
cd emsdk
call emsdk.bat install latest
call emsdk.bat activate latest
cd ..