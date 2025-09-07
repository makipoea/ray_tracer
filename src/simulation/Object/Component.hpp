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
using optical_index = std::function<float(const Eigen::Vector3d&)>;
using point = Eigen::Vector3d;

struct Placement{
    point p;
    Eigen::Vector3d rot;
}

struct Mesh {
    point Sommet;  // Sommet: tableau x * 3 comprtant les coordonné de chaque sommet ex : [[3.12, 23.34, 78], [-0.2, 1, 0], [0, 0, 0]]
    Eigen::MatrixXi Face;  // tableau x * 3 comportant les numéros de sommet formant les faces ex : [[0, 1, 2]] 
};

class Solid{
    public:
        Mesh boundaries;
        
        optical_index n; // optical index n'a une definission valide que dans la boundaries 

        Solid(std::string stl_filename, optical_index n);
    };

class Component{
    public  : 
        std::variant<std::pair(point, Solid), std::vector<std::pair(point, Component)>> data; // move to private     
    private :

};