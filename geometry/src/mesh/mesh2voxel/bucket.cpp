#include "bucket.h"
#include "rayiterator.h"
#include <typeinfo>
#include <iostream>
struct absless{
    bool operator()(double a, double b) {
        return std::abs(a) < std::abs(b);
    }
};

Bucket::Bucket(const TriangleMesh & tm, Lattice & l, unsigned char dim): tm(tm), l(l), dim(dim), rg(l,dim), I((dim+1)%3), J((dim+2)%3) {
    std::vector<unsigned int> vert_bucket(tm.vertices.size());
    triangles.resize(l.N()(I) * l.N()(J));
    std::transform(tm.vertices.begin(), tm.vertices.end(), vert_bucket.begin(),[&](const Eigen::Vector3d & v) -> unsigned int {
            return l.getIndex(v);
            });
    for(auto&& t: tm.triangles) {
        unsigned int idx = &t - tm.triangles.data();
        Eigen::AlignedBox<double,3> bbox;
        for(auto&& tv: t) {
            bbox.extend(tm.vertices[tv]);
        }
        Eigen::AlignedBox<unsigned int,3> area(
                l.getIJK(bbox.min())
                ,
                l.getIJK(bbox.max())
                );
        for(int i=area.min()(I); i <= area.max()(I); ++i) {
            for(int j=area.min()(J); j <= area.max()(J); ++j) {
                triangles[rg.ij2idx(i,j)].insert(idx);
            }
        }
    }
}

#include <iostream>
void Bucket::getVoxels() {

    std::vector<unsigned int> counter(l.N()(dim));
    std::vector<bool> m(triangles.size(),false);
    std::set<std::array<unsigned int,3> > & voxels = l.getVoxels();
    for(auto&& r: rg) {
        std::set<double, absless > ipoints;
        for(auto&& ti: triangles[rg.ij2idx(r.ij())]) {
            const Triangle & t = tm.triangles[ti];
            ipoints.insert(tm.intersect(t,r.value()));
        }
        bool inside=false;
        double dx = l.dx()(dim);
        std::array<unsigned int,3> start;
        start[dim]=0;
        start[I] = r.ij()(0);
        start[J] = r.ij()(1);
        for(auto&& p: ipoints) {
            unsigned int end = (unsigned int)(std::abs(p)/dx);
            if(inside && end < l.N()(dim)) {
                inside = !inside;
                for(int i=start[dim]; i < end; ++i) {
                    start[dim]=i;
                    voxels.insert(start);
                }
            }
            inside = !inside;
            start[dim] = end;
        }
        
        m[rg.ij2idx(r.ij())] = ipoints.size() > 0;

    }
    for(int i=0; i < l.N()(I); ++i) {
        for(int j=0; j < l.N()(J); ++j) {
            if(m[rg.ij2idx(i,j)])
                std::cout << "O";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }



}

