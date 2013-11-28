#include "trellis.hpp"
#include <iostream>


Trellis::Trellis(const std::vector<Vertex>& v, const std::vector<Edge>& e, const std::vector<double>& r ):
vertices(v), edges(e), radii(r)  {}

Trellis::Trellis(std::vector<Vertex>&& v, std::vector<Edge>&& e, std::vector<double>&& r):
    vertices(std::move(v)), edges(std::move(e)), radii(std::move(r)) {}

    inline LinePos Trellis::distToLine(
            const Trellis::Vertex& a,
            const Trellis::Vertex& b,
            const Trellis::Vertex& x) const {
        auto&&  xa = x - a;
        auto&&  xb = x - b;
        auto&& ba = b - a;
        auto&& dab = (b-a).length();
        ba.normalize();


        double proj = xa.dot(ba);
        double d2 = (xa -  proj * ba).length();

        double nproj = proj / dab;

        if(proj < 0) {
            return LinePos{0,(xa).length()};
        } else if (proj > dab) {
            return LinePos{1,(xb).length()};
        } else {
            return LinePos{nproj,d2};
        }
    }

void Trellis::createSDF(openvdb::DoubleGrid::Ptr& grid)  {
    if(this->radii.size() != this->vertices.size()) {
        this->radii.resize(this->vertices.size(),this->default_radius);
    }
    grid->setBackground(1.0);
    int count = 0;


    for(auto&& e: this->edges) {
        if(count++%200 == 0) {
            std::cout << count << "/" << this->edges.size() << std::endl;
        }
        auto&& v0 = this->vertices[e[0]];
        auto&& v1 = this->vertices[e[1]];
        double r0 = this->radii[e[0]];
        double r1 = this->radii[e[1]];
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
                    LinePos lp = distToLine(v0,v1,world_coord);
                    double radius = this->lerpfunc(lp.lerpval,world_coord,r0,r1);
                    double newval = lp.dist - radius;
                    if(std::abs(newval) >  radius) {
                        continue;
                    }
                    double oldval = accessor.getValue(coord);
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
