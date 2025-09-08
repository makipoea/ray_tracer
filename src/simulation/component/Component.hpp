#include <eigen3/Eigen/src/Core/Matrix.h>
#include<vector>
#include <igl/read_triangle_mesh.h>
#include <Eigen/Core>
#include<functional>
#include<variant>


struct rayon{
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
};

using source = std::function<rayon(void)>;
//using Optical_index = std::function<float(const Eigen::Vector3d&)>;

class Optical_index{
    private:
        std::function<float(const Eigen::Vector3d&)> n; // the actual index function 
    public:
        // no idea if i am supposed to make an acess function ??
        static void translate_function(Eigen::Vector3d);
};

using point = Eigen::Vector3d;

using bbox = point[4];

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
        Optical_index n; // optical index n'a une definission valide que dans la boundaries 

        Solid(std::string stl_filename, Optical_index n); // constructeur voir le fichier main.cpp    
    
    };

class Component{
    public  : 
        std::variant<Solid, std::vector<Component> > data; // move to private     
        bool is_a_solid(void);
        void add_component(Component);  
        bbox bounding_box; // la bounding box sera utilisé pour l'optimisation des collisions / et la recherche des stl prenant le point s
        void compute_bounding_box(Component);
        void translate_componenet(Eigen::Vector3d, bool);
        // /!\ pensser à ajouter les bounding box des composants pour l'optimisation d'appartenance 
    private :
        bool collide_with(Component); // return True si l'intersection entre les composant est non vide 

};