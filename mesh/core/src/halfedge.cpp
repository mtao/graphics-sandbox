#include "halfedge.h"


HalfEdge::HalfEdge(int a, int b): std::array<int,2>{{a,b}} {}

auto HalfEdge::make_half_edge(int a, int b) -> std::array<pointer_type,2> {
    pointer_type x(new HalfEdge(a,b));
    pointer_type y(new HalfEdge(b,a));
    x->m_dual = y;
    y->m_dual = x;
    return std::array<std::shared_ptr<HalfEdge>,2>{{x,y}};
}


void HalfEdge::set_triangle(pointer_type& a, pointer_type& b, pointer_type& c) {
    //TODO: assert a[1] == b[0], b[1] == c[0], c[1] == a[0]
    a->m_next = b;
    b->m_next = c;
    c->m_next = a;
}
