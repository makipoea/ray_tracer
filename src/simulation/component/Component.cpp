#include "Component.hpp"
#include <eigen3/Eigen/src/Core/Matrix.h>
#include<iostream>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/opengl/glfw/Viewer.h>

void Optical_index::translate_function(Eigen::Vector3d dep) {
    auto old_n = this->n;
    this->n = [old_n, dep](Eigen::Vector3d y) { return old_n(y + dep); };
}


Solid::Solid(std::string stl_filename, Optical_index opt_index)
    : n(opt_index)  
    {
    if (!igl::read_triangle_mesh(stl_filename, maillage.sommets, maillage.faces)) {
        std::cerr << "Erreur : impossible de charger le fichier " << stl_filename << std::endl;
    }

    bounding_box.min = maillage.sommets.row(0);
    bounding_box.max = maillage.sommets.row(0);

    //std::cout << bounding_box.min.rows() << bounding_box.min.cols() << std::endl;
    //std::cout << maillage.sommets.row(0).transpose().rows() << maillage.sommets.row(0).transpose().cols()<< std::endl;
    for (int i = 1; i < maillage.sommets.rows(); i++) {
        bounding_box.min = bounding_box.min.cwiseMin(maillage.sommets.row(i).transpose());
        bounding_box.max = bounding_box.max.cwiseMax(maillage.sommets.row(i).transpose());
    }
}

void Solid::translate(Eigen::Vector3d dep) {
    for (int i = 0; i < maillage.sommets.rows(); ++i) {
        maillage.sommets.row(i) += dep.transpose();
    }
    bounding_box.min += dep;
    bounding_box.max += dep;
}

void Solid::load_viewer(igl::opengl::glfw::Viewer* viewer){
    viewer->append_mesh();
    viewer->data().set_mesh(maillage.sommets, maillage.faces);
}

void bbox::merge(bbox other_bbox){
    // no comment 
    this->min = this->min.cwiseMin(other_bbox.min);
    this->max = this->max.cwiseMax(other_bbox.max);
}   

bool bbox::collide(const bbox& b) const {
    return (this->min.array() <= b.max.array()).all() &&
           (this->max.array() >= b.min.array()).all();
}

 
Component::Component(std::variant<Solid*, std::vector<Component*>> data) 
    : data(data) 
{
    if (std::holds_alternative<Solid*>(this->data)) {
        Solid* solid = std::get<Solid*>(this->data);
        this->bounding_box = solid->bounding_box;
    } else {
        auto components = std::get<std::vector<Component*>>(this->data);
        if (!components.empty()) {
            this->bounding_box = components[0]->bounding_box;
            for (auto comp : components) {
                if (comp != components[0]) { 
                    this->bounding_box.merge(comp->bounding_box);
                }
            }
        }
    }
}


bool Component::is_a_solid(void){
    return (std::holds_alternative<Solid*>(this->data)); // verifie si le type de data ets le couple poisiton, solide 
}


bool Component::collide_with(Component* other_c) {
    if (!this->bounding_box.collide(other_c->bounding_box)) { // la seul et unique raison d'etre des boundings box
        return false;
    }

    if (this->is_a_solid() && other_c->is_a_solid()) {
        Solid* this_solid = std::get<Solid*>(this->data);
        Solid* other_solid = std::get<Solid*>(other_c->data);
        
        Eigen::MatrixXd V_intersect;
        Eigen::MatrixXi F_intersect;
        // on ne questionnera en aucun cas la complexité d'une telle solution 
        //igl::copyleft::cgal::mesh_boolean(
        //    this_solid->maillage.sommets, this_solid->maillage.faces,
        //   other_solid->maillage.sommets, other_solid->maillage.faces,
        //    igl::MESH_BOOLEAN_TYPE_INTERSECT, V_intersect, F_intersect);
        
        //return V_intersect.rows() > 0; // on verifie si l'intersection est non vide
    }
/*
    if (this->is_a_solid()) {
        for (auto& c : std::get<std::vector<Component>>(other_c->data)) {
            if (this->collide_with(&c)) return true;
        }
        return false;
    }

    if (other_c->is_a_solid()) {
        for (auto& c : std::get<std::vector<Component>>(this->data)) {
            if (c.collide_with(other_c)) return true;
        }
        return false;
    }

    // la recursivité est un choix et j'ai resisiter
    for (auto& c1 : std::get<std::vector<Component>>(this->data)) {
        for (auto& c2 : std::get<std::vector<Component>>(other_c->data)) {
            if (c1.collide_with(&c2)) return true;
        }
    }
*/  
    return false;
}

/*

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
    
    //Step :
    //- Translate each sub_component
    //- Durant le parcours de l'arbre on translate la fonction d'indice
    //
    if (this->is_a_solid()) {
        this->data.translate(dep);
    }
    else if () {

    }
}

Component::is_in_component(Eigen::Vector3d point){
    // Cette fonction pourra etre ameliorer avec un quadtree calculer prealablement 
}
*/