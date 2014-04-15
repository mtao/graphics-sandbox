#include "trianglemesh.h"
#include "lattice.h"
#include <iostream>
#include <typeinfo>
#include <Eigen/Eigenvalues>
#include "bucket.h"

int main(int argc, char * argv[]) {
    if(argc < 5) {
        return 1;
    }
    int nx = atoi(argv[2]);
    int ny = atoi(argv[3]);
    int nz = atoi(argv[4]);
    int dim=0;
    if(argc == 6) {
        dim = atoi(argv[5]);
    }

    
    std::cout << "Mesh filename: " << argv[1] << std::endl;
    TriangleMesh m(TriangleMesh::readObj(argv[1]));
    std::cout << m.vertices.size() << " " << m.triangles.size() << std::endl;

    Lattice l(m,nx,ny,nz,dim);
    std::vector<bool> s(l.size(),false);
    for(auto&& v: l) {
        s[l.ijk2idx(v[0],v[1],v[2])] = true;
    };
    for(unsigned int i=0; i < l.NI(); ++i) {
        for(unsigned int j=0; j < l.NJ(); ++j) {
            for(unsigned int k=0; k < l.NK(); ++k) {

                if(s[l.ijk2idx(i,j,k)]) {
                    std::cout << "O" ;
                } else {
                    std::cout << "_" ;
                }
            }
        std::cout << std::endl;
            
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
    l.write("a.vox");


    return 0;
}
