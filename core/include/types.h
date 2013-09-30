#ifndef TYPES_H
#define TYPES_H
#include "coord.h"

#include <Eigen/Dense>
namespace mtao{ 

    typedef Eigen::Matrix3f Mat3f;
    typedef Eigen::Matrix2f Mat2f;
    typedef Eigen::Vector3f Vec3f;
    typedef Eigen::Vector2f Vec2f;

    typedef Eigen::Matrix3d Mat3d;
    typedef Eigen::Matrix2d Mat2d;
    typedef Eigen::Vector3d Vec3d;
    typedef Eigen::Vector2d Vec2d;

    typedef Eigen::Vector3i Vec3i;
    typedef Eigen::Vector2i Vec2i;
    typedef Coord<3> Coord3;
    typedef Coord<2> Coord2;
    template <int Dim>
        struct dim_types {
            typedef Eigen::Matrix<double,Dim,1> Vecf;
            typedef Eigen::Matrix<float,Dim,1> Vecd;
            typedef Eigen::Matrix<int,Dim,1> Veci;
            typedef Eigen::Matrix<double,Dim,Dim> Matf;
            typedef Eigen::Matrix<float,Dim,Dim> Matd;
            typedef Coord<Dim> Coord;
            template <typename T>
                struct scalar_types {
                    typedef Eigen::Matrix<T,Dim,1> Vec;
                    typedef Eigen::Matrix<T,Dim,Dim> Mat;
                };
            typedef Veci Coord;
        };
}
#endif
