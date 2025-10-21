#include<iostream>
#include <igl/read_triangle_mesh.h>
//#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include"../src/simulation/component/Component.hpp"

//using namespace std;

int main() {

    Solid lentille1 = Solid("../resources/lens.stl");
    //Solid lentille_fine = Solid("../resources/lens_fine.stl");
    //Solid clip = Solid("../resources/clip.stl");
    //Solid roue = Solid("../resources/roue.stl");
    igl::opengl::glfw::Viewer viewer;

    //std::cout << "roue min = " << roue.bounding_box.min << "\n  -- \n max = " << roue.bounding_box.max << "\n ----------------------------- \n \n " <<std::endl;

    //std::cout << "lentille1 min = " << lentille1.bounding_box.min << "\n -- \n  max = " << lentille1.bounding_box.max << std::endl;

    //lentille1.translate({0.05, 0, 0});

    //Component lunette(&lentille1);
    //Component C_lentille_fine(&lentille_fine);
    //lunette.add_component(&C_lentille_fine);

    //Component World(&lentille1);
    //lunette.pprint();
    //World.add_component(&lunette);
    //World.pprint();
    //lunette.pprint();
    //orld.load_viewer(&viewer);

    //lunette.load_viewer(&viewer);
    //viewer.launch();
    
    /*
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.launch();
    */
    return 0;
}
