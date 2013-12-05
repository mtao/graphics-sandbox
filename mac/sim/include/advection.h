#ifndef ADVECTION_H
#define ADVECTION_H
#include "mac.h"
#include "traits.h"

#define MTAO_ADVECTIONTYPE_DEFINITIONS_2 \
                typedef typename mtao::dim_types<embed_dim>::template scalar_types<Scalar>::Vec Vec; \
                typedef MACGrid<Scalar,embed_dim,form_dim,0> UType; \
                typedef MACGrid<Scalar,embed_dim,form_dim,1> VType; 

#define MTAO_ADVECTIONTYPE_DEFINITIONS_3 \
                typedef typename mtao::dim_types<embed_dim>::template scalar_types<Scalar>::Vec Vec;\
                typedef MACGrid<Scalar,3,form_dim,0> UType;\
                typedef MACGrid<Scalar,3,form_dim,1> VType;\
                typedef MACGrid<Scalar,3,form_dim,2> WType;

namespace mtao{ namespace advection{
    template <template <typename,int,int> class MethodType, typename Scalar, int EmbedDim, int FormDim>
        class Advection {
        };
    template <template <typename,int,int> class MethodType, typename Scalar>
        class Advection<MethodType,Scalar,2,1> {
            public:
                enum {embed_dim = 2, form_dim = 1};

                MTAO_ADVECTIONTYPE_DEFINITIONS_2

                typedef MethodType<Scalar,embed_dim,form_dim> Derived;
                Advection(const UType& up, const VType& vp)
                    : m_u(up), m_v(vp) {}

                Derived& derived() {return *static_cast<Derived*>(this);}
                const Derived& derived() const {return *static_cast<Derived*>(this);}

                Vec velocity(const Eigen::Ref<const Vec>& p) {
                    return Vec(m_u.lerp(p),m_v.lerp(p));
                }

                void evalInPlace(Vec& p,Scalar dt) {
                    return derived().evalInPlace(p,dt);
                }
                void eval(const Vec& p, Vec& r, Scalar dt) {
                    r = p;
                    evalInPlace(r,dt);
                }
                Vec eval(const Vec& p, Scalar dt) {
                    Vec r;
                    eval(p,r,dt);
                    return r;
                }
            private:
                const UType& m_u;
                const VType& m_v;
        };

    template <template <typename,int,int> class MethodType, typename Scalar, int FormDim>
        class Advection<MethodType,Scalar,3,FormDim> {
            public:
                enum {embed_dim = 3, form_dim = FormDim};

                MTAO_ADVECTIONTYPE_DEFINITIONS_3

                typedef MethodType<Scalar,embed_dim,form_dim> Derived;
                Advection(const UType& up, const VType& vp, const WType& wp)
                    : m_u(up), m_v(vp), m_w(wp) {}

                Derived& derived() {return *static_cast<Derived*>(this);}
                const Derived& derived() const {return *static_cast<Derived*>(this);}

                Vec velocity(const Vec& p) {
                    return Vec(m_u.lerp(p),m_v.lerp(p), m_w.lerp(p));
                }

                void evalInPlace(Vec& p,Scalar dt) {
                    return derived().evalInPlace(p,dt);
                }
                void eval(const Vec& p, Vec& r, Scalar dt) {
                    r = p;
                    evalInPlace(r,dt);
                }
                Vec eval(const Vec& p, Scalar dt) {
                    Vec r;
                    eval(p,r,dt);
                    return r;
                }
            private:
                const UType& m_u;
                const VType& m_v;
                const WType& m_w;
        };





}}
#endif
