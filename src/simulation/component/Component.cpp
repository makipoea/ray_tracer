#include "Component.hpp"
#include <eigen3/Eigen/src/Core/Matrix.h>
#include<iostream>


Optical_index::translate_function(Eigen::Vector3d dep){
    // a simple composition x, n -> (n+x) : y -> (n(x+y))
    auto old_n = n; // sauvegarde l'ancienne fonction
    n = [this, old_n, dep](Eigen::Vector3d y) -> Eigen::Vector3d {
        return old_n(y + dep);  // on traduit y par dep
    };
}

Solid::Solid(std::string stl_filename, optical_index n){
    this->n = n;

    if (!igl::read_triangle_mesh(stl_filename, this->maillage.sommets, this->maillage.faces)) {
        std::cerr << "Erreur : impossible de charger le fichier" << stl_filename << std::endl;
    }
        // Compute the bounding box of the Solid
    this->bounding_box.min = this->maillage.sommets[0];
    this->bounding_box.max = this->maillage.sommets[0];

    for (auto i=1; i<this->maillage.sommets.rows(); i++){
        this->bounding_box.min = this->bounding_box.min.cwiseMin(this->maillage.sommets[i]);
        this->bounding_box.max = this->bounding_box.max.cwiseMax(this->maillage.sommets[i]);
    }
}

Solid::translate(Eigen::Vector3d dep){
    for (auto i = 0; i< this->maillage.sommets; ++i){
        this->maillage.sommets += dep;
    }
    this->bounding_box
}

bbox::merge(bbox other_bbox){
    // no comment 
    this->min = this->min.cwiseMin(other_bbox.min);
    this->max = this->max.cwiseMax(other_bbox.max);
}   

bbox::collide(bbox b){
    Eigen::Vector3d mask_min = (this->min > b.min) && (this->min > b.max); 
    Eigen::Vector3d mask_max = (this->max < b.min) && (this->max < b.max);
    return ! (mask_min.any() | mask_max.any());
}

Component::Component(std::variant<Solid, std::vector<Component>> data){
    this->data = data;
    if this.is_a_solid() {
        this->bounding_box = data.bounding_box;
        }
    else {
        if (data != {}){
            this->bounding_box = data[0].bounding_box;
            for (auto c : data){
                this->bounding_box.merge(c.bounding_box); // seul pb : on merge la premier bbox avec elle même
            } 
        }
    }
}

Component::is_a_solid(void){
    return (std::holds_alternative<std::pair<point, Solid>>(this->data)) // verifie si le type de data ets le couple poisiton, solide 
}

Component::collide_with(Commponent other_c){
    if (! this->bounding_box.collide(other_c.bounding_box)){
        return false;
    }

    if (this->is_a_solid() && other_c.is_a_solid()) {  
        //return this->data.collide_with(other_c.data); 
        Eigen::MatrixXd V_intersect;
        Eigen::MatrixXi F_intersect;
        
        igl::copyleft::cgal::mesh_boolean(this->data.maillage.sommet, this->data.maillage.faces, other_c.data.maillage.sommets, other_c.data.maillage.faces,
                                      igl::MESH_BOOLEAN_TYPE_INTERSECT,
                                      V_intersect, F_intersect);
        return V_intersect.size() != 0;      
    }
    if (this->is_a_solid()){
        for (auto c : other_c.data){
            if this->collide_with(c){
                return true;
            }
        }
    }
    else {
        for (auto c : this->data){
            if c.collide_with(other_c){
                return true;
            }
        }
    }
}

Component::add_component(Component c_add, bool cheke_collision){
    if (cheke_collision && this->collide_with(c_add)) {
        std::cout << "Une detection entre les deux objets a été détécter : l'objet n'as pas été ajouter " << std::endl;
        return false 
    }
    else {
        std::cout << "Warning : la verification de collision n'as pas été activer" << std::endl; // the Warning output doivent etre convertit apres l'implementation d'une gestion des logs satisfaisante
    }

    if (this->is_a_solid()){
        this->data = Component({this->data});
    }
    if (c_add.is_a_solid()){
        c_add.data = Component({this->data})
    }
  
    this->data.insert(this->data.end(), c_add.data.begin(), c_add.data.end());
        
    return true;
}

Component::translate_component(Eigen::Vector3d dep, bool cheke_collision){
    /*
    Step :
    - Translate each sub_component
    - Durant le parcours de l'arbre on translate la fonction d'indice
    */
    if (this->is_a_solid()) {
        this->data.translate(dep);
    }
    else if () {

    }
}

Component::is_in_component(Eigen::Vector3d point){
    // Cette fonction pourra etre ameliorer avec un quadtree calculer prealablement 
}