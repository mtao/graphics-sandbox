#ifndef TRELLIS_HPP
#define TRELLIS_HPP
#include <openvdb/openvdb.h>
#include <vector>
struct LinePos {
    double lerpval;
    double dist;
};

struct Trellis {
    public:
    Trellis() {}
        typedef openvdb::Vec3d Vertex;
        typedef std::array<size_t,2> Edge;
    Trellis(const std::vector<Vertex>& v, const std::vector<Edge>& e, const std::vector<double>& r= std::vector<double>());
    Trellis(std::vector<Vertex>&& v, std::vector<Edge>&& e, std::vector<double>&& r= std::vector<double>());
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        std::vector<double> radii;
        double default_radius = 0.05;
        void createSDF(openvdb::DoubleGrid::Ptr& grid) ;
        LinePos distToLine(
                const Trellis::Vertex& a,
                const Trellis::Vertex& b,
                const Trellis::Vertex& x) const;
        std::function<double(double, const Vertex&, double,double)> lerpfunc;
};

#endif
