#include <iostream>
#include <openvdb/openvdb.h>
#include "trellis_generator.hpp"
int main(int argc, char * argv[]) {
    openvdb::initialize();
    //openvdb::io::File file(argv[1]);
    
#define AUTO_GENERATE
#ifdef AUTO_GENERATE
    TrellisGenerator generator;
    generator.setDensity(100.0);
    generator.setMaxEdgeLength(.4);
    auto&& trellis = generator.generate();
#else //AUTOGENERATE: This one is for manual trellis generation
    Trellis trellis;
    trellis.vertices.emplace_back(Trellis::Vertex{0,0,0});
    trellis.vertices.emplace_back(Trellis::Vertex{1,1,1});
    trellis.edges.emplace_back(Trellis::Edge{{0,1}});
#endif //AUTOGENERATE


    //examples of lammbdas: 
    int whichfunc = 3;
    switch(whichfunc) {
        case 0: 
            trellis.lerpfunc = [](double l, const Trellis::Vertex&, double ra, double rb) -> double {
                return .1 * (std::sin(5 * 3.14 * l) + std::cos(4 * 3.14 * l));
            };
            break;
        case 1:
            trellis.lerpfunc = [](double l, const Trellis::Vertex& p, double ra, double rb) -> double {
                return std::abs(.1 * (std::sin(5 * 3.14 * l) + std::cos(4 * 3.14 * l)) * (p - Trellis::Vertex(0.5,0.5,0.5)).length());
            };
            break;
         default:
            trellis.lerpfunc = [](double l, const Trellis::Vertex& p, double ra, double rb) -> double {
                return (1-l) *ra + l*rb;
            };


    }


    auto grid = openvdb::DoubleGrid::create(1.0);
    int num_voxels_per_dim = 100;
    grid->setTransform(
            openvdb::math::Transform::createLinearTransform(1.0 / num_voxels_per_dim));
    trellis.createSDF(grid);

    openvdb::math::BBox<Trellis::Vertex> bbox;
    for(auto&& v: trellis.vertices) {
        bbox.expand(v);
    }
    auto&& min = bbox.min();
    auto&& max = bbox.max();
    std::cout << min.x() << " " << min.y() << " " << min.z() << std::endl;
    std::cout << max.x() << " " << max.y() << " " << max.z() << std::endl;

    grid->setName("Trellis");
    openvdb::io::File file("output2.vdb");
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    file.write(grids);
    file.close();







    return 0;
}
