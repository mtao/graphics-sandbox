#include "plane.h"
#include <algorithm>
#include <iostream>


TriangleMesh Plane::cut(const TriangleMesh & tm) {

    std::set<Triangle> triangles;
    std::vector<Vector3> vertices;
    vertices = tm.vertices;

    Vector3 p1, p2;
    size_t o;
    for(auto&& t: tm.triangles) {
        switch(intersect(tm,t,p1,p2,o)) {
            case IntersectionType::None:
                break;
            case IntersectionType::Interior:
                triangles.insert(t);
                break;
            case IntersectionType::Intersect1:
                //TODO
                add(vertices,triangles,t,p1,p2,o,true);
                break;
            case IntersectionType::Intersect2:
                add(vertices,triangles,t,p1,p2,o,false);
                break;

        }


        
    }

    return {vertices, triangles};
}
void Plane::add(std::vector<Vector3> &verts, std::set<Triangle> & tris, const Triangle & t, const Vector3 & p1, const Vector3 & p2, size_t o, bool single) {
    size_t v1 = verts.size();
    verts.push_back(p1);
//    std::cout << "New vertex: "<< v1 <<": " << p1.transpose() << std::endl;
    size_t v2 = verts.size();
//    std::cout << "New vertex: "<< v2 <<": " << p2.transpose() << std::endl;
    verts.push_back(p2);
    tris.insert(Triangle{{t[(o+1)%3],t[(o+2)%3],v1}});
    //std::cout << o << std::endl;
    //tris.insert(Triangle{{t[(o+1)%3],t[(o+2)%3],t[o]}});
    //if(!single)
    //tris.insert(Triangle{{v2,v1,t[(o+1)%3]}});

}
auto Plane::intersect(const TriangleMesh & tm, const Triangle & t, Vector3 & p1, Vector3 & p2, size_t & o) -> IntersectionType {
    std::array<Vector3,3> verts;
    std::transform(t.begin(), t.end(),verts.begin(), 
            [&tm](size_t i) -> Vector3 {
            return tm[i];
            });
    std::array<double,3> distance;
    std::transform(verts.begin(), verts.end(),distance.begin(), 
            [&](const Vector3 & v) -> double {
            return m_plane.signedDistance(v);
            });
    std::array<bool,3> is_inside;
    std::transform(distance.begin(), distance.end(), is_inside.begin(),
            [](double m) -> bool {return m > 0;});


    if(std::all_of(is_inside.begin(),is_inside.end(), [](bool a){return a;})) {
        return IntersectionType::Interior;
    } else {
        o=3;
        bool inside = false;
        for(size_t i=0; i < 3; ++i) {
            if(
                    (is_inside[i] && is_inside[(i+1)%3])
                    ||
                    (!is_inside[i] && !is_inside[(i+1)%3] && is_inside[(i+2)%3])
              ) {
                o = 3 - i - ((i+1)%3);
                inside =  is_inside[i];
                break;
            }
        }
        if(o == 3) {
            return IntersectionType::None;
        } else {
            auto&& ov = tm[t[o]];
            bool first = true;
            for(size_t idx: {(o+1)%3,(o+2)%3}) {
                auto&& v = tm[t[idx]];
                double po = distance[o] / (distance[o] - distance[idx]);

                Vector3 intersect = Eigen::ParametrizedLine<double,3>::Through(v,ov).intersectionPoint(m_plane);
                intersect = ov;

                if(first) {
                    p1 = intersect;
                    first = !first;
                } else {
                    p2 = intersect;
                    if(inside)
                        return IntersectionType::Intersect1;
                    else
                        return IntersectionType::Intersect2;
                }
            }
        }

    }
    return IntersectionType::None;
}
