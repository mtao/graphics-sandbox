#include "lattice.h"

Lattice::Lattice(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox): m_N(i,j,k) {
    calculateDx();
}


void Lattice::calculateDx() {
    m_dx = m_bbox.sizes().cwiseQuotient(m_N.cast<double>());
}


unsigned int Lattice::ijk2idx(unsigned int i, unsigned int j, unsigned int k) const{
    return NK() * (NJ() * i + j) + k;
}
unsigned int Lattice::ijk2idx(const Eigen::Vector3ui & ijk) const{
    return ijk2idx(ijk(0),ijk(1),ijk(2));
}
Eigen::Vector3ui Lattice::idx2ijk(unsigned int ind) const{
    unsigned int jk = NJ() * NK();
    Eigen::Vector3ui ijk(ind/(jk),(ind%jk)/NK(),ind%NK());
    return ijk;
}

unsigned int Lattice::getIndex(const Eigen::Vector3d &v) const {
    return ijk2idx((v-bbox().min()).cwiseQuotient(dx()).cast<unsigned int>());
}

Eigen::Vector3ui Lattice::getIJK(const Eigen::Vector3d & v) const{
    return (v-bbox().min()).cwiseQuotient(dx()).cast<unsigned int>();
}
