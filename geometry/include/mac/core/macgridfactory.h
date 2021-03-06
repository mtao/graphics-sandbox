#ifndef MACGRIDFACTORY_H
#define MACGRIDFACTORY_H

#include "mac.h"
#include <memory>
#include <list>

#include <iostream>

namespace mtao{ 
    enum GridEnum {NGrid,CGrid,UGrid,VGrid,WGrid,DUGrid,DVGrid,DWGrid,UVGrid,UVWGrid,DUVWGrid};
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
        constexpr int nCrm1(int n, int r) {
            return nCr(n,r)-1;
        }



        template <typename Scalar, int EmbedDim, int FormDim, int WhichForm>
            struct ManagedGridStructureFeather: public ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm-1>{
                typedef MACGrid<Scalar,EmbedDim,FormDim,WhichForm> GridType;
                typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
                typedef Eigen::Matrix<int,EmbedDim,1> Veci;
                typedef Eigen::AlignedBox<Scalar,EmbedDim> BBox;
                std::list<typename GridType::weak_ptr> m_grids;
                void prune() {
                    foreach_grid([](GridType&){});
                    ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm-1>::prune();
                }
                void add(typename GridType::ptr& grid) {
                    m_grids.push_back(grid);
                }
                void resize(const Veci& N, const Vec& origin, const Vec& dx) {
                    Vec myOrigin = offset(origin,dx);
                    Veci myN = extraCells(N);
                    foreach_grid([&](GridType& g){
                            g.resize(myN);
                            g.setOrigin(myOrigin);
                            g.setDx(dx);
                            });
                    ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm-1>::resize(N,origin,dx);
                }
                void setOrigin(const Vec& origin, const Vec& dx) {
                    Vec myOrigin = offset(origin,dx);
                    foreach_grid([&](GridType& g){
                            g.setOrigin(myOrigin);
                            g.setDx(dx);
                            });
                    ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm-1>::setOrigin(origin,dx);
                }
                private:
                void foreach_grid(const std::function<void(GridType&)>& f) {
                    typename GridType::ptr ptr;
                    for(auto&& it=m_grids.begin(); it != m_grids.end();) {
                        if(!(ptr=it->lock())) {
                            m_grids.erase(it++);
                        } else {
                            f(*ptr);
                            ++it;
                        }
                    }
                }
                typedef typename mtao::internal::mac_offsets<GridType> MyMACOffsets;
                Vec offset(const Vec& origin, const Vec& dx) {
                    return origin+dx.cwiseProduct(Eigen::Map<const Vec>(MyMACOffsets::offsets().data()));
                }
                Veci extraCells(const Veci& N) {
                    return N+Eigen::Map<const Veci>(MyMACOffsets::extra_cells().data());
                }
            };

        //Base case
        template <typename Scalar, int EmbedDim, int FormDim>
            struct ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,-1> {
                typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
                typedef Eigen::Matrix<int,EmbedDim,1> Veci;
                typedef Eigen::AlignedBox<Scalar,EmbedDim> BBox;
                void prune() {}
                void resize(const Veci&, const Vec&, const Vec&) {}
                void setOrigin(const Vec&, const Vec&) {}
            };




        template <typename Scalar, int EmbedDim, int FormDim>
            struct ManagedGridStructureWing: public ManagedGridStructureWing<Scalar,EmbedDim,FormDim-1>, public ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,nCrm1(EmbedDim,FormDim)>{
                typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
                typedef Eigen::Matrix<int,EmbedDim,1> Veci;
                typedef Eigen::AlignedBox<Scalar,EmbedDim> BBox;
                void prune() {
                    ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,nCrm1(EmbedDim,FormDim)-1>::prune();
                    ManagedGridStructureWing<Scalar,EmbedDim,FormDim-1>::prune();
                }
                template <int WhichForm>
                    void add(typename MACGrid<Scalar,EmbedDim,FormDim,WhichForm>::ptr& grid) {
                        ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,WhichForm>::add(grid);
                    }
                void resize(const Veci& v, const Vec&  origin, const Vec& dx) {
                    ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,nCrm1(EmbedDim,FormDim)>::resize(v, origin,dx);
                    ManagedGridStructureWing<Scalar,EmbedDim,FormDim-1>::resize(v,origin,dx);
                }
                void setOrigin(const Vec& origin, const Vec& dx) {
                    ManagedGridStructureFeather<Scalar,EmbedDim,FormDim,nCrm1(EmbedDim,FormDim)>::setOrigin(origin,dx);
                    ManagedGridStructureWing<Scalar,EmbedDim,FormDim-1>::setOrigin(origin,dx);

                }
            };

