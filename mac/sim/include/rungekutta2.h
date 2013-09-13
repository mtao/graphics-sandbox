#ifndef RUNGE_KUTTA2_H
#define RUNGE_KUTTA2_H
#include "advection.h"
namespace mtao{ namespace advection{
    template <typename Scalar, int EmbedDim, int FormDim>
        class RungeKutta2 {};

    template <typename Scalar>
        class RungeKutta2<Scalar,2,1>: public Advection<template RungeKutta2,Scalar,2,1> {
            public:
                enum {embed_dim = 2, form_dim = 1};
                MTAO_ADVECTIONTYPE_DEFINITIONS_2

                typedef Advection<RungeKutta2,Scalar,embed_dim,form_dim> Base;

                using Base::velocity;
                RungeKutta2(const UType& up, const VType& vp)
                    : Base(up,vp) {}
                void evalInPlace(Vec& p, Scalar dt) {
                    p += dt * velocity(p+0.5*dt*velocity(p));
                }
        };

    template <typename Scalar, int FormDim>
        class RungeKutta2<Scalar,3,FormDim>: public Advection<RungeKutta2,Scalar,3,FormDim> {
            public:
                enum {embed_dim = 3, form_dim = FormDim};
                MTAO_ADVECTIONTYPE_DEFINITIONS_3

                typedef Advection<RungeKutta2,Scalar,embed_dim,form_dim> Base;


                using Base::velocity;
                RungeKutta2(const UType& up, const VType& vp, const WType& wp)
                    : Base(up,vp,wp) {}
                void evalInPlace(Vec& p, Scalar dt) {
                    p += dt * velocity(p+0.5*dt*velocity(p));
                }
        };
}}
#endif
