#ifndef HETRIANGLEMESH_H
#define HETRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include <set>
#include "hetriangletopology.h"
#include "meshconstructor.h"


//Half Edge Triangle Mesh
template <typename Scalar>
class HETriangleMesh: public mtao::HETriangleTopology {
    public:
        typedef Eigen::Matrix<Scalar,3,1> Vec3;
        typedef std::vector<Vec3> VertexVector;
        HETriangleMesh(const internal::MeshConstructor<HETriangleMesh<Scalar> >& hemcon): HETriangleTopology(hemcon), m_vertices(hemcon.vertices()) {}

        Vec3& get_vertex(size_t idx);
        const Vec3& get_vertex(size_t idx) const;
    private:
        VertexVector m_vertices;


};
namespace internal {
    //A MeshConstructor is useful here because it accelerates searching for existing edges/triangles
template <typename Scalar>
class MeshConstructor<HETriangleMesh<Scalar> >: public mtao::HETriangleTopologyConstructor {
    public:
        typedef Eigen::Matrix<Scalar,3,1> Vec3;
        typedef std::vector<Vec3> VertexVector;
        MeshConstructor(const std::string& filename);
        void add_vertex(Scalar a, Scalar b, Scalar c);
        void add_triangle(size_t a, size_t b, size_t c);
        //size_t add_vertex(const Vec3&);
        //size_t emplace_vertex(Vec3&&);
        const Vec3& get_vertex(size_t idx) const;
        const VertexVector& vertices() const {return m_vertices;}

    private:
        VertexVector m_vertices;
};
}
template <typename Scalar>
class HETriangleMeshCleaner: public mtao::HETriangleTopologyOperators {
    public:
        HETriangleMeshCleaner(HETriangleMesh<Scalar>* mesh): mtao::HETriangleTopologyOperators(mesh) {}
        void clean();
};

#include "hetrianglemesh.ipl"

#endif
