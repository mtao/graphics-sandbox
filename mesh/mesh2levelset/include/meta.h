#include <algorithm>


template <typename Iterable1, typename Iterable2>
void append(Iterable1 & a, const Iterable2 & b, std::function<void(typename Iterable1::value_type&, const typename Iterable2::value_type&)> f) {
    typename Iterable1::iterator ait=a.begin();
    typename Iterable2::const_iterator bit=b.cbegin();
    for(;ait != a.end(); ++ait, ++bit) {
        f(*ait,*bit);
    }
}


