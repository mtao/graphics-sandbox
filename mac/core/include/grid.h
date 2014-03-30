#ifndef GRID_H
#define GRID_H
#include "types.h"
#include "traits.h"
#include "lerp.h"
#include "bboxiterator.h"
#include <vector>
#include <memory>

namespace mtao {

template <typename Scalar_, int EmbedDim>
class Grid{
//    enum {dim = mtao::internal::traits<Derived>::embed_dim};
    enum {dim = EmbedDim};
//    typedef typename mtao::internal::traits<Derived> derived_traits;
//    typedef typename derived_traits::Scalar Scalar;
    typedef Scalar_ Scalar;
    typedef typename mtao::dim_types<dim> _dt;
    public:
    typedef typename _dt::template scalar_types<Scalar>::Vec Vec;
    typedef typename _dt::template scalar_types<Scalar>::BBox BBox;
    typedef typename _dt::Veci Veci;
    typedef IterableBBox<EmbedDim> ItBBoxi;
    typedef typename _dt::BBoxi BBoxi;
    typedef typename Eigen::Map<Eigen::Matrix<Scalar,Eigen::Dynamic,1> > MapVec;
    typedef typename Eigen::Map<const Eigen::Matrix<Scalar,Eigen::Dynamic,1> > ConstMapVec;
    typedef std::shared_ptr<Grid<Scalar,dim> > ptr;
    typedef std::weak_ptr<Grid<Scalar,dim> > weak_ptr;

    Grid(): m_lerp(*this) {}
    Grid(const Veci & dim,const Vec & origin,  const Vec & dx): m_N(dim),m_origin(origin),  m_dx(dx), m_data(m_N.prod()), m_lerp(*this) {}
    Grid(const Grid& other): m_N(other.m_N),m_origin(other.m_origin),  m_dx(other.m_dx), m_data(other.m_data), m_lerp(*this) {}
    Grid(Grid&& other): m_N(other.m_N),m_origin(other.m_origin),  m_dx(other.m_dx), m_data(std::move(other.m_data)), m_lerp(*this) {}
    Grid& operator=(Grid&& other) {
        m_N=other.m_N;
        m_origin=other.m_origin;
        m_dx=other.m_dx;
        m_data=std::move(other.m_data);
    }
    Grid& operator=(const Grid& other) {
        m_N=other.m_N;
        m_origin=other.m_origin;
        m_dx=other.m_dx;
        m_data=other.m_data;
    }
    public:

    typedef Scalar value_type;
    int size() const {return stlVector().size();}


//    //Derived handlers
//    Derived& derived(){return *static_cast<Derived*>(this);}
//    const Derived & derived()const {return *static_cast<const Derived*>(this);}

    //Internal data accessors
    const Veci& N() const {return m_N;}
    int N(int idx) const {return m_N(idx);}
    const Vec& dx() const {return m_dx;}
    Scalar dx(int idx) const {return m_dx(idx);}
    const Vec& origin() const {return m_origin;}
    Scalar origin(int idx) const {return m_origin(idx);}
    Vec center() const {return m_origin.array() + m_N.template cast<float>().array() * m_dx.array() / 2.0;}
    //modifiers
    void resize(const Veci& N) {m_N = N; m_data.resize(m_N.prod());}
    void setOrigin(const Vec& origin) {m_origin = origin;}
    void setDx(const Vec& dx) {m_dx = dx;}
    void reset(const Grid& other) {
        setOrigin(other.origin());
        setDx(other.dx());
        resize(other.N());
    }
    protected:
    

    public:
    //There are several nice ways to access the data...
    std::vector<Scalar> & stlVector(){return m_data;}
    const std::vector<Scalar> & stlVector()const{return m_data;}
    Scalar * data() {return m_data.data();}
    const Scalar * data() const {return m_data.data();}
    MapVec mapVector(){return MapVec(data());}
    ConstMapVec mapVector()const{return ConstMapVec(data());}

    //Some transformations
    Vec worldToIndex(const Vec & v) const {return (v-origin()).cwiseQuotient(dx());}
    Vec indexToWorld(const Vec & v) const {return v.cwiseProduct(dx()) + origin();}
    Vec indexToWorld(const Veci & v) const {return v.template cast<Scalar>().cwiseProduct(dx()) + origin();}

    Vec indexToWorld(int x,int y) const {return indexToWorld(Veci(x,y));}
    Vec indexToWorld(int x,int y, int z) const {return indexToWorld(Veci(x,y,z));}
    
    BBox worldToIndex(const BBox & bb) const {return BBox(worldToIndex(bb.min()),worldToIndex(bb.max()));}
    BBox indexToWorld(const BBox & bb) const {return BBox(indexToWorld(bb.min()),indexToWorld(bb.max()));}
    BBox indexToWorld(const BBoxi & bb) const {return BBox(indexToWorld(bb.min()),indexToWorld(bb.max()));}

