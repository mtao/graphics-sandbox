#include <Eigen/Dense>
#include "trianglemesh.h"

struct Plane {
    typedef Eigen::Vector3d Vector3;
    Vector3 center;
    Vector3 normal;
    TriangleMesh cut(const TriangleMesh & t);
    private:
    typedef std::pair<size_t,size_t> Edge;
    enum class IntersectionType: char {None, Intersect, Interior};
    IntersectionType intersect(const TriangleMesh & tm, const Triangle & t, Vector3 & p1, Vector3 & p2, size_t o);
    void add(std::vector<Vector3> &, std::set<Triangle> & tris, const Triangle & t, const Vector3 & p1, const Vector3 & p2, size_t o);
};
