#include <algorithm>


template <typename Iterable0, typename Iterable1, typename Iterable2>
void append(Iterable0 & a, const Iterable1 & b, const Iterable2 & c, std::function<typename Iterable0::value_type(const typename Iterable1::value_type&, const typename Iterable2::value_type&)> f) {
    typename Iterable0::iterator ait=a.begin();
    typename Iterable1::const_iterator bit=b.cbegin();
    typename Iterable2::const_iterator cit=c.cbegin();
    for(;ait != a.end(); ++ait, ++bit, ++cit) {
        *ait = f(*bit,*cit);
    }
}
template <typename Iterable0, typename Iterable1, typename Iterable2>
void append(Iterable0 & a, const Iterable1 & b, const Iterable2 & c, std::function<typename Iterable0::value_type(const typename Iterable1::value_type, const typename Iterable2::value_type)> f) {
    typename Iterable0::iterator ait=a.begin();
    typename Iterable1::const_iterator bit=b.cbegin();
    typename Iterable2::const_iterator cit=c.cbegin();
    for(;ait != a.end(); ++ait, ++bit, ++cit) {
        *ait = f(*bit,*cit);
    }
}


template <typename Iterable1, typename Iterable2>
void append(Iterable1 & a, const Iterable2 & b, std::function<void(typename Iterable1::value_type&, const typename Iterable2::value_type&)> f) {
    typename Iterable1::iterator ait=a.begin();
    typename Iterable2::const_iterator bit=b.cbegin();
    for(;ait != a.end(); ++ait, ++bit) {
        f(*ait,*bit);
    }
}
template <typename Iterable1, typename Iterable2>
void append(Iterable1 & a, const Iterable2 & b, std::function<typename Iterable1::value_type(const typename Iterable1::value_type&, const typename Iterable2::value_type&)> f) {
    append(a,a,b,f);
}
template <typename Iterable1, typename Iterable2>
void appendV(Iterable1 & a, const Iterable2 & b, std::function<typename Iterable1::value_type(typename Iterable1::value_type, typename Iterable2::value_type)> f) {
    append(a,a,b,f);
}

#include "lattice.h"
void foreachLevelset(Levelset & ls, const Eigen::Vector3i & begin, Eigen::Vector3i end, const std::function<void(const Eigen::Vector3i& pos, double & value)> & f) {
    Eigen::Vector3i ijk = (begin.array() > 0).select(begin,0);
    end = (end.array() < ls.N().array()).select(end,ls.N().array()-1);
    int& i = ijk(0);
    int& j = ijk(1);
    int& k = ijk(2);
    for(; i < end(0); ++i) {
        for(; j < end(1); ++j) {
            for(; k < end(2); ++k) {
                f(ijk,ls(ls.ijk2idx(ijk)));
            }
        }
    }
}

