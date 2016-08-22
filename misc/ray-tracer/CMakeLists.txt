cmake_minimum_required(VERSION 2.8)
project( ray-tracer )

message("-- Source code dir: " ${CMAKE_CURRENT_SOURCE_DIR})
set( SOURCE_CODE_DIR  ${CMAKE_CURRENT_SOURCE_DIR} )
message("-- Set source code dir: " ${SOURCE_CODE_DIR})
set(CMAKE_BUILD_TYPE Debug)
message("-- Set build type: " ${CMAKE_BUILD_TYPE})

add_definitions("-DBUILD_LIB")
message("-- Add build option: BUILD_LIB.")

if(UNIX)
  message ("-- OS: Linux platform")
    # Should add c++11 support check here.
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message ("-- Enable c++11")
endif(UNIX)
 
include_directories("${SOURCE_CODE_DIR}/include")
message ("-- Add ./include directory into header file search path.")

# Build Library
add_subdirectory( src  lib) 
message("-- Add src directory. " ) 

add_executable( render cmd/render.cpp )
target_link_libraries( render raytracer )

set ( ENABLE_TEST NO )
# Testes
if ( ENABLE_TEST )
add_executable( compress test/compress.cpp )
target_link_libraries( compress huffman )

add_executable( uncompress test/uncompress.cpp )
target_link_libraries( uncompress huffman )
endif ( ENABLE_TEST )
