#ifndef PRECONDITOINED_CONJUGATE_GRADIENT_H
#define PRECONDITOINED_CONJUGATE_GRADIENT_H
#include <Eigen/Dense>
#include "linear.h"
#include "ldlt.hpp"



template <typename MatrixType, typename VectorType, typename Preconditioner>
struct PreconditionedConjugateGradientCapsule: public LinearSolverCapsule<MatrixType, VectorType>
{
    typedef MatrixType Matrix;
    typedef VectorType Vector;
    typedef typename Vector::Scalar Scalar;
    typedef LinearSolverCapsule<Matrix,Vector> CapsuleBase;
    using CapsuleBase::A ;
    using CapsuleBase::b ;
    using CapsuleBase::x ;
    PreconditionedConjugateGradientCapsule(const Matrix & A, const Vector & b, Vector & x):
        CapsuleBase(A,b,x)
    {
        precond = Preconditioner(A);
        r = b-A*x;
        precond.solve(r,z);
        p = z;
        Ap = A*p;
        rdz = r.dot(z);
    }
    Scalar error()
    {
        return r.template lpNorm<Eigen::Infinity>();
    }
    void step()
    {
        alpha = (rdz)/(p.dot(Ap));
        x+=alpha * p;
        r-=alpha * Ap;
        precond.solve(r,z);
        beta=1/rdz;
        rdz = r.dot(z);
        beta*=rdz;
        p=z+beta*p;
        Ap=A*p;
    }
private:
    Vector r;
    Vector z;
    Vector p;
    Vector Ap;
    Scalar rdz;
    Scalar alpha, beta;
    Preconditioner precond;

};

template <typename Preconditioner, typename Matrix, typename Vector>
void PreconditionedConjugateGradientSolve(const Matrix & A, const Vector & b, Vector & x)
{
    auto residual = (b-A*x).template lpNorm<Eigen::Infinity>();
    auto solver = IterativeLinearSolver<PreconditionedConjugateGradientCapsule<Matrix,Vector, Preconditioner> >(5*A.rows(), 1e-5*residual);
    //auto solver = IterativeLinearSolver<PreconditionedConjugateGradientCapsule<Matrix,Vector, Preconditioner> >(A.rows(), 1e-5);
    solver.solve(A,b,x);
}


template <typename Matrix, typename Vector>
void DenseCholeskyPreconditionedConjugateGradientSolve(const Matrix & A, const Vector & b, Vector & x)
{
    PreconditionedConjugateGradientSolve<DenseLDLT<decltype(A)> >(A,b,x);
}

template <typename Matrix, typename Vector>
void SparseCholeskyPreconditionedConjugateGradientSolve(const Matrix & A, const Vector & b, Vector & x)
{
    PreconditionedConjugateGradientSolve<SparseLDLT<Matrix,Vector> >(A,b,x);

}





#endif
