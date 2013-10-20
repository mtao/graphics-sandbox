#include "lattice.h"
#include "trianglemesh.h"



class Levelset: public Lattice{
    public:
        typedef Eigen::Vector3d Vector3;
        
        Levelset(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox);
        double & operator()(unsigned int i){return m_data[i];}
        double & operator()(unsigned int i, unsigned int j, unsigned int k){return this->operator()(ijk2idx(i,j,k));}
        double operator()(unsigned int i)const{return m_data[i];}
        double operator()(unsigned int i, unsigned int j, unsigned int k)const{return this->operator()(ijk2idx(i,j,k));}


        typedef double value_type;
        typedef std::vector<double>::iterator iterator;
        typedef std::vector<double>::const_iterator const_iterator;
        iterator begin(){return m_data.begin();}
        iterator end(){return m_data.end();}
        const_iterator cbegin()const{return m_data.cbegin();}
        const_iterator cend()const{return m_data.cend();}
        void unionInPlace(const Levelset & other);
        void intersectInPlace(const Levelset & other);
        void unionInPlace(const TriangleMesh & other);
        void signedTriangleUnion(const TriangleMesh & tm, const Triangle & t);
        void pointIntersection(const Eigen::Hyperplane<double,3> & hp, const Vector3 &);
        void edgeIntersection( const Eigen::Hyperplane<double,3> & hp, const Vector3 &, const Vector3 &);
        //void intersectInPlace(const TriangleMesh & other);
    private:
        std::vector<double> m_data;
        std::vector<bool> m_active;

};
