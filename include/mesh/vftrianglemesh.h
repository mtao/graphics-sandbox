#ifndef VFTRIANGLEMESH_H
#define VFTRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include <set>
#include "vftriangletopology.h"
#include "meshconstructor.h"
//Vertex+Face triangle mesh
namespace mtao {
template <typename Scalar_>
class VFTriangleMesh: public VFTriangleTopology {
    public:
    typedef Scalar_ Scalar;
    typedef VFTriangleTopology topology_type;
    typedef Eigen::Matrix<Scalar,3,1> Vec3;
    typedef std::vector<Vec3> VertexVector;
    //Constructors
    VFTriangleMesh();
    //VFTriangleMesh(const std::string& str);
    VFTriangleMesh(const VertexVector& v, const topology_type& t): topology_type(t), m_vertices(v) {}
    VFTriangleMesh(VertexVector&& v, topology_type&& t): topology_type(t), m_vertices(v) {}
    //Bulk Accessors
    VertexVector & vertices() {return m_vertices;}
    const VertexVector & vertices()const {return m_vertices;}

    //Individual accessors
    Vec3& getVertex(int idx) {return m_vertices[idx];}
    const Vec3& getVertex(int idx)const {return m_vertices[idx];}
    protected:
    VertexVector m_vertices;
};
typedef VFTriangleMesh<float> VFTriangleMeshf;
typedef VFTriangleMesh<double> VFTriangleMeshd;
}
#include "vftrianglemesh.ipl"



#endif

