#ifndef GEOMETRY_TYPES_H
#define GEOMETRY_TYPES_H

#include "config.h"

#include <Eigen/Dense>

namespace mtao {
    namespace geometry {
        using Scalar = config::Scalar;
        using Vec2 = Eigen::Matrix<Scalar,2,1>;
        using Vec3 = Eigen::Matrix<Scalar,3,1>;
    }
}

#endif//GEOMETRY_TYPES_H
