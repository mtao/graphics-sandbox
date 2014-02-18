#include <iostream>
#include <fstream>
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h> 
#include <cstdlib>



#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
void readOBJ(std::istream & is, std::vector<openvdb::math::Vec3s>& verts, std::vector<openvdb::Vec4I>& poly)
{

    //temporary variables
    std::string line;
    std::string s;
    openvdb::math::Vec3s x;

    //expect lines to only be up to this long
    line.reserve(64);
    while( is.good() )
    {
        getline(is,line);
        if(line.length()>0)
        {
            boost::trim(line);
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if(type.compare("v") == 0)
            {
                //vertex
                ss >> x[0] >> x[1] >> x[2];
                if(!ss.eof())
                {
                    float w;
                    ss >> w;
                    x/=w;
                }
                verts.push_back(x);
            }
            else if(type[0]=='f')
            {
                openvdb::Vec4I f(
                openvdb::util::INVALID_IDX
                ,openvdb::util::INVALID_IDX
                ,openvdb::util::INVALID_IDX
                ,openvdb::util::INVALID_IDX);
                //Face (assumed to be triangular or quad)
                for( int i=0; i<4; ++i)
                {

                    if(ss.eof()) {
                        break;
                    }
                    ss >> s;
                    boost::tokenizer<> tok(s);

                    boost::tokenizer< >::iterator tok_it = tok.begin();
                    if(tok_it!=tok.end())
                    {//vertex
                        int val = atoi(tok_it->c_str());
                        if(val < 0)
                            val = verts.size() - val;
                        f[i]=val-1;

                    } else { 
                        continue;
                    }
                }
                poly.push_back(f);

            }
            else
            {
                //Comment or something we don't know how to parse

            }

        }

    }


}


void readOBJ(const std::string & str, std::vector<openvdb::math::Vec3s>& vert, std::vector<openvdb::Vec4I>& poly)
{
    std::ifstream is(str.c_str());
    if(!is.is_open()){return;}
    readOBJ(is,vert,poly);
}



int main(int argc, char * argv[]) {
    openvdb::initialize();
    std::vector<openvdb::math::Vec3s> verts;
    std::vector<openvdb::Vec4I> poly;
    std::cout << "Reading file: " << argv[1] << std::endl;
    readOBJ(argv[1], verts, poly);
    float scale = 1.0 / 100;
    if(argc == 3) {
        scale = atof(argv[2]);
    }
    std::cout << "Using scale: " << scale << std::endl;
    /*
    for(auto&& v: verts) {
        std::cout << v << std::endl;
    }
    for(auto&& f: poly) {
        std::cout << f << std::endl;
    }
    */


    auto&& tptr = openvdb::math::Transform::createLinearTransform(scale);
    std::cout << "Creating levelset" << std::endl;
    auto&& gptr = openvdb::tools::meshToLevelSet<openvdb::FloatGrid>(*tptr, verts, poly);

    std::cout << "Writing grid" << std::endl;
    openvdb::io::File file(std::string(argv[1]) + ".vdb");
    openvdb::GridPtrVec grids;
    grids.push_back(gptr);
    file.write(grids);
    file.close();









    return 0;
}
