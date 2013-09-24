#ifndef GRID_H
#define GRID_H
#include <Eigen/Core>
#include <map>
#include <set>
#include "types.h"




/*
 * Lattice should contain a set of points where each point is associated with some index or value type
 *
 * Lattice is not designed to be explicitly created, this merely sets the requirements on derived classes
 * and sets some general values
 */





template <typename DerivedType>
class Lattice {
    public:
        DerivedType& derived() {return *static_cast<DerivedType*>(this);}
        const DerivedType& derived() const {return *static_cast<const DerivedType*>(this);}
        //Lattice status accessors
        inline mtao::Coord3 N() const {return m_N;}
        inline int NI() const {return m_N(0);}
        inline int NJ() const {return m_N(1);}
        inline int NK() const {return m_N(2);}
        inline int N() const {return m_N.prod();}
        //Warning: this isn't necessarily accurate with deletion operations 
        //until after a compactify
        unsigned int activeCount() const {return m_active_count;}
        //Array accessors
        inline int & operator()(size_t i, size_t j, size_t k);
        inline int operator()(size_t i, size_t j, size_t k) const;
        inline int & operator()(const mtao::Coord3 & coord);
        inline int operator()(const mtao::Coord3 & coord) const;

        inline int add(const mtao::Coord3 & c);
        inline int add(size_t i, size_t j, size_t k, bool & b);
        inline int add(const mtao::Coord3& c, bool & b);
        inline int add(size_t i, size_t j, size_t k);

        inline void resize(size_t i, size_t j, size_t k);
        //General cleaning
        void reset();
        //Compactification only needs to be done if deletions have happened
        //Should reset m_active_count to be proper
        std::map<int,int> compactifyIndices();
        template <typename Object>
            static void remap(const std::map<int,int> & newmap, std::vector<Object> & vec);

    protected:
        Lattice(size_t i, size_t j, size_t k, unsigned int default_active=0);
    private:
        mtao::Coord3 m_N;
    protected:
        size_t m_active_count;
        std::set<Index> m_vertexIndices;
};




class DenseLattice: public Lattice<DenseLattice>{
    public:
        typedef ActiveLatticeIterator<DenseLattice> ActiveIteratorType;
        DenseLattice(size_t ni, size_t nj, size_t nk);
        using Lattice<NaiveSparseLattice>::operator();
        //Array accessors
        inline int & operator()(size_t i, size_t j, size_t k);
        inline int operator()(size_t i, size_t j, size_t k) const;
        inline int getIndex(size_t i, size_t j, size_t k) const;
        inline int getIndex(const mtao::Coord3 & coord) const;



        //General cleaning
        void reset();
        inline void resize(size_t i, size_t j, size_t k);
        void fill();
        std::map<int,int> compactifyIndices();
    private:
        std::vector<int> m_data;
        inline int _coord2Index(int i, int j, int k) const;
        inline int _coord2Index(const mtao::Coord3 & c) const;
        inline mtao::Coord3 _index2Coord(int idx) const;

};






class NaiveSparseLattice: public Lattice<NaiveSparseLattice> {
    public:
        NaiveSparseLattice(size_t ni, size_t nj, size_t nk);
        typedef ActiveLatticeIterator<NaiveSparseLattice> ActiveIteratorType;
        //Status accessor
        //        unsigned int activeCount() const;
        //Array accessors
        using Lattice<NaiveSparseLattice>::operator();
        inline int & operator()(size_t i, size_t j, size_t k);
        inline int operator()(size_t i, size_t j, size_t k) const;
        //General cleaning
        inline void resize(size_t i, size_t j, size_t k);
        void reset();
        std::map<int,int> compactifyIndices();
    private:
        std::map<mtao::Coord3,int> m_data;
};






//A space/time tradeoff of the above sparse lattice, where we now assume that the first two 
// dimensions are the relevant 

class SparseLattice: public Lattice<SparseLattice>, public std::vector<std::map<size_t,int> >{
    public:
        SparseLattice(size_t ni, size_t nj, size_t nk);
        //Status accessors
        //        unsigned int activeCount() const;
        //Array accessors
        using Lattice<SparseLattice>::operator();
        inline int & operator()(size_t i, size_t j, size_t k);
        inline int operator()(size_t i, size_t j, size_t k) const;
        //General cleaning
        void reset();
        inline void resize(size_t i, size_t j, size_t k);
        std::map<int,int> compactifyIndices();
    private:
        //TODO: Someone please give me a better name for this :)
        //Note: i_ji_i simply means contract i then contract j, where i is the contraction operator
        //Sorry for overloading, can just assume i_ is overload operator :)
        inline std::map<size_t,int> & i_ji_i(size_t i, size_t j);
        inline const std::map<size_t,int> & i_ji_i(size_t i, size_t j) const;
};





#include "src/lattice.hpp"
#include "voxelIterators.h"

#endif
