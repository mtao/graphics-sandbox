#ifndef HETRIANGLEMESH_H
#define HETRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include <set>
#include "hetriangletopology.h"
#include "meshconstructor.h"
#include "meshbase.h"


namespace mtao {
//Half Edge Triangle Mesh
template <typename Scalar_>
class HETriangleMesh: public MeshBase<HETriangleMesh,Scalar_ >, public mtao::HETriangleTopology {
public:
    typedef HETriangleMesh<Scalar_> type;
    typedef HETriangleTopology topology_type;
    typedef Scalar_ Scalar;
    typedef Eigen::Matrix<Scalar,3,1> Vec3;
    typedef std::vector<Vec3> VertexVector;
    typedef typename mtao::mesh_traits<type > traits;
    HETriangleMesh() {}
    //HETriangleMesh(const typename traits::constructor_type& hemcon): traits::topology_type(hemcon), m_vertices(hemcon.vertices()) {}
    HETriangleMesh(VertexVector&& v, topology_type&& t): topology_type(t), m_vertices(v) {}
    HETriangleMesh(const VertexVector& v, const topology_type& t): topology_type(t), m_vertices(v) {}

        const VertexVector & vertices() const {return m_vertices;}
        VertexVector & vertices() {return m_vertices;}
        Vec3& vertex(size_t idx);
        const Vec3& vertex(size_t idx) const;
        void add_triangle(size_t a, size_t b, size_t c);
        void add_vertex(Scalar a, Scalar b, Scalar c);
    private:
        VertexVector m_vertices;


};
template <typename Scalar>
class HETriangleMeshCleaner: public mtao::HETriangleTopologyOperators {
    public:
        HETriangleMeshCleaner(HETriangleMesh<Scalar>* mesh): mtao::HETriangleTopologyOperators(mesh) {}
        void clean();
};
}

#include "hetrianglemesh.ipl"

#endif
