#ifndef LOADER_H
#define LOADER_H
#include <string>
#include "meshconstructor.h"
namespace mtao {
class MeshLoader {
    public:
    MeshLoader() {}
        MeshLoader(MeshConstructorBase* constructor): m_constructor(constructor) {}
        virtual void open(const std::string& str) = 0;
        virtual void save(const std::string& str) = 0;
        void add_triangle(size_t a, size_t b, size_t c ) {
            m_constructor->add_triangle(a,b,c);
        }

        void add_vertex(float a, float b, float c) {
            m_constructor->add_vertexf(a,b,c);
        }
        void add_vertex(double a, double b, double c) {
            m_constructor->add_vertexd(a,b,c);
        }

    protected:
        MeshConstructorBase * m_constructor;


};
}

#endif
