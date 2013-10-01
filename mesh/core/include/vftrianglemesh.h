#ifndef VFTRIANGLEMESH_H
#define VFTRIANGLEMESH_H
#include <Eigen/Core>
#include <vector>
#include <set>
#include "eigen_ordering.h"


namespace internal {
    class VFTriangleMeshConstructor: public MeshConstructor {
    };
};

//Vertex+Face triangle mesh
template <typename Scalar>
class VFTriangleMesh {
    protected:
    typedef Eigen::Vector3i Triangle;
    typedef Eigen::Matrix<Scalar,3,1> Vector3;
    typedef std::set<Triangle> TriangleSet;
    typedef std::vector<Vector3> VertexVector;
    public:
    //Public typedefs
    typedef Triangle triangle_type;
    typedef Eigen::Matrix<Scalar,3,1> vector_type;
    //Constructors
    VFTriangleMesh();
    VFTriangleMesh(const std::string& str);
    VFTriangleMesh(const TriangleSet& triangles, const VertexVector& vertices);
    //Bulk Accessors
    TriangleSet & triangles() {return m_triangles;}
    const TriangleSet & triangles()const {return m_triangles;}
    VertexVector & vertices() {return m_vertices;}
    const VertexVector & vertices()const {return m_vertices;}

    //Individual accessors
    Triangle& get_triangle(int idx) {return m_triangles[idx];}
    const Triangle& get_triangle(int idx)const {return m_triangles[idx];}
    Vector3& get_vertex(int idx) {return m_vertices[idx];}
    const Vector3& get_vertex(int idx)const {return m_vertices[idx];}
    //Inserters
    void insert_triangle(const Triangle& triangle){m_triangles.insert(triangle);}
    int add_vertex(const Vector3& vertex){m_vertices.push_back(vertex);return m_vertices.size()-1;}
    protected:
    TriangleSet m_triangles;
    VertexVector m_vertices;
};
#include "vftrianglemesh.ipl"

typedef VFTriangleMesh<float> VFTriangleMeshf;
typedef VFTriangleMesh<double> VFTriangleMeshd;


#endif

