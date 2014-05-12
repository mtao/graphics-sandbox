#include "hetrianglemesh.h"
#include <iostream>

typedef HETriangleMesh<float> Mesh;
int main() {
    internal::MeshConstructor<Mesh> hecon;
    hecon.add_triangle(0,1,2);
    hecon.add_triangle(2,3,0);
    hecon.add_vertex(0,0,0);
    hecon.add_vertex(1,0,0);
    hecon.add_vertex(1,1,0);
    hecon.add_vertex(0,1,0);
    Mesh m(hecon);

    std::cout << "Triangles: " << std::endl;
    for(auto&& he: m.triangles()) {
            std::cout << he->head << " " << he->next->head << " " << he->next->next->head << std::endl;
    }
    std::cout << "Boundary: " << std::endl;
    for(auto&& he: m.halfedges()) {
        if(!he->next) {
        std::cout << he->head << " " << he->dual->head << std::endl;
        }
    }



}
