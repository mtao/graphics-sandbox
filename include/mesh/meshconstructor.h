#ifndef MESHCONSTRUCTOR_H
#define MESHCONSTRUCTOR_H
#include "core/types.h"
#include "meshtopologyconstructor.h"
#include "meshtraits.h"
namespace mtao {

class MeshConstructorBase {
public:
    virtual void add_triangle(size_t a, size_t b, size_t c) = 0;
    virtual void add_vertexf(float a, float b, float c) = 0;
    virtual void add_vertexd(double a, double b, double c) = 0;
};

template <typename MeshType>
class MeshConstructor: public MeshConstructorBase, public mtao::mesh_traits<MeshType>::topology_constructor_type {
    public:

    typedef typename mtao::mesh_traits<MeshType>::Scalar Scalar;
     typedef typename mtao::scalar_types<Scalar>::Vec3 Vec3;
    typedef typename mtao::mesh_traits<MeshType>::topology_constructor_type TopologyConstructor;
        MeshConstructor() {}
        MeshConstructor(const std::string& filename);
        void add_triangle(size_t a, size_t b, size_t c);

        void add_vertex(Scalar a, Scalar b, Scalar c);
        void add_vertexd(double a, double b, double c) {add_vertex(a,b,c);}
        void add_vertexf(float a, float b, float c) {add_vertex(a,b,c);}

        const MeshType& construct();
        MeshType move();

    private:
    std::vector<Vec3> m_vertices;



};
}




#include "meshconstructor.ipl"

#endif
