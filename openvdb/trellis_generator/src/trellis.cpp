#include "trellis.hpp"
#include <iostream>


inline double distToLine(
        const Trellis::Vertex& a,
        const Trellis::Vertex& b,
        const Trellis::Vertex& x) {
    auto&&  xa = x - a;
    auto&&  xb = x - b;
    auto&& ba = b - a;
    auto&& dab = (b-a).length();
    ba.normalize();
    

    double proj = xa.dot(ba);
    double d2 = (xa -  proj * ba).length();

    double nproj = proj / dab;
    double scale = std::abs(nproj - .5);
    scale = .1*std::max(std::min(0.1,scale),0.0);

    if(proj < 0) {
        return (xa).length();
    } else if (proj > dab) {
        return (xb).length();
    } else {
        return d2 - scale;
    }
}

void Trellis::createSDF(openvdb::DoubleGrid::Ptr& grid, double contour)  const{
    grid->setBackground(1.0);
    int count = 0;

    for(auto&& e: this->edges) {
        if(count++%200 == 0) {
            std::cout << count << "/" << this->edges.size() << std::endl;
        }
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

        for(x=cbbox.min().x()-10; x < cbbox.max().x()+10; ++x) {
            for(y=cbbox.min().y()-10; y < cbbox.max().y()+10; ++y) {
                for(z=cbbox.min().z()-10; z < cbbox.max().z()+10; ++z) {
                   Vertex world_coord = grid->indexToWorld(coord);
                   double oldval = accessor.getValue(coord);
                   double newval = distToLine(v0,v1,world_coord) - contour;
                   accessor.setValue(coord,
                        std::min(
                        newval
                            ,
                            oldval)
                   );
                }
            }
        }
    }
}
