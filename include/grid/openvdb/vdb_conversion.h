#ifndef VDB_CONVERSION_H
#define VDB_CONVERSION_H

#include "../embedded_grid.h"
#include <openvdb/openvdb.h>

namespace mtao {
    namespace vdb {
        template <typename T>
            grid_selector<T>::grid_type gridToVDB(const mtao::EmbeddedGrid<T,3>& g) {
                typedef grid_selector<T>::grid_type GridType;
                auto transform = openvdb::math::Transform::Ptr(
                        new openvdb::math::Transform(
                            openvdb::math::MapBase(
                                new openvdb::math::ScaleTranslateMap(
                                    eigen2vdb(g.dx())
                                    ,
                                    eigen2vdb(g.origin())
                                    )
                                )
                            )
                        );
                GridType vg = GridType::create();
                vg->setTransform(transform);
                auto acc = vg->getAccessor();
                openvdb::Coord 

            }
    }
}

#endif
