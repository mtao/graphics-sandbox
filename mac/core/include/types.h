#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#
namespace mtao{ 
    typedef Eigen::AlignedBox<float,3> BBox3f;
    typedef Eigen::AlignedBox<double,3> BBox3d;
    typedef Eigen::AlignedBox<int,3> BBox3i;

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
    template <int Dim>
        struct dim_types {
            typedef Eigen::Matrix<double,Dim,1> Vecf;
            typedef Eigen::Matrix<float,Dim,1> Vecd;
            typedef Eigen::Matrix<int,Dim,1> Veci;
            typedef Eigen::Matrix<double,Dim,Dim> Matf;
            typedef Eigen::Matrix<float,Dim,Dim> Matd;
            typedef Eigen::AlignedBox<float,Dim> BBoxf;
            typedef Eigen::AlignedBox<double,Dim> BBoxd;
            typedef Eigen::AlignedBox<int,Dim> BBoxi;
            template <typename T>
            struct scalar_types {
                typedef Eigen::Matrix<T,Dim,1> Vec;
                typedef Eigen::Matrix<T,Dim,Dim> Mat;
                typedef Eigen::AlignedBox<T,Dim> BBox;
            };
        };
}
#endif
