cmake_minimum_required(VERSION 3.10)
project(hachi)

set(CMAKE_CXX_STANDARD 17)

# Set Emscripten as the compiler when targeting WebAssembly
if(EMSCRIPTEN)
  set( CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

add_executable(hachi src/main.cc)

# Only needed for Emscripten
if(EMSCRIPTEN)
  set_target_properties(hachi PROPERTIES
    LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS=['_main'] -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
  )
endif()