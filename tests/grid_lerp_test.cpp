#include "grid/grid.h"
#include <iostream>


void test1() {
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
    assert(std::abs(c.lerp(center)-2) < 1e-5);
    assert(g1.lerp(center) == c.lerp(center));
}
void test2() {
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
    assert(std::abs(c.lerp(center)-2.5) < 1e-5);
    assert(g.lerp(center) == c.lerp(center));
}
void test3() {
    mtao::Grid<float,3> g;
    mtao::Vec3i N;
    N(0) = 2;
    N(1) = 2;
    N(2) = 2;
    g.reshape(N);
    g(0,0,0) = 1;
    g(0,1,0) = 2;
    g(0,0,1) = 3;
    g(0,1,1) = 4;
    g(1,0,0) = 5;
    g(1,1,0) = 6;
    g(1,0,1) = 7;
    g(1,1,1) = 8;
    mtao::lerp::Cube<float,3> c(g,mtao::Vec3i::Zero());
    mtao::Vec3f center;
    center[0] = 0.5;
    center[1] = 0.5;
    center[2] = 0.5;
    std::cout << c.lerp(center) << std::endl;
    assert(std::abs(c.lerp(center)-4.5) < 1e-5);
    assert(g.lerp(center) == c.lerp(center));
}
int main(int argc, char * argv[]) {
    test1();
    test2();
    test3();
    return 0;
}
