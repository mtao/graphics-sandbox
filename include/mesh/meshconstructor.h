#ifndef MESHCONSTRUCTOR_H
#define MESHCONSTRUCTOR_H
#include "core/types.h"
#include "meshtopologybase.h"
#include "meshtraits.h"
namespace internal {
template <typename MeshType>
class MeshConstructorBase {
    public:
    typedef typename mtao::mesh_traits<MeshType>::Scalar Scalar;
    typedef typename mtao::mesh_traits<MeshType>::topology_constructor_type TopologyConstructor;
        MeshConstructorBase() {}
        MeshConstructorBase(const std::string& filename);
        virtual void add_triangle(size_t a, size_t b, size_t c) {
            m_topo.add_triangle(a,b,c);
        }

        virtual void add_vertex(Scalar a, Scalar b, Scalar c) = 0;
protected:
        TopologyConstructor m_topo;

};



template <typename MeshType>
class MeshConstructor {
    typedef typename mtao::type_traits<MeshType>::Scalar Scalar;
    static internal::MeshConstructorBase<MeshType> autodetectConstructor(const std::string& filename);
};
}

#include "meshconstructor.ipl"

#endif
