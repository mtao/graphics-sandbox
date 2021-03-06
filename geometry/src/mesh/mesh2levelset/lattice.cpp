#include "lattice.h"
#include "levelset.h"
Lattice::Lattice(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox): m_N(i,j,k) {
    calculateDx();
}


void Lattice::calculateDx() {
    m_dx = m_bbox.sizes().cwiseQuotient(m_N.cast<double>());
}


unsigned int Lattice::ijk2idx(unsigned int i, unsigned int j, unsigned int k) const{
    return NK() * (NJ() * i + j) + k;
}
unsigned int Lattice::ijk2idx(const Eigen::Vector3i & ijk) const{
    return ijk2idx(ijk(0),ijk(1),ijk(2));
}
Eigen::Vector3i Lattice::idx2ijk(unsigned int ind) const{
    unsigned int jk = NJ() * NK();
    Eigen::Vector3i ijk(ind/(jk),(ind%jk)/NK(),ind%NK());
    return ijk;
}

unsigned int Lattice::getIndex(const Eigen::Vector3d &v) const {
    return ijk2idx((v-bbox().min()).cwiseQuotient(dx()).cast<int>());
}

Eigen::Vector3i Lattice::getIJK(const Eigen::Vector3d & v) const{
    return (v-bbox().min()).cwiseQuotient(dx()).cast<int>();
}
Eigen::Vector3d Lattice::getVertex(const Eigen::Vector3i & v) const{
    return v.cast<double>().cwiseProduct(dx()) + bbox().min();
}

Lattice::Lattice(const TriangleMesh & tm, unsigned int nx, unsigned int ny, unsigned int nz, unsigned char dim, int buf): m_N(nx,ny,nz) {
    Eigen::Vector3d slices(nx-2*buf,ny-2*buf,nz-2*buf);
    for(auto&& v: tm.vertices) {
        bbox().extend(v);
    }
    Eigen::Vector3d dx = bbox().sizes().cwiseQuotient(slices);
    bbox().min() -= buf * dx;
    bbox().max() += buf * dx;
    calculateDx();

}
