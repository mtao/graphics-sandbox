#include <iostream>
#include <fstream>
#include <openvdb/openvdb.h>
#include <openvdb/tools/GridOperators.h>
#include <openvdb/tools/LevelSetAdvect.h>
#include <openvdb/tools/ValueTransformer.h>
#include <sstream>


struct Sum {
    Sum(float a): val(a) {}
    inline void operator()(openvdb::FloatGrid::ValueOnCIter& iter) const {
        iter.setValue(*iter+val);
    }
    float val;
};

template <typename GridType>
void advect(GridType& grid, float dt) {

    openvdb::tools::valxform::SumOp<float> sum(dt);
    //Sum sum(dt);
    openvdb::tools::foreach(grid.beginValueOn(),sum);
    grid.signedFloodFill();


    return;

#ifdef ENRIGHT_TEST
    typedef typename openvdb::tools::LevelSetAdvection<GridType> LSAdvection;
    LSAdvection lsa(grid, openvdb::tools::EnrightField<typename GridType::ValueType>());
#else
    typedef typename openvdb::tools::Gradient<GridType> Gradient;
    Gradient grad(grid);
    typedef typename Gradient::OutGridType GradGrid;
    typename GradGrid::Ptr gradGrid = grad.process();
    typedef typename openvdb::tools::DiscreteField<GradGrid> GradDiscreteField;
    GradDiscreteField discField(*gradGrid);

    typedef typename openvdb::tools::LevelSetAdvection<GridType,GradDiscreteField> LSAdvection;
    LSAdvection lsa(grid, discField);
#endif
    

    lsa.advect(0,dt);
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



    
    openvdb::GridBase::Ptr baseGrid;
    openvdb::FloatGrid::Ptr grid;
    for(openvdb::io::File::NameIterator nameIter = infile.beginName();
    nameIter != infile.endName(); ++nameIter) {
        baseGrid = infile.readGrid(nameIter.gridName());

    }

    grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
    for(size_t i = 0; i < 30; ++i) {
        std::cout << "Starting frame " << i << std::endl;
        advect(*grid,-0.05);

        std::cout << "Saving frame " << i << std::endl;
        openvdb::GridPtrVec outgrids;
        outgrids.push_back(grid);
        std::stringstream ss;
        ss << argv[2] << "." << i;
        std::string output_name(std::string(ss.str()));
        openvdb::io::File outfile(output_name);
        outfile.write(outgrids);
        outfile.close();
    }



    return 0;
}
