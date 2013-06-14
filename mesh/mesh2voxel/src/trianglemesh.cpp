#include "trianglemesh.h"

#include <fstream>
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
            triangles.push_back(Triangle{{
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
double TriangleMesh::intersect(const Triangle & t, const Eigen::ParametrizedLine<double,3> & ray)const {
    const TriangleMesh & tm = *this;
    auto&& l = tm[t[1]]-tm[t[0]];
    auto&& r = tm[t[2]]-tm[t[0]];
    Eigen::Vector3d orientation = l.cross(r).normalized();

    //std::cout << (ray.origin() - tm[t[0]]).transpose() << std::endl;
    Eigen::Hyperplane<double,3> p(orientation,tm[t[0]]);
    double x = ray.intersection(p);
//    std::cout << x << " ";
    if(x < 0) {
        return std::numeric_limits<double>::infinity();
    }
    Eigen::Vector3d ip = ray.pointAt(x);

    for(int i=1; i < 3; ++i) {
        auto&& o = tm[t[i]];
        auto&& dt = ip - o;
        auto&& d = tm[t[(i+1)%3]] - o;
        if(d.cross(dt).dot(orientation) < 0) {
            return std::numeric_limits<double>::infinity();
        }
    }
    double dist = ray.direction().dot(orientation);

    if(dist > 1e-5) {
        //std::cout << "+" << l.cross(r).normalized()(0);
        return (ip-ray.origin()).norm();
    } else if(dist < -1e-5) {
        //std::cout << "-" << l.cross(r).normalized()(0);
        return -(ip-ray.origin()).norm();
    }
    return std::numeric_limits<double>::infinity();

}
