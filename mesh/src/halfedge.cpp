#include "halfedge.h"
std::array<std::shared_ptr<HalfEdge>,2> HalfEdge::make_half_edge(int a, int b) {
    auto&& aa = std::make_shared<HalfEdge>(a,b);
    auto&& bb = std::make_shared<HalfEdge>(b,a);
    return {aa,bb};

}
