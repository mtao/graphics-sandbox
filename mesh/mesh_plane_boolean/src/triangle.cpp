#include <fstream>
#include "triangle.h"

std::ostream & write(std::ostream & os,const Triangle & t) {
    os << "f";
    os << " " << t[0]+1;
    os << " " << t[1]+1;
    os << " " << t[2]+1;
    os << std::endl;
    return os;
}
