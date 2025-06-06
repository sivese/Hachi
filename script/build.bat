@echo off
REM Check if emsdk exists, if not initialize the submodule
if not exist emsdk (
    echo Initializing Emscripten SDK...
    git submodule update --init --recursive

    cd emsdk
    call emsdk install latest
    call emsdk activate latest
    cd ..
)

REM Source the Emscripten environment
call emsdk/emsdk_env.bat

REM Build the project
call emcmake cmake -B build

echo Building camke script...
call cmake --build build

echo Build complete. Starting local server...
start http://localhost:8080

cd build
python -m http.server 8080
