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
    }
}

#endif
