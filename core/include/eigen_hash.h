#ifndef EIGEN_HASH_H
#define EIGEN_HASH_H
#include "types.h"
#include <functional>
namespace internal {
constexpr std::size_t bits_allocated(int num) {
    return sizeof(size_t)/num;
}
}
namespace std {
    template <typename T, int Dim>
        struct hash<typename mtao::dim_types<Dim>::template scaler_types<T>::Vec > {
            public:
                    static const std::size_t shift_num = bits_allocated(Dim);
                    static const std::size_t mask = (1 << (shift_num+1)) - 1;
                std::size_t operator()(typename mtao::dim_types<Dim>::template scaler_types<T>::Vec const& v) const{
                    std::size_t ret = 0; 
                    for(int i=0; i < Dim; ++i) {
                        ret << shift_num;
                        ret |= mask & std::size_t(v(i));

                    }
                    return ret;
                }
        };
}

#endif
