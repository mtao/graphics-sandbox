#ifndef MACROS_H
#define MACROS_H

#ifndef SCALAR_TYPE
#define SCALAR_TYPE float
#endif

#define EIGEN_SPARSE_MATRIX_TYPEDEFS \
        typedef Eigen::SparseMatrix<SCALAR_TYPE> SparseMatrix;\
        typedef Eigen::Triplet<SCALAR_TYPE> Triplet;\
        typedef std::vector<Triplet> Triplets;
#endif

#define EIGEN_DENSE_MATRIX_TYPEDEFS \
    typedef Eigen::Matrix<SCALAR_TYPE,Eigen::Dynamic,Eigen::Dynamic> Matrix;\
    typedef Eigen::Matrix<SCALAR_TYPE,Eigen::Dynamic,1> Vector;\
    typedef Eigen::Matrix<SCALAR_TYPE,3,1> Vector3;

//TODO: USE_BOUNDARY makes the boundary voxels boundary voxels
#ifdef ENABLE_BOUNDARY
#define USE_BOUNDARY
#endif
