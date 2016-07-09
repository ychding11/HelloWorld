cmake_minimum_required(VERSION 2.8)
project( huffman )

set(CMAKE_BUILD_TYPE Debug)
message("-- Set build type: " ${CMAKE_BUILD_TYPE})

add_definitions("-DBUILD_LIB")
message("-- Add build option: BUILD_LIB.")

if (WIN32)
	#set(OpenCV_STATIC true)
	add_definitions("-D_SCL_SECURE_NO_WARNINGS")
endif()

if(UNIX)
  message ("-- OS: Linux platform")
    # Should add c++11 support check here.
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message ("-- Enable c++11")
endif(UNIX)
 
include_directories("./include")
message ("-- Incude include directory into header file search path.")

file( GLOB LIB_SOURCE_FILES  *.cpp )
message("-- Library source files: " ${LIB_SOURCE_FILES}) 

# Build Library
add_library( huffman  ${LIB_SOURCE_FILES}) 

set ( ENABLE_TEST YES)
# Testes
if ( ENABLE_TEST )
add_executable( compress test/compress.cpp )
target_link_libraries( compress huffman )

add_executable( uncompress test/uncompress.cpp )
target_link_libraries( uncompress huffman )
endif ( ENABLE_TEST )
