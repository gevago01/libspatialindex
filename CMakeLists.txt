cmake_minimum_required(VERSION 3.6)
project(SpatialIndexRtree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -g ")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")


find_package(SpatialIndex REQUIRED)

set(SOURCE_FILES main.cpp callSpatialLibrary.cpp callSpatialLibrary.h Point.cpp Point.h)
add_executable(SpatialIndexRtree ${SOURCE_FILES})

target_link_libraries(SpatialIndexRtree ${SPATIALINDEX_LIBRARIES} )