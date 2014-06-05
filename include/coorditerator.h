#ifndef COORD_ITERATOR_H
#define COORD_ITERATOR_H
#include "coord.h"

namespace mtao {
    //Note: Higher indices change the fastest
    template <int Dim>
    class CoordIterator {
        public:
            typedef dim_types<Dim> _dt;
            typedef typename _dt::Coord Coord;
            typedef typename _dt::CoordBBox CoordBBox;
            CoordIterator() {}
            CoordIterator(const CoordBBox& bb): cur(bb.min()), bbox(bb) {}
            CoordIterator(const Coord& max): cur(Coord::Zero()), bbox(Coord::Zero(),max) {}
            CoordIterator(const CoordIterator& other) = default;
            CoordIterator(CoordIterator&& other) = default;

            const Coord& operator*(){ return cur;}

            operator bool() const {
                return cur <= (bbox.max() - Coord::Ones());
            }
            CoordIterator& operator++() {
                auto&& m = bbox.min();
                auto&& M = bbox.max();
                for(int i = Dim-1; i > 0; --i) {
                    if(++cur(i) >= M(i)) {
                        cur(i)=m(i);
                        continue;
                    } else {
                        return *this;
                    }
                }
                //Make sure that the 0th item is unbounded so looping works
                ++cur(0);
                return *this;
            }
        private:
            Coord cur;
            CoordBBox bbox;
    };
}

#endif
