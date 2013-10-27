#ifndef COORD_H
#define COORD_H
#include <Eigen/Dense>

template <int N>
class Coord: public Eigen::Matrix<int,N,1> {
    public:
        typedef Eigen::Matrix<int,N,1> Base;
        Coord();
        Coord(const int& a);
        Coord(const int& a, const int& b);
        Coord(const int& a, const int& b, const int& c);
        Coord(const int& a, const int& b, const int& c, const int& d);
        Coord(const std::initializer_list<int>& list);
        //Lexicographical ordering
        inline bool operator<(const Coord& other) const;
        inline bool operator<=(const Coord& other) const;
        inline bool operator>(const Coord& other) const;
        inline bool operator>=(const Coord& other) const;
    protected:
        using Base::m_storage;
};

template <int N>
Coord<N>::Coord() {
}
template <int N>
Coord<N>::Coord(const int& a) {
    m_storage.data()[0] = a;
}
template <int N>
Coord<N>::Coord(const int& a, const int& b) {
    m_storage.data()[0] = a;
    m_storage.data()[1] = b;
}
template <int N>
Coord<N>::Coord(const int& a, const int& b, const int& c) {
    m_storage.data()[0] = a;
    m_storage.data()[1] = b;
    m_storage.data()[2] = c;
}
template <int N>
Coord<N>::Coord(const int& a, const int& b, const int& c, const int& d) {
    m_storage.data()[0] = a;
    m_storage.data()[1] = b;
    m_storage.data()[2] = c;
    m_storage.data()[3] = d;
}
template <int N>
Coord<N>::Coord(const std::initializer_list<int>& list) {
    eigen_assert(list.size() == N);
    std::copy(list.begin(), list.end(), m_storage.data());
}

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
