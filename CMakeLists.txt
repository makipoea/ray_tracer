cmake_minimum_required(VERSION 3.6)
project(ray_tracer)

message("Message de teste")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g") # censsé ajouter le flag de debug (pour pouvoir utiliser gdn)

# chemin vers libigl

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


include(libigl)

# Enable the target igl::glfw
igl_include(glfw)
igl_include(copyleft/cgal)


# eigen
find_package(Eigen3 REQUIRED)
set(CGAL_DATA_DIR ".")
set(CMAKE_BUILD_TYPE Release)
find_package(CGAL REQUIRED)

# ajout des include paths
#include_directories(${LIBIGL_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})


add_executable(ray_tracer src/main.cpp src/simulation/component/Component.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC 
  igl::glfw
  Eigen3::Eigen
  ## Other modules you could link to
  # igl::embree
  # igl::imgui
  # igl::opengl
  # igl::stb
  # igl::predicates
  # igl::xml
  igl_copyleft::cgal
  # igl_copyleft::comiso
  # igl_copyleft::core
  # igl_copyleft::cork
  # igl_copyleft::tetgen
  # igl_restricted::matlab
  # igl_restricted::mosek
  # igl_restricted::triangle
  )

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ray_tracer PRIVATE -Wno-narrowing) #vire les warning inutile de libigl
endif()