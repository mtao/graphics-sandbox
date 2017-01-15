#include "trianglemesh.h"
#include "lattice.h"
#include <iostream>
#include <typeinfo>
#include <Eigen/Eigenvalues>

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
    Lattice l(nx,ny,nz,Eigen::AlignedBox<double,3>());
    int buf = 2;
    Eigen::Vector3d slices(nx-2*buf,ny-2*buf,nz-2*buf);

    
    std::cout << "Mesh filename: " << argv[1] << std::endl;
    TriangleMesh m(TriangleMesh::readObj(argv[1]));
    Eigen::AlignedBox<double,3> & bbox = l.bbox();
    for(auto&& v: m.vertices) {
        bbox.extend(v);
    }
    Eigen::Vector3d dx = bbox.sizes().cwiseQuotient(slices);
    bbox.min() -= buf * dx;
    bbox.max() += buf * dx;
    l.calculateDx();
    std::cout << bbox.min().transpose() << "|||" << bbox.max().transpose() << std::endl;

    /*
    Bucket b(m,l,dim);
    b.getVoxels();
    std::vector<bool> s(l.size(),false);
    auto&& voxels = b.getVoxels();
    for(auto&& v: voxels) {
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
    */


    return 0;
}
