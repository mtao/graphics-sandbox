#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <set>
#include "triangle.h"
#include <Eigen/Dense>
#include <string>
struct TriangleMesh {
    typedef Eigen::Vector3d Vector3;
    void writeObj(const std::string & path);
    static TriangleMesh readObj(const std::string & path);
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
    const Vector3 & operator[](size_t idx) const {return vertices[idx];}
    Vector3 & operator[](size_t idx){return vertices[idx];}
    double intersect(const Triangle & t, const Eigen::ParametrizedLine<double,3> & r) const;
};
#endif
