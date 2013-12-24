#include "kdop.h"

#include <iostream>


void twoPtCube() {
    mtao::KDOP<double,2> kdopd2;
    kdopd2.expand(mtao::Vec2d(0,0));
    std::cout << kdopd2.min().transpose() << " | " << kdopd2.max().transpose() << std::endl;
    kdopd2.expand(mtao::Vec2d(1,1));
    std::cout << kdopd2.min().transpose() << " | " << kdopd2.max().transpose() << std::endl;
    mtao::KDOP<double,3> kdopd3;
    kdopd3.expand(mtao::Vec3d(0,0,0));
    std::cout << kdopd3.min().transpose() << " | " << kdopd3.max().transpose() << std::endl;
    kdopd3.expand(mtao::Vec3d(1,1,0));
    std::cout << kdopd3.min().transpose() << " | " << kdopd3.max().transpose() << std::endl;
    kdopd3.expand(mtao::Vec3d(1,1,1));
    std::cout << kdopd3.min().transpose() << " | " << kdopd3.max().transpose() << std::endl;
}
void kdopExpand() {
    mtao::KDOP<double,3> kdopd3;
    kdopd3.expand(mtao::Vec3d(0,0,0));
    std::cout << kdopd3.min().transpose() << " | " << kdopd3.max().transpose() << std::endl;
    kdopd3.expand(mtao::Vec3d(1,1,1));
    std::cout << kdopd3.min().transpose() << " | " << kdopd3.max().transpose() << std::endl;
    mtao::KDOP<double,3> kdopd32;
    kdopd32.expand(mtao::Vec3d(4,2,2));
    std::cout << kdopd32.min().transpose() << " | " << kdopd32.max().transpose() << std::endl;
    kdopd32.expand(kdopd3);
    std::cout << kdopd32.min().transpose() << " | " << kdopd32.max().transpose() << std::endl;
}

int main(int argc, char * argv[]) {
    twoPtCube();
    kdopExpand();
    return 0;


}
