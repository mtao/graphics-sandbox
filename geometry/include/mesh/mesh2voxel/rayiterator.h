#ifndef RAYITERATOR_H
#define RAYITERATOR_H
#include "lattice.h"
#include <Eigen/Geometry>
class RayGenerator;
class RayIterator{
    public:
        typedef Eigen::ParametrizedLine<double,3> Ray;
        RayIterator(const RayGenerator * rg, unsigned int pos=0);
        RayIterator & operator++();
        const Ray & value()const ;
        unsigned int p()const {return pos;}
        Eigen::Vector2ui ij() const ;
        bool operator!=(const RayIterator & other) const;
        RayIterator & operator*() {return *this;}
        operator bool() const;
    private:
        const RayGenerator * rg;
        unsigned int pos;
        Ray r;

};
class RayGenerator{
    public:
        friend class RayIterator;
        RayGenerator(const Lattice & l, unsigned char dim);
        RayIterator begin() const {return RayIterator(this,0);}
        RayIterator end() const {return RayIterator(this,max);}
        unsigned int ij2idx(unsigned int i, unsigned int j) const{
            return l.N()((dim+2)%3) *  i + j;
        }
        unsigned int ij2idx(const Eigen::Vector2ui & v) const{
            return ij2idx(v(0),v(1));
        }
        Eigen::Vector2ui idx2ij(unsigned int idx) const{
            Eigen::Vector2ui v;
            const unsigned int  st = l.N()((dim+2)%3);
            v(0) = idx/st;
            v(1) = idx%st;
            return v;
            
        }
    protected:
        const Lattice & l;
        unsigned char dim;
        unsigned int max;
        unsigned int stride;
        Eigen::Vector3d origin;
};
#endif
