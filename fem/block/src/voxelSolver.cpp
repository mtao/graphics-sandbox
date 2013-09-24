#include "../include/voxelSolver.h"
#include "../include/voxelObject.h"
#include "../include/solvers/pcg.hpp"
#include "../include/solvers/cg.hpp"
#include <iostream>





VoxelSolver::
VoxelSolver(const std::shared_ptr<VoxelObjectBase> & voxelobj)
: m_voxel_obj(voxelobj)
{

}

void VoxelSolver::
setObject(const std::shared_ptr<VoxelObjectBase> & voxelobj) {
    m_voxel_obj = voxelobj;
}

bool VoxelSolver::
solve() {
    if(!getObject()) {
        std::cerr << "No object found!" << std::endl;
        return false;
    }
    std::cout << "Starting to solve!" << std::endl;
    //minimize the number of unused indices / clean up the active set in lattice
    
    std::cout << "Compressing object..." << std::endl;
    getObject()->makeCompressed();
    std::cout << "Computing number of active voxels:" << getObject()->activeVoxels() << std::endl;
    std::cout << "Computing number of DOFS:" << getObject()->numDOFs() << std::endl;
    std::cout <<"Generating A" << std::endl;
    Vector rhs;
    SparseMatrix A = getObject()->getStiffnessMatrix(rhs);
    //A *= 1.0f/(getObject()->N());
    //std::cout << A << std::endl;
    size_t rows = A.rows();

    auto&& x = m_voxel_obj->getData() = (Vector::Zero(rows));

//    rhs /= getObject()->N()*getObject()->NI();
    std::cout << "Calling the solver..." << std::endl;
    ConjugateGradientSolve(A,rhs,x);
    //SparseCholeskyPreconditionedConjugateGradientSolve(A,rhs,x);
    //std::cout << rhs.transpose() << std::endl;
    //std::cout << std::endl;
    //std::cout << x.transpose() << std::endl;
    //std::cout << std::endl;
    

    return true;
}

