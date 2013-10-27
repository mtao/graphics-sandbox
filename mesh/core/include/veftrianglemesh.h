#ifndef VEFTRIANGLEMESH_H
#define VEFTRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include "vftrianglemesh.h"
//Vertex+Face triangle mesh
template <typename Scalar>
class VEFTriangleMesh: public VFTriangleMesh<Scalar> {
    typedef mtao::Vec2i Edge;
    typedef typename mtao::scalar_types<Scalar>::Vec3 Vec3;
    typedef VFTriangleMesh<Scalar> VFMesh;
    typedef typename VFMesh::TriangleSet TriangleSet;
    typedef typename VFMesh::VertexVector VertexVector;
    public:
    //Public typedefs
    typedef Edge edge_type;
    typedef std::set<Edge> EdgeSet;
    //Constructors
    VEFTriangleMesh();
    VEFTriangleMesh(const VEFTriangleMesh& vf);
    VEFTriangleMesh(VEFTriangleMesh&& vf);
    VEFTriangleMesh(const VFMesh& vf);
    VEFTriangleMesh(VFMesh&& vf);
    VEFTriangleMesh(const TriangleSet& triangles, const VertexVector& vertices);
    static VEFTriangleMesh<Scalar>&& openObj(const std::string& str);
    //Bulk Accessors
    EdgeSet & edges() {return m_edges;}
    const EdgeSet & edges()const {return m_edges;}

    //Individual Accessors
    Edge& getEdge(int idx) {return m_edges[idx];}
    const Edge& getEdge(int idx)const {return m_edges[idx];}
    //Inserters
    void insertEdge(const Edge& edge){m_edges.insert(edge);}
    protected:
    EdgeSet m_edges;
    private:
    void generateEdgesFromTriangles();
};

namespace internal {
    template <typename Scalar>
        class MeshConstructor<VEFTriangleMesh<Scalar> >: public MeshConstructor<VFTriangleMesh<Scalar> > {
        };
}

#include "veftrianglemesh.ipl"
#endif
