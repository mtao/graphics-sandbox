#ifndef GRID_H
#define GRID_H
#include "types.h"
#include "traits.h"

template <typename Derived>
class GridBase{
    enum {dim = Derived::embed_dim};
    typedef mtao::internal::traits<Derived> derived_traits;
    typedef typename derived_traits::Scalar Scalar;
    typedef typename mtao::dim_types<dim> _dt;
    typedef typename _dt::Veci Veci;
    typedef typename _dt::template scalar_types<Scalar>::Vec Vec;
    typedef typename Eigen::Map<Eigen::Matrix<Scalar,Eigen::Dynamic,1> > MapVec;
    typedef typename Eigen::Map<const Eigen::Matrix<Scalar,Eigen::Dynamic,1> > ConstMapVec;
    protected:
    GridBase(const Veci & dim,const Vec & origin,  const Vec & dx): m_N(dim),m_origin(origin),  m_dx(dx), m_data(N(0)*N(1)*N(2)) {}

    int size() const {return data().size();}
    

    //Derived handlers
    Derived& derived(){return *static_cast<Derived*>(this);}
    const Derived & derived()const {return *static_cast<const Derived*>(this);}

    //Internal data accessors
    int N() const {return m_N;}
    int N(int idx) const {return m_N(idx);}
    int dx() const {return m_dx;}
    int dx(int idx) const {return m_dx(idx);}
    int origin() const {return m_origin;}
    int origin(int idx) const {return m_origin(idx);}

    //There are several nice ways to access the data...
    std::vector<Scalar> & stlVector(){return m_data;}
    const std::vector<Scalar> & stlVector()const{return m_data;}
    Scalar * data() {return m_data.data();}
    const Scalar * data() const {return m_data.data();}
    MapVec mapVector(){return MapVec(data());}
    ConstMapVec mapVector()const{return ConstMapVec(data());}

    //Some transformations
    Vec worldToIndex(const Vec & v) {return (v-origin()).cwiseQuotient(dx());}
    Vec indexToWorld(const Vec & v) {return v.cwiseProduct(dx()) + origin();}
    Vec indexToWorld(const Veci & v) {return v.template cast<Scalar>().cwiseProduct(dx()) + origin();}

    private:
    Veci m_N;
    Vec m_origin;
    Vec m_dx;
    std::vector<Scalar> m_data;

        
};

#endif
