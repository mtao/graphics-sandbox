#ifndef MESHCONSTRUCTOR_H
#define MESHCONSTRUCTOR_H

namespace internal {
template <typename Scalar>
class MeshConstructorBase {
    public:
        virtual void add_triangle(int a, int b, int c) = 0;
        virtual void add_vertex(Scalar a, Scalar b, Scalar c) = 0;

};


template <typename MeshType>
class MeshConstructor {};
}

#endif
