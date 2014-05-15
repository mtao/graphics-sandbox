#ifndef MESHCONSTRUCTOR_H
#define MESHCONSTRUCTOR_H
#include "core/types.h"
#include "meshtopologyconstructor.h"
#include "meshtraits.h"
namespace mtao {
template <typename MeshType>
class MeshConstructor: public mtao::mesh_traits<MeshType>::topology_constructor_type {
    public:

    typedef typename mtao::mesh_traits<MeshType>::Scalar Scalar;
     typedef typename mtao::scalar_types<Scalar>::Vec3 Vec3;
    typedef typename mtao::mesh_traits<MeshType>::topology_constructor_type TopologyConstructor;
        MeshConstructor() {}
        MeshConstructor(const std::string& filename);
        void add_triangle(size_t a, size_t b, size_t c);

        void add_vertex(Scalar a, Scalar b, Scalar c);

        const MeshType& construct();
        MeshType move();

    private:
    std::vector<Vec3> m_vertices;



};
}




#include "meshconstructor.ipl"

#endif
