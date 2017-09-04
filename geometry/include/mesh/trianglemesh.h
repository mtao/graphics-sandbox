#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <set>
#include <Eigen/Dense>
#include <string>
#include <memory>
#include "simplexmesh.h"

namespace mtao { namespace geometry { namespace mesh {
    class TriangleMesh: public SimplexMesh<2> {
        public:

        using BaseMesh = SimplexMesh<2>;

        using TriangleMat = BaseMesh::SimplexMat;

        using BaseMesh::BaseMesh;


        static std::shared_ptr<TriangleMesh> readObj(const std::string & path);
        void writeObj(const std::string & path);


        int numFaces() const { return numSimplices(); }

        auto& faces() const { return simplices(); }
        auto& faces() { return simplices(); }

    };
}}}

#endif
