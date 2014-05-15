#ifndef TOPOLOGYCONSTRUCTOR_H
#define TOPOLOGYCONSTRUCTOR_H
#include "core/types.h"
#include "meshtraits.h"
namespace mtao {
template <typename TopologyType>
class MeshTopologyConstructor {
    public:
        MeshTopologyConstructor() {}
        void add_triangle(size_t a, size_t b, size_t c);

        TopologyType move();
        const TopologyType& construct();

    private:

};
}




#include "meshtopologyconstructor.ipl"

#endif
