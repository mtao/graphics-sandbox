#include <iostream>
#include <coord.h>
#include <lattice.h>

int main(int argc, char * argv[]) {
    typedef Coord<3> Coord3;
    Coord3 c({1,2,3});
    Coord3 d({2,2,2});
    std::cout << (c+d).transpose() << std::endl;
    std::cout << c.transpose() << std::endl;

    DenseLattice(5,5,5) a;
    
}
