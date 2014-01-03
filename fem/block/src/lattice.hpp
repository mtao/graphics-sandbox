
/*
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
    std::vector<std::map<int,int> >::const_iterator m_outer_it;
    std::map<int,int>::const_iterator m_inner_it;
    Index m_ind;
    inline void setIndex() {
        m_ind.k = m_inner_it->first;
        int ind = m_outer_it - m_obj.cbegin();
        m_ind.j = ind/m_obj.NI();
        m_ind.i = ind%m_obj.NI();
    }
};
*/

template <typename DerivedType>
Lattice<DerivedType>::
    Lattice(int i, int j, int k, unsigned int default_active)
    : m_N(i,j,k)
      , m_active_count(default_active)
{
}

template <typename DerivedType>
inline void Lattice<DerivedType>::
resize(int i, int j, int k) {
    m_N = mtao::Coord3(i,j,k);
}

//Accessors
template <typename DerivedType>
inline int Lattice<DerivedType>::
operator()(int i, int j, int k) const {
    return derived()(i,j,k);
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
operator()(const mtao::Coord3 & c) const {
    return this->operator()(c.x(),c.y(),c.z());
}

//Adders
template <typename DerivedType>
inline bool Lattice<DerivedType>::
addRef(int& v) {
    if(v == -1) {
        v = m_active_count++;
        return true;
    }
    return false;
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(int i, int j, int k) {
    return derived().add(i,j,k);
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(int i, int j, int k, bool& b) {
    return derived().add(i,j,k,b);
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(const mtao::Coord3 & c) {
    return this->add(c.x(),c.y(),c.z());
}
template <typename DerivedType>
inline int Lattice<DerivedType>::
add(const mtao::Coord3 & c, bool& b) {
    return this->add(c,b);
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
inline mtao::Coord3 DenseLattice::
_index2Coord(int idx) const {
    mtao::Coord3 val;
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
    return _coord2Index(c.x(),c.y(),c.z());
}

inline int& DenseLattice::
getRef(int i, int j, int k) {
    return m_data[_coord2Index(i,j,k)];
}
inline int DenseLattice::
add(int i, int j, int k) {
    int v;
    Base::addRef(v);
    return v;
}
inline int DenseLattice::
add(int i, int j, int k, bool& b) {
    int v;
    b = Base::addRef(v);
    return v;
}
inline int DenseLattice::
operator()(int i, int j, int k) const {
    return m_data[_coord2Index(i,j,k)];
}




inline int & NaiveSparseLattice::
getRef(int i, int j, int k) {
    //try to insert the value with default -1 for unset index
    return m_data.insert(std::pair<mtao::Coord3,int>(mtao::Coord3{i,j,k},-1)).first->second;
}
inline int NaiveSparseLattice::
add(int i, int j, int k) {
    int v;
    Base::addRef(v);
    return v;
}
inline int NaiveSparseLattice::
add(int i, int j, int k, bool& b) {
    int v;
    b = Base::addRef(v);
    return v;
}

inline int NaiveSparseLattice::
operator()(int i, int j, int k) const {
    std::map<mtao::Coord3,int>::const_iterator result = m_data.find(mtao::Coord3{i,j,k});
    if(result == m_data.end()) return -1;
    return result->second;
}

inline std::map<int,int> & SparseLattice::
i_ji_i(int i, int j) {
    return m_data[j*NI()+i];
}
inline const std::map<int,int> & SparseLattice::
i_ji_i(int i, int j) const {
    return m_data[j*NI()+i];
}
inline int SparseLattice::
getRef(int i, int j, int k) {
    //try to insert the value with default -1 for unset index
    return i_ji_i(i,j).insert(std::pair<int,int>(k,-1)).first->second;
}

inline int SparseLattice::
add(int i, int j, int k) {
    int v;
    Base::addRef(v);
    return v;
}
inline int SparseLattice::
add(int i, int j, int k, bool& b) {
    int v;
    b = Base::addRef(v);
    return v;
}

inline int SparseLattice::
operator()(int i, int j, int k) const {
    const std::map<int,int> & kmap = i_ji_i(i,j);
    std::map<int,int>::const_iterator result = kmap.find(k);
    if(result == kmap.end()) return -1;
    return result->second;
}
