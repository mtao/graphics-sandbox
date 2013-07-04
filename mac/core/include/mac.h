#ifndef MAC_H
#define MAC_H
#include "grid.h"
#include "traits.h"


template <typename Scalar, int EmbedDim,int FormDim,int WhichForm> 
class MACGrid;
template <typename Scalar, int D>
struct grid_types{};
template <typename Scalar>
struct grid_types<Scalar,2>{
    typedef MACGrid<Scalar,2,0,0> NGrid;
    typedef MACGrid<Scalar,2,1,0> UGrid;
    typedef MACGrid<Scalar,2,1,1> VGrid;
    typedef MACGrid<Scalar,2,2,0> CGrid;
};
template <typename Scalar>
struct grid_types<Scalar,3> {
    typedef MACGrid<Scalar,3,0,0> NGrid;
    typedef MACGrid<Scalar,3,1,0> UGrid;
    typedef MACGrid<Scalar,3,1,1> VGrid;
    typedef MACGrid<Scalar,3,1,2> WGrid;
    typedef MACGrid<Scalar,3,2,0> DUGrid;
    typedef MACGrid<Scalar,3,2,1> DVGrid;
    typedef MACGrid<Scalar,3,2,2> DWGrid;
    typedef MACGrid<Scalar,3,3,0> CGrid;
};
//TODO: Make sure grids live in index space

namespace mtao{namespace internal{
template <typename Scalar_, int EmbedDim,int FormDim,int WhichForm> 
struct traits<MACGrid<Scalar_,EmbedDim,FormDim,WhichForm> > {
    enum {embed_dim = EmbedDim, form_dim = FormDim, which_form = WhichForm};
    typedef Scalar_ Scalar;
};
/*
template <typename Scalar> 
struct traits<typename grid_types<Scalar,2>::NGrid > {
    typedef typename grid_types<Scalar,2>::NGrid grid_type;
    enum {embed_dim = grid_type::embed_dim, form_dim = grid_type::form_dim, which_form = grid_type::which_form};
};
*/
template <typename GridType>
struct mac_offsets {
};
/*
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,2>::NGrid > {
    constexpr static Eigen::Matrix<Scalar,2,1> offset(0,0);
    constexpr static Eigen::Matrix<int,2,1> added_cells(1,1);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,2>::UGrid > {
    constexpr static Eigen::Matrix<Scalar,2,1> offset(0,0.5);
    constexpr static Eigen::Matrix<int,2,1> added_cells(1,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,2>::VGrid > {
    constexpr static Eigen::Matrix<Scalar,2,1> offset(0.5,0);
    constexpr static Eigen::Matrix<int,2,1> added_cells(0,1);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,2>::CGrid > {
    constexpr static Eigen::Matrix<Scalar,2,1> offset(0.5,0.5);
    constexpr static Eigen::Matrix<int,2,1> added_cells(0,0);
};
*/
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,0,0> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0,0,0);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(1,1,1);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,1,0> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0,0.5,0.5);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(1,0,0);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,1,1> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0.5,0.0,0.5);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(0,1,0);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,1,2> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0.5,0.5,0.0);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(0,0,1);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,3,0> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(.5,.5,.5);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(0,0,0);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,2,0> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0.5,0.0,0.0);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(0,1,1);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,2,1> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0.0,0.5,0.0);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(1,0,1);}
};
template <typename Scalar> 
struct mac_offsets<MACGrid<Scalar,3,2,2> > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset() {return Eigen::Matrix<Scalar,3,1>(0.0,0.0,0.5);}
    constexpr static Eigen::Matrix<int,3,1> added_cells() {return Eigen::Matrix<int,3,1>(1,1,0);}
};
/*
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,3>::VGrid > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset(0.5,0,0.5);
    constexpr static Eigen::Matrix<int,3,1> added_cells(0,1,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,3>::WGrid > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset(0.5,0.5,0);
    constexpr static Eigen::Matrix<int,3,1> added_cells(0,0,1);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,3>::DUGrid > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset(0.5,0.0,0.0);
    constexpr static Eigen::Matrix<int,3,1> added_cells(0,1,1);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,3>::DVGrid > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset(0.0,0.5,0.0);
    constexpr static Eigen::Matrix<int,3,1> added_cells(1,0,1);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,3>::DWGrid > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset(0.0,0.0,0.5);
    constexpr static Eigen::Matrix<int,3,1> added_cells(1,1,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types<Scalar,3>::CGrid > {
    constexpr static Eigen::Matrix<Scalar,3,1> offset(0.5,0.5,0.5);
    constexpr static Eigen::Matrix<int,3,1> added_cells(0,0,0);
};
template <typename Scalar>
struct mac_offsets< Eigen::Matrix<Scalar,3,1> > {
    typedef float merg;
};

typedef mac_offsets<Eigen::Vector3f>::merg bert;
typedef mac_offsets<Eigen::Vector3d>::merg berg;
*/
}}
template <typename Scalar, int EmbedDim> 
struct MACGridFactory;
template <typename Scalar_, int EmbedDim,int FormDim,int WhichForm> 
class MACGrid: public GridBase<MACGrid<Scalar_,EmbedDim,FormDim,WhichForm> > {
    typedef MACGrid<Scalar_,EmbedDim,FormDim,WhichForm> MyType;
    public:
    //Make template parameters externally available
    typedef Scalar_ Scalar;
    enum {embed_dim = EmbedDim, form_dim = FormDim, which_form = WhichForm};
    //Some helpful typedefs, not necessarily for external use
    typedef GridBase<MACGrid<Scalar,EmbedDim,FormDim,WhichForm> > Base;
    typedef typename mtao::dim_types<EmbedDim>::template scalar_types<Scalar>::Vec Vec;
    typedef typename mtao::dim_types<EmbedDim>::Veci Veci;

    protected:
    friend class MACGridFactory<Scalar,EmbedDim>;
    MACGrid(const Veci & dim,const Vec & origin,  const Vec & dx): Base(dim,origin,dx) {}
    //using Base::Base;
};
#endif
