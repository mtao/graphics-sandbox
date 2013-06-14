#include "rayiterator.h"
RayGenerator::RayGenerator(const Lattice & l, unsigned char dim): l(l), dim(dim) {
    max = l.size() / l.N()(dim);
    origin = l.dx()/2;
    origin(dim) = 0;
    origin += l.bbox().min();
    stride = l.N()((dim+2)%3);

}
RayIterator::RayIterator(const RayGenerator *rg, unsigned int pos): rg(rg), pos(pos) {
    r.direction().setZero();
    r.direction()(rg->dim) = 1;
    r.origin() = rg->origin;
}

RayIterator & RayIterator::operator++() {
    ++pos;
    r.origin() = rg->origin;
    unsigned int d = (rg->dim+1)%3;
    auto&& p = rg->idx2ij(pos);
    r.origin()(d) += p(0) * rg->l.dx()(d);
    d = (d+1)%3;
    r.origin()(d) += p(1) * rg->l.dx()(d);
    return *this;
}
auto RayIterator::value() const -> const Ray & {
    return r;
}


        Eigen::Vector2ui RayIterator::ij() const { return Eigen::Vector2ui(pos/rg->stride,pos%rg->stride);}
bool RayIterator::operator!=(const RayIterator & other) const {
    return this->pos != other.pos || this->rg != other.rg;
}
RayIterator::operator bool() const {
    return pos < rg->max;
}

