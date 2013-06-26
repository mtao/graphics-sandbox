#include "levelset.h"
#include "meta.h"
#include <limits>


Levelset::Levelset(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox): Lattice(i,j,k,bbox), m_data(i*j*k,std::numeric_limits<double>::max()) {}

void Levelset::unionInPlace(const Levelset & other) {
    append(*this,other,[](double & a, const double & b) {
            a = std::min(a,b);
            });
}
void Levelset::intersectInPlace(const Levelset & other) {
    append(*this,other,[](double & a, const double & b) {
            a = std::max(a,b);
            });
}
void Levelset::unionInPlace(const TriangleMesh & other) {
}
//void Levelset::intersectInPlace(const TriangleMesh & other);
