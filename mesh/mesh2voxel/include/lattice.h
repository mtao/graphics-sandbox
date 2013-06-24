#ifndef LATTICE_H
#define LATTICE_H
#include <Eigen/Geometry>
#include "trianglemesh.h"
#include "serialization.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/array.hpp>
namespace Eigen{
        typedef Eigen::Matrix<unsigned int,3,1> Vector3ui;
        typedef Eigen::Matrix<unsigned int,2,1> Vector2ui;
        typedef Eigen::Matrix<unsigned int,Eigen::Dynamic,1> VectorXui;
};


#include <iostream>
class Lattice{
    public:
        Lattice(unsigned int i, unsigned int j, unsigned int k, const Eigen::AlignedBox<double,3> & bbox);
        void write(const std::string & str) const;
        void read(const std::string & str);
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
        Lattice(const TriangleMesh & t, unsigned int ni, unsigned int nj, unsigned int nk, unsigned char dim=0, int buf=2);

        std::set<std::array<unsigned int, 3> >::const_iterator begin()const {return m_voxels.begin();}
        std::set<std::array<unsigned int, 3> >::const_iterator end()const {return m_voxels.end();}
        std::set<std::array<unsigned int, 3> > & getVoxels() {return m_voxels;}
        const std::set<std::array<unsigned int, 3> > & getVoxels() const {return m_voxels;}

    private:
        friend class boost::serialization::access;
        Eigen::Vector3ui m_N;
        Eigen::AlignedBox<double,3> m_bbox;
        Eigen::Vector3d m_dx;
        std::set<std::array<unsigned int, 3> > m_voxels;

        template <class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                ar & m_N;
                ar & m_voxels;
                ar & m_bbox.min();
                ar & m_bbox.max();
                ar & m_dx;

            }

};



#endif
