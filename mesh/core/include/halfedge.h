#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <array>
#include <memory>
//Half Edge
//This should never create itself

struct HalfEdge: public std::array<int,2> {
    public:
        friend std::shared_ptr<HalfEdge> std::make_shared<HalfEdge>
        HalfEdge* next() const {return m_next;}
        HalfEdge* dual() const {return m_dual;}
        static std::array<std::shared_ptr<HalfEdge>,2> make_half_edge(int a, int b);
    private:
        HalfEdge(int a, int b);
        HalfEdge* m_next = 0;
        HalfEdge* m_dual = 0;

};


#endif
