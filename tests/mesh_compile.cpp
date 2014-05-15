//#include "veftrianglemesh.h"
#include "mesh/hetrianglemesh.h"
#include "mesh/meshexceptions.h"
#include <iostream>

void printHE(const mtao::HETriangleTopology::ptr_type& p) {
    std::cout << "[" << p->dual->head << "->" << p->head << ")";
}

int he_badedge_test() {
    mtao::MeshConstructor<mtao::HETriangleMesh<float> > mc;
    mc.add_triangle(0,1,2);
    try {
    mc.add_triangle(1,2,3);
    }
    catch (mtao::exceptions::NonManifoldEdge& e) {
        return 0;
    }
    return 1;
}

int he_construction_test() {
    mtao::HETriangleMesh<float> vef;
    mtao::MeshConstructor<mtao::HETriangleMesh<float> > mc;
    mc.add_triangle(0,1,2);
    mc.add_triangle(1,3,2);
    mc.add_vertex(0,0,0);
    mc.add_vertex(1,0,0);
    mc.add_vertex(0,1,0);
    mc.add_vertex(1,1,0);
    vef = mc.move();
    return 0;

}

int main() {
    /*
    for(auto&& v: vef.vertices()) {
        std::cout << v.transpose() << std::endl;
    }
    for(auto&& hep: vef.triangles()) {
        auto it = hep;
        for(int i=0; i < 3; ++i) {
        if(!it) break;
            printHE(it);
            it = it->next;

        }
        std::cout << std::endl;
    }
    */
    /*
    VEFTriangleMesh<float> vef;
    VFTriangleMesh<double> vf("bunny_regular.obj");
    for(auto&& v: vf.vertices()) {
        std::cout << v.transpose() << std::endl;
    }
    for(auto&& f: vf.triangles()) {
        std::cout << f.transpose() << std::endl;
    }
    */
    int ret = 0;
    ret |= he_construction_test();
    ret |= he_badedge_test();
    return ret;
}
