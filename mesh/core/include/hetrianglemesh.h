#ifndef HETRIANGLEMESH_H
#define HETRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include <set>
#include "halfedge.h"
#include <map>


namespace internal {
    class HEMeshEdgeConstructor: public MeshConstructor {
        public:
            HEMeshEdgeConstructor();
        private:
            void add_triangle(int a, int b, int c);
            HalfEdge::ptr& get_edge(int a, int b);
            void add_edge(const HalfEdge::ptr&);
        private:
            std::map<int,std::map<int,HalfEdge::ptr> > m_edges;
    };
};


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


#endif
