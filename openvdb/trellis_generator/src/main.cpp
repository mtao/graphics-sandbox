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
                    openvdb::math::Transform::createLinearTransform(/*voxel size=*/0.005));

    trellis.createSDF(grid);

    std::cout << "Sizes: " << trellis.vertices.size() << " " << trellis.edges.size() << std::endl;
    for(auto&& e: trellis.edges) {
        std::cout << e[0] << " " << e[1] << std::endl;
    }
    grid->setName("Trellis");
    openvdb::io::File file("output.vdb");
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    file.write(grids);
    file.close();

    
    




    return 0;
}
