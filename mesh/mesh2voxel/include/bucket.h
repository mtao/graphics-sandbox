#ifndef BUCKET_H
#define BUCKET_H
#include "trianglemesh.h"
#include "lattice.h"
#include "rayiterator.h"
#include <array>

//A very fragile trianglemesh bucket-er
class Bucket{
    public:
        Bucket(const TriangleMesh & tm, Lattice & lattice, unsigned char dim=0);

        void getVoxels();

        
    protected:
        const TriangleMesh & tm;
        Lattice & l;
        const unsigned char dim;
        RayGenerator rg;
        const unsigned char I;
        const unsigned char J;
        std::vector<std::set<unsigned int> > triangles;
    private:

};

#endif
