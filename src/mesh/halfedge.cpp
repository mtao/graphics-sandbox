#include "mesh/halfedge.h"


HalfEdge::HalfEdge(size_t a): head(a) {}

auto HalfEdge::make_half_edge(size_t a, size_t b) -> std::array<ptr_type,2> {
    //for other purposes it's useful to know which order the edges are coming out?
    /*
    if(a > b) {
        size_t tmp = a;
        a = b;
        b = tmp;
    }
    */
    ptr_type x(new HalfEdge(b));
    ptr_type y(new HalfEdge(a));
    x->dual = y;
    y->dual = x;
    return ptr_pair_type{{x,y}};
}


void HalfEdge::set_triangle(ptr_type& a, ptr_type& b, ptr_type& c) {
    //TODO: assert a[1] == b[0], b[1] == c[0], c[1] == a[0]
    a->next = b;
    b->next = c;
    c->next = a;
}
