#ifndef MESH_IO_H
#define MESH_IO_H
#include <string>

namespace mtao {
    template <typename T>
class MeshConstructorBase;
template <template<typename> class MeshType, typename T=float>
class MeshLoader {
    public:
        MeshLoader(MeshConstructorBase<T>* constructor): m_constructor(constructor) {}
        virtual void open(const std::string& str) = 0;
        void add_triangle(size_t i, size_t j, size_t k) {
            m_constructor->add_triangle(i,j,k);
        }
        void add_vertex(T i, T j, T k) {
            m_constructor->add_vertex(i,j,k);
        }

    protected:
        MeshConstructorBase<T> * m_constructor;
};
template <typename T>
class MeshSaver {
    public:
        MeshLoader(<T>* constructor): m_constructor(constructor) {}
        virtual void open(const std::string& str) = 0;
        void add_triangle(size_t i, size_t j, size_t k) {
            m_constructor->add_triangle(i,j,k);
        }
        void add_vertex(T i, T j, T k) {
            m_constructor->add_vertex(i,j,k);
        }

    protected:
        MeshConstructorBase<T> * m_constructor;
};
}

#endif
