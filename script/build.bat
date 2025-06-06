@echo off
REM Check if emsdk exists, if not initialize the submodule
if not exist emsdk (
    echo Initializing Emscripten SDK...
    git submodule update --init --recursive
    cd emsdk
    call emsdk.bat install latest
    call emsdk.bat activate latest
    cd ..
)

REM Source the Emscripten environment
call emsdk\emsdk_env.bat

REM Build the project
emcmake cmake -B build
cmake --build build

echo Build complete. Run 'cd build && python -m http.server 8080' to test