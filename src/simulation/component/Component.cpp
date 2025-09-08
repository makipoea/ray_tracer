#include "Component.hpp"
#include <eigen3/Eigen/src/Core/Matrix.h>
#include<iostream>


Optical_index::translate_function(Eigen::Vector3d){
    // a simple composition x, n -> (n+x) : y -> (n(x+y))
}

Solid::Solid(std::string stl_filename, optical_index n){
    this->n = n;

    if (!igl::read_triangle_mesh(stl_filename, this->boundaries.sommets, this->boundaries.faces)) {
        std::cerr << "Erreur : impossible de charger le fichier" << stl_filename << std::endl;
    }
}

Component::is_a_solid(void){
    return (std::holds_alternative<std::pair<point, Solid>>(this->data)) // verifie si le type de data ets le couple poisiton, solide 
}

Component::collide_with(Commponent c){
    if (this->is_a_solid() && c.is_a_solid()) {  
        return true;
    }

}

Component::add_component(Component c_add){
    if (this->is_a_solid() && c_add.is_a_solid()) {  
        
    }

}

Component::translate_componenet(Eigen::Vector3d, bool cheke_collision){
    /*
    Step :
    - Translate each sub_component
    - Durant le parcours de l'arbre on translate la fonction d'indice
    */
}