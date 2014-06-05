#ifndef VDB_CONVERSION_H
#define VDB_CONVERSION_H

#include "../grid/embeddedgrid.h"
#include "vdb_utils.h"
#include <openvdb/openvdb.h>

namespace mtao {
    namespace vdb {
        template <typename T>
            typename grid_selector<T>::grid_type::Ptr gridToVDB(const mtao::EmbeddedGrid<T,3>& g) {
                typedef typename grid_selector<T>::grid_type GridType;
                auto transform = openvdb::math::Transform::Ptr(
                        new openvdb::math::Transform(
                            openvdb::math::MapBase::Ptr(
                                new openvdb::math::ScaleTranslateMap(
                                    eigen2vdb(g.dx())
                                    ,
                                    eigen2vdb(g.origin())
                                    )
                                )
                            )
                        );
                typename GridType::Ptr vg = GridType::create();
                vg->setTransform(transform);
                auto acc = vg->getAccessor();
                g.loop([&](const mtao::Coord3& c, T v) {
                        openvdb::Coord vc = eigen2vdb(c);
                        acc.setValue(vc,v);
                        });

                return vg;
            }
        template <typename GridType>
            mtao::EmbeddedGrid<typename GridType::ValueType,3> vdbToGrid(const GridType& g) {
                typedef typename GridType::ValueType ValueType;

                typedef mtao::scalar_types<ValueType> _st;
                typedef typename _st::Vec3 Vec3;
                Vec3 dx = vdb2eigen(g.voxelSize()).template cast<ValueType>();
                auto cbb = g.evalActiveVoxelBoundingBox();
                Vec3 origin = vdb2eigen(cbb.min()).template cast<ValueType>();

                mtao::EmbeddedGrid<ValueType,3> mg(vdb2eigen(cbb.extents()),origin,dx);



                auto acc = g.getAccessor();
                mg.loop([&](const mtao::Coord3& c, ValueType v) {
                        openvdb::Coord vc = eigen2vdb(c);
                        return acc.getValue(vc);
                        });

                return mg;
            }
    }
}

#endif
