#include <iostream>
#include <fstream>
#include <openvdb/openvdb.h>
#include <openvdb/tools/GridTransformer.h>
#include <cstdlib>

template <typename Grid>
auto resampleGridToMatch(const Grid& sourceGrid, const openvdb::math::Transform::Ptr& transform) -> typename Grid::Ptr{
    typename Grid::Ptr targetGrid = Grid::create();
    targetGrid->setTransform(transform);
    targetGrid->setBackground(sourceGrid.background());
    // Get the source and target grids' index space to world space transforms.
    const openvdb::math::Transform
        &sourceXform = sourceGrid.transform(),
        &targetXform = targetGrid->transform();
    // Compute a source grid to target grid transform.
    // (For this example, we assume that both grids' transforms are linear,
    // so that they can be represented as 4 x 4 matrices.)
    openvdb::Mat4R xform =
        sourceXform.baseMap()->getAffineMap()->getMat4() *
        targetXform.baseMap()->getAffineMap()->getMat4().inverse();
    // Create the transformer.
    openvdb::tools::GridTransformer transformer(xform);
    // Resample using nearest-neighbor interpolation.
    // Resample using triquadratic interpolation.
    transformer.transformGrid<openvdb::tools::QuadraticSampler, Grid>(
            sourceGrid, *targetGrid);
    targetGrid->signedFloodFill();
    // Prune the target tree for optimal sparsity.
    targetGrid->tree().prune();
    return targetGrid;

}





int main(int argc, char * argv[]) {
    openvdb::initialize();
    std::string grid_name;
    if(argc < 2) {
        return 1;
    } else if (argc == 3) {
        grid_name = argv[2];
    }
    std::string input_name(argv[1]);
    openvdb::io::File infile(argv[1]);
    infile.open();

    float ratio = atof(argv[3]);

    openvdb::GridPtrVec outgrids;
    auto&& transform = openvdb::math::Transform::createLinearTransform(ratio);

    
    openvdb::GridBase::Ptr baseGrid;
    for(openvdb::io::File::NameIterator nameIter = infile.beginName();
    nameIter != infile.endName(); ++nameIter) {
        openvdb::GridBase::Ptr grid = infile.readGrid(nameIter.gridName());
        if(grid->isType<openvdb::FloatGrid>()) {
            outgrids.push_back(resampleGridToMatch(*openvdb::gridPtrCast<openvdb::FloatGrid>(grid),transform));
        } else if(grid->isType<openvdb::DoubleGrid>()) {
            outgrids.push_back(resampleGridToMatch(*openvdb::gridPtrCast<openvdb::DoubleGrid>(grid),transform));
        }

    }





    std::string output_name(argv[2]);
    openvdb::io::File outfile(output_name);
    outfile.write(outgrids);
    outfile.close();


    return 0;
}
