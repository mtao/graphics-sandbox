#ifndef CONJUGATE_GRADIENT_H
#define CONJUGATE_GRADIENT_H
#include <Eigen/Dense>
#include "linear.h"



/*
#include "../cholesky/cholesky.hpp"

template <typename Scalar, typename MatrixTag, unsigned int DIM>
class CholeskyPCGEngine
{
    public:
        typedef SolverEngineTraits<ConjugateGradientEngine<Scalar,MatrixTag,DIM> > Traits;
        friend Traits;
        typedef typename Traits::Matrix Matrix;
        typedef typename Traits::Vector Vector;
        CholeskyPCGEngine(): tolerance(Traits::tolerance) {}
        CholeskyPCGEngine(Scalar s): tolerance(s) {}
        void compute(Matrix & A)
        {
            m_A = &A;
        }
        void solve(const Matrix & A, const Vector & b, Vector & x)
        {
            Tao::LDLT<T,N> ldlt(A);

            Vector r = b-A*x;
            if (r.dot(r)<eps) return;
            Vector z;
            ldlt.solve(r,z);
            Vector p = z;
            Vector Ap = A*p;
            Scalar rdz=r.dot(z);
            Scalar alpha, beta;
            int k=0;
            do {
                alpha = (rdz)/(p.dot(Ap));
                x+=alpha * p;
                r-=alpha * Ap;
                if (r.squaredNorm()<eps) break;
                ldlt.solve(r,z);
                beta=1/rdz;
                rdz=r.dot(z);
                beta*=rdz;
                p=z+beta*p;
                Ap=A*p;
                k+=1;

            } while ( k<2*N );
        }
    protected:
        Matrix * m_A;
        Scalar tolerance;
};
*/
template <typename MatrixType, typename VectorType>
struct ConjugateGradientCapsule: public LinearSolverCapsule<MatrixType, VectorType>
{
    typedef MatrixType Matrix;
    typedef VectorType Vector;
    typedef typename Vector::Scalar Scalar;
    typedef LinearSolverCapsule<Matrix,Vector> CapsuleBase;
    using CapsuleBase::A ;
    using CapsuleBase::b ;
    using CapsuleBase::x ;
    ConjugateGradientCapsule(const Matrix & A, const Vector & b, Vector & x):
        CapsuleBase(A,b,x)
    {
        r = b-A*x;
        p = r;
        Ap = A*p;
        rsnorm=r.squaredNorm();
    }
    Scalar error(){return rsnorm;}
    void step()
    {
        alpha = (rsnorm)/(p.dot(Ap));
        x+=alpha * p;
        r-=alpha * Ap;
        beta=1/rsnorm;
        rsnorm=r.squaredNorm();
        beta*=rsnorm;
        p=r+beta*p;
        Ap=A*p;
    }
private:
    Vector r;
    Vector p;
    Vector Ap;
    Scalar rsnorm;
    Scalar alpha, beta;

};

template <typename Matrix, typename Vector>
void ConjugateGradientSolve(const Matrix & A, const Vector & b, Vector & x)
{
    auto residual = (b-A*x).squaredNorm();
    auto solver = IterativeLinearSolver<ConjugateGradientCapsule<Matrix,Vector> >(A.rows(), 1e-8*residual);
    solver.solve(A,b,x);
}







#endif
