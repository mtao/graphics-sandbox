#ifndef MACGRIDFACTORY_H
#define MACGRIDFACTORY_H

#include "mac.h"
#include <memory>


namespace mtao{ 
    enum GridEnum {NGrid,CGrid,UGrid,VGrid,WGrid,DUGrid,DVGrid,DWGrid};
    namespace internal {
        template <int EmbedDim, int FormDim, int WhichForm>
            struct GridEnumSelector{
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,0,0>{
                enum {value = NGrid};
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,1,0>{
                enum {value = UGrid};
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,1,1>{
                enum {value = VGrid};
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,1,2>{
                enum {value = WGrid};
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,2,0>{
                enum {value = DUGrid};
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,2,1>{
                enum {value = DVGrid};
            };
        template <int EmbedDim>
            struct GridEnumSelector<EmbedDim,2,2>{
                enum {value = DWGrid};
            };
        template <>
            struct GridEnumSelector<1,1,0>{
                enum {value = CGrid};
            };
        template <>
            struct GridEnumSelector<2,2,0>{
                enum {value = CGrid};
            };
        template <>
            struct GridEnumSelector<3,3,0>{
                enum {value = CGrid};
            };
    template <typename Scalar, int EmbedDim, GridEnum Type>
        struct GridSelector {
        };
    template <typename Scalar>
        struct GridSelector<Scalar,2,NGrid> {
            typedef grid_types<Scalar,2> Types;
            typedef typename Types::NGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,2,UGrid> {
            typedef grid_types<Scalar,2> Types;
            typedef typename Types::UGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,2,VGrid> {
            typedef grid_types<Scalar,2> Types;
            typedef typename Types::VGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,2,CGrid> {
            typedef grid_types<Scalar,2> Types;
            typedef typename Types::CGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,NGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::NGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,CGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::CGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,UGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::UGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,VGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::VGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,WGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::WGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,DUGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::DUGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,DVGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::DVGrid type;};
    template <typename Scalar>
        struct GridSelector<Scalar,3,DWGrid> {
            typedef grid_types<Scalar,3> Types;
            typedef typename Types::DWGrid type;};



}}


template <typename Scalar, int EmbedDim> 
class MACGridFactory {
    public:


    typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
    typedef Eigen::Matrix<int,EmbedDim,1> Veci;

    MACGridFactory(const Veci & dim,const Vec & origin,  const Vec & dx)
        : m_N(dim)
          ,m_origin(origin)
          ,  m_dx(dx) {}
    MACGridFactory(const Veci& dim, const Eigen::AlignedBox<Scalar,EmbedDim> & bbox)
        : m_N(dim)
          , m_origin(bbox.min())
          , m_dx(bbox.sizes().cwiseQuotient(dim.template cast<Scalar>())) {}



    template <mtao::GridEnum Type>
        typename mtao::internal::GridSelector<Scalar,EmbedDim,Type>::type create() {
            typedef typename mtao::internal::GridSelector<Scalar,EmbedDim,Type>::type ReturnType;
            typedef typename mtao::internal::mac_offsets<ReturnType> MyMACOffsets;
            return ReturnType(
                    m_N+Eigen::Map<const Veci>(MyMACOffsets::extra_cells().data()),
                    m_origin+m_dx.cwiseProduct(Eigen::Map<const Vec>(MyMACOffsets::offsets().data())),
                    m_dx
                    );
        }
    template <mtao::GridEnum Type>
        typename mtao::internal::GridSelector<Scalar,EmbedDim,Type>::type createPtr() {
            typedef typename mtao::internal::GridSelector<Scalar,EmbedDim,Type>::type ReturnType;
            typedef typename mtao::internal::mac_offsets<ReturnType> MyMACOffsets;
            return std::make_shared<ReturnType>(
                    m_N+Eigen::Map<const Veci>(MyMACOffsets::extra_cells().data()),
                    m_origin+m_dx.cwiseProduct(Eigen::Map<const Vec>(MyMACOffsets::offsets().data())),
                    m_dx
                    );
        }
    private:
    Veci m_N;
    Vec m_origin;
    Vec m_dx;
};

#endif
