#ifndef SIMPLEXMESH_H
#define SIMPLEXMESH_H

#include <set>
#include <Eigen/Dense>
#include <string>
#include "../types.h"
namespace mtao { namespace geometry { namespace mesh {
    template <int Dim>
        class SimplexMesh {
            public:
                using Vec3 = mtao::geometry::Vec3;
                using Scalar = mtao::geometry::config::Scalar;

                using VertexMat = Eigen::Matrix<Scalar,3,Eigen::Dynamic>;
                using SimplexMat = Eigen::Matrix<int,Dim+1,Eigen::Dynamic>;


                SimplexMesh(const VertexMat& v, const SimplexMat& f): mVertices(v), mSimplices(f) {}



                int numVertices() const { return mVertices.cols(); }
                int numSimplices() const { return mSimplices.cols(); }

                auto operator[](size_t idx) const {return mVertices.col(idx);}
                auto operator[](size_t idx) {return mVertices.col(idx);}
                //double intersect(const Simplex & t, const Eigen::ParametrizedLine<double,3> & r) const;

                auto& vertices() { return mVertices; }
                auto& vertices() const { return mVertices; }
                auto& simplices() { return mSimplices; }
                auto& simplices() const { return mSimplices; }
            private:
                VertexMat mVertices;
                SimplexMat mSimplices;
        };
}}}

#endif
