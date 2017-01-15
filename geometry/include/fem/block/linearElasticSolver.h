#ifndef LINEARElastic_SOLVER
#define LINEARElastic_SOLVER

#include <iostream>

#include "voxelSolver.h"
#include "VDBVoxelSolver.h"
#include "quadrature.h"
#include <boost/array.hpp>

//#define HETEROGENEOUS_SYSTEM

struct LinearElasticVertexProperties{
    EIGEN_DENSE_MATRIX_TYPEDEFS
    LinearElasticVertexProperties(const Vector3 & baseVal = Vector3()): neighborCount(0), isConstrained(false), pos(Vector3()), undeformedPos(baseVal), externalForce(Vector3(0,0,0)) {
    }
    int32_t neighborCount ;
    bool isConstrained;
    Vector3 pos;
    Vector3 undeformedPos;
    Vector3 externalForce;
};
#ifdef HETEROGENEOUS_SYSTEM
struct LinearElasticVoxelProperties{
    EIGEN_DENSE_MATRIX_TYPEDEFS
    LinearElasticVertexProperties(Scalar lambda=35962.12, Scalar mu=137826): lambda(lambda),mu(mu){}
    Scalar lambda,mu;
};
#endif

class LinearElasticSolver: public VoxelSolver {
    public:
//        LinearElasticSolver(const SHARED_PTR<VoxelObjectBase> & voxelobj = SHARED_PTR<VoxelObjectBase>());
        EIGEN_DENSE_MATRIX_TYPEDEFS

        LinearElasticSolver();
        bool doTimestep(Scalar dt);
        SparseMatrix getStiffnessMatrix(Vector & rhs, Scalar dt);
        void addValue(SparseMatrix & A,const Index3 & idx, const SIndex3 & offset);
        VoxelRenderPackage getRenderData() const;
        void clearConstraints();
        void setConstraint(uint32_t i, uint32_t j, uint32_t k, const Vector3 & v);
        void setObject(const SHARED_PTR<VoxelObjectBase> &);
        void setLameCoefficients(Scalar lambda, Scalar mu) {m_lambda=lambda;m_mu=mu;}
        void makeCompressed();
        void setGravity();
        void setExternalForce(uint32_t i, uint32_t j, uint32_t k, const Vector3 & v);
        void setExternalForce(uint32_t idx, const Vector3 & v);
    private:



        inline int32_t mat_ind(int32_t value, int32_t dim) const {return 3*value + dim;}

