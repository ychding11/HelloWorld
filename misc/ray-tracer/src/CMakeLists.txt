cmake_minimum_required(VERSION 2.8)
project( ray-tracer )

#set( SOURCE_CODE_DIR /home/ding/workspace/ray-tracing )

message("------------ src ----------------." )
message("-- Source code root dir: " ${SOURCE_CODE_DIR})
message("-- Set settings for directory src." )

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
message ("-- Add include directory into header file search path.")

file( GLOB_RECURSE LIB_SOURCE_FILES  *.cpp )
message("-- Library source files: " ${LIB_SOURCE_FILES}) 
file( GLOB_RECURSE LIB_HEADER_FILES  *.h )
message("-- Library header files: " ${LIB_HEADER_FILES}) 

file( COPY ${LIB_HEADER_FILES}  DESTINATION ${SOURCE_CODE_DIR}/include )
message("-- Copy Header Files into include" ) 
file( GLOB_RECURSE LIB_HEADER_FILES ${SOURCE_CODE_DIR}/include/*.h )
message("-- Library header files: " ${LIB_HEADER_FILES}) 

# Build Library
add_library( raytracer  ${LIB_SOURCE_FILES}) 
message("------------ src ----------------." )
