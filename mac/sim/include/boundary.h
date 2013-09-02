#ifndef BOUNDARY_H
#define BOUNDARY_H
#include "types.h"
#include "macgridfactory.h"




namespace mtao{namespace ddg{
    namespace internal{
    template <typename Scalar>
        BoundaryContainerBaseImpl;
    template <typename Scalar, int EmbedDim, int CurDim>
        BoundaryContainerImpl;
    template <typename Scalar, int EmbedDim, int Cur>
        struct BoundarySelector{ 
            typedef typename std::conditional<Cur <= 0
                , BoundaryContainerBaseImpl<Scalar>
                , BoundaryCountainerImpl<Scalar,EmbedDim,Cur> >::type type;
        };
    template <typename Scalar, int EmbedDim>
        BoundaryContainerBaseImpl {
                typedef MACGridFactory<Scalar,EmbedDim> FactoryType;
            protected:
                FactoryType::weak_ptr m_factory;
        };
    template <typename Scalar, int EmbedDim, int CurDim>
        BoundaryContainerImpl: public BoundarySelector<Scalar,EmbedDim,CurDim-1>::type {
        };


    }

    template <typename Scalar, int EmbedDim>
        class BoundaryContainer: public internal::BoundarySelector<Scalar,EmbedDim,EmbedDim>::type {
            public:
                typedef MACGridFactory<Scalar,EmbedDim> FactoryType;
                BoundaryContainer(const MACGridFactory<Scalar,EmbedDim>::ptr& factory): m_factory(factory) {
                }
        };
}}

#endif
