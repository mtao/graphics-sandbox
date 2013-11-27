#ifndef TRELLIS_GEN_HPP
#define TRELLIS_GEN_HPP
#include "trellis.hpp"

class TrellisGenerator{
    public:
        typedef Trellis::Vertex Vertex;
        typedef Trellis::Edge Edge;
        typedef openvdb::math::BBox<Vertex> BBox;
        TrellisGenerator();
        Trellis generate() const ;
        std::vector<Vertex> generate_vertices(const openvdb::DoubleGrid::Ptr& grid = openvdb::DoubleGrid::Ptr()) const;

    private:
        BBox bbox = {Vertex{0,0,0},Vertex{1,1,1}};
        double density = 500.0;
        double max_edge_length = 0.2;
        double min_vertex_dist = 0.1;
};
#endif

