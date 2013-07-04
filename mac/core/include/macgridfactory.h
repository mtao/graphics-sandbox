#ifndef MACGRIDFACTORY_H
#define MACGRIDFACTORY_H

#include "mac.h"
#include <Eigen/Geometry>

//To make things easier we provide an enum to declare grid types, and then crud to map the 
//enum to the appropriate grid types
namespace mtao{ 
    enum GridEnum {NGrid,UGrid,VGrid,WGrid,DUGrid,DVGrid,DWGrid,CGrid};
    namespace internal {

template <typename Scalar, int EmbedDim, GridEnum Enum>
struct GridSelector {};

//2D
template <typename Scalar>
struct GridSelector<Scalar,2,NGrid> {typedef typename grid_types<Scalar,2>::NGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,2,UGrid> {typedef typename grid_types<Scalar,2>::UGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,2,VGrid> {typedef typename grid_types<Scalar,2>::VGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,2,CGrid> {typedef typename grid_types<Scalar,2>::CGrid type;};

//3D
template <typename Scalar>
struct GridSelector<Scalar,3,NGrid> {typedef typename grid_types<Scalar,3>::NGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,UGrid> {typedef typename grid_types<Scalar,3>::UGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,VGrid> {typedef typename grid_types<Scalar,3>::VGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,WGrid> {typedef typename grid_types<Scalar,3>::WGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,DUGrid> {typedef typename grid_types<Scalar,3>::DUGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,DVGrid> {typedef typename grid_types<Scalar,3>::DVGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,DWGrid> {typedef typename grid_types<Scalar,3>::DWGrid type;};
template <typename Scalar>
struct GridSelector<Scalar,3,CGrid> {typedef typename grid_types<Scalar,3>::CGrid type;};
}}


template <typename Scalar, int EmbedDim> 
struct MACGridFactory {
    template <mtao::GridEnum GridType>
        struct selector {
            typedef typename mtao::internal::GridSelector<Scalar,EmbedDim,GridType>::type type;
        };


    typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
    typedef Eigen::Matrix<int,EmbedDim,1> Veci;

    MACGridFactory(const Veci & dim,const Vec & origin=Vec::Zero(),  const Vec & dx=Vec::Ones()): m_N(dim),m_origin(origin),  m_dx(dx) {}

    MACGridFactory(const Veci & dim,const Eigen::AlignedBox<Scalar,EmbedDim>& bbox): m_N(dim),m_origin(bbox.min()),  m_dx(bbox.sizes().cwiseQuotient(dim.template cast<Scalar>())) {}


    template <mtao::GridEnum Type>
        typename selector<Type>::type create() {
            typedef typename selector<Type>::type ReturnType;
            typedef typename mtao::internal::mac_offsets<ReturnType> MyMACOffsets;
            return ReturnType(
                    m_N+MyMACOffsets::added_cells(),
                    m_origin+m_dx.cwiseProduct(MyMACOffsets::offset()),
                    m_dx
                    );
        }
    private:
    Veci m_N;
    Vec m_origin;
    Vec m_dx;
};

#endif
