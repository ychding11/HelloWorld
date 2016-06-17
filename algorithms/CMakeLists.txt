cmake_minimum_required(VERSION 2.8)
project(algorithms)

set(CMAKE_BUILD_TYPE Debug)
message("-- Set build type: " ${CMAKE_BUILD_TYPE})

add_definitions("-DBUILD_LIB")
message("-- Add build option: BUILD_LIB.")

if (WIN32)
	#set(OpenCV_STATIC true)
	add_definitions("-D_SCL_SECURE_NO_WARNINGS")
endif()

if(UNIX)
    # Should add c++11 support check here.
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message ("-- Enable c++11")
endif(UNIX)
 
include_directories("./")
message ("-- Incude current directory into search path.")

# Build Library
add_library(algorithms 
	combination.cpp
    random.cpp
    misc.cpp
    bits.cpp
)
# target_link_libraries(inpaint ${OpenCV_LIBRARIES})
	
# Testes
add_executable(subsetTester algorithms.h test/subsetTester.cpp)
target_link_libraries(subsetTester algorithms)

add_executable(dataSourceTester algorithms.h test/dataSourceTester.cpp)
target_link_libraries(dataSourceTester algorithms)

add_executable(miscTester algorithms.h test/miscTester.cpp)
target_link_libraries(miscTester algorithms)
