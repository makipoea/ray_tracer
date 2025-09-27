#include<iostream>
#include <igl/read_triangle_mesh.h>
//#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include"../src/simulation/component/Component.hpp"

//using namespace std;

int main() {

    Solid lentille1 = Solid("../resources/lens.stl");
    Solid lentille2 = Solid("../resources/lens.stl");
    igl::opengl::glfw::Viewer viewer;

    lentille2.translate({0.01, 0, 0});

    // Premier mesh sur le data() par défaut
    
    lentille1.load_viewer(&viewer);
    lentille2.load_viewer(&viewer);

    viewer.launch();
    
    /*
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.launch();
    */
    return 0;
}
