#include<iostream>
#include <igl/read_triangle_mesh.h>
// #include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include"../src/simulation/component/Component.hpp"

//using namespace std;

int main() {

    Eigen::MatrixXd V; // sommets
    Eigen::MatrixXi F; // faces


    std::string filename = "../resources/lens.stl";
    if (!igl::read_triangle_mesh(filename, V, F)) {
        std::cerr << "Erreur : impossible de charger " << filename << std::endl;
        return -1;
    }


    std::cout << "Maillage chargé : " << filename << std::endl;
    std::cout << "Nombre de sommets : " << V.rows() << std::endl;
    std::cout << "Nombre de faces   : " << F.rows() << std::endl;

    if (V.rows() > 0) {
        std::cout << "Premier sommet : " 
                  << V(0,0) << ", " 
                  << V(0,1) << ", " 
                  << V(0,2) << std::endl;
    }

    //igl::opengl::glfw::Viewer viewer;
    //viewer.data().set_mesh(V, F);
    //viewer.launch();

    return 0;
}
