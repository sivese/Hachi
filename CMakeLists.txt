cmake_minimum_required(VERSION 3.13)
project(hachi)

set(CMAKE_CXX_STANDARD 17)

# Set Emscripten as the compiler when targeting WebAssembly
if(EMSCRIPTEN)
  set( CMAKE_EXECUTABLE_SUFFIX ".html")
  add_compile_options(-sUSE_SDL=2)
endif()

# Add ImGui source files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add include directories
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

set(
    CHIP8_SOURCES
    ${CMAKE_SOURCE_DIR}/src/chip8.cc
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(hachi src/main.cc ${CHIP8_SOURCES} ${IMGUI_SOURCES})

# Only needed for Emscripten
if(EMSCRIPTEN)
  set_target_properties(hachi PROPERTIES
    LINK_FLAGS "-s WASM=1 
                -s EXPORTED_FUNCTIONS=['_main','_malloc','_free'] 
                -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAPU8'] 
                -s USE_SDL=2 
                -s FULL_ES3=1 
                -s ALLOW_MEMORY_GROWTH=1 
                -s EXIT_RUNTIME=1 
                -s ASSERTIONS=1"
  )
endif()