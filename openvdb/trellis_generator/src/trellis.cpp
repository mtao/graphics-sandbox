#include "trellis.hpp"
#include <iostream>


inline double distToLine(
        const Trellis::Vertex& a,
        const Trellis::Vertex& b,
        const Trellis::Vertex& x) {
    auto&&  xa = x - a;
    auto&& ba = b - a;
    ba.normalize();
    double d0 = (xa).length();
    double d1 = (b-x).length();

    double d2 = (xa - xa.dot(ba) * ba).length();
    return std::min({d0,d1,d2});
}

void Trellis::createSDF(openvdb::DoubleGrid::Ptr& grid, double contour)  const{
    grid->setBackground(1.0);
    for(auto&& e: this->edges) {
        auto&& v0 = this->vertices[e[0]];
        auto&& v1 = this->vertices[e[1]];
        openvdb::math::BBox<Vertex> b;
        b.expand(v0);
        b.expand(v1);
        auto&& wmin = grid->worldToIndex(b.min());
        auto&& wmax = grid->worldToIndex(b.max());


        openvdb::math::CoordBBox cbbox(
                openvdb::math::Coord::floor(wmin)
                ,
                openvdb::math::Coord::ceil(wmax)
                );

        
        openvdb::math::Coord coord;
        int& x = coord.x();
        int& y = coord.y();
        int& z = coord.z();
        auto&& accessor = grid->getAccessor();

        for(x=cbbox.min().x(); x < cbbox.max().x(); ++x) {
            for(y=cbbox.min().y(); y < cbbox.max().y(); ++y) {
                for(z=cbbox.min().z(); z < cbbox.max().z(); ++z) {
                   Vertex world_coord = grid->indexToWorld(coord);
                   accessor.setValue(coord,distToLine(v0,v1,world_coord) - contour);
                }
            }
        }
    }
}
