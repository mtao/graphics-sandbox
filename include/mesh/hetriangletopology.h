#ifndef HETRIANGLE_TOPOLOGY_H
#define HETRIANGLE_TOPOLOGY_H
#include "halfedge.h"
#include "meshtopologyconstructor.h"
#include <map>
#include <set>
//#include <vector>
namespace mtao {
    class HETriangleTopology;


        class HETriangleTopology {
            public:
                typedef HalfEdge::ptr_type ptr_type;
                typedef HalfEdge::ptr_pair_type ptr_pair;
            HETriangleTopology() {}
            HETriangleTopology(const std::set<ptr_type>& p): ptrs(p) {}
                std::set<ptr_type>& halfedges() {
                    return ptrs;
                }
                const std::set<ptr_type>& halfedges() const {
                    return ptrs;
                }
                std::set<ptr_type> triangles() const;
            private:
                ptr_type triangle(const ptr_type& he) const;
                std::set<ptr_type> ptrs;
        };



        template <>
        class MeshTopologyConstructor<HETriangleTopology>{
            public:
                typedef HalfEdge::ptr_type ptr_type;
                typedef HalfEdge::ptr_pair_type ptr_pair;
                void add_triangle(size_t a, size_t b, size_t c);
                ptr_type create_edge(size_t a, size_t b);
                std::set<ptr_type> toSet() const;
                //std::vector<ptr_type> toVector() const;
                std::set<ptr_type> triangles;
                HETriangleTopology move();
                HETriangleTopology construct() {return move();}
            private:
                std::map<size_t, std::map<size_t, ptr_type> > m_edgeMap;
                //This breaks the structure if ptr already exists!
                void insert(const ptr_type& ptr);
        };


        struct HETriangleTopologyOperators {
            public:
                typedef HalfEdge::ptr_type ptr_type;
                typedef HalfEdge::ptr_pair_type ptr_pair;
                void flip(ptr_type&);
                void split(ptr_type&, size_t idx);
                //Warning: this doesn't destroy the edge itself, just reconfigures all of the neighbors correctly
                void collapse(ptr_type&);
            private:
                void remove(ptr_type&);
                HETriangleTopology* topology;
        };
        //TODO: split if edge longer than some alpha
        //TODO: flip if distance between non-edge points  is less than distance between edge points
        //TODO: If edge shorter than alpha, edge collapse
}
#endif
