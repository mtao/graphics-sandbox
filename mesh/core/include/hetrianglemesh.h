#ifndef HETRIANGLEMESH_H
#define HETRIANGLEMESH_H
#include "types.h"
#include <vector>
#include <set>
#include "halfedge.h"
#include "meshconstructor.h"
#include <map>

//Half Edge Triangle Mesh
template <typename Scalar>
class HETriangleMesh {
    public:
        typedef typename mtao::scalar_types<Scalar>::Vec3  Vec3;
        typedef std::vector<Vec3> VertexVector;
        inline size_t add_vertex(const Vec3& v);
        inline size_t emplace_vertex(Vec3&& v);
        inline Vec3& get_vertex(size_t idx);
        inline const Vec3& get_vertex(size_t idx) const;

    private:
        VertexVector m_vertices;
        std::set<HalfEdge> m_edges;


};

namespace internal {
    class HEMeshEdgeConstructor {
        public:
            HEMeshEdgeConstructor();
            void add_triangle(int a, int b, int c);
            HalfEdge::ptr& get_edge(int a, int b);
            void add_edge(const HalfEdge::ptr&);
        private:
            std::map<int,std::map<int,HalfEdge::ptr> > m_edges;
    };

    template <typename Scalar>
    class MeshConstructor<HETriangleMesh<Scalar> > : public MeshConstructorBase<Scalar> {
        public:
        typedef typename mtao::scalar_types<Scalar>::Vec3  Vec3;
        typedef std::vector<Vec3> VertexVector;
            MeshConstructor(const std::string& filename);
            void add_triangle(int a, int b, int c);
            void add_vertex(Scalar a, Scalar b, Scalar c);
        private:
            HEMeshEdgeConstructor m_edge_manager;
            VertexVector m_vertices;
    };
}




#include "hetrianglemesh.ipl"
#endif
