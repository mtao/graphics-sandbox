#include "grid/embeddedgrid.h"
#include <iostream>


void test1() {
    mtao::EmbeddedGrid<float,2> g(mtao::Vec2i(20,20), mtao::Vec2f(0,0), mtao::Vec2f(0.05,0.05));
    g(10,10) = 1;
    g(11,10) = 2;
    g(10,11) = 3;
    g(11,11) = 4;
    assert(std::abs(g.wlerp(mtao::Vec2f(.525,.525)) - 2.5) < 1e-5);

}

int main(int argc, char * argv[]) {
    test1();
    return 0;
}
