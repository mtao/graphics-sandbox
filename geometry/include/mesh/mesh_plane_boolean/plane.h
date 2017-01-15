#include <Eigen/Geometry>
#include "trianglemesh.h"

struct Plane {
    typedef Eigen::Vector3d Vector3;
    Plane(Eigen::Hyperplane<double,3>&& plane): m_plane(plane){}
    Plane(const Eigen::Hyperplane<double,3>& plane): m_plane(plane){}
    Plane &operator=(const Plane & other){m_plane = other.m_plane;return *this;}
    Plane &operator=(Plane&& other){m_plane = std::move(other.m_plane);return *this;}
    Eigen::Hyperplane<double,3> m_plane;
    TriangleMesh cut(const TriangleMesh & t);
    private:
    typedef std::pair<size_t,size_t> Edge;
    enum class IntersectionType: char {None, Intersect1, Intersect2, Interior};
    IntersectionType intersect(const TriangleMesh & tm, const Triangle & t, Vector3 & p1, Vector3 & p2, size_t &o);
    void add(std::vector<Vector3> &, std::set<Triangle> & tris, const Triangle & t, const Vector3 & p1, const Vector3 & p2, size_t o, bool single);
};
