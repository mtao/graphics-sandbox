#ifndef BRIDSON_POISSON_SAMPLING_H
#define BRIDSON_POISSON_SAMPLING_H

#include "../types.h"
#include <vector>
#include <Eigen/StdVector>

namespace mtao {
    template <typename Scalar, int Dim>
        class BridsonSampler {
            public:
                typedef numerical_types<Scalar,Dim> NumTraits;
                typedef typename NumTraits::Vec Vec;
                typedef typename NumTraits::Veci Veci;
                typedef typename NumTraits::BBox BBox;
                typedef typename std::vector<Vec,Eigen::aligned_allocator<Vec> > VecVector;
                BridsonSampler() = default;

                VecVector sample(Scalar r=Scalar(0.01), size_t k=30, const BBox& bb = BBox(Vec::Zero(),Vec::Ones())) const;//r = radius, k = num retries
                void sample(VecVector& v, Scalar r=Scalar(0.01), size_t k=30, const BBox& bb = BBox(Vec::Zero(),Vec::Ones())) const;//r = radius, k = num retries

            private:
                class IGrid {

                    public:
                        IGrid(const BBox& bb, Scalar r) {
                            Vec bbs = bb.sizes() ;
                            
                            Vec Ns = (bbs * std::sqrt<Scalar>(Dim).real() / r);
                            Ns = Ns.unaryExpr([](Scalar i) -> Scalar {return std::ceil(i);});
                            N = Ns.template cast<int>();
                            data = std::vector<int>(N.prod(),-1);
                            translate = bb.min();
                            scale = bbs.cwiseQuotient(Ns);


                        }
                        int& operator()(const Veci& c) {
                            return data[coord2idx(c)];
                        }
                        Vec idx2wld(const Veci& v) const {
                            return v.template cast<Scalar>().cwiseProduct(scale) + translate;
                        }
                        Veci wld2idx(const Vec& v) const {
                            Vec v2 = (v - translate).cwiseQuotient(scale);
                            return v2.template cast<int>();
                        }
                        Veci N;
                        Vec scale;//bbox size / index
                        Vec translate;
                        std::vector<int> data;
                        int coord2idx(const Veci& c)const {
                            //Note:: Higher indices change the fastest
                            int idx=c(0);
                            for(int i=1; i <Dim; ++i) {
                                idx = N(i) * idx + c(i);
                            }
                            return idx;
                        }

                };


        };
}
#include "poisson_bridson.ipl"

#endif
