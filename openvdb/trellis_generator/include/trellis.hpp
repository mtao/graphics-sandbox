#ifndef TRELLIS_HPP
#define TRELLIS_HPP
#include <openvdb/openvdb.h>
#include <vector>

struct Trellis {
    public:
        typedef openvdb::Vec3d Vertex;
        typedef std::array<size_t,2> Edge;
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        void createSDF(openvdb::DoubleGrid::Ptr& grid, double contour=0.1) const;
};

#endif
