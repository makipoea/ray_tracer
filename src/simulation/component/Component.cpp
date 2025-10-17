#include "Component.hpp"
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <iostream>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/opengl/glfw/Viewer.h>
#include <filesystem>

#include <stdexcept>

void Optical_index::translate_function(Eigen::Vector3d dep) {
    auto old_n = this->n;
    this->n = [old_n, dep](Eigen::Vector3d y) { return old_n(y + dep); };
}


Solid::Solid(std::string str_fil_path, Optical_index opt_index)
    : n(opt_index)  
    {
    std::filesystem::path stl_path(str_fil_path);

    if (stl_path.extension() != ".stl") {
            std::cerr << "le solid  : " << str_fil_path<< " a l'extenssion : " << stl_path.extension() << "au lieu de '.stl'"<< std::endl;
    }

    name = stl_path.stem().string(); // on recupere le nom du fichier (privé de son extenssion)

    if (!igl::read_triangle_mesh(str_fil_path, maillage.sommets, maillage.faces)) {
        std::cerr << "impossible de charger le fichier " << str_fil_path << std::endl;
    }
    // ajouter le parsing du noom de fichier
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

void bbox::translate(Eigen::Vector3d p){
    min += p;
    max += p;
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
    if (this->bounding_box.collide(other_c->bounding_box)) { // la seul et unique raison d'etre des boundings box
        return false;
    }

    if (this->is_a_solid() && other_c->is_a_solid()) {
        Solid* this_solid = std::get<Solid*>(this->data);
        Solid* other_solid = std::get<Solid*>(other_c->data);
        
        Eigen::MatrixXd V_intersect;
        Eigen::MatrixXi F_intersect;
        // on ne questionnera en aucun cas la complexité d'une telle solution 
        igl::copyleft::cgal::mesh_boolean(
            this_solid->maillage.sommets, this_solid->maillage.faces,
            other_solid->maillage.sommets, other_solid->maillage.faces,
            igl::MESH_BOOLEAN_TYPE_INTERSECT, V_intersect, F_intersect);
        return V_intersect.rows() > 0; // on verifie si l'intersection est non vide
    }

    if (this->is_a_solid()) {
        for (auto c : other_c->get_l_Component()) {
            if (this->collide_with(c)) return true;
        }
        return false;
    }

    if (other_c->is_a_solid()) {
        for (auto c : this->get_l_Component()) {
            if (c->collide_with(other_c)) return true;
        }
        return false;
    }

    // la recursivité est un choix et j'ai resisiter
    for (auto c1 : std::get<std::vector<Component*>>(this->data)) {
        for (auto c2 : std::get<std::vector<Component*>>(other_c->data)) {
            if (c1->collide_with(c2)) return true;
        }
    }
  
    return false;
}

Solid* Component::get_Solid(){
    return std::get<Solid*>(this->data);
}

std::vector<Component*>& Component::get_l_Component()  {
    return std::get<std::vector<Component*>>(this->data);
}


void Component::load_viewer(igl::opengl::glfw::Viewer* viewer){
    if (this->is_a_solid()){
        this->get_Solid()->load_viewer(viewer); // oui c'est horrible et ca merite une methode a part entiere
    }
    else {
        for (auto c : this->get_l_Component()){
            c->load_viewer(viewer);
        }
    }

}

bool Component::add_component(Component* c_add, bool check_collision, bool merge){
    // the output represent the sucess of the adding operation
    // si merge est vraie : les enfants des deux arbres sont concatener sinon c_add est ajouter comme feuille de this
    if (check_collision && this->collide_with(c_add)) {
        std::cout << "Une detection entre les deux objets a été détécter : l'objet n'as pas été ajouter " << std::endl;
        return false;
    }
    if (!check_collision){
        std::cout << "la dtection de collision n'as pas ete activer " << std::endl;
    }

    if (this->is_a_solid()) {
        Solid* temporary_solid = this->get_Solid();

        this->data = std::vector<Component*>{new Component(temporary_solid)};
        
        //delete temporary_solid; // ?
    }
    if (c_add->is_a_solid()) {
        
        Solid* temporary_solid = c_add->get_Solid();
        c_add->data = std::vector<Component*>{new Component(temporary_solid)};
        merge = true;
        //delete temporary_solid;
    }
  
    auto& comps_this = this->get_l_Component();
    
    if (merge){    
        auto& comps_c_add = c_add->get_l_Component();
        comps_this.insert(comps_this.end(), comps_c_add.begin(), comps_c_add.end());
    }
    else {
        comps_this.push_back(c_add);
    }
    return true;
}

bool Component::translate_component(Eigen::Vector3d dep, bool cheke_collision){
    // the output represente the sucess of the operation
    //Step :
    //- Translate each sub_component
    //- Durant le parcours de l'arbre on translate la fonction d'indice
    //
    if (this->is_a_solid()) {
        this->get_Solid()->translate(dep);
    }
    else {
        for (auto comp : this->get_l_Component()){
            comp->translate_component(dep, cheke_collision);
        }
    }
}

void Component::pprint(int niveau){
    /*
    la classe Solid recoit le chemin vers le fichier, on pourra faire du parcing pour le recupperer ca pourra toujours servir
    */
    for (int i = 0; i < niveau; ++i){
        std::cout << "  ";
    }

    if (this->is_a_solid()){
        std::cout << this->get_Solid()->name << std::endl;
    }
    else {
        std::cout << "Componant" << std::endl;
        for (auto comp : this->get_l_Component()){
            comp->pprint(niveau + 1);
    }
    }
   
 }


/*
bool Component::is_in_component(Eigen::Vector3d point){
    // Cette fonction pourra etre ameliorer avec un quadtree calculer prealablement 
    std::cout << "enter collide function" << std::endl;
    if (this->bounding_box.collide(other_c->bounding_box)) { // la seul et unique raison d'etre des boundings box
        return false;
    }

    if (this->is_a_solid() && other_c->is_a_solid()) {
        std::cout << "enter 1" << std::endl;
        Solid* this_solid = std::get<Solid*>(this->data);
        Solid* other_solid = std::get<Solid*>(other_c->data);
        
        Eigen::MatrixXd V_intersect;
        Eigen::MatrixXi F_intersect;
        // on ne questionnera en aucun cas la complexité d'une telle solution 
        std::cout<< "intersection" << std::endl;
        //igl::copyleft::cgal::mesh_boolean(
        //    this_solid->maillage.sommets, this_solid->maillage.faces,
        //    other_solid->maillage.sommets, other_solid->maillage.faces,
        //    igl::MESH_BOOLEAN_TYPE_INTERSECT, V_intersect, F_intersect);
        std::cout << "fin intersection" << std::endl;
        return V_intersect.rows() > 0; // on verifie si l'intersection est non vide
    }

    if (this->is_a_solid()) {
        for (auto c : other_c->get_l_Component()) {
            if (this->collide_with(c)) return true;
        }
        return false;
    }

    if (other_c->is_a_solid()) {
        for (auto c : this->get_l_Component()) {
            if (c->collide_with(other_c)) return true;
        }
        return false;
    }

    // la recursivité est un choix et j'ai resisiter
    for (auto c1 : std::get<std::vector<Component*>>(this->data)) {
        for (auto c2 : std::get<std::vector<Component*>>(other_c->data)) {
            if (c1->collide_with(c2)) return true;
        }
    }
  
    return false;
}
*/