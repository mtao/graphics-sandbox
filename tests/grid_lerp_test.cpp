#include "grid/grid.h"
#include <iostream>


int test1() {
    mtao::Grid<float,1> g1;
    mtao::Vec1i N;
    N(0) = 2;
    g1.reshape(N);
    g1(0) = 1;
    g1(1) = 3;
    mtao::lerp::Cube<float,1> c(g1,mtao::Vec1i::Zero());
    mtao::Vec1f center;
    center[0] = 0.5;
    std::cout << c.lerp(center) << std::endl;
    return std::abs(c.lerp(center)-2) > 1e-5;
}
int test2() {
    mtao::Grid<float,2> g;
    mtao::Vec2i N;
    N(0) = 2;
    N(1) = 2;
    g.reshape(N);
    g(0,0) = 1;
    g(1,0) = 2;
    g(0,1) = 3;
    g(1,1) = 4;
    mtao::lerp::Cube<float,2> c(g,mtao::Vec2i::Zero());
    mtao::Vec2f center;
    center[0] = 0.5;
    center[1] = 0.5;
    std::cout << c.lerp(center) << std::endl;
    return std::abs(c.lerp(center)-2.5) > 1e-5;
}
int main(int argc, char * argv[]) {
    int ret = 0;
    ret |= test1();
    std::cout << ret << std::endl;
    ret |= test2();
    std::cout << ret << std::endl;
    return ret;
}
