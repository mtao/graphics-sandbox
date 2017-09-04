#ifndef SPHERICAL_HARMONICS_HPP
#define SPHERICAL_HARMONICS_HPP
#include <array>
#include <cmath>
#include <utility>
#include <Eigen/Dense>

namespace internal {
    constexpr int factorial(int n) {
        return n>0?n*factorial(n-1):1;
    }
    template <std::size_t... I>
    constexpr std::array<int,sizeof...(I)> _make_factorials(std::index_sequence<I...>) {
        return std::array<int,sizeof...(I)>{
            factorial(I)...
        };

    }
    template <size_t N>
    constexpr std::array<int,N> make_factorials() {
        return _make_factorials(std::make_index_sequence<N>{});
    }

}

template <typename Scalar, int L_, int NumRows=Eigen::Dynamic>
class SHValueAccessor {
    public:
        constexpr static int L = L_;
        constexpr static int Size = (L+1)*(L+1);
        using StorageType = typename Eigen::Matrix<Scalar,NumRows,Size>;
        SHValueAccessor(int rows=-1) {
            m_values.resize(rows,Size);
        }
        const auto operator()(int l, int m) const {
            return val(l,m);
        }

        size_t index(int l, int m) const {
            //std::cout << "[" << l << "," << m << ":" << l*(l+1)+m << "]";
            return l*(l+1)+m;
        }
        const auto val(int l, int m) const {
            return m_values.col(index(l,m));
        }
        auto val(int l, int m) {
            return m_values.col(index(l,m));
        }
        const auto aval(int l, int m) const {
            return val(l,m).array();
        }
        auto aval(int l, int m) {
            return val(l,m).array();
        }
        const StorageType& values() const { return m_values;}
        static size_t size() { return Size; }
    private:
        //std::array<ValueType,Size> m_values;
        StorageType m_values;
};


template <typename Vec, int L, typename Scalar_ = typename Vec::Scalar, int NumRows = Vec::RowsAtCompileTime>
class LegendrePolynomial: public SHValueAccessor<Scalar_, L, NumRows> {
    public:

        using Scalar = Scalar_;
        using ValueType = Vec;
        using ParentType = SHValueAccessor<Scalar,L,NumRows>;
        using ParentType::val;
        using ParentType::aval;
        //using Scalar = ValueType;




        //Recurrence relation naming protocol:
        //functions follow a naming of
        //r l m _ a b
        //for recurrence L index, M index, and offsets
        //rlm_01 means from index l,m we will get a value for l,m+1
        //rlm_0m means from index l,m we will get a value for l,-m
        //rll_11 means from index l,l we will get a value for l+1,l+1
        template <typename VecType>
        LegendrePolynomial(const VecType& x_): ParentType(x_.rows()), x(x_) {

            m_sqrt1mx2 = (1-x.array().pow(2)).sqrt();

            val(0,0) = ValueType::Constant(x.rows(),1);
            val(1,0) = x;
            /*
            //Unncessary to just evaluate legendre polynomials
            for(int l = 1; l < L; ++l) {
                val(l+1,0) = rl0_10(l);
            }
            */

            //evaluate the two outer bands
            for(int l = 0; l < L; ++l) {
                val(l+1,l+1) = rll_11(l);
                val(l+1,l) = rll_10(l);
            }

            //Fill hte positive values
            for(int l = 1; l < L; ++l) {
                for(int m = 0; m < l; ++m) {
                    val(l+1,m) = rlm_10(l,m);
                }
            }
            //get the negative values
            for(int l = 1; l <= L; ++l) {
                for(int m = 0; m <= l; ++m) {
                    val(l,-m) = rlm_0m(l,m);
                }
            }
        }
        //P_{l+1}^m using P_l^m P_{l-1}^m
        ValueType rlm_10(int l, int m) const {
            Scalar quotient = (l - m + 1);
            return ((2*l+1) * x.array() * aval(l,m) - (l + m) * aval(l-1,m)) / quotient;
        }

        //P_{l}^{-m} using P_l^m 
        ValueType rlm_0m(int l, int m) const {
            return (m%2==0?1:-1) * Scalar(s_factorials[l-m]) / Scalar(s_factorials[l+m]) * aval(l,m);
        }
        //P_{l+1}^0 using P_{l}^0 P_{l-1}^0
        ValueType rl0_10(int l) const {
            Scalar quotient = (l+1);
            return ((2*l+1) * x.array() * aval(l,0) - l * aval(l-1,0)) / quotient;
        }
        //P_{l+1}^{l+1} from P_{l}^l
        ValueType rll_11(int l) const {
            return - (2 * l + 1 ) * m_sqrt1mx2.array() * aval(l,l);
        }
        //P_{l+1}^{l} from P_{l}^l
        ValueType rll_10(int l) const {
            return (2 * l + 1 ) * x.array() * aval(l,l);
        }



        const std::array<int,2*L+1> factorials() const { return s_factorials;}

    private:
        ValueType x;
        ValueType m_sqrt1mx2;
        //TODO: remember how to static this properly
        const std::array<int,2*L+1> s_factorials = internal::make_factorials<2*L+1>();
};

template <int L, typename Vec = Eigen::Matrix<float,Eigen::Dynamic,1>>
class SphericalHarmonics: public SHValueAccessor<typename Vec::Scalar, L, Vec::RowsAtCompileTime>  {
    public:
        using Scalar = typename Vec::Scalar;
        using Mat2X = Eigen::Matrix<Scalar,2,Eigen::Dynamic>;
        using LegPolyType = LegendrePolynomial<Vec,L>;
        using ParentType = SHValueAccessor<Scalar,L,Vec::RowsAtCompileTime>;
        using ParentType::val;
        using ParentType::aval;
        //phi theta
        SphericalHarmonics(const Mat2X& coords): ParentType(coords.cols()) {
            auto phi = coords.row(0);
            auto ct = Eigen::cos(coords.row(1).array());

            LegPolyType leg(ct.transpose());
            for(int l = 0; l <= L; ++l) {
                for(int m = 0; m <= l; ++m) {
                    val(l,m) = coeff(l,m, leg.factorials(), phi.transpose(), leg(l,m));
                    val(l,-m) = coeff(l,-m, leg.factorials(), phi.transpose(), leg(l,-m));
                }
            }
        }

        template <typename VecType>
            auto eval(const VecType& v) const {
                return ParentType::values() * v;
            }


        template <typename VecType>
        Vec coeff(int l, int m, const std::array<int,2*L+1>& factorials, const VecType& v, const Vec& leg) const {
            auto s = std::sqrt( (2 * l + 1) / (4 * M_PI ) * Scalar(factorials[l-m]) / Scalar(factorials[l+m]));
            if (m > 0) {
                auto b = (m*v.array()).cos();
                return s*b*leg.array();
            } else if(m < 0) {
                auto b = (m*v.array()).sin();
                return s*b*leg.array();
            } else {
                return s * leg;
            }

        }

    private:

};
#endif//SPHERICAL_HARMONICS_HPP

