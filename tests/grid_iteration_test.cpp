#include "grid/grid.h"
#include <iostream>



int test_2d() {
    auto N = mtao::Vec2i(13,19);
    mtao::Grid<char,2> g(mtao::Vec2i(13,19));
    for(int i=0; i < g.size(); ++i) {
        assert(i == g.coord2idx(g.idx2coord(i)));
//        std::cout << i << ") " << g.coord2idx(g.idx2coord(i)) << " [" << g.idx2coord(i).transpose() << "]" << std::endl;
    }

    for(int i=0; i < N(0); ++i) {
        for(int j=0; j < N(1); ++j) {
                auto v = mtao::Vec2i(i,j);
                assert(g.coord2idx(i,j) == g.coord2idx(v));
        }
    }


    return 0;

}

int test_3d() {
    auto N = mtao::Vec3i(7,13,19);
    mtao::Grid<char,3> g(N);
    for(int i=0; i < g.size(); ++i) {
        assert(i == g.coord2idx(g.idx2coord(i)));
//        std::cout << i << ") " << g.coord2idx(g.idx2coord(i)) << " [" << g.idx2coord(i).transpose() << "]" << std::endl;
    }
    for(int i=0; i < N(0); ++i) {
        for(int j=0; j < N(1); ++j) {
            for(int k=0; k < N(2); ++k) {
                auto v = mtao::Vec3i(i,j,k);
                assert(g.coord2idx(i,j,k) == g.coord2idx(v));
            }
        }
    }
    return 0;
}

int main(int argc,char* argv[]) {
    test_2d();
    test_3d();
}
