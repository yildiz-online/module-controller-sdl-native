cmake_minimum_required (VERSION 2.8.6)
project (module-controller-sdl-native)

set(CMAKE_CXX_FLAGS "-O2 -std=gnu++11")

include_directories("../include/sdl")

set(CMAKE_SHARED_LINKER_FLAGS "-Xlinker --add-stdcall-alias")

file(
        GLOB_RECURSE
        source_files
        ${CMAKE_SOURCE_DIR}/../../c++/*.cpp
        ${CMAKE_SOURCE_DIR}/../../c++/*.c
        ${CMAKE_SOURCE_DIR}/../../c++/*.hpp
        ${CMAKE_SOURCE_DIR}/../../c++/*.h
)

add_library(controller-sdl SHARED ${source_files})

target_link_libraries(controller-sdl ${CMAKE_SOURCE_DIR}/lib/SDL2.dll)
