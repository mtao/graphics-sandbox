#ifndef GRID_H
#define GRID_H
#include <Eigen/Core>
#include <map>
#include <set>
#include "types.h"
#include "coord.h"




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
        inline int size() const {return m_N.prod();}
        //Warning: this isn't necessarily accurate with deletion operations 
        //until after a compactify
        unsigned int activeCount() const {return m_active_count;}
        //Array accessors
        inline int operator()(int i, int j, int k) const;
        inline int operator()(const mtao::Coord3 & coord) const;

        inline int add(const mtao::Coord3 & c);
        inline int add(const mtao::Coord3& c, bool & b);
        inline int add(int i, int j, int k);
        inline int add(int i, int j, int k, bool & b);

        inline void resize(int i, int j, int k);
        //General cleaning
        void reset();
        //Compactification only needs to be done if deletions have happened
        //Should reset m_active_count to be proper
        std::map<int,int> compactifyIndices();
        template <typename Object>
            static void remap(const std::map<int,int> & newmap, std::vector<Object> & vec);

    protected:
        Lattice(int i, int j, int k, unsigned int default_active=0);
        bool addRef(int& v);
    private:
        mtao::Coord3 m_N;
    protected:
        int m_active_count;
        std::set<mtao::Coord3> m_vertexCoords;
};




class DenseLattice: public Lattice<DenseLattice>{
    public:
        typedef Lattice<DenseLattice> Base;
        DenseLattice(int ni, int nj, int nk);
        using Base::operator();
        //Array accessors
        inline int add(int i, int j, int k);
        inline int add(int i, int j, int k, bool & b);
        inline int operator()(int i, int j, int k) const;
        inline int getIndex(int i, int j, int k) const;
        inline int getIndex(const mtao::Coord3 & coord) const;



        //General cleaning
        void reset();
        inline void resize(int i, int j, int k);
        void fill();
        std::map<int,int> compactifyIndices();
    private:
        std::vector<int> m_data;
        inline int _coord2Index(int i, int j, int k) const;
        inline int _coord2Index(const mtao::Coord3 & c) const;
        inline mtao::Coord3 _index2Coord(int idx) const;
        inline int& getRef(int i, int j, int k);

};






class NaiveSparseLattice: public Lattice<NaiveSparseLattice> {
    public:
        NaiveSparseLattice(int ni, int nj, int nk);
        //Status accessor
        //        unsigned int activeCount() const;
        //Array accessors
        using Lattice<NaiveSparseLattice>::operator();
        inline int add(int i, int j, int k);
        inline int add(int i, int j, int k, bool & b);
        inline int operator()(int i, int j, int k) const;
        //General cleaning
        inline void resize(int i, int j, int k);
        void reset();
        std::map<int,int> compactifyIndices();
    private:
        std::map<mtao::Coord3,int> m_data;
        inline int& getRef(int i, int j, int k);
};






//A space/time tradeoff of the above sparse lattice, where we now assume that the first two 
// dimensions are the relevant 

class SparseLattice: public Lattice<SparseLattice>{
    public:
        SparseLattice(int ni, int nj, int nk);
        //Status accessors
        //        unsigned int activeCount() const;
        //Array accessors
        using Lattice<SparseLattice>::operator();
        inline int add(int i, int j, int k);
        inline int add(int i, int j, int k, bool & b);
        inline int operator()(int i, int j, int k) const;
        //General cleaning
        void reset();
        inline void resize(int i, int j, int k);
        std::map<int,int> compactifyIndices();
    private:
        //TODO: Someone please give me a better name for this :)
        //Note: i_ji_i simply means contract i then contract j, where i is the contraction operator
        //Sorry for overloading, can just assume i_ is overload operator :)
        inline std::map<int,int> & i_ji_i(int i, int j);
        inline const std::map<int,int> & i_ji_i(int i, int j) const;
        std::vector<std::map<int,int> > m_data;
        inline int& getRef(int i, int j, int k);
};





#include "src/lattice.hpp"

#endif
