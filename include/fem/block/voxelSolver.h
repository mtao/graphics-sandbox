#ifndef VOXEL_SOLVER
#define VOXEL_SOLVER
#include "macros.h"

#include <vector>
#include <string>
#include <memory>
#include "../include/solvers/pcg.hpp"
#include <iostream>
#include "voxelObject.h"

struct VoxelVertex{
    EIGEN_DENSE_MATRIX_TYPEDEFS

        Vector3 vec;
    float val;

};




class VoxelSolver{
    public:
        EIGEN_DENSE_MATRIX_TYPEDEFS
        EIGEN_SPARSE_MATRIX_TYPEDEFS
        VoxelSolver(const std::shared_ptr<VoxelObjectBase> & voxelobj = 0);
        virtual void setObject(const std::shared_ptr<VoxelObjectBase> & voxelobj);
        virtual std::shared_ptr<VoxelObjectBase> & getObject() {return m_voxel_obj;}
        virtual bool solve();
        //void computeDOFs();
    protected:
        std::shared_ptr<VoxelObjectBase> m_voxel_obj;


};





#endif
