#ifndef MACGRIDFACTORY_H
#define MACGRIDFACTORY_H

#include "mac.h"
#include <memory>
#include <list>


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

    constexpr int factorial(int n) {
        return (n>1)?(n*factorial(n-1)):1;
    }
    constexpr int nCr(int n, int r) {
        return factorial(n)/(factorial(n-r)*factorial(r));
    }

    template <typename Scalar, int EmbedDim, int FormDim, int WhichForm>
        struct ManagedGridStructureFeather: public ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm-1>{
            typedef MACGrid<Scalar,EmbedDim,FormDim,WhichForm> GridType;
            std::list<typename GridType::weak_ptr> m_grids;
            void prune() {
                for(auto&& it=m_grids.begin(); it != m_grids.end();) {
                    if(!m_grids.lock()) {
                        m_grids.erase(it++);
                    } else {
                        ++it;
                    }
                }
                ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm-1>::prune();
            }
        };

    template <typename Scalar, int EmbedDim, int FormDim>
        struct ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,-1> {
            void prune() {}
        };

    template <typename Scalar, int EmbedDim, int FormDim>
        struct ManagedGridStructureWing: public ManagedGridStructureWing<Scalar,EmbedDim,FormDim-1>, public ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,nCr(EmbedDim,FormDim)>{
            void prune() {
                ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,nCr(EmbedDim,FormDim)>::prune();
                ManagedGridStructureWing<Scalar,EmbedDim,FormDim-1>::prune();
            }
        };
    template <typename Scalar, int EmbedDim>
        struct ManagedGridStructureWing<Scalar,EmbedDim,-1>{
            void prune() {}
        };
    template <typename Scalar, int EmbedDim>
    struct ManagedGridStructure: public ManagedGridStructureWing<Scalar,EmbedDim,EmbedDim> {
        //public interface for manager
        void prune() {
        ManagedGridStructureWing<Scalar,EmbedDim,EmbedDim>::prune();
        }
        /*
        void resize(const Veci v) {

        }
        */

    };

    }
}


template <typename Scalar, int EmbedDim> 
class MACGridFactory {
    public:


        typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
        typedef Eigen::Matrix<int,EmbedDim,1> Veci;
        typedef Eigen::AlignedBox<Scalar,EmbedDim> BBox;

        MACGridFactory(const Veci & dim,const Vec & origin,  const Vec & dx)
            : m_N(dim)
              ,m_bbox(origin, dim.template cast<Scalar>().array()*dx.array()+origin)
              ,  m_dx(dx) {}
        MACGridFactory(const Veci& dim, const BBox & bbox)
            : m_N(dim)
              , m_bbox(bbox)
              , m_dx(bbox.sizes().cwiseQuotient(dim.template cast<Scalar>())) {}

        template <mtao::GridEnum Type>
            struct Selector {
                typedef typename mtao::internal::GridSelector<Scalar,EmbedDim,Type>::type type;
            };

        template <mtao::GridEnum Type>
            typename Selector<Type>::type create() {
                typedef typename Selector<Type>::type ReturnType;
                return ReturnType(
                        extraCells<Type>(),
                        offsets<Type>(),
                        m_dx
                        );
            }
        template <mtao::GridEnum Type>
            typename Selector<Type>::type::ptr createPtr() {
                typedef typename Selector<Type>::type ReturnType;
                return std::shared_ptr<ReturnType>(new ReturnType(//Can't use make_shared without allowing make_shared to create its own grids
                            extraCells<Type>(),
                            offsets<Type>(),
                            m_dx
                            ));
            }
        template <mtao::GridEnum Type>
            Veci extraCells() {
                typedef typename Selector<Type>::type ReturnType;
                typedef typename mtao::internal::mac_offsets<ReturnType> MyMACOffsets;
                return m_N+Eigen::Map<const Veci>(MyMACOffsets::extra_cells().data());
            }
        template <mtao::GridEnum Type>
            Vec offsets() {
                typedef typename Selector<Type>::type ReturnType;
                typedef typename mtao::internal::mac_offsets<ReturnType> MyMACOffsets;
                return m_bbox.min()+m_dx.cwiseProduct(Eigen::Map<const Vec>(MyMACOffsets::offsets().data()));
            }
        template <mtao::GridEnum Type>
            typename Selector<Type>::type::ptr createManagedPtr() {
                auto&& ret = createPtr<Type>();
                //m_managed_grids.add(ret);
                return ret;
            }
        const BBox& bbox()const {return m_bbox;}
        const Veci& N()const {return m_N;}
        const Vec& dx()const {return m_dx;}
    private:
        //mtao::internal::ManagedGridStructure<int,EmbedDim> m_managed_grids;
        Veci m_N;
        BBox m_bbox;
        Vec m_dx;
};

#endif
