
#include "mesh/vftriangletopology.h"
#include "mesh/meshexceptions.h"
#include <limits>

#include <iostream>
namespace mtao {
        void MeshTopologyConstructor<VFTriangleTopology>::add_triangle(size_t a, size_t b, size_t c) {
            m_triangles.emplace(triangle_type{{a,b,c}});
        }

        VFTriangleTopology MeshTopologyConstructor<VFTriangleTopology>::move() {
            return VFTriangleTopology(m_triangles);
        }
}
