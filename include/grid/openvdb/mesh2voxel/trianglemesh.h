#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H
#include <openvdb/math/Vec3.h>
#include <openvdb/math/Vec4.h>
#include <openvdb/Types.h>

#include <set>
#include <string>
struct TriangleMesh {
    typedef openvdb::Vec3s Vector3;
    typedef openvdb::Vec3I Triangle;
    static TriangleMesh readObj(const std::string & path);
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
    const Vector3 & operator[](size_t idx) const {return vertices[idx];}
    Vector3 & operator[](size_t idx){return vertices[idx];}
};
#endif
