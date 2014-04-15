#include "levelset.h"
#include "meta.h"
#include <limits>


Levelset::Levelset(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox): Lattice(i,j,k,bbox), m_data(i*j*k,std::numeric_limits<double>::max()), m_active(m_data.size(),false) {}

void Levelset::unionInPlace(const Levelset & other) {
    append(*this,other,[](double & a, const double & b) {
            a = std::min(a,b);
            });
    appendV(this->m_active,other.m_active,[](bool a, const bool b)->bool {
            return a || b;
            });
}
void Levelset::intersectInPlace(const Levelset & other) {
    append(*this,other,[](double & a, const double & b) {
            a = std::max(a,b);
            });

    appendV(this->m_active,other.m_active,[](bool a, const bool b)->bool {
            return a || b;
            });
}
void Levelset::unionInPlace(const TriangleMesh & other) {
    for(auto&& t: other.triangles) {
        signedTriangleUnion(other,t);
    }
}
//void Levelset::intersectInPlace(const TriangleMesh & other);
void Levelset::signedTriangleUnion(const TriangleMesh & tm, const Triangle & t) {
    std::function<const Vector3 &(int)> v = [&](int i)->const Vector3 & {
        return tm.vertices[t[i]];
    };
    auto&& v0 = v(0);
    auto&& v1 = v(1);
    auto&& v2 = v(2);
    auto&& e10 = v1 - v0;
    auto&& e20 = v2 - v0;
    Vector3::Index m1,m2;

    int n1 = int(e10.cwiseQuotient(dx()).maxCoeff(&m1));
    int n2 = int(e20.cwiseQuotient(dx()).maxCoeff(&m2));
    int b1 = 2;
    int b2 = 2;
    int other;
    int other2;
    if(m1 == m2) {
        b2 = 0;
        other = (m1+1)%3;
        other2 = (m1+2)%3;
    } else {
        other = 5 - m1 - m2;
    }

    Eigen::Hyperplane<double,3> plane(v0,e10.cross(e20));//TODO: Check orientation
    for(int i=-b1; i < n1+b1; ++i) {
        double c1 = double(i)/n1;
        for(int j=-b2; j < n2+b2; ++j) {
            double c2 = double(j)/n2;
            Eigen::Vector3i nearest = (c1 * e10 + c2 * e20).cast<int>();
            Eigen::Vector3i ijk;
            if(m1 == m2) {
                int& a=ijk(m1);
                int& b=ijk(other);
                int& c=ijk(other2);
                for(b=nearest(other)-2; b < nearest(other)+2; ++b) {
                    for(c=nearest(other2)-2; c < nearest(other2)+2; ++c) {
                        //TODO: do shit
                    }
                }

            } else {
                int& a=ijk(m1);
                int& b=ijk(m2);
                int& c=ijk(other2);
                for(c=nearest(other2)-2; c < nearest(other2)+2; ++c) {
                    distanceToTriangle(v);
                    auto&& p = getVertex(ijk);
                    double dToPlane = plane.signedDistance(p);
                    //TODO: do shit
                }
            }
        }
    }
    for(int i=0; i < 3; ++i) {
        auto&& v0 = v(i);
        auto&& v1 = v((i+1)%3);
        pointIntersection(plane,v0);
        edgeIntersection(plane,v0,v1);
    }

}

void Levelset::pointIntersection(const Eigen::Hyperplane<double,3> & hp, const Vector3 &v) {
    const Eigen::Vector3i band(2,2,2);
    auto p = getIJK(v);
    foreachLevelset(*this,p-band,p+band,[&](const Eigen::Vector3i& p, double & v) {
            /*
               if(hp.  
               v = p-
               */
    });
}
void Levelset::edgeIntersection( const Eigen::Hyperplane<double,3> & hp, const Vector3 &, const Vector3 &) {
}

