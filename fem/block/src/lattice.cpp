#include "lattice.h"


//Lattice Constructors


DenseLattice::
    DenseLattice(int ni, int nj, int nk)
    : Lattice<DenseLattice>(ni,nj,nk,0), m_data(ni*nj*nk,-1)
{
    std::fill(m_data.begin(),m_data.end(),-1);
}


NaiveSparseLattice::
    NaiveSparseLattice(int ni, int nj, int nk)
    : Lattice<NaiveSparseLattice>(ni,nj,nk,0), m_data()
{
}

SparseLattice::
    SparseLattice(int ni, int nj, int nk)
    : Lattice<SparseLattice>(ni,nj,nk,0)
    , m_data(ni*nj,std::map<int,int>())
{
}

//Lattice Resetters

void DenseLattice::
reset() {
    std::fill(m_data.begin(),m_data.end(),-1);
}

void NaiveSparseLattice::
reset() {
    m_data.clear();
}

void SparseLattice::
reset() {
    for(std::map<int,int> & k: m_data) {
        k.clear();
    }
}


//Lattice Resizers
void DenseLattice::resize(int i, int j, int k) {
    m_data.resize(i*j*k);
    Lattice<DenseLattice>::resize(i,j,k);
}

void NaiveSparseLattice::resize(int i, int j, int k) {
    Lattice<NaiveSparseLattice>::resize(i,j,k);
}
void SparseLattice::resize(int i, int j, int k) {
    m_data.resize(i*j);
    Lattice<SparseLattice>::resize(i,j,k);
}
//Lattice Compactification


std::map<int,int> DenseLattice::
compactifyIndices() {
    std::map<int,int> conversion;
    int index=0;
    for(int& ind: m_data) {
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
    for(std::pair<const mtao::Coord3,int> & p: m_data) {
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
    for(std::map<int,int> & m: m_data) {
        for(std::pair<const int,int> & p: m) {
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
    for(const std::map<int, int> & i_ii_j: *this) {
        for(const std::pair<int,int> & k: i_ii_j) {
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
    for(int& i: m_data) {
        i=idx++;
    }
}


