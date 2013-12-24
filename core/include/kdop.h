#ifndef KDOP_H
#define KDOP_H

#include "types.h"

namespace mtao {
    namespace internal {
        template <typename T, int Dim_>
        struct CubeNormals {
            typedef T Scalar;
            enum {Dim = Dim_, DynamicStorage=false, DefaultStorageSize=Dim_};
            typedef typename mtao::numerical_types<T,Dim>::Vec Vec;
            constexpr Vec operator()(int i) {return Vec::Unit(i);}
            constexpr static int size() {return Dim;}
        };
    }
    template <typename T, int Dim_, typename Normals_=internal::CubeNormals<T,Dim_> >
        class KDOP {
            public:
                typedef T Scalar;
                typedef Normals_ Normals;
                enum {Dim = Dim_,DynamicStorage=Normals::DynamicStorage,DefaultStorageSize=Normals::DefaultStorageSize};
                typedef typename mtao::numerical_types<T,DynamicStorage?Eigen::Dynamic:DefaultStorageSize>::Vec Vector;
                typedef typename mtao::numerical_types<T,Dim>::Vec Vec;
                KDOP();
                void expand(const Eigen::Ref<const Vec>&);
                void expand(const KDOP<T,Dim,Normals>&);
                bool operator()(const Vec&) const;
                bool operator()(const KDOP<T,Dim,Normals>&);
                operator bool();
                const Vector& min() const {return m_min;}
                const Vector& max() const {return m_max;}
                int size() const {return m_normals.size();}
            private:
                Vector m_min;
                Vector m_max;
                Normals m_normals;

        };
    template <typename T, int Dim, typename Normals>
        KDOP<T,Dim,Normals>::KDOP()
        : m_min(Vector::Constant(DefaultStorageSize,std::numeric_limits<T>::max()))
          , m_max(Vector::Constant(DefaultStorageSize,std::numeric_limits<T>::min()))
    {}

    template <typename T, int Dim, typename Normals>
        void KDOP<T,Dim,Normals>::expand(const Eigen::Ref<const Vec>& v) {
            for(int i=0; i < m_normals.size(); ++i) {
                T val = m_normals(i).dot(v);
                T& min = m_min[i];
                T& max = m_max[i];
                min = std::min(val,min);
                max = std::max(val,max);
            }
        }
    template <typename T, int Dim, typename Normals>
        void KDOP<T,Dim,Normals>::expand(const KDOP<T,Dim,Normals>& other) {
            int othersize = other.size();
            assert(size() >= othersize);
            auto&& minhead = m_min.head(othersize);
            auto&& maxhead = m_max.head(othersize);
            minhead.noalias() = minhead.cwiseMin(other.min());
            maxhead.noalias() = maxhead.cwiseMax(other.max());
        }

    template <typename T, int Dim, typename Normals>
        bool KDOP<T,Dim,Normals>::operator()(const Vec& v) const {
            for(int i=0; i < m_normals.size(); ++i) {
                T val = m_normals(i).dot(v);
                T& min = m_min[i];
                T& max = m_max[i];
                //include end points to be conservative
                if(val <= min || val >= max) {
                    return false;
                }
            }
            return true;
        }
    template <typename T, int Dim, typename Normals>
        bool KDOP<T,Dim,Normals>::operator()(const KDOP<T,Dim,Normals>& other) {
            int minsize = std::min(size(),other.size());
            auto&& min = m_min.head(minsize).max(other.min().head(minsize));
            auto&& max = m_max.head(minsize).min(other.max().head(minsize));
            return (min.array() > max.array()).all();
        }
    template <typename T, int Dim, typename Normals>
        KDOP<T,Dim,Normals>::operator bool() {
            //include end points to be conservative
            return (m_min.array() <= m_min.array()).all();
        }
}

#endif
