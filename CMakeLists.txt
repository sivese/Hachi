cmake_minimum_required(VERSION 3.10)
project(hachi)

set(CMAKE_CXX_STANDARD 17)

add_executable(hachi src/main.cpp)

# Only needed for Emscripten
set_target_properties(hachi PROPERTIES
  LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS=['_main','_your_function'] -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
)