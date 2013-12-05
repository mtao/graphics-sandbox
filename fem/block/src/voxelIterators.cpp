#include "include/lattice.h"




template<>
ActiveLatticeIterator<DenseLattice> & ActiveLatticeIterator<DenseLattice>::
operator++() {
    //Find first nonzero value

    while(++m_it != m_obj.cend()) {
        if(value() != -1)
            break;
    }

    m_ind = m_obj.idx2ijk(m_it - m_obj.cbegin());
    return *this;
}
template<>
ActiveLatticeIterator<NaiveSparseLattice> & ActiveLatticeIterator<NaiveSparseLattice>::
operator++() {
    //Find first nonzero value

    while(++m_it != m_obj.cend()) {
        if(value() != -1)
            break;
    }

    return *this;
}
template<>
ActiveLatticeIterator<SparseLattice> & ActiveLatticeIterator<SparseLattice>::
operator++() {
    //Find first nonzero value

    //If we can move inner, try moving
    if(m_inner_it != m_outer_it->cend()) {
        ++m_inner_it;
    }
    //If we're now at the end of inner, move outer
    if(m_inner_it == m_outer_it->cend()) {
        do{
        ++m_outer_it;
        if(!*this) return *this;
        } while(m_outer_it->size() == 0);
        m_inner_it = m_outer_it->cbegin();
        setIndex();
        return *this;
    } else {
        setIndex();
        return *this;
    }
    /*
    while(*this) {
        if(m_outer_it->size() > 0) {
            do {
                if(value() != -1)
                {
                    setIndex();
                    return *this;
                }

            } while (++m_inner_it != m_outer_it->cend());
        }
        ++m_outer_it;
        m_inner_it = m_outer_it->cbegin();
    }
    */
    std::cerr << "ITERATOR WENT PAST VALID AREA" << std::endl;
    assert(false);

    return *this;
}
template<>
ActiveLatticeIterator<DenseLattice>::
ActiveLatticeIterator(const DenseLattice & obj): Storage(obj)
{
    m_it = obj.cbegin();
    if(m_it == obj.cend()) return;
    if(value() == -1)
        operator++();
    m_ind=obj.idx2ijk(m_it-m_obj.cbegin());
}

template<>
ActiveLatticeIterator<NaiveSparseLattice>::
ActiveLatticeIterator(const NaiveSparseLattice & obj): Storage(obj)
{
    m_it = obj.cbegin();
    if(m_it == obj.cend()) return;
    if(value() == -1)
        operator++();
}

template<>
ActiveLatticeIterator<SparseLattice>::
ActiveLatticeIterator(const SparseLattice & obj): Storage(obj)
{
    m_outer_it = obj.cbegin();
    if(m_outer_it == obj.cend()) return;
    for(;*this && m_outer_it->size() == 0; ++m_outer_it);
    if(!*this) {
        return;
    }
    m_inner_it = m_outer_it->cbegin();

    setIndex();
    if(value() == -1)
        operator++();
}
