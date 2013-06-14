#ifndef BUCKET_H
#define BUCKET_H
#include "trianglemesh.h"
#include "lattice.h"
#include "rayiterator.h"
#include <array>

//A very fragile trianglemesh bucket-er
class Bucket{
    public:
        Bucket(const TriangleMesh & tm, const Lattice & lattice, unsigned char dim=0);

        std::set<std::array<unsigned int, 3> > && getVoxels() const;

        
//        RayIterator getRayIterator(unsigned char dim) { return RayIterator(l,dim);}
    protected:
        const TriangleMesh & tm;
        const Lattice & l;
        const unsigned char dim;
        RayGenerator rg;
        const unsigned char I;
        const unsigned char J;
        std::vector<std::set<unsigned int> > triangles;
    private:

};

#endif
