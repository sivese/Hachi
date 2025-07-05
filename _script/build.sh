#!/bin/bash

# Check if emsdk directory exists
if [ ! -d "emsdk" ]; then
    echo "Initializing Emscripten SDK..."
    git submodule update --init --recursive

    cd emsdk
    ./emsdk install latest
    ./emsdk activate latest
    cd ..
fi

# Source the Emscripten environment
source emsdk/emsdk_env.sh

if [ ! -d "build" ]; then
    mkdir build
fi

# Build the project using emcmake and cmake


emcmake cmake -B build
emmake make -C build

echo "Build complete."

read -p "Do you want to run the server? (y/n) " run

if [[ "$run" =~ ^[Nn]$ ]]; then
    exit 0
fi

echo "Starting local server..."
xdg-open http://localhost:8080 >/dev/null 2>&1 &

cd build
python3 -m http.server 8080