#include <openvdb/openvdb.h>
#include <openvdb/tools/GridOperators.h>
#include <iostream>
#include "Stencils.h"

template <class GridType>
void fillGrid(GridType& grid, int radius) {
    typedef typename GridType::ValueType ValueT;


    typename GridType::Accessor accessor = grid.getAccessor();

    openvdb::Coord ijk;
    int &i = ijk[0], &j = ijk[1], &k=ijk[2];
    for(i=-radius; i < radius; ++i) {
        for(j=-radius; j <  radius; ++j) {
            for(k=-radius; k <  radius; ++k) {
                //accessor.setValue(ijk,std::sqrt(std::abs(i*j*k)));
                accessor.setValue(ijk,i*j*k);


            }
        }

    }
    grid.signedFloodFill();
}
template <>
void fillGrid<openvdb::Vec3DGrid>(openvdb::Vec3DGrid& grid, int radius) {
    typedef openvdb::Vec3DGrid GridType;
    typedef typename GridType::ValueType ValueT;


    typename GridType::Accessor accessor = grid.getAccessor();

    openvdb::Coord ijk;
    int &i = ijk[0], &j = ijk[1], &k=ijk[2];
    for(i=-radius; i < radius; ++i) {
        for(j=-radius; j <  radius; ++j) {
            for(k=-radius; k <  radius; ++k) {
                accessor.setValue(ijk,openvdb::Vec3d(i,j,k));


            }
        }

    }
    grid.signedFloodFill();
}

void vec() {
    openvdb::Vec3DGrid::Ptr v3grid = openvdb::Vec3DGrid::create();
    openvdb::BoolGrid::Ptr bgrid = openvdb::BoolGrid::create();
    fillGrid(*v3grid,5);
    bgrid->topologyUnion(*v3grid);
    openvdb::math::InteriorGradStencil<openvdb::Vec3DGrid> ggrid(*v3grid);

    for(openvdb::BoolGrid::ValueOnCIter iter = bgrid->cbeginValueOn(); iter; ++iter) {
        ggrid.moveTo(iter.getCoord());
    std::cout << "Grid" << iter.getCoord() << " = " << ggrid.gradient() << std::endl;

    }
    std::cout << "Active voxels: " << v3grid->activeVoxelCount();
}

void flo() {
    std::cout << "float" << std::endl;
    openvdb::FloatGrid::Ptr v3grid = openvdb::FloatGrid::create();
    openvdb::BoolGrid::Ptr bgrid = openvdb::BoolGrid::create();
    fillGrid(*v3grid,5);
    bgrid->topologyUnion(*v3grid);
    openvdb::math::InteriorGradStencil<openvdb::FloatGrid> ggrid(*v3grid);

    for(openvdb::BoolGrid::ValueOnCIter iter = bgrid->cbeginValueOn(); iter; ++iter) {
        ggrid.moveTo(iter.getCoord());
    std::cout << "Grid" << iter.getCoord() << " = " << ggrid.gradient() << std::endl;

    }
    std::cout << "Active voxels: " << v3grid->activeVoxelCount();
}

int main() {
    openvdb::initialize();
    flo();
    vec();

    /*
    for(openvdb::Vec3DGrid::ValueOnCIter iter = v3grid->cbeginValueOn(); iter; ++iter) {
    std::cout << "Grid" << iter.getCoord() << " = " << *iter << std::endl;
    }
    */


}
