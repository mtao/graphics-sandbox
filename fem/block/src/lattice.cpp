#include "../include/lattice.h"


//Lattice Constructors


DenseLattice::
    DenseLattice(size_t ni, size_t nj, size_t nk)
    : Lattice<DenseLattice>(ni,nj,nk,0), std::vector<int>(ni*nj*nk,-1)
{
    std::fill(begin(),end(),-1);
}


NaiveSparseLattice::
    NaiveSparseLattice(size_t ni, size_t nj, size_t nk)
    : Lattice<NaiveSparseLattice>(ni,nj,nk,0), std::map<Index,int>()
{
}

SparseLattice::
    SparseLattice(size_t ni, size_t nj, size_t nk)
    : Lattice<SparseLattice>(ni,nj,nk,0)
    , std::vector<std::map<size_t,int> >(ni*nj,std::map<size_t,int>())
{
}

//Lattice Resetters

void DenseLattice::
reset() {
    std::fill(begin(),end(),-1);
}

void NaiveSparseLattice::
reset() {
    clear();
}

void SparseLattice::
reset() {
    for(std::map<size_t,int> & k: *this) {
        k.clear();
    }
}


//Lattice Resizers
void DenseLattice::resize(size_t i, size_t j, size_t k) {
    resize(i*j*k);
    Lattice<DenseLattice>::resize(i,j,k);
}

void NaiveSparseLattice::resize(size_t i, size_t j, size_t k) {
    Lattice<NaiveSparseLattice>::resize(i,j,k);
}
void SparseLattice::resize(size_t i, size_t j, size_t k) {
    std::vector<std::map<size_t,int> >::resize(i*j);
    Lattice<SparseLattice>::resize(i,j,k);
}
//Lattice Compactification


std::map<int,int> DenseLattice::
compactifyIndices() {
    std::map<int,int> conversion;
    int index=0;
    for(int& ind: *this) {
        if(ind != -1) {
            conversion.insert(std::pair<int,int>(ind,index));
            ind=index++;
        }
    }
    m_active_count = index;
    return conversion;
}
std::map<int,int> NaiveSparseLattice::
compactifyIndices() {
    std::map<int,int> conversion;
    int index=0;
    for(std::pair<const Index,int> & p: *this) {
        int & ind = p.second;
        if(ind != -1) {
            conversion.insert(std::pair<int,int>(ind,index));
            ind=index++;
        }
    }
    m_active_count = index;
    return conversion;
}
std::map<int,int> SparseLattice::
compactifyIndices() {
    std::map<int,int> conversion;
    int index=0;
    for(std::map<size_t,int> & m: *this) {
        for(std::pair<const size_t,int> & p: m) {
            int & ind = p.second;
            if(ind != -1) {
                conversion.insert(std::pair<int,int>(ind,index));
                ind=index++;
            }
        }
    }
    m_active_count = index;
    return conversion;
}

//Active count


/*
unsigned int DenseLattice::
activeCount() const{
    unsigned int count=0;
    for(int val: *this) {
        if(val != -1) {
            ++count;
        }
    }
}
unsigned int NaiveSparseLattice::
activeCount() const{
    unsigned int count=0;
    for(const std::pair<Index,int> & val: *this) {
        if(val.second != -1) {
            ++count;
        }
    }
}



unsigned int SparseLattice::
activeCount() const{
    unsigned int count=0;
    for(const std::map<size_t, int> & i_ii_j: *this) {
        for(const std::pair<size_t,int> & k: i_ii_j) {
            if(k.second != -1) {
                ++count;
            }
        }
    }
}
*/

//Dense is special in that filling isn't stupid

void DenseLattice::
fill() {
    int idx=0;
    for(int& i: *this) {
        i=idx++;
    }
}


