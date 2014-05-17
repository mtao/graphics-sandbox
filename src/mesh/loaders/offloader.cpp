#include "mesh/loaders/offloader.h"
#include "linereader.hpp"
#include <vector>
#include <iostream>
namespace mtao {
void OFFLoader::open(const std::string& filename) {
    mtao::FileParser fp(filename);
    std::array<double,3> t;
    std::array<size_t,3> s;
//    std::array<int,3> uv;
    auto lineiter = fp.begin();
    if(*lineiter != "OFF") {
        std::cout << "Not a vaild OFF file" << std::endl;
        std::cout << *lineiter << std::endl;
    }
    ++lineiter;

    int nV,nF,nE;
    std::stringstream ss(*lineiter);
    ss >> nV >> nF >> nE;


    for(int i=0; i < nV; ++i,++lineiter) {
        std::stringstream ss(*lineiter);
        ss >> t[0] >> t[1] >> t[2];
        add_vertex(t[0],t[1],t[2]);

    }
    for(int i=0; i < nV; ++i,++lineiter) {
        std::stringstream ss(*lineiter);
        int verts;
        ss >> verts;
        ss >> s[0] >> s[1];
        for(int j=2; j < verts; ++j) {
            ss >> s[2];
            add_triangle(s[0],s[1],s[2]);
            s[1] = s[2];
        }


    }

}
void OFFLoader::save(const std::string& str) {
}

}
