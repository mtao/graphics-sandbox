#include "trianglemesh.h"

#include <fstream>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <openvdb/util/Util.h>
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

int readFirstIntFromIterator(tokenizer::iterator & it) {
    static boost::char_separator<char> sep("//");
    tokenizer tok(*it,sep);
    it++;
    return atoi(tok.begin()->c_str())-1;

}
double readFirstDoubleFromIterator(tokenizer::iterator & it) {
    double d = 50*atof(it->c_str());
    it++;
    return d;

}

TriangleMesh TriangleMesh::readObj(const std::string & path) {
    std::ifstream ifs(path.c_str());
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
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
            triangles.push_back(Triangle(
                    readFirstIntFromIterator(it)
                    ,
                    readFirstIntFromIterator(it)
                    ,
                    readFirstIntFromIterator(it)
                    ));
        } else if(*it == "v") {
            it++;

            vertices.push_back(Vector3(
                        readFirstDoubleFromIterator(it)
                        ,
                        readFirstDoubleFromIterator(it)
                        ,
                        readFirstDoubleFromIterator(it)
                        ));
            std::cout << vertices.back() << std::endl;

        }
    }
    return {vertices,triangles};
}
