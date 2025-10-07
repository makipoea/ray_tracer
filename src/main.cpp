#include<iostream>
#include <igl/read_triangle_mesh.h>
//#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include"../src/simulation/component/Component.hpp"

//using namespace std;

int main() {

    Solid lentille1 = Solid("../resources/lens.stl");
    Solid lentille_fine = Solid("../resources/lens_fine.stl");
    Solid clip = Solid("../resources/clip.stl");
    Solid roue = Solid("../resources/roue.stl");
    igl::opengl::glfw::Viewer viewer;

    lentille_fine.translate({0.01, 0, 0});

    Component lentille2_comp = Component(&roue);
    Component lunette(&roue);
    lunette.add_component(&lentille2_comp);
    
    //Component World(&clip);
    //World.add_component(&lentille2_comp);
    //World.load_viewer(&viewer);
    lunette.load_viewer(&viewer);
    viewer.launch();
    
    /*
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.launch();
    */
    return 0;
}
