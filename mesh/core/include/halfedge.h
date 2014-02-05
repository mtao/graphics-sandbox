#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <array>
#include <memory>
//Half Edge
//This should never create itself


struct HalfEdge {
    public:
        typedef std::shared_ptr<HalfEdge> ptr_type;
        typedef std::array<ptr_type,2> ptr_pair_type;
        //friend std::shared_ptr<HalfEdge> std::make_shared<>
        ptr_type next = 0;
        ptr_type dual = 0;
        //Assume CCW oriented mesh

        //Circulate with arrow pointing away from vertex
        ptr_type circulateCW() const {
            return dual->next;
        }
        
        //Circulate with arrow pointing array from vertex
        ptr_type circulateCCW() const {
            return next->dual;
        }
        size_t head;
    private:
        HalfEdge(size_t a);

        //public static functions
    public:
        //Half edges always come in pairs, so here's a pair
        static std::array<std::shared_ptr<HalfEdge>,2> make_half_edge(size_t a, size_t b);
        static void set_triangle(ptr_type&,ptr_type&, ptr_type&);


};


#endif
