#include "kdtree.h"

int main() {
    KDTree tree;
    tree.insert(Eigen::Vector3d(1,2,3));
    tree.insert(Eigen::Vector3d(2,4,3));
    tree.insert(Eigen::Vector3d(3,3,8));
    tree.insert(Eigen::Vector3d(4,2,7));
    tree.insert(Eigen::Vector3d(5,2,8));
    std::vector<Eigen::Vector3d> v;
    v.push_back(Eigen::Vector3d(1,2,3));
    v.push_back(Eigen::Vector3d(2,4,3));
    v.push_back(Eigen::Vector3d(3,3,8));
    v.push_back(Eigen::Vector3d(4,2,7));
    v.push_back(Eigen::Vector3d(5,2,8));
    KDTreeP treep(v);
}
