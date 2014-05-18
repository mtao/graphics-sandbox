#ifndef EMBEDDED_GRID_H
#define EMBEDDED_GRID_H
#include "grid.h"

template <typename Scalar_, int EmbedDim>
class EmbeddedGrid: public Grid {


    public:
        typedef Grid<Scalar_,EmbedDim> ParentGrid;

        enum {dim = EmbedDim};
        typedef Scalar_ Scalar;
        typedef typename mtao::dim_types<dim> _dt;
        typedef typename _dt::template scalar_types<Scalar>::Vec Vec;
        typedef typename _dt::Veci Veci;
        EmbeddedGrid(const Veci & dim,const Vec & origin,  const Vec & dx): ParentGrid(dim),m_origin(origin),  m_dx(dx) {}
        EmbeddedGrid(const BBox& bb, const Veci& dim): EmbeddedGrid(dim,bb.min(),bb.sizes()/dim.template cast<Scalar>()) {}
        EmbeddedGrid(EmbeddedGrid&& other): ParentGrid(std::move(other)),m_origin(other.m_origin),  m_dx(other.m_dx) {}
        EmbeddedGrid& operator=(EmbeddedGrid&& other) {
            ParentGrid::operator=( std::move(other) );
            m_N=other.m_N;
            m_origin=other.m_origin;
            m_dx=other.m_dx;
            m_data=std::move(other.m_data);
        }
        EmbeddedGrid& operator=(const EmbeddedGrid& other) {
            m_N=other.m_N;
            m_origin=other.m_origin;
            m_dx=other.m_dx;
            m_data=other.m_data;
        }

        //Internal data accessors
        const Vec& dx() const {return m_dx;}
        Scalar dx(int idx) const {return m_dx(idx);}
        const Vec& origin() const {return m_origin;}
        Scalar origin(int idx) const {return m_origin(idx);}
        //protected modifiers
    protected:
        void setOrigin(const Vec& origin) {m_origin = origin;}
        void setDx(const Vec& dx) {m_dx = dx;}
        //Some transformations
        Vec worldToIndex(const Vec & v) const {return (v-origin()).cwiseQuotient(dx());}
        Vec indexToWorld(const Vec & v) const {return v.cwiseProduct(dx()) + origin();}
        Vec indexToWorld(const Veci & v) const {return v.template cast<Scalar>().cwiseProduct(dx()) + origin();}
        Vec indexToWorld(int x,int y) const {return indexToWorld(Veci(x,y));}
        Vec indexToWorld(int x,int y, int z) const {return indexToWorld(Veci(x,y,z));}

        //linear interpolation for a index-space vector
        Scalar ilerp(const Vec & iv) const {
            Veci idx = iv.template cast<int>();
            Vec bary = iv - idx.template cast<Scalar>();
            return m_lerp(bary,idx);
        }
        //linear interpolation for a world-space vector
        Scalar lerp(const Vec & v) const {
            return ilerp(worldToIndex(v));
        }
    private:
        Veci m_N;
        Vec m_origin;
        Vec m_dx;
};

#endif
