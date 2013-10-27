#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <array>
#include <memory>
//Half Edge
//This should never create itself

struct HalfEdge: public std::array<int,2> {
    public:
        typedef std::shared_ptr<HalfEdge> ptr;
        typedef ptr pointer_type;

        pointer_type next() const {return m_next;}
        pointer_type dual() const {return m_dual;}
        static std::array<pointer_type,2> make_half_edge(int a, int b);
        static void set_triangle(pointer_type& a, pointer_type& b, pointer_type& c);
    private:
        HalfEdge(int a, int b);
        pointer_type m_next = 0;
        pointer_type m_dual = 0;

};


#endif
