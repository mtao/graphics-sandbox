#ifndef BBOXITERATOR_H
#define BBOXITERATOR_H
#include "types.h"

namespace mtao {
    template <int Dim>
    class BBoxIterator {
        public:
            typedef typename mtao::template dim_types<Dim> _dt;
            typedef typename _dt::Veci Veci;
            typedef typename _dt::BBoxi BBoxi;
            BBoxIterator(const BBoxi& bbox, const Veci& init): m_bbox(bbox), m_pos(init) {}
            BBoxIterator(const BBoxi& bbox): m_bbox(bbox), m_pos(bbox.min()) {}
            
            const Veci& operator*() {return m_pos;}
            operator bool() const {return m_bbox.contains(m_pos);}
            const Veci& operator++() {
                for(int i = Dim-1; i >= 0; --i) {
                    if(++m_pos(i) > m_bbox.max()(i)) {
                        m_pos(i)=m_bbox.min()(i);
                        continue;
                    } else {
                        break;
                    }
                }
                return m_pos;
            }
            bool operator==(const BBoxIterator& other) const {
                return (
                        m_bbox.min() == other.m_bbox.min() 
                        &&m_bbox.max() == other.m_bbox.max() 
                        && m_pos == other.m_pos);
            }
            bool operator!=(const BBoxIterator& other) const {
                return !((*this) == other);
            }
        private:
            BBoxi m_bbox;
            Veci m_pos;


    };
    template <int Dim>
        class IterableBBox: public mtao::template dim_types<Dim>::BBoxi {
            public:
                typedef typename mtao::template dim_types<Dim> _dt;
                typedef typename _dt::BBoxi BBoxi;
                typedef typename _dt::Veci Veci;
                typedef BBoxIterator<Dim> iterator_type;
                template <typename Vec1, typename Vec2>
                    IterableBBox(const Vec1&a, const Vec2&b): BBoxi(a,b) {
                        itmax = this->max().array() - 1;
                    itmax(Dim-1) +=1;
                }
                IterableBBox(const BBoxi& b): BBoxi(b) {
                    itmax = this->max().array() - 1;
                    itmax(Dim-1) +=1;
                }



            iterator_type begin() {return iterator_type(BBoxi(this->min(),itmax));}
            iterator_type end() {
                return iterator_type(BBoxi(this->min(),itmax),itmax);
            }
            private:
            Veci itmax;
    };
};
#endif