    private:
        std::vector<LinearElasticVertexProperties> m_vertex_properties;
        Vector m_data;
#ifdef HETEROGENEOUS_SYSTEM
        std::vector<LinearElasticVoxelProperties> m_voxel_properties;
#else
        Scalar m_lambda, m_mu;
#endif
        LegendreCubeIntegrator integrator;
};        
class StiffnessEntryFunctor{
    typedef float Scalar;
    EIGEN_DENSE_MATRIX_TYPEDEFS
            public:
                StiffnessEntryFunctor(Scalar lambda, Scalar mu, const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props, const LegendreCubeIntegrator & integrator);
                void setJacobians(const Index3 & idx);
                Scalar dldx(unsigned char case_, int32_t dim, const Vector3 & v) const;
                Scalar dldxi(unsigned char case_, int32_t dim, int) const;
                Scalar genericTerm(unsigned char, unsigned char, int32_t di, int32_t dj,const Vector3 &) const;
                Scalar diagonalTerm(unsigned char, unsigned char, const Vector3 &) const;
                Scalar genericTermi(unsigned char, unsigned char, int32_t di, int32_t dj,int ijk) const;
                Scalar genericTermi(unsigned char, unsigned char, int32_t di, int32_t dj) const;
                Scalar diagonalTermi(unsigned char, unsigned char, int ijk )const;
                Scalar jacobian(int ijk, const Index3 & idx,const Vector3 & origin);
                const std::vector<Scalar> &getJacobians() const {return jacobians;}
                const std::vector<boost::array<boost::array<Scalar,3>,8> > & getgetDldx() const {return m_dldx;}
            private:
                //const Vector3 & origin;
                Scalar lambda;
                Scalar mu;
                const VoxelObjectBase & obj;
                const std::vector<LinearElasticVertexProperties> & props;
                std::vector<Scalar> jacobians;
                std::vector<boost::array<boost::array<Scalar,3>, 8> > m_dldx;
                std::vector<
                    boost::array<boost::array<
                    boost::array<boost::array<
                    Scalar
                    ,3>, 8>
                    ,3>, 8>
                    > m_dldxdldx;
                Scalar scale;

        };

        class StiffnessEntryStorage{
            typedef float Scalar;
            EIGEN_DENSE_MATRIX_TYPEDEFS
            public:
                StiffnessEntryStorage(const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props, const LegendreCubeIntegrator & integrator);
                StiffnessEntryStorage(float scale, const LegendreCubeIntegrator & integrator);
                Scalar dldx(unsigned char case_, int32_t dim, const Vector3 & v) const;
                Scalar _genericTerm(unsigned char, unsigned char, int32_t di, int32_t dj,const Vector3 &, Scalar lambda, Scalar mu) const;
                Scalar _genericTermi(unsigned char, unsigned char, int32_t di, int32_t dj,int ijk, Scalar lambda, Scalar mu) const;
                Scalar _genericTermLefti(unsigned char, unsigned char, int32_t di, int32_t dj,int ijk) const;
                Scalar _genericTermRighti(unsigned char, unsigned char, int32_t di, int32_t dj,int ijk) const;
                Scalar genericTerm(unsigned char, unsigned char, int32_t di, int32_t dj, Scalar lambda, Scalar mu) const;
                Scalar _diagonalTerm(unsigned char, unsigned char, const Vector3 &, Scalar mu) const;
                Scalar _diagonalTermi(unsigned char, unsigned char, int ijk, Scalar mu )const;
                Scalar _diagonalTermCenteri(unsigned char, unsigned char, int ijk )const;
                Scalar diagonalTerm(unsigned char, unsigned char, Scalar mu )const;
                void setScale(const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props  );
                Scalar jacobian(int ijk, const Index3 & idx, const Vector3 & origin,const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props  );
            private:
                Scalar m_scale;
                std::vector<boost::array<boost::array<Scalar,3>, 8> > m_dldx;
                std::vector<
                    boost::array<boost::array<
                    boost::array<boost::array<
                    Scalar
                    ,3>, 8>
                    ,3>, 8>
                    > m_dldxdldx;
                boost::array<boost::array<
                    boost::array<boost::array<
                    std::pair<Scalar,Scalar>
                    ,3>, 8>
                    ,3>, 8>
                    m_genericTerms;
                boost::array<boost::array<
                    Scalar
                    ,8> ,8>
                    m_diagonalTerms;

        };












#include <Eigen/Sparse>
template <typename LatticeType_>
class VDBLinearFEMSolverModule: public VDBVoxelSolverModule<LatticeType_> {
    EIGEN_TYPEDEFS
        typedef Vector::Scalar Scalar;
    typedef VDBVoxelSolverModule<LatticeType_> Base;
    using Base::m_obj;
    using Base::m_minCoord;
    using Base::VDBtoLatticeCoord;
    using Base::latticeToVDBCoord;
    typedef LatticeType_ LatticeType;
    //TODO: currently this is always a dense grid, should change that someday
#if 0
    typedef MeshVolumeVoxelObject<LatticeType> ObjectType;
#else
    typedef MeshVolumeVoxelObject<DenseLattice> ObjectType;
#endif
    typedef ObjectType::Ptr ObjectPtr;
public:
    VDBLinearFEMSolverModule(): integrator(3) {}
    void solve();
    void updateStencil();
    void setObject(ObjectPtr obj);
    rms::VectorGridPtr writeVectorResult(int mode=0);
    //void setConstraints() {}
private:
    inline int32_t mat_ind(int32_t value, int32_t dim) const {return 3*value + dim;}
    SparseMatrix M;
    Vector rhs;
    Vector x;
    
    std::vector<int> m_vertex_neighbors;
            LegendreCubeIntegrator integrator;
};
#include "linearElasticSolver.hpp"

#endif
