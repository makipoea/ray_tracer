cmake_minimum_required(VERSION 3.1)
project(ray_tracer)

message("Message de teste")

set(CMAKE_CXX_STANDARD 17)

# chemin vers libigl

#include_directories(${PROJECT_SOURCE_DIR}/external/libigl)
#add_subdirectory(${PROJECT_SOURCE_DIR}/external/libigl)
#include(libigl)
set(LIBIGL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/libigl/include)

#list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/libigl/cmake)

#include(libigl)

# eigen
find_package(Eigen3 REQUIRED)

# ajout des include paths
include_directories(${LIBIGL_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})


add_executable(ray_tracer src/main.cpp)
