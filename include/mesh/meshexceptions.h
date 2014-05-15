#ifndef MESH_EXCEPTIONS_H
#define MESH_EXCEPTIONS_H
#include <exception>
#include <cstddef>

namespace mtao { namespace exceptions {
    struct NonManifoldEdge: public std::exception {
        NonManifoldEdge(size_t a, size_t b): a(a),b(b) {}
        const char * what() const throw();
        size_t a, b;
    };
    struct NonManifoldTriangle: std::exception {
        NonManifoldTriangle(size_t a, size_t b, size_t c): a(b),b(b),c(c) {}
        const char * what() const throw();
        size_t a,b,c;
    };
    
}
}

#endif
