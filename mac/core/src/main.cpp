#include "macgridfactory.h"
#include <iostream>

int main() {
    Eigen::AlignedBox<double,3> m(Eigen::Vector3d(0,0,0),Eigen::Vector3d(1,1,1));
    MACGridFactory<double,3> factory(Eigen::Vector3i(10,10,10),m);
    auto&& g = factory.create<mtao::CGrid>();
    std::cout << g.size() << std::endl;
    std::cout << g.N().transpose() << std::endl;
    std::cout << g.origin().transpose() << std::endl;
    std::cout << g.dx().transpose() << std::endl;

    auto&& g2 = factory.create<mtao::NGrid>();
    std::cout << g2.size() << std::endl;
    std::cout << g2.N().transpose() << std::endl;
    std::cout << g2.indexToWorld(0,0,0).transpose() << std::endl;
    std::cout << g2.dx().transpose() << std::endl;

    g2(0,0,0) = 1;
    auto&& v = g2.lerp(Eigen::Vector3d(0.01,0.01,0.01));
    std::cout << "Value" << v << std::endl;
    g2(0,0,0) = 1;
    g2(0,1,0) = 1;
    g2(0,1,1) = 1;
    g2(0,0,1) = 1;
    v = g2.lerp(Eigen::Vector3d(0.05,0.05,0.05));
    std::cout << "Value" << v << std::endl;
}
