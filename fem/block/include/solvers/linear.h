#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H
#include <Eigen/Sparse>

#include <memory>

template <typename Matrix, typename Vector>
struct LinearSolverCapsule
{
    LinearSolverCapsule(const Matrix & A_, const Vector & b_, Vector & x_): A(A_), b(b_), x(x_) {}
    typedef typename Vector::Scalar Scalar;
    virtual void step() = 0;
    virtual Scalar error()
    {
        return (b-A*x).template lpNorm<Eigen::Infinity>();
    }

    protected:
    const Matrix & A;
    const Vector & b;
    Vector & x;
};


template <typename DerivedCapsule, typename Matrix, typename Vector>
DerivedCapsule createLinearCapsule(const Matrix & A, const Vector & b, Vector & x)
{
    return DerivedCapsule(A,b,x);
}


#include <iostream>

template <typename Capsule>
struct IterativeLinearSolver
{
    typedef typename Capsule::Matrix Matrix;
    typedef typename Capsule::Vector Vector;
    typedef typename Vector::Scalar Scalar;
    IterativeLinearSolver(uint max_its=1000, Scalar eps=0.001):
        epsilon(eps), max_iterations(max_its) {}
    void solve()
    {
        if(!capsule){return;}
        uint iterations = 0;
        while(++iterations < max_iterations &&
                capsule->error() > epsilon)
        {
            capsule->step();
        }
        //if(iterations >= max_iterations)
    }
    void solve(const Matrix & A, const Vector & b, Vector & x)
    {
        capsule.reset(new Capsule(A,b,x));
        solve();
    }
    private:
        std::unique_ptr<Capsule> capsule;
        Scalar epsilon;
        uint max_iterations;
};

#endif
