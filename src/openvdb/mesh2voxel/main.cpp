#include <iostream>
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/math/Transform.h>
#include "trianglemesh.h"
int main(int argc, char * argv[]) {
    TriangleMesh tm=TriangleMesh::readObj(argv[1]);
    openvdb::math::Transform::Ptr lt = openvdb::math::Transform::createLinearTransform(0.01);
    openvdb::v1_2_0::tools::MeshToVolume<openvdb::FloatGrid> mtv(lt);
    openvdb::math::BBox<openvdb::Vec3s> bbox;

    for(auto&& v: tm.vertices) {
        bbox.expand(v);
    }
    bbox.min() = lt->worldToIndex(bbox.min());
    bbox.max() = lt->worldToIndex(bbox.max());
    float radius = bbox.extents().length()/2;
    std::vector<openvdb::Vec4I> q(tm.triangles.size());
    std::transform(tm.triangles.begin(), tm.triangles.end(), q.begin(), [](const openvdb::Vec3I& tr)->openvdb::Vec4I {
            return openvdb::Vec4I(tr.x(),tr.y(),tr.z(),openvdb::util::INVALID_IDX);
            });
    mtv.convertToLevelSet(tm.vertices,q,0,radius);
    openvdb::FloatGrid::Ptr grid = mtv.distGridPtr();
    openvdb::io::File file("mygrids.vdb");
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    file.write(grids);
    file.close();

    return 0;
}
