#include "kdop.h"

#include <iostream>

void rayIntersection() {
    mtao::KDOP<double,2> kdop;
    kdop.expand(mtao::Vec2d(0,0));
    kdop.expand(mtao::Vec2d(1,1));
    mtao::Vec2d d(0,1);
    mtao::Vec2d o(.5,-4);

    std::cout << "Should intersect: " << kdop(o,d) << std::endl;
    d(1) = -1;
    std::cout << "Should not intersect: " << kdop(o,d) << std::endl;
    d=mtao::Vec2d(0,1);
    o=mtao::Vec2d(.5,-4);
    std::cout << "Should not intersect: " << kdop(o,d,0,1) << std::endl;
    std::cout << "Should not intersect: " << kdop(o,d,0,2) << std::endl;
    std::cout << "Should not intersect: " << kdop(o,d,0,3) << std::endl;
    std::cout << "Should intersect: " << kdop(o,d,0,4) << std::endl;

}

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
    rayIntersection();
    return 0;


}
