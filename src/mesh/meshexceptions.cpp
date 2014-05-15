#include "mesh/meshexceptions.h"
#include <sstream>

namespace mtao {namespace exceptions {
    const char * NonManifoldEdge::what() const throw() {
        std::stringstream ss;
        ss << "Non Manifold Edge found: [" << a << "->" << b << ")";
        return ss.str().c_str();
    }
    const char * NonManifoldTriangle::what() const throw() {
        std::stringstream ss;
        ss << "Non Manifold Edge found: (" << a << "," << b << "," << c << ")";
        return ss.str().c_str();
    }
}}
