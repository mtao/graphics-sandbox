#include <iostream>
#include <openvdb/openvdb.h>
#include "trellis_generator.hpp"
int main(int argc, char * argv[]) {
    openvdb::initialize();
    //openvdb::io::File file(argv[1]);
    
    TrellisGenerator generator;
    auto&& trellis = generator.generate();
    auto grid = openvdb::DoubleGrid::create(1.0);
    grid->setTransform(
                    openvdb::math::Transform::createLinearTransform(0.01));

    std::cout << "Creating SDF" << std::endl;
    trellis.createSDF(grid,0.01);

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
