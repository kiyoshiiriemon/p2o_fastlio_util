cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(rearrange_pointcloud)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(PCL 1.8 REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(rearrange_pointcloud rearrange_pointcloud.cpp)

target_link_libraries(rearrange_pointcloud ${PCL_LIBRARIES} Eigen3::Eigen)

