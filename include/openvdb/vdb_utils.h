#ifndef VDB_UTILS_H
#define VDB_UTILS_H

#include <openvdb/openvdb.h>
#include "../types.h"
#include "../coord.h"
#include <openvdb/math/Vec2.h>
#include <openvdb/math/Vec3.h>
#include <openvdb/math/Vec4.h>

namespace mtao {
    namespace vdb {
        namespace internal {

            //openvdb selector based on dim and scalar type
            template <int Dim>
                struct vec_selector {};
            template <>
                struct vec_selector<2> {
                    template <typename Scalar>
                        struct Vec {
                            typedef typename openvdb::math::Vec2<Scalar> type;
                        };
                };
            template <>
                struct vec_selector<3> {
                    template <typename Scalar>
                        struct Vec {
                            typedef typename openvdb::math::Vec3<Scalar> type;
                        };
                };
            template <>
                struct vec_selector<4> {
                    template <typename Scalar>
                        struct Vec {
                            typedef typename openvdb::math::Vec4<Scalar> type;
                        };
                };
            //method to extract the type of a vdb grid
            template <template <class> class VecType>
                struct vdb_dim{
                    enum {value = VecType::size};
                };
            /*
               template <>
               struct vdb_dim<openvdb::math::Vec2> {
               enum {value = 2};
               };
               template <>
               struct vdb_dim<openvdb::math::Vec3> {
               enum {value = 3};
               };
               template <>
               struct vdb_dim<openvdb::math::Vec4> {
               enum {value = 4};
               };
               */
            template <typename VecType>
                struct vdb_traits {};

            template <template <class> class VecType, typename Scalar_>
                struct vdb_traits <VecType<Scalar_> >{
                    typedef Scalar_ Scalar;
                    enum {dim = vdb_dim<VecType>::value};
                };

        }

        //The worst spaghetti code ever :)
        //Basically we select the dimension and type of a vector individually through selection structs.
        template <int Dim, typename Scalar>
            typename mtao::vdb::internal::vec_selector<Dim>::template Vec<Scalar>::type eigen2vdb(const Eigen::Matrix<Scalar,Dim,1> & v) {
                return typename mtao::vdb::internal::vec_selector<Dim>::template Vec<Scalar>::type(v.data());
            }
        //Eigen can do maps, which is better than a full copy :)
        template <typename VecType>
            Eigen::Map<const Eigen::Matrix<typename mtao::vdb::internal::vdb_traits<VecType>::Scalar,mtao::vdb::internal::vdb_traits<VecType>::dim,1> > vdb2eigen(const VecType& v) {
                return Eigen::Map<const Eigen::Matrix<typename mtao::vdb::internal::vdb_traits<VecType>::Scalar,mtao::vdb::internal::vdb_traits<VecType>::dim,1> >(v.asV());
            }
        template <typename T>
            struct grid_selector {
                typedef typename openvdb::tree::Tree4<T,5,4,3>::Type tree_type;
                typedef typename openvdb::Grid<tree_type> grid_type;
            };
        openvdb::Coord eigen2vdb(const mtao::Coord<3>& c) {
            return openvdb::Coord(c(0),c(1),c(2));
        }
        mtao::Coord<3> vdb2eigen(const openvdb::Coord& c) {
            return mtao::Coord<3>(c(0),c(1),c(2));
        }

    }
}

#endif
