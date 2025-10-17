#include <Eigen/Core>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include<igl/opengl/glfw/Viewer.h>
#include<vector>
#include <igl/read_triangle_mesh.h>
#include<functional>
#include<variant>


struct rayon{
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
};

using source = std::function<rayon(void)>;
//using Optical_index = std::function<float(const Eigen::Vector3d&)>;

using Optical_function = std::function<float(const Eigen::Vector3d&)>; 

class Optical_index{
    private:
        Optical_function n; // the actual index function 
    public:
        // no idea if i am supposed to make an acess function ??
        void translate_function(Eigen::Vector3d);

        Optical_index() : n([](Eigen::Vector3d){ return 1; }) {};
        Optical_index(Optical_function n_){
            this->n = n_;
        }
};


using point = Eigen::Vector3d;

//using bbox = point[4];

class bbox{
    public:
        Eigen::Vector3d min; // [min_x, min_y, min_z]
        Eigen::Vector3d max; // [max_x, max_y, max_z]
        //point[4] point_array; // in order : (min_x, min_y, min_z), (max_x, min_y, min_z), (max_x, max_y, min_z), (min_x, max_y, min_z)... la meme avec max_z
        void merge(bbox);
        bool collide(const bbox&) const;
        void translate(Eigen::Vector3d);
        bbox() : min(Eigen::Vector3d::Zero()), max(Eigen::Vector3d::Zero()) {};
        bbox(const Eigen::Vector3d& minVec, const Eigen::Vector3d& maxVec)
        : min(minVec), max(maxVec) {}
};

struct Placement{
    point p;
    Eigen::Vector3d rot;
};

struct Mesh {
    Eigen::MatrixXd sommets;  // Sommet: tableau x * 3 comprtant les coordonné de chaque sommet ex : [[3.12, 23.34, 78], [-0.2, 1, 0], [0, 0, 0]]
    Eigen::MatrixXi faces;  // tableau x * 3 comportant les numéros de sommet formant les faces ex : [[0, 1, 2]] 
};

class Solid{
    public:
        Optical_index n; // optical index n'a une definission valide que dans le maillage 
        Mesh maillage;
        bbox bounding_box;
        void translate(Eigen::Vector3d);
        Solid(std::string, Optical_index = Optical_index()); 
        void load_viewer(igl::opengl::glfw::Viewer* viewer);
        bool is_in_Solid(Eigen::Vector3d point);
        //bool collide_with(Solid); 
        std::string name;
    };


class Component {
    public:
        Component(std::variant<Solid*, std::vector<Component*>> data);
        bool is_a_solid(void);
        bool add_component(Component* c_add, bool check_collision=true, bool merge=false);  
        void compute_bounding_box(); // Peut-être à revoir son utilité
        bool translate_component(Eigen::Vector3d, bool);
        bool is_in_component(Eigen::Vector3d);
        void load_viewer(igl::opengl::glfw::Viewer* viewer);
        Solid* get_Solid();
        std::vector<Component*>& get_l_Component();
        void pprint(int niveau=0);
        bbox bounding_box;

    private:
        std::variant<Solid*, std::vector<Component*>> data;
        bool collide_with(Component*);
};