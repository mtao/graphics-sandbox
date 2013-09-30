#ifndef HETRIANGLEMESH_H
#define HETRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include <set>
#include "halfedge.h"

//Half Edge Triangle Mesh
template <typename Scalar>
class HETriangleMesh {
    public:
        typedef Eigen::Matrix<Scalar,3,1> Vector3;
        typedef std::vector<Vector3> VertexVector;
    private:
        VertexVector m_vertices;
        std::set<HalfEdge> m_edges;


};
