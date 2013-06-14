#include "trianglemesh.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
void TriangleMesh::writeObj(const std::string & path) {
    std::ofstream of(path.c_str());

    if(!of.is_open()) return;
    for(auto&& t: triangles) {
        write(of,t);
    }
    for(auto&& v: vertices) {
        of << "v " << v.transpose() << std::endl;
    }
}

size_t readFirstIntFromIterator(tokenizer::iterator & it) {
    static boost::char_separator<char> sep("//");
    tokenizer tok(*it,sep);
    it++;
    return atoi(tok.begin()->c_str())-1;

}
double readFirstDoubleFromIterator(tokenizer::iterator & it) {
    double d = atof(it->c_str());
    it++;
    return d;

}

TriangleMesh TriangleMesh::readObj(const std::string & path) {
    std::ifstream ifs(path.c_str());
    std::vector<Vector3> vertices;
    std::set<Triangle> triangles;
    while(ifs) {
        std::string type;
        std::string line;
        getline(ifs,line);
        if(line.length() == 0) continue;
        boost::char_separator<char> sep(" ");
        tokenizer tok(line,sep);
        tokenizer::iterator it = tok.begin();
        if(*it == "f") {
            it++;
            triangles.insert(Triangle{{
                    readFirstIntFromIterator(it)
                    ,
                    readFirstIntFromIterator(it)
                    ,
                    readFirstIntFromIterator(it)
                    }});
        } else if(*it == "v") {
            it++;

            vertices.push_back(Vector3(
                        readFirstDoubleFromIterator(it)
                        ,
                        readFirstDoubleFromIterator(it)
                        ,
                        readFirstDoubleFromIterator(it)
                        ));

        }
    }
    return {vertices,triangles};
}
