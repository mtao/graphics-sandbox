#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <array>
#include <ostream>
typedef std::array<size_t,3> Triangle;
std::ostream & write(std::ostream & os,const Triangle & t);

#endif
