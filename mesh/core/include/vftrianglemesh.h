#ifndef VFTRIANGLEMESH_H
#define VFTRIANGLEMESH_H
#include "types.h"
#include <vector>
#include "eigen_ordering.h"
#include "eigen_hash.h"
#include <unordered_set>
#include "meshconstructor.h"



//Vertex+Face triangle mesh
template <typename Scalar>
class VFTriangleMesh {
    protected:
        typedef mtao::Vec3i Triangle;
        typedef typename mtao::scalar_types<Scalar>::Vec3 Vec3;
        typedef std::unordered_set<Triangle> TriangleSet;
        typedef std::vector<Vec3> VertexVector;
    public:
        //Public typedefs
        typedef Triangle triangle_type;
        typedef Eigen::Matrix<Scalar,3,1> vector_type;
        //Constructors
        VFTriangleMesh();
        VFTriangleMesh(const std::string& str);
        //VFTriangleMesh(const TriangleSet& triangles, const VertexVector& vertices);
        //Bulk Accessors
        TriangleSet & triangles() {return m_triangles;}
        const TriangleSet & triangles()const {return m_triangles;}
        VertexVector & vertices() {return m_vertices;}
        const VertexVector & vertices()const {return m_vertices;}

        //Individual accessors
        Triangle& get_triangle(int idx) {return m_triangles[idx];}
        const Triangle& get_triangle(int idx)const {return m_triangles[idx];}
        Vec3& get_vertex(int idx) {return m_vertices[idx];}
        const Vec3& get_vertex(int idx)const {return m_vertices[idx];}
        //Inserters
        void insert_triangle(const Triangle& triangle){m_triangles.insert(triangle);}
        int add_vertex(const Vec3& vertex){m_vertices.push_back(vertex);return m_vertices.size()-1;}
    protected:
        TriangleSet m_triangles;
        VertexVector m_vertices;
};

#include "mesh_reader.h"
namespace internal {
    template <typename Scalar>
    class MeshConstructor<VFTriangleMesh<Scalar> >: public MeshConstructorBase<Scalar> {
        public:
            typedef VFTriangleMesh<Scalar> mesh_type;
            typedef MeshConstructorBase<Scalar> Base;
            MeshConstructor() {}
            MeshConstructor(const std::string& str);
            void load(const std::string& str, internal::MeshFileType type=internal::MeshFileType::UNKNOWN);
            void write(mesh_type& mesh) const;
            void emplace(mesh_type& mesh) ;
            void add_triangle(int a, int b, int c);
            void add_vertex(Scalar a, Scalar b, Scalar c);
            mesh_type get();
            typedef mtao::Vec3i Triangle;
            typedef typename mtao::scalar_types<Scalar>::Vec3 Vec3;
            typedef std::unordered_set<Triangle> TriangleSet;
            typedef std::vector<Vec3> VertexVector;
            TriangleSet triangles;
            VertexVector vertices;

    };
}




#include "vftrianglemesh.ipl"

typedef VFTriangleMesh<float> VFTriangleMeshf;
typedef VFTriangleMesh<double> VFTriangleMeshd;


#endif

