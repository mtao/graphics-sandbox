#include "openvdb/vdb_conversion.h"
#include <iostream>
int main(int argc, char * argv[]) {
    openvdb::initialize();

    mtao::EmbeddedGrid<float,3> g(mtao::Vec3i(20,20,20),mtao::Vec3f(0,0,0),mtao::Vec3f(1,1,1));
    g(0,0,0) = 10;
    auto gridptr = mtao::vdb::gridToVDB(g);
    auto acc = gridptr->getAccessor();
    std::cout << acc.getValue(openvdb::Coord(0,0,0)) << std::endl;
    auto g2 = mtao::vdb::vdbToGrid(*gridptr);
    std::cout << g2(0,0,0) << std::endl;
    return 0;
}
