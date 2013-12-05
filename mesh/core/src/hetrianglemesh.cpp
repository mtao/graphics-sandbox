#include "hetrianglemesh.h"

internal::HEMeshEdgeConstructor::HEMeshEdgeConstructor() {}

HalfEdge::ptr& internal::HEMeshEdgeConstructor::get_edge(int a, int b) {
    decltype(m_edges)::iterator ait = m_edges.find(a);
    if(ait != m_edges.end()) {
        auto&& amap = ait->second;
        auto bit = amap.find(b);
        if(bit != ait->second.end()) {
            return bit->second;

        }
    }
    auto ep = HalfEdge::make_half_edge(a,b);
    add_edge(ep[0]);
    add_edge(ep[1]);
    return ep[0];
}


void internal::HEMeshEdgeConstructor::add_edge(const HalfEdge::ptr& ptr) {
    const int a = (*ptr)[0];
    const int b = (*ptr)[1];
    decltype(m_edges)::iterator ait = m_edges.find(a);
    if(ait == m_edges.end()) {
        ait = m_edges.emplace(std::make_pair(a,
                    decltype(m_edges)::mapped_type())).first;
    }
    auto&& amap = ait->second;
    auto bit = amap.find(b);
    if(bit == amap.end()) {
        amap.emplace(std::make_pair(b,ptr));
    }

}



void internal::HEMeshEdgeConstructor::add_triangle(int a, int b, int c) {
    auto x = get_edge(a,b);
    auto y = get_edge(b,c);
    auto z = get_edge(c,a);
    HalfEdge::set_triangle(x,y,z);
}
