#ifndef MESHTRAITS_H
#define MESHTRAITS_H
namespace mtao {
template <typename MeshType>
class MeshConstructor;
template <typename MeshType>
class MeshConstructorBase;
template <typename TopologyType>
class MeshTopologyConstructor;
template <typename MeshType>
class MeshBase;

template <typename MeshType>
class mesh_traits {
    typedef typename MeshType::Scalar Scalar;
    typedef MeshConstructor<MeshType> constructor_type;
    typedef MeshConstructorBase<MeshType> constructor_base_type;
    typedef typename MeshType::topology_type topology_type;
    typedef MeshTopologyConstructor<topology_type> topology_constructor_type;
};
}

#endif // MESHTRAITS_H
