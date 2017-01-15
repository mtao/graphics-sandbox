#include <iostream>
#include <openvdb/openvdb.h>
int main(int argc, char * argv[]) {
    openvdb::initialize();
    openvdb::io::File file(argv[1]);
    file.open();
    openvdb::GridBase::Ptr baseGrid;
    for(openvdb::io::File::NameIterator nameIter = file.beginName(); nameIter != file.endName(); ++nameIter) {
       baseGrid = file.readGridMetadata(nameIter.gridName());
       if(baseGrid->isType<openvdb::FloatGrid>()) {
            //gridPtr = openvdb::gridPtrCast<openvdb::FloatGrid>(file.readGrid(nameIter.gridName()));
           break;
       }
    }
    openvdb::math::BBox<openvdb::Vec3s> bbox;

    auto&& coord_bbox = baseGrid->evalActiveVoxelBoundingBox();
    bbox.min() = baseGrid->indexToWorld(coord_bbox.min());
    bbox.max() = baseGrid->indexToWorld(coord_bbox.max());
    std::cout << bbox << std::endl;

    return 0;
}
