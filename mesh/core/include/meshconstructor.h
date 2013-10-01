#ifndef MESHCONSTRUCTOR_H
#define MESHCONSTRUCTOR_H

template <typename Scalar>
class MeshConstructor {
    public:
        virtual void add_triangle(int a, int b, int c) = 0;
        virtual void add_vertex(Scalar a, Scalar b, Scalar c) = 0;
};

#endif
