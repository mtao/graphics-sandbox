
template <>
struct LatticeIteratorStorage<DenseLattice>{
    typedef DenseLattice LatticeType;
    LatticeIteratorStorage(const LatticeType & obj): m_obj(obj) {}
    const LatticeType & m_obj;
    std::vector<int>::const_iterator m_it;
    Index m_ind;
};
template <>
struct LatticeIteratorStorage<NaiveSparseLattice>{
    typedef NaiveSparseLattice LatticeType;
    LatticeIteratorStorage(const LatticeType & obj): m_obj(obj) {}
    const LatticeType & m_obj;
    std::map<Index,int>::const_iterator m_it;
};
template <>
struct LatticeIteratorStorage<SparseLattice>{
    typedef SparseLattice LatticeType;
    LatticeIteratorStorage(const LatticeType & obj): m_obj(obj) {}
    const LatticeType & m_obj;
    std::map<Index,int>::const_iterator m_it;
    std::vector<std::map<size_t,int> >::const_iterator m_outer_it;
    std::map<size_t,int>::const_iterator m_inner_it;
    Index m_ind;
    inline void setIndex() {
        m_ind.k = m_inner_it->first;
        size_t ind = m_outer_it - m_obj.cbegin();
        m_ind.j = ind/m_obj.NI();
        m_ind.i = ind%m_obj.NI();
    }
};

template <typename DerivedType>
Lattice<DerivedType>::
    Lattice(size_t i, size_t j, size_t k, unsigned int default_active)
    : m_N(i,j,k)
      , m_active_count(default_active)
{
}

template <typename DerivedType>
inline void Lattice<DerivedType>::
resize(size_t i, size_t j, size_t k) {
    m_N = mtao::Coord3(i,j,k);
}

//Accessors
template <typename DerivedType>
inline int & Lattice<DerivedType>::
operator()(size_t i, size_t j, size_t k) {
    return derived()(i,j,k);
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
operator()(size_t i, size_t j, size_t k) const {
    return derived()(i,j,k);
}
template <typename DerivedType>
inline int & Lattice<DerivedType>::
operator()(const mtao::Coord3 & c) {
    return this->operator()(c.x(),c.y(),c.z());
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
operator()(const mtao::Coord3 & c) const {
    return this->operator()(c.x(),c.y(),c.z());
}

//Adders
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(size_t i, size_t j, size_t k) {
    int& curval = derived()(i,j,k);
    if(curval == -1)
        curval = m_active_count++;
    return curval;
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(const mtao::Coord3 & c) {
    return add(c.x(),c.y(),c.z());
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(size_t i, size_t j, size_t k, bool & b) {
    int& curval = derived()(i,j,k);
    if((b = (curval == -1))) 
        curval = m_active_count++;
    return curval;
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(const mtao::Coord3 & c, bool & b) {
    return add(c.x(),c.y(),c.z(),b);
}

template <typename DerivedType>
inline void Lattice<DerivedType>::
reset() {
    derived().reset();
}

template <typename DerivedType>
inline std::map<int,int> Lattice<DerivedType>::
compactifyIndices() {
    derived().compactifyIndices();
}




template <typename DerivedType>
template <typename Object>
void Lattice<DerivedType>::
remap(const std::map<int,int> & newmap, std::vector<Object> & vec) {
    //TODO: This can be done in place at the cost of having to check for cycles.
    //Already transfered components could be represented by a std::vector<bool>
    std::vector<Object> newvec(newmap.size());
    for(const std::pair<int,int> & m: newmap) {
        newvec[m.second] = vec[m.first];
    }
    vec = std::move<std::vector<Object> >(newvec);

}

//Index Accessors
inline Index DenseLattice::
_index2Coord(int idx) const {
    mtao::Coord val;
    val.z() = idx%NJ();
    idx/=NJ();
    val.y() = idx%NI();
    val.x() = idx/NI();

    return val;
}
inline int DenseLattice::
_coord2Index(int i, int j, int k) const {
    return k + NJ() * (j + NI() * (i));
}
inline int DenseLattice::
_coord2Index(const mtao::Coord3 & c) const {
    return coord2Index(c.x(),c.y(),c.z());
}

inline int & DenseLattice::
operator()(int i, int j, int k) {
    return m_data[_coord2Index(i,j,k)];
}
inline int DenseLattice::
operator()(int i, int j, int k) const {
    return m_data[_coord2Index(i,j,k)];
}




inline int & NaiveSparseLattice::
operator()(size_t i, size_t j, size_t k) {
    //try to insert the value with default -1 for unset index
    return m_data.insert(std::pair<mtao::Coord3,int>(mtao::Coord3{i,j,k},-1)).first->second;
}

inline int NaiveSparseLattice::
operator()(size_t i, size_t j, size_t k) const {
    std::map<mtao::Coord3,int>::const_iterator result = m_data.find(mtao::Coord3{i,j,k});
    if(result == m_data.end()) return -1;
    return result->second;
}

inline std::map<size_t,int> & SparseLattice::
i_ji_i(size_t i, size_t j) {
    return this->operator[](j*NI()+i);
}
inline const std::map<size_t,int> & SparseLattice::
i_ji_i(size_t i, size_t j) const {
    return this->operator[](j*NI()+i);
}
inline int & SparseLattice::
operator()(size_t i, size_t j, size_t k) {
    //try to insert the value with default -1 for unset index
    return i_ji_i(i,j).insert(std::pair<size_t,int>(k,-1)).first->second;
}

inline int SparseLattice::
operator()(size_t i, size_t j, size_t k) const {
    const std::map<size_t,int> & kmap = i_ji_i(i,j);
    std::map<size_t,int>::const_iterator result = kmap.find(k);
    if(result == kmap.end()) return -1;
    return result->second;
}
