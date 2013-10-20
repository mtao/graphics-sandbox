#ifndef EIGEN_ORDERING_H
#define EIGEN_ORDERING_H
#include <Eigen/Core>

template <typename Derived1, typename Derived2>
bool operator<(const Eigen::DenseBase<Derived1>& a, const Eigen::DenseBase<Derived2>& b) {
    eigen_assert(a.rows() == b.rows() && a.cols() == b.cols());
    const int rows = a.rows();
    const int cols = a.cols();
    for(int i=0; i < rows; ++i) {
        for(int j=0; j < cols; ++j) {
            if(a(i,j) < b(i,j)) return true;
            else if(a(i,j) > b(i,j)) return false;
        }
    }
    return false;
}


#endif

