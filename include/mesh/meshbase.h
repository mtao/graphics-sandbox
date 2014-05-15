#ifndef MESH_H
#define MESH_H
#include "core/types.h"

namespace mtao {
    template <template<typename> class MeshType, typename T=float>
    class MeshBase {
        public:
            typedef T scalar_type;
            typedef MeshType<scalar_type> mesh_type;
            typedef typename mtao::scalar_types<T>::Vec3 Vec3;
            mesh_type& derived() { return *static_cast<mesh_type*>(this); }
            const mesh_type& derived() const { return *static_cast<const mesh_type*>(this); }

        void add_triangle(size_t a, size_t b, size_t c) { derived().add_triangle(a,b,c); }
        void add_triangle(const Vec3& a, const Vec3& b, const Vec3& c) { derived().add_triangle(a,b,c); }
        void add_vertex () { derived().add_triangle(); }
        template <typename ContainerType>
        void add_vertices (const ContainerType& data) { derived().add_vertices(data); }
    };
}

#endif
