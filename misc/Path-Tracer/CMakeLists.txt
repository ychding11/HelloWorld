cmake_minimum_required(VERSION 2.8.4)
project(pathtracer)

set(CMAKE_BUILD_TYPE Debug)
message("-- Build type: " ${CMAKE_BUILD_TYPE})

message("-- Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message("-- Compiler ID is not MSVC")
endif()

#FIND_PACKAGE(OpenMP)
if(OPENMP_FOUND)
	message("- OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


set(SOURCE_FILES
    src/main.cpp
    src/material.cpp
    src/objects.cpp
    src/kdtree.cpp
    src/texture.cpp
    src/camera.cpp
    src/scene.cpp
    src/renderer.cpp
    src/parallel.cpp
    src/progressreporter.cpp
    lib/rand48/rand48.c
    lib/lodepng/lodepng.cpp
    lib/tiny_obj_loader/tiny_obj_loader.cc)

add_executable(pathtracer ${SOURCE_FILES}) #defines an executable target

if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
	message("-- CMake is runnning on Linux platform.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-narrowing")
	message("-- Set C++ 11 flag.")
	target_link_libraries(pathtracer -lpthread)
elseif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
	message("-- CMake is runnning on Windows platform.")
else()
	message("-- CMake is runnning neither on Windows nor Linux platform.")

endif()


