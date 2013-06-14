
#include "latticemarcher.h"
LatticeMarcher::LatticeMarcher(const Lattice & l, const Eigen::Vector3ui & origin, unsigned char dim): l(l), dim(dim), origin(origin) {
    max = l.N()(dim);

}
LatticeMarchingIterator::LatticeMarchingIterator(const LatticeMarcher *lm, unsigned int pos): lm(lm), pos(pos) {
    v = lm->origin;
}

LatticeMarchingIterator & LatticeMarchingIterator::operator++() {
    ++pos;
    v = lm->origin;
    v(lm->dim) += pos;
    return *this;
}
auto LatticeMarchingIterator::value() -> const Eigen::Vector3ui & {
    return v;
}
bool LatticeMarchingIterator::operator!=(const LatticeMarchingIterator & other) const {
    return this->pos != other.pos || this->lm != other.lm;
}
LatticeMarchingIterator::operator bool() const {
    return pos < lm->max;
}

