#ifndef MESHTRAITS_H
#define MESHTRAITS_H
namespace mtao {
template <typename MeshType>
class MeshConstructor;
template <typename TopologyType>
class MeshTopologyConstructor;
template <template<typename> class MeshType, typename T>
class MeshBase;

template <typename TopologyType>
struct topology_traits {
    typedef MeshTopologyConstructor<TopologyType> topology_constructor_type;
};

template <typename MeshType>
struct mesh_traits {
    typedef typename MeshType::Scalar Scalar;
    typedef MeshConstructor<MeshType> constructor_type;
    typedef typename MeshType::topology_type topology_type;
    typedef MeshTopologyConstructor<topology_type> topology_constructor_type;
};
}

#endif // MESHTRAITS_H
