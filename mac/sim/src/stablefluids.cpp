#include "macgridfactory.h"
#include "rungekutta2.h"





class StableFluids {
    public:
        typedef float Scalar;
        typedef MACGridFactory<Scalar,2> FactoryType;
        typedef mtao::advection::RungeKutta2<Scalar,2,1> AdvectorType;
        typedef FactoryType::Selector<mtao::UGrid> UGrid;
        typedef FactoryType::Selector<mtao::VGrid> VGrid;
        typedef FactoryType::Selector<mtao::NGrid> NGrid;
        typedef FactoryType::Selector<mtao::CGrid> CGrid;
        typedef FactoryType::Vec Vec;
        typedef FactoryType::Veci Veci;
        StableFluids(MACGridFactory::ptr factory);

        void step(Scalar dt);
        void advect(Scalar dt);
        void forces(Scalar dt);
        void project();
        Scalar cfl();


    private:
        UGrid u;
        VGrid v;
        UGrid utmp;
        VGrid vtmp;
        CGrid p;
        AdvectorType advector;
        MACGridFactory::ptr factory;
        

};

StableFluids::StableFluids(MACGridFactory& factory)
    : u(factory.create<mtao::UGrid>())
    , v(factory.create<mtao::VGrid>())
    : utmp(factory.create<mtao::UGrid>())
    , vtmp(factory.create<mtao::VGrid>())
    , p(factory.create<mtao::CGrid>())
    , advector(u,v)
    , factory(factory)
{}



auto StableFluids::cfl() -> Scalar {
    return std::min(
              u.dx(0)/u.mapVector().lpNorm<Eigen::Infinity>()
            , v.dx(1)/v.mapVector().lpNorm<Eigen::Infinity>()
            );
}


void StableFluids::step(Scalar dt) {
    Scalar t=0;
    while(t < dt) {
        Scalar substep = cfl();
        if(t+substep > dt) {
            substep = dt - t;
        }

        forces(substep);
        advect(substep);
        project();
        t+=substep;

    }
}



void StableFluids::forces(Scalar dt) {
    v.mapVector().array() -= 0.8 * dt;//Gravity
}

void StableFluids::advect(Scalar dt) {
    
    utmp.loop([&](const Veci& ijk, Scalar) -> Scalar {
            Vec p(utmp.indexToWorld(ijk));
            advector.evalInPlace(p,-dt);
            return u.lerp(p);
            });
    vtmp.loop([&](const Veci& ijk, Scalar) -> Scalar {
            Vec p(utmp.indexToWorld(ijk));
            advector.evalInPlace(p,-dt);
            return v.lerp(p);
            });
    utmp.swap(u);
    vtmp.swap(v);

}


void StableFluids::project() {
    typedef Eigen::SparseMatrix<Scalar> SparseMatrix;
    typedef Eigen::Triplet<Scalar> Triplet;
    std::vector<Triplet> triplets;
    const Veci& N = factory.N();
    auto&& i2c = factory.coordToIndexFunction2();
    int& ni = N(0);
    int& nj = N(1);
    int joff = u.size();
    for(int i=0; i < ni; ++i) {
        for(int j=0; j < nj; ++j) {
            triplets.push_back(Triplet(-1,i2c(i,j),u.coord2idx(i,j)));
            triplets.push_back(Triplet(1,i2c(i,j),u.coord2idx(i+1,j)));
            triplets.push_back(Triplet(-1,i2c(i,j),joff + v.coord2idx(i,j)));
            triplets.push_back(Triplet(1,i2c(i,j),joff + v.coord2idx(i,j+1)));
        }
    }
    int rows = N.prod();
    int cols = u.size() + v.size();
    SparseMatrix d(rows,cols);
    d.setFromTriplets(triplets);
    SparseMatrix L = d * d.transpose();
    
    typedef Eigen::Matrix<Scalar,Eigen::Dynamic,1> Vector;
    Vector rhs(cols);
    rhs.topRows(u.size()) = u.mapVector();
    rhs.bottomRows(v.size()) = v.mapVector();
    rhs = d * rhs;
    
    Eigen::ConjugateGradient<SparseMatrix> solver(A);
    p.mapVector() = solver.solve(rhs);


    rhs = d * p.mapVector();
    u.mapVector() -= rhs.topRows(u.size());
    v.mapVector() -= rhs.bottomRows(v.size());


}















int main() {
    
}
