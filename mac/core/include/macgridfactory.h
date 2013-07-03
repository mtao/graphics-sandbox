#ifndef MACGRIDFACTORY_H
#define MACGRIDFACTORY_H

#include "mac.h"


namespace mtao{ internal {
    enum GridEnum {NGrid,CGrid,UGrid,VGrid,WGrid,DUGrid,DVGrid,DWGrid,NGrid};
}}
template <typename Scalar, int EmbedDim>
struct GridSelector {
    template <GridEnum Type>
        struct select {
        };
};
template <typename Scalar>
struct GridSelector<Scalar,2> {
    template <GridEnum Type>
        struct select {};
    typedef grid_types<Scalar,2> Types;
    template <>
        struct select<NGrid> {typedef Types::NGrid type};
    template <>
        struct select<UGrid> {typedef Types::UGrid type};
    template <>
        struct select<VGrid> {typedef Types::VGrid type};
    template <>
        struct select<NGrid> {typedef Types::NGrid type};
};
template <typename Scalar>
struct GridSelector<Scalar,3> {
    template <GridEnum Type>
        struct select {};
    typedef grid_types<Scalar,3> Types;
    template <>
        struct select<NGrid> {typedef Types::NGrid type};
    template <>
        struct select<UGrid> {typedef Types::UGrid type};
    template <>
        struct select<VGrid> {typedef Types::VGrid type};
    template <>
        struct select<WGrid> {typedef Types::WGrid type};
    template <>
        struct select<UGrid> {typedef Types::DUGrid type};
    template <>
        struct select<VGrid> {typedef Types::DVGrid type};
    template <>
        struct select<WGrid> {typedef Types::DWGrid type};
    template <>
        struct select<NGrid> {typedef Types::NGrid type};
};
template <typename Scalar, int EmbedDim> 
struct MACGridFactory {
    typedef GridSelector<Scalar,EmbedDim> MySelector;


    typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
    typedef Eigen::Matrix<int,EmbedDim,1> Vec;

    MACGridFactory(const Veci & dim,const Vec & origin,  const Vec & dx): m_N(dim),m_origin(origin),  m_dx(dx) {}


    
    template <GridEnum Type>
        MySelector<Type>::type create() {
            typedef MySelector<Type>::type ReturnType;
            typedef typename mac_offsets<ReturnType> MyMACOffsets;
            return ReturnType(
                    m_origin+m_dx.cwiseProduct(MyMACOffsets::offset),
                    m_N+MyMACOffsets::added_cells,
                    m_dx
                    );
        }
    private:
    Veci m_N;
    Vec m_origin;
    Vec m_dx;
};

#endif
