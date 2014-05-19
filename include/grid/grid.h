#ifndef GRID_H
#define GRID_H
#include "types.h"
#include "traits.h"
#include "lerp.h"
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
    typedef typename mtao::scalar_types<Scalar> _st;
    typedef typename _dt::Veci Veci;
    typedef typename Eigen::Map<Eigen::Matrix<Scalar,Eigen::Dynamic,1> > MapVec;
    typedef typename Eigen::Map<const Eigen::Matrix<Scalar,Eigen::Dynamic,1> > ConstMapVec;
    typedef std::shared_ptr<Grid<Scalar,dim> > ptr;
    typedef std::weak_ptr<Grid<Scalar,dim> > weak_ptr;


    public:

    typedef Scalar value_type;
    int size() const {return stlVector().size();}
    const Veci& N() const {return m_N;}
    int N(int idx) const {return m_N(idx);}
    void reshape(const Veci& N) {m_N = N; m_data.resize(N.prod());}


    //    //Derived handlers
    //    Derived& derived(){return *static_cast<Derived*>(this);}
    //    const Derived & derived()const {return *static_cast<const Derived*>(this);}



    public:
    //There are several nice ways to access the data...
    std::vector<Scalar> & stlVector(){return m_data;}
    const std::vector<Scalar> & stlVector()const{return m_data;}
    Scalar * data() {return m_data.data();}
    const Scalar * data() const {return m_data.data();}
    MapVec mapVector(){return MapVec(data());}
    ConstMapVec mapVector()const{return ConstMapVec(data());}

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

        Scalar operator()(const mtao::Vec1i& idx) const {
            return coeff(idx[0]);
        }
        Scalar operator()(const mtao::Vec2i& idx) const {
            return coeff(idx[0],idx[1]);
        }
        Scalar operator()(const mtao::Vec3i& idx) const {
            return coeff(idx[0],idx[1],idx[2]);
        }


    //Iterators
    typename std::vector<Scalar>::iterator begin() {return m_data.begin();}
    typename std::vector<Scalar>::iterator end() {return m_data.end();}
    typename std::vector<Scalar>::const_iterator cbegin() {return m_data.cbegin();}
    typename std::vector<Scalar>::const_iterator cend() {return m_data.cend();}

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
    

    //Interpolation tools...
    //
    /*
    value_type lerp(const typename _st::template dim_types<1>::Vec& v) const {
        _st::template dim_types<T>::Veci idx = iv.template cast<int>();
        _st::template dim_types<T>::Vec bary = iv - idx.template cast<Scalar>();
        const Grid& g = *this;
        const typename GridType::value_type& x=v(0);
        const int& a=idx(0);
        return lerp(g(a),g(a+1),x);
    }
    value_type lerp(const _st::template dim_types<2>::Vec& v) const {
        const Grid& g = *this;
        const typename GridType::value_type& x=v(0);
        const typename GridType::value_type& y=v(1);
        const int& a=idx(0);
        const int& b=idx(1);
        return bilerp(g(a,b),g(a+1,b),g(a,b+1),g(a+1,b+1),x,y);
    }
    value_type lerp(const _st::template dim_types<3>::Vec& v) const {
        const Grid& g = *this;
        const typename GridType::value_type& x=v(0);
        const typename GridType::value_type& y=v(1);
        const typename GridType::value_type& z=v(2);
        const int& a=idx(0);
        const int& b=idx(1);
        const int& c=idx(2);
        return trilerp(
                g(a,b,c),g(a+1,b,c),g(a,b+1,c),g(a+1,b+1,c),
                g(a,b,c+1),g(a+1,b,c+1),g(a,b+1,c+1),g(a+1,b+1,c+1)
                ,x,y,z);
    }
    */

    private:
        Veci m_N;
    std::vector<Scalar> m_data;


};
}

#endif