        //Base case
        template <typename Scalar, int EmbedDim>
            struct ManagedGridStructureWing<Scalar,EmbedDim,-1>{
                typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
                typedef Eigen::Matrix<int,EmbedDim,1> Veci;
                typedef Eigen::AlignedBox<Scalar,EmbedDim> BBox;
                void prune() {}
                void resize(const Veci&, const Vec&, const Vec&) {}
                void setOrigin(const Vec&, const Vec&) {}
            };



        template <typename Scalar, int EmbedDim>
            struct ManagedGridStructure: public ManagedGridStructureWing<Scalar,EmbedDim,EmbedDim> {
                //public interface for manager
                typedef Eigen::Matrix<Scalar,EmbedDim,1> Vec;
                typedef Eigen::Matrix<int,EmbedDim,1> Veci;
                typedef Eigen::AlignedBox<Scalar,EmbedDim> BBox;
                void prune() {
                    ManagedGridStructureWing<Scalar,EmbedDim,EmbedDim>::prune();
                }
                template <int FormDim, int WhichForm>
                    void add(typename MACGrid<Scalar,EmbedDim,FormDim,WhichForm>::ptr& grid) {
                        ManagedGridStructureWing<Scalar,EmbedDim,FormDim>::template add<WhichForm>(grid);
                    }
                void resize(const Veci& v, const Vec& origin, const Vec& dx) {
                    ManagedGridStructureWing<Scalar,EmbedDim,EmbedDim>::resize(v,origin,dx);
                }
                void setOrigin(const Vec& origin, const Vec& dx) {
                    ManagedGridStructureWing<Scalar,EmbedDim,EmbedDim>::setOrigin(origin,dx);
                }

            };

    }
}


template <typename Scalar, int EmbedDim> 
class MACGridFactory {
    public:


        typedef MACGridFactory<Scalar,EmbedDim> type;
        typedef typename std::shared_ptr<type> ptr;
        typedef typename std::weak_ptr<type> weak_ptr;

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
                typedef typename Selector<Type>::type ReturnType;
                m_managed_grids.template add<ReturnType::form_dim,ReturnType::which_form>(ret);
                return ret;
            }
        void resize(const Veci& N){
            m_N = N;
            m_dx=m_bbox.sizes().cwiseQuotient(m_N.template cast<Scalar>());
            m_managed_grids.prune();
            m_managed_grids.resize(m_N,m_bbox.min(),m_dx);
        }
        void setBounds(const BBox& bbox){
            m_bbox = bbox;
            m_dx=bbox.sizes().cwiseQuotient(m_N.template cast<Scalar>());
            m_managed_grids.prune();
            m_managed_grids.setOrigin(m_bbox.min(),m_dx);
        }
        void setBoundsAndResize(const BBox& bbox, const Veci& N){
            m_N = N;
            m_bbox = bbox;
            m_dx=bbox.sizes().cwiseQuotient(m_N.template cast<Scalar>());
            m_managed_grids.prune();
            m_managed_grids.resize(m_N,bbox.min(),m_dx);
        }
        const BBox& bbox()const {return m_bbox;}
        const Veci& N()const {return m_N;}
        const Vec& dx()const {return m_dx;}

        template <mtao::GridEnum Type>
        std::function<Vec(const Vec&)> indexToWorldFunction() {
            return [=](const Vec& v) -> Vec {
                return v.cwiseProduct(dx())+offsets<Type>();
            };
        }
        template <mtao::GridEnum Type>
        std::function<Vec(const Veci&)> indexToWorldIntegralFunction() {
            return [=](const Veci& v) -> Vec {
                return v.template cast<Scalar>().cwiseProduct(dx())+offsets<Type>();
            };
        }
        template <mtao::GridEnum Type>
            std::function<int(int,int)> coordToIndexFunction2() {
                const Vec n = offsets<Type>();
                return [=](int a, int b) -> int {
            return  n(1) * a + b;
                };
            }
        template <mtao::GridEnum Type>
            std::function<int(int,int,int)> coordToIndexFunction3() {
                const Veci n = offsets<Type>();
                return [=](int a, int b, int c) -> int {
                    return n(2) * (n(1) * a + b) + c;
                };
            }
    private:
        mtao::internal::ManagedGridStructure<Scalar,EmbedDim> m_managed_grids;
        Veci m_N;
        BBox m_bbox;
        Vec m_dx;
};

#endif
