#ifndef RAYITERATOR_H
#define RAYITERATOR_H
#include "lattice.h"
#include <Eigen/Geometry>
class LatticeMarcher;
class LatticeMarchingIterator{
    public:
        LatticeMarchingIterator(const LatticeMarcher * lm, unsigned int pos=0);
        LatticeMarchingIterator & operator++();
        const Eigen::Vector3ui & value();
        bool operator!=(const LatticeMarchingIterator & other) const;
        LatticeMarchingIterator * operator*() {return this;}
        operator bool() const;
    private:
        const LatticeMarcher * lm;
        unsigned int pos;
        Eigen::Vector3ui v;

};
class LatticeMarcher{
    public:
        friend class LatticeMarchingIterator;
        LatticeMarcher(const Lattice & l, const Eigen::Vector3ui & origin, unsigned char dim);
        LatticeMarchingIterator begin(){return LatticeMarchingIterator(this,0);}
        LatticeMarchingIterator end(){return LatticeMarchingIterator(this,max);}
    protected:
        const Lattice & l;
        unsigned char dim;
        unsigned int max;
        Eigen::Vector3ui origin;
};
#endif