    template <int N>
    typename mtao::template dim_types<N>::template scalar_types<Scalar>::Vec indexToWorldSlice(const typename mtao::template dim_types<N>::Veci & v) const {
        return v.template cast<Scalar>().cwiseProduct(dx().template topRows<N>()) + origin().template topRows<N>();
        }

    //BBox stuff
    BBox bbox() const {
        return BBox(origin(), origin() + m_N.template cast<Scalar>().cwiseProduct(dx()));
    }
    ItBBoxi bboxi() const {
        return ItBBoxi(Veci::Zeros(), N());
    }
    BBox clampBB(const BBox& bb) const {
        return bbox().intersect(bb);
    }
    ItBBoxi clampBBi(const BBoxi& bb) const {
        return bboxi().intersect(bb);
    }
    ItBBoxi clampBBi(const ItBBoxi& bb) const {
        return bboxi().intersect(bb);
    }

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
    //Maps from 3d to 2d elements
    int coord2idx(int a) const{
        return a;
    }
    int coord2idx(int a, int b) const{
        return m_N(1) * a + b;
    }
    int coord2idx(int a, int b, int c)const {
        return m_N(2) * (m_N(1) * a + b) + c;
    }
    int coord2idx(const Veci& c)const {
        int idx=c(0);
        for(int i=1; i <EmbedDim; ++i) {
            idx = m_N(i) * idx + c(i);
        }
        return idx;
    }
    void idx2coord2(int idx, mtao::dim_types<2>::Veci & c) {
        c(0) = idx%m_N(2);
        c(1) = idx/m_N(2);
    }
    void idx2coord3(int idx, mtao::dim_types<3>::Veci& c) {
        c(2) = idx%m_N(1);
        idx /= m_N(1);
        c(1) = idx%m_N(2);
        c(0) = idx/m_N(2);
    }
    //Accessors
    Scalar& coeffRef(int a) {return m_data[a];}
    Scalar& coeffRef(int a, int b) {return m_data[coord2idx(a,b)];}
    Scalar& coeffRef(int a, int b, int c) {return m_data[coord2idx(a,b,c)];}
    Scalar coeff(int a)const {return m_data[a];}
    Scalar coeff(int a, int b) const{return m_data[coord2idx(a,b)];}
    Scalar coeff(int a, int b, int c)const {return m_data[coord2idx(a,b,c)];}
    Scalar& operator()(int a) {return coeffRef(a);}
    Scalar& operator()(int a, int b) {return coeffRef(a,b);}
    Scalar& operator()(int a, int b, int c) {return coeffRef(a,b,c);}
    Scalar operator()(int a)const {return coeff(a);}
    Scalar operator()(int a, int b) const {return coeff(a,b);}
    Scalar operator()(int a, int b, int c)const {return coeff(a,b,c);}
    //Iterators
    typename std::vector<Scalar>::iterator begin() {return m_data.begin();}
    typename std::vector<Scalar>::iterator end() {return m_data.end();}
    typename std::vector<Scalar>::const_iterator begin() const{return m_data.begin();}
    typename std::vector<Scalar>::const_iterator end() const{return m_data.end();}
    typename std::vector<Scalar>::const_iterator cbegin() const{return m_data.cbegin();}
    typename std::vector<Scalar>::const_iterator cend() const{return m_data.cend();}

    void incrementLoop(Veci& c)const {
        for(int i = EmbedDim-1; i >= 0; --i) {
            if(++c(i) >= m_N(i)) {
                c(i)=0;
                continue;
            } else {
                return;
            }
        }
    }
    //Looping
    //Provides the index-space coordinate of the vector and the current value in the grid
    //Expects a value returned for writing
    Grid& loop(const std::function<Scalar(const Veci&, Scalar)>& f) {
        Veci coord(Veci::Zero());
        for(Scalar& v: m_data) {
            v=f(coord,v);
            incrementLoop(coord);
        }
        return *this;
    }
    //Provides the index-space coordinate of the vector and the current value in the grid
    //Doesn't expect any modification of the grid
    void loop(const std::function<void(const Veci&, Scalar)>& f)const {
        Veci coord(Veci::Zero());
        for(Scalar v: m_data) {
            f(coord,v);
            incrementLoop(coord);
        }
    }
    //Provides a value-by-value evaluation
    Grid& loop(const std::function<Scalar(Scalar)>& f) {
        std::transform(m_data.begin(),m_data.end(),m_data.begin(), f);
        return *this;
    }

    private:
    Veci m_N;
    Vec m_origin;
    Vec m_dx;
    std::vector<Scalar> m_data;
    const typename mtao::lerp::template LinearInterpolator<Vec::RowsAtCompileTime, Grid<Scalar,EmbedDim> > m_lerp;


    };


}
typedef mtao::Grid<float,3> Grid3f;
typedef mtao::Grid<double,3> Grid3d;
#endif
