#include "mesh/trianglemesh.h"
#include "util/matrix_iterator.h"

#include <fstream>
#include <sstream>
#include <boost/tokenizer.hpp>

namespace mtao { namespace geometry { namespace mesh {

    void TriangleMesh::writeObj(const std::string & path) {
        std::ofstream of(path.c_str());

        if(!of.is_open()) return;
        for(auto&& f: mtao::geometry::util::col_iterator_container(faces())) {
            of << "f " << f.transpose() << std::endl;
        }
        for(auto&& v: mtao::geometry::util::col_iterator_container(vertices())) {
            of << "v " << v.transpose() << std::endl;
        }
    }


    std::shared_ptr<TriangleMesh> TriangleMesh::readObj(const std::string & path) {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        std::ifstream ifs(path);


        auto readFirstIntFromIterator = [](tokenizer::iterator & it) -> size_t {
            static boost::char_separator<char> sep("//");
            tokenizer tok(*it,sep);
            int v = atoi(tok.begin()->c_str())-1;
            it++;
            return v;

        };
        auto readFirstDoubleFromIterator = [](tokenizer::iterator & it) -> double {
            double d = atof(it->c_str());
            it++;
            return d;

        };



        std::string line;
        int nverts=0;
        int nfaces=0;
        while (std::getline(ifs, line)) {
            if(line.length() == 0) continue;
            boost::char_separator<char> sep(" ");
            tokenizer tok(line,sep);
            tokenizer::iterator it = tok.begin();
            if(*it == "f") {
                nfaces++;
            } else if(*it == "v") {
                nverts++;
            }
        }


        Eigen::Matrix<Scalar,3,Eigen::Dynamic> vertices(3,nverts);
        Eigen::Matrix<int,3,Eigen::Dynamic> faces(3,nfaces);



        int vind = 0;
        int find = 0;

         ifs = std::ifstream(path);
        //ifs.seekg(0, std::ios::beg);
        //ifs.seekg(0);



        while(getline(ifs,line)) {
            if(line.length() == 0) continue;
            boost::char_separator<char> sep(" ");
            tokenizer tok(line,sep);
            tokenizer::iterator it = tok.begin();
            if(*it == "f") {
                it++;
                auto&& f = faces.col(find++);
                f = Eigen::Vector3i(
                        readFirstIntFromIterator(it)
                        ,
                        readFirstIntFromIterator(it)
                        ,
                        readFirstIntFromIterator(it)
                        );
            } else if(*it == "v") {
                it++;
                auto&& t = vertices.col(vind++);



                t = Vec3(
                        readFirstDoubleFromIterator(it)
                        ,
                        readFirstDoubleFromIterator(it)
                        ,
                        readFirstDoubleFromIterator(it)
                        );

            }
        }
        return std::make_shared<TriangleMesh>(vertices,faces);
    }
    /*
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
    */
}}}
