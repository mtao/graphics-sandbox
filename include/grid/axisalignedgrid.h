#ifndef EMBEDDED_GRID_H
#define EMBEDDED_GRID_H
#include "grid.h"

namespace mtao {
template <typename Scalar_, int Dim>
class AxisAlignedGrid: public Grid<Scalar_,Dim> {


    public:
        typedef Grid<Scalar_,Dim> ParentGrid;

//        enum {embed_dim = EmbedDim};
        typedef Scalar_ Scalar;
        typedef typename mtao::dim_types<Dim> _dt;
        typedef typename _dt::template scalar_types<Scalar>::Vec Vec;
        typedef typename _dt::template scalar_types<Scalar>::BBox BBox;
        typedef typename _dt::Veci Veci;
        AxisAlignedGrid(const Veci & dim,const Vec & origin,  const Vec & dx): ParentGrid(dim),m_origin(origin),  m_dx(dx) {}
        AxisAlignedGrid(const BBox& bb, const Veci& dim): AxisAlignedGrid(dim,bb.min(),bb.sizes()/dim.template cast<Scalar>()) {}
        AxisAlignedGrid(AxisAlignedGrid&& other): ParentGrid(std::move(other)),m_origin(other.m_origin),  m_dx(other.m_dx) {}
        AxisAlignedGrid(const AxisAlignedGrid& other) = default;
        AxisAlignedGrid& operator=(AxisAlignedGrid&& other) {
            ParentGrid::operator=( std::move(other) );
            m_origin=other.m_origin;
            m_dx=other.m_dx;
        }
        AxisAlignedGrid& operator=(const AxisAlignedGrid& other) = default;

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
