#ifndef LATTICE_ITERATORS
#define LATTICE_ITERATORS
template<>
ActiveLatticeIterator<DenseLattice> & ActiveLatticeIterator<DenseLattice>::
operator++();
template<>
ActiveLatticeIterator<NaiveSparseLattice> & ActiveLatticeIterator<NaiveSparseLattice>::
operator++();
template<>
ActiveLatticeIterator<SparseLattice> & ActiveLatticeIterator<SparseLattice>::
operator++();
template <>
inline const Index & ActiveLatticeIterator<DenseLattice>::
index() const {
    return m_ind;
}

template <>
inline int ActiveLatticeIterator<DenseLattice>::
value() const {
    return *m_it;
}

template <>
inline ActiveLatticeIterator<DenseLattice>::
operator bool() {
    //std::cout << "Distance:" << (m_it-m_obj.cbegin()) << std::endl;
    return m_it != m_obj.cend();
}

template<>
inline const Index & ActiveLatticeIterator<NaiveSparseLattice>::
index() const {
    return m_it->first;
}

template<>
inline int ActiveLatticeIterator<NaiveSparseLattice>::
value() const {
    return m_it->second;
}

template<>
inline ActiveLatticeIterator<NaiveSparseLattice>::
operator bool() {
    return m_it != m_obj.cend();
}
template<>
inline const Index & ActiveLatticeIterator<SparseLattice>::
index() const {
    return m_ind;
}

template<>
inline int ActiveLatticeIterator<SparseLattice>::
value() const {
    return m_inner_it->second;
}

template<>
inline ActiveLatticeIterator<SparseLattice>::
operator bool() {
    return m_outer_it != m_obj.cend();
}


#endif
