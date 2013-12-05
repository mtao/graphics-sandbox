#ifndef EXPLICIT_EULER_H
#define EXPLICIT_EULER_H
#include "advection.h"
namespace mtao{ namespace advection{
    template <typename Scalar, int EmbedDim, int FormDim>
        class ExplicitEuler {};

    template <typename Scalar>
        class ExplicitEuler<Scalar,2,1>: public Advection<ExplicitEuler,Scalar,2,1> {
            public:
                enum {embed_dim = 2, form_dim = 1};
                MTAO_ADVECTIONTYPE_DEFINITIONS_2

                typedef Advection<ExplicitEuler,Scalar,embed_dim,form_dim> Base;

                using Base::velocity;
                ExplicitEuler(const UType& up, const VType& vp)
                    : Base(up,vp) {}
                void evalInPlace(Vec& p, Scalar dt) {
                    p += dt * velocity(p);
                }
        };

    template <typename Scalar, int FormDim>
        class ExplicitEuler<Scalar,3,FormDim>: public Advection<ExplicitEuler,Scalar,3,FormDim> {
            public:
                enum {embed_dim = 3, form_dim = FormDim};
                MTAO_ADVECTIONTYPE_DEFINITIONS_3

                typedef Advection<ExplicitEuler,Scalar,embed_dim,form_dim> Base;


                using Base::velocity;
                ExplicitEuler(const UType& up, const VType& vp, const WType& wp)
                    : Base(up,vp,wp) {}
                void evalInPlace(Vec& p, Scalar dt) {
                    p += dt * velocity(p);
                }
        };
}}
#endif
