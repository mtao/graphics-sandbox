#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <array>
#include "eigen_boost_serialization.hpp"

namespace boost{namespace serialization{

    template <class Archive, typename _Scalar, std::size_t _Size>
        void serialize(Archive & ar, std::array<_Scalar,_Size> & m, const unsigned int version) {
            ar & boost::serialization::make_array(&(m[0]),_Size);
        }
}}
#endif
