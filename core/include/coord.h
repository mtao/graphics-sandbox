#ifndef COORD_H
#define COORD_H
#include <Eigen/Dense>

template <int N>
class Coord: public Eigen::Matrix<int,N,1> {
    public:
        //Lexicographical ordering
        inline bool operator<(const Coord& other) const;
        inline bool operator<=(const Coord& other) const;
        inline bool operator>(const Coord& other) const;
        inline bool operator>=(const Coord& other) const;
};


template <int N>
inline bool Coord<N>::operator<(const Coord& other) const {
    for(int i=0; i < N; ++i) {
        auto&& me = (*this)(i);
        auto&& ot = other(i);
        if(me < ot) {
            return true;
        } else if( me == ot) {
            continue;
        } else {
            return false;
        }
    }
    return false;
}
template <int N>
inline bool Coord<N>::operator>(const Coord& other) const {
    return other < *this;
}

template <int N>
inline bool Coord<N>::operator<=(const Coord& other) const {
    return !(other < *this);
}
template <int N>
inline bool Coord<N>::operator>=(const Coord& other) const {
    return !(*this < other);
}

#endif
