#ifndef EMBEDDED_GRID_H
#define EMBEDDED_GRID_H
#include "grid.h"

namespace mtao {
template <typename Scalar_, int Dim, int EmbedDim=Dim>
class EmbeddedGrid: public Grid<Scalar_,Dim> {


    public:
        typedef Grid<Scalar_,Dim> ParentGrid;

        enum {embed_dim = EmbedDim};
        typedef Scalar_ Scalar;
        typedef typename mtao::dim_types<embed_dim> _dt;
        typedef typename _dt::template scalar_types<Scalar>::Vec Vec;
        typedef typename _dt::template scalar_types<Scalar>::BBox BBox;
        typedef typename _dt::Veci Veci;
        EmbeddedGrid(const Veci & dim,const Vec & origin,  const Vec & dx): ParentGrid(dim),m_origin(origin),  m_dx(dx) {}
        EmbeddedGrid(const BBox& bb, const Veci& dim): EmbeddedGrid(dim,bb.min(),bb.sizes()/dim.template cast<Scalar>()) {}
        EmbeddedGrid(EmbeddedGrid&& other): ParentGrid(std::move(other)),m_origin(other.m_origin),  m_dx(other.m_dx) {}
        EmbeddedGrid(const EmbeddedGrid& other) = default;
        EmbeddedGrid& operator=(EmbeddedGrid&& other) {
            ParentGrid::operator=( std::move(other) );
            m_origin=other.m_origin;
            m_dx=other.m_dx;
        }
        EmbeddedGrid& operator=(const EmbeddedGrid& other) = default;

        //Internal data accessors
        const Vec& dx() const {return m_dx;}
        Scalar dx(int idx) const {return m_dx(idx);}
        const Vec& origin() const {return m_origin;}
        Scalar origin(int idx) const {return m_origin(idx);}
        //protected modifiers
        void setOrigin(const Vec& origin) {m_origin = origin;}
        void setDx(const Vec& dx) {m_dx = dx;}
        //Some transformations
        Vec worldToIndex(const Vec & v) const {return (v-origin()).cwiseQuotient(dx());}
        Vec indexToWorld(const Vec & v) const {return v.cwiseProduct(dx()) + origin();}
        Vec indexToWorld(const Veci & v) const {return v.template cast<Scalar>().cwiseProduct(dx()) + origin();}
        Vec indexToWorld(int x,int y) const {return indexToWorld(Veci(x,y));}
        Vec indexToWorld(int x,int y, int z) const {return indexToWorld(Veci(x,y,z));}

        //linear interpolation for a world-space vector
        Scalar wlerp(const Vec & v) const {
            return ParentGrid::lerp(worldToIndex(v));
        }
    private:
        Veci m_N;
        Vec m_origin;
        Vec m_dx;
};

}
#endif
