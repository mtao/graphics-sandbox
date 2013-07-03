#ifndef MAC_H
#define MAC_H
#include "grid.h"


template <typename Scalar, int EmbedDim,int FormDim,int WhichForm> 
class MACGrid<Scalar,EmbedDim,FormDim,WhichForm>;
template <typename Scalar>
struct grid_types2d {
    typedef MACGrid<Scalar,2,0,0> NGrid;
    typedef MACGrid<Scalar,2,1,0> UGrid;
    typedef MACGrid<Scalar,2,1,1> VGrid;
    typedef MACGrid<Scalar,2,2,0> CGrid;
};
template <typename Scalar>
struct grid_types3d {
    typedef MACGrid<Scalar,3,0,0> NGrid;
    typedef MACGrid<Scalar,3,1,0> UGrid;
    typedef MACGrid<Scalar,3,1,1> VGrid;
    typedef MACGrid<Scalar,3,1,2> VGrid;
    typedef MACGrid<Scalar,3,2,0> DUGrid;
    typedef MACGrid<Scalar,3,2,1> DVGrid;
    typedef MACGrid<Scalar,3,2,2> DVGrid;
    typedef MACGrid<Scalar,3,3,0> CGrid;
};
//TODO: Make sure grids live in index space

namespace mtao{namespace internal{
template <typename Scalar, int EmbedDim,int FormDim,int WhichForm> 
struct traits<MACGrid<Scalar,EmbedDim,FormDim,WhichForm> > {
    enum {embed_dim = EmbedDim, form_dim = FormDim, which_form = WhichForm};
};
template <typename Scalar> 
struct traits<typename grid_types2d<Scalar>::NGrid > {
    typedef typename grid_types2d<Scalar>::NGrid grid_type;
    enum {embed_dim = grid_type::embed_dim, form_dim = grid_type::form_dim, which_form = grid_type::which_form};
};
template <typename Scalar> 
struct mac_offsets<typename grid_types2d<Scalar>::NGrid > {
    const static Eigen::Matrix<Scalar,2,1> offset(0,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types2d<Scalar>::UGrid > {
    const static Eigen::Matrix<Scalar,2,1> offset(0,0.5);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types2d<Scalar>::VGrid > {
    const static Eigen::Matrix<Scalar,2,1> offset(0.5,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types2d<Scalar>::CGrid > {
    const static Eigen::Matrix<Scalar,2,1> offset(0.5,0.5);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::NGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0,0,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::UGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0,0.5,0.5);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::VGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0.5,0,0.5);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::WGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0.5,0.5,0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::DUGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0.5,0.0,0.0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::DVGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0.0,0.5,0.0);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::DWGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0.0,0.0,0.5);
};
template <typename Scalar> 
struct mac_offsets<typename grid_types3d<Scalar>::CGrid > {
    const static Eigen::Matrix<Scalar,3,1> offset(0.5,0.5,0.5);
};

}}
template <typename Scalar_, int EmbedDim,int FormDim,int WhichForm> 
class MACGrid<Scalar,EmbedDim,FormDim,WhichForm>: public GridBase<MACGrid<Scalar_,EmbedDim,FormDim,WhichForm> > {
    public:
    typedef Scalar_ Scalar;
    enum {embed_dim = EmbedDim, form_dim = FormDim, which_form = WhichForm};
    typedef GridBase<MACGrid<Scalar,EmbedDim,FormDim,WhichForm> > Base;
    typedef typename Base::Vec Vec;
    typedef typename Base::Veci Veci;
    protected:
    using Base::Base;
};
iendif
