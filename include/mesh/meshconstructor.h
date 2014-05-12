#ifndef MESHCONSTRUCTOR_H
#define MESHCONSTRUCTOR_H

namespace internal {
template <typename Scalar>
class MeshConstructorBase {
    public:
        MeshConstructorBase() {}
        MeshConstructorBase(const std::string& filename);
        virtual void add_triangle(size_t a, size_t b, size_t c) = 0;
        virtual void add_vertex(Scalar a, Scalar b, Scalar c) = 0;

};


template <typename MeshType>
class MeshConstructor {};
}

#include "meshconstructor.ipl"

#endif
