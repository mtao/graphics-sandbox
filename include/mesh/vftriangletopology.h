#ifndef VFTRIANGLE_TOPOLOGY_H
#define VFTRIANGLE_TOPOLOGY_H
#include "halfedge.h"
#include "meshtopologyconstructor.h"
#include <map>
#include <set>
//#include <vector>
namespace mtao {
    class VFTriangleTopology;


        class VFTriangleTopology {
            public:
            VFTriangleTopology() {}
            typedef std::array<size_t,3> triangle_type;
            VFTriangleTopology(const std::set<triangle_type> triangles): m_triangles(triangles) {}
                std::set<triangle_type>& triangles() {return m_triangles;}
                const std::set<triangle_type>& triangles() const {return m_triangles;}
            private:
                std::set<triangle_type> m_triangles;
        };



        template <>
        class MeshTopologyConstructor<VFTriangleTopology>{
            public:
            typedef std::array<size_t,3> triangle_type;
            void add_triangle(size_t a, size_t b, size_t c);
                VFTriangleTopology move();
                VFTriangleTopology construct() {return move();}
            private:
                std::set<triangle_type> m_triangles;
        };


}
#endif
