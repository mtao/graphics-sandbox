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



        void setDensity(double d) {density = d;}
        void setMaxEdgeLength(double d) {max_edge_length = d; min_vertex_dist = .5 * d;}

    private:
        BBox bbox = {Vertex{0,0,0},Vertex{1,1,1}};
        double density = 100.0;
        double max_edge_length = 0.4;
        double min_vertex_dist = 0.2;
};
#endif

