cmake_minimum_required(VERSION 3.1)
project(ray_tracer)

message("Message de teste")

set(CMAKE_CXX_STANDARD 17)

# chemin vers libigl

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
include(libigl)

# Enable the target igl::glfw
igl_include(glfw)

# eigen
find_package(Eigen3 REQUIRED)

# ajout des include paths
#include_directories(${LIBIGL_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})


add_executable(ray_tracer src/main.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC 
  igl::glfw
  ## Other modules you could link to
  # igl::embree
  # igl::imgui
  # igl::opengl
  # igl::stb
  # igl::predicates
  # igl::xml
  # igl_copyleft::cgal
  # igl_copyleft::comiso
  # igl_copyleft::core
  # igl_copyleft::cork
  # igl_copyleft::tetgen
  # igl_restricted::matlab
  # igl_restricted::mosek
  # igl_restricted::triangle
  )

