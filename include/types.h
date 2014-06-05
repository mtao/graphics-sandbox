#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "coord.h"
namespace mtao{ 
    template <typename T>
    class type_traits {};
    typedef Eigen::Matrix<int,1,1> Vec1i;
    typedef Eigen::Matrix<float,1,1> Vec1f;
    typedef Eigen::Matrix<double,1,1> Vec1d;

    typedef Eigen::Matrix3f Mat3f;
    typedef Eigen::Matrix2f Mat2f;
    typedef Eigen::MatrixXf MatXf;
    typedef Eigen::Vector3f Vec3f;
    typedef Eigen::Vector2f Vec2f;
    typedef Eigen::VectorXf VecXf;
    typedef Eigen::AlignedBox<float,3> BBox3f;
    typedef Eigen::AlignedBox<float,2> BBox2f;
    typedef Eigen::AlignedBox<float,Eigen::Dynamic> BBoxXf;

    typedef Eigen::Matrix3d Mat3d;
    typedef Eigen::Matrix2d Mat2d;
    typedef Eigen::MatrixXd MatXd;
    typedef Eigen::Vector3d Vec3d;
    typedef Eigen::Vector2d Vec2d;
    typedef Eigen::VectorXd VecXd;
    typedef Eigen::AlignedBox<double,3> BBox3d;
    typedef Eigen::AlignedBox<double,2> BBox2d;
    typedef Eigen::AlignedBox<double,Eigen::Dynamic> BBoxXd;

    typedef Eigen::Vector3i Vec3i;
    typedef Eigen::Vector2i Vec2i;
    typedef Eigen::AlignedBox<int,3> BBox3i;
    typedef Eigen::AlignedBox<int,2> BBox2i;
    typedef Eigen::AlignedBox<int,Eigen::Dynamic> BBoxXi;
    typedef Coord<3> Coord3;
    typedef Coord<2> Coord2;
    typedef Eigen::AlignedBox<int,2> CoordBBox2;
    typedef Eigen::AlignedBox<int,3> CoordBBox3;
    template <typename T, int Dim>
        struct numerical_types {
            typedef Eigen::Matrix<T,Dim,1> Vec;
            typedef Eigen::Matrix<int,Dim,1> Veci;
            typedef Eigen::Matrix<T,Dim,Dim> Mat;
            typedef Eigen::AlignedBox<T,Dim> BBox;
        };
    template <typename T>
        struct scalar_types {
            typedef Eigen::Matrix<T,2,1> Vec2;
            typedef Eigen::Matrix<T,3,1> Vec3;
            typedef Eigen::Matrix<T,Eigen::Dynamic,1> VecX;
            typedef Eigen::Matrix<T,2,2> Mat2;
            typedef Eigen::Matrix<T,3,3> Mat3;
            typedef Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> MatX;
            typedef Eigen::AlignedBox<T,3> BBox3;
            typedef Eigen::AlignedBox<T,2> BBox2;
            typedef Eigen::AlignedBox<T,Eigen::Dynamic> BBoxX;
            template <int Dim>
                using dim_types = numerical_types<T,Dim>;
        };
    template <int Dim>
        struct dim_types {
            typedef Eigen::Matrix<double,Dim,1> Vecf;
            typedef Eigen::Matrix<float,Dim,1> Vecd;
            typedef Eigen::Matrix<int,Dim,1> Veci;
            typedef Eigen::Matrix<double,Dim,Dim> Matf;
            typedef Eigen::Matrix<float,Dim,Dim> Matd;
            typedef Eigen::AlignedBox<float,Dim> BBoxf;
            typedef Eigen::AlignedBox<double,Dim> BBoxd;
            typedef Eigen::AlignedBox<int,Dim> BBox;
            typedef mtao::Coord<Dim> Coord;
            typedef Eigen::AlignedBox<int,Dim> CoordBBox;
            template <typename T>
                using scalar_types = numerical_types<T,Dim>;
        };
}
#endif
