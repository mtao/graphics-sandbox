#ifndef LATTICE_H
#define LATTICE_H
#include <Eigen/Geometry>
namespace Eigen{
        typedef Eigen::Matrix<unsigned int,3,1> Vector3ui;
        typedef Eigen::Matrix<unsigned int,2,1> Vector2ui;
        typedef Eigen::Matrix<unsigned int,Eigen::Dynamic,1> VectorXui;
};


class Lattice{
    public:
        Lattice(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox);
        Eigen::AlignedBox<double,3>& bbox() {return m_bbox;}
        const Eigen::AlignedBox<double,3> & bbox() const {return m_bbox;}
        unsigned int NI()const{return m_N(0);}
        unsigned int NJ()const{return m_N(1);}
        unsigned int NK()const{return m_N(2);}
        const Eigen::Vector3ui & N()const{return m_N;}
        unsigned int size()const{return NI()*NJ()*NK();}
        unsigned int ijk2idx(unsigned int i, unsigned int j, unsigned int k) const;
        unsigned int ijk2idx(const Eigen::Vector3ui & ijk) const;
        Eigen::Vector3ui idx2ijk(unsigned int idx) const;
        void calculateDx();
        const Eigen::Vector3d & dx() const {return m_dx;}

        unsigned int getIndex(const Eigen::Vector3d & v) const;
        Eigen::Vector3ui getIJK(const Eigen::Vector3d & v) const;

    private:
        Eigen::Vector3ui m_N;
        Eigen::AlignedBox<double,3> m_bbox;
        Eigen::Vector3d m_dx;

};



#endif