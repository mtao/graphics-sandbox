#include "mesh/loaders/objloader.h"
#include "linereader.hpp"
#include <vector>
namespace mtao {
void OBJLoader::open(const std::string& filename) {
    mtao::FileParser fp(filename);
    std::array<double,3> t;
    std::array<std::string,3> s;
    std::array<unsigned int,3> i;
//    std::array<int,3> uv;
    for(auto&& line: fp) {
        char v;
        std::stringstream ss(line);
        ss >> v;
        switch(v) {
            case 'v':
                ss >> t[0] >> t[1] >> t[2];
                add_vertex(t[0],t[1],t[2]);
                break;
            case 'f':
                ss >> s[0] >> s[1] >> s[2];
                for(size_t m=0; m < 3; ++m) {
                    auto&& str = s[m];
                    auto it = str.begin();
                    for(; it != str.end(); ++it) {
                        if(*it == '/') {
                            break;
                        }
                    }
                    std::stringstream ss2(str.substr(0,std::distance(str.begin(),it)));
                    ss2 >> i[m];

                }
                add_triangle(i[0],i[1],i[2]);
                break;
            default:
                break;
        }
    }
}
void OBJLoader::save(const std::string& str) {
}

}
