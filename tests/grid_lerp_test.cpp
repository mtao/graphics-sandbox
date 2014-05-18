#include "grid/grid.h"
#include <iostream>

int main(int argc, char * argv[]) {

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
    
}
