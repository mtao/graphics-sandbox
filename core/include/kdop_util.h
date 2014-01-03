#ifndef KDOP_UTIL_H
#define KDOP_UTIL_H

#include "kdop.h"
#include <vector>
namespace mtao {
    template <typename T>
        std::tuple<std::vector<mtao::numerical_types<T,3>::Vec>,std::vector<mtao::Vec3i> > kdopToPolytope(const mtao::KDOP<T,3>& kdop) {
            typedef mtao::numerical_types<T,3>::Vec Vec;
            typedef mtao::Vec3i Vec3i;
            std::vector<Vec> vertices;
            std::vector<Vec3i> indices;

            for(int i=0; i < kdop.size(); ++i) {
                std::vector<Vec> face_vertices;//vertices for a particular face
                auto&& in = kdop.normal(i);
                const T& imin = ;
                const T& imax = ;
                auto&& ifunc = [&](T iplane_val) {
                    for(int j=0; j < kdop.size(); ++j) {
                        if(j==i) {
                            continue;
                        }
                        auto&& jn = kdop.normal(j);
                        auto&& jfunc = [&](T jplane_val) {
                            const Vec& d = in.cross(jn);
                        };
                        jfunc(kdop.min(j));
                        jfunc(kdop.max(j));
                    }
                };
                ifunc(kdop.min(i));
                ifunc(kdop.max(i));
            }

        }
};
#endif
