//#ifndef HEADLESS_BUILD
#include "stdafx.h"
//#endif
#include "linearElasticSolver.h"
#include <boost/bind.hpp>
#include <Eigen/SparseCholesky>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/PardisoSupport>
/*
LinearElasticSolver::LinearElasticSolver(const SHARED_PTR<VoxelObjectBase> & voxelobj)
: VoxelSolver(voxelobj), m_lambda(Scalar(0.1)),m_mu(Scalar(0.1)) {
    std::cout << "Constructing..." << std::endl;
}
*/
//G = 137826 lbf/in^2
//  = 95027.6818688 N/cm^2
//\mu = 0.31
//
LinearElasticSolver::LinearElasticSolver()
: VoxelSolver(), m_lambda(Scalar(95027.6818688)),m_mu(Scalar(0.31))
//: VoxelSolver(), m_lambda(Scalar(35962.12)),m_mu(Scalar(137826))
, integrator(3){
    std::cout << "Constructing..." << std::endl;
}

bool LinearElasticSolver:: 
doTimestep(Scalar dt) {
    std::cout << "Starting to solve linear FEM!" << std::endl;
    if(!getObject()) {
        std::cerr << "No object found!" << std::endl;
        return false;
    }
    //minimize the number of unused indices / clean up the active set in lattice
    
    std::cout << "Compressing object..." << std::endl;
    makeCompressed();
    std::cout << "Computing number of active voxels:" << getObject()->activeVoxels() << std::endl;
    std::cout << "Computing number of DOFS:" << getObject()->numDOFs() << std::endl;
    std::cout <<"Generating A" << std::endl;
    Vector rhs;
    SparseMatrix A = getStiffnessMatrix(rhs,dt);
    //A *= 1.0f/(getObject()->N());
    uint32_t rows = A.rows();

    //std::cout << A << std::endl;
    Vector & x = m_data = (Vector::Zero(rows));

//    rhs /= getObject()->N()*getObject()->NI();
    std::cout << "A size: " << A.rows() << " " << A.cols() << std::endl;
    std::cout << "Calling the solver..." << std::endl;
//    Eigen::ConjugateGradient<SparseMatrix, Eigen::Lower, Eigen::DiagonalPreconditioner<Scalar> > solver;//(A);
    Eigen::PardisoLDLT<SparseMatrix,Eigen::Lower> solver(A);
    x = solver.compute(A).solve(rhs);
    //std::cout << "#iterations : " << solver.iterations() << std::endl;
    //std::cout << "error: " << solver.error() << std::endl;
    std::cout << "L2 difference: " << (rhs - A*x).norm()/rhs.norm() << std::endl;
    switch(solver.info()) {
        case Eigen::NumericalIssue:
            std::cout << "Numerical Issue!" << std::endl;
            //return false;
            break;
        case Eigen::NoConvergence:
            std::cout << "No Convergence!" << std::endl;
            //return false;
            break;
        case Eigen::InvalidInput:
            std::cout << "Invalid Input!" << std::endl;
            //return false;
            break;
        case Eigen::Success:
            std::cout << "Success!" << std::endl;
            break;
    }
//    ConjugateGradientSolve(A,rhs,x);
    //SparseCholeskyPreconditionedConjugateGradientSolve(A,rhs,x);
    std::cout << "RHS is: " << std::endl;
    std::cout << rhs.transpose() << std::endl;
    std::cout << "Reconstructed RHS: " << std::endl;
    std::cout << (A * x).transpose() << std::endl;
    //std::cout << std::endl;
    std::cout << "Answer x is: " << std::endl;
    x /= 100;
    std::cout << x.transpose() << std::endl;
    //std::cout << std::endl;
    
    for(int32_t i=0; i < m_vertex_properties.size(); ++i) {
        int32_t s = 3*i;
        m_vertex_properties[i].pos(0) += x(s+0);
        m_vertex_properties[i].pos(1) += x(s+1);
        m_vertex_properties[i].pos(2) += x(s+2);
    }
    

    return true;
}



void LinearElasticSolver::
setObject(const SHARED_PTR<VoxelObjectBase> & voxelobj) {
    m_voxel_obj = voxelobj;
    VoxelObjectBase & obj = *voxelobj;
    obj.compactifyVertices();
    obj.compactifyVoxels();
    m_vertex_properties.resize(obj.numDOFs());
    //m_voxel_properties.resize(obj.numVoxels());
    std::vector<LinearElasticVertexProperties>::iterator it=m_vertex_properties.begin();
    std::vector<LinearElasticVertexProperties>::iterator end=m_vertex_properties.end();
//    std::fill(begin,end,LinearElasticVertexProperties());
    /*
    for(std::vector<PoissonVertexProperties> it=m_vertex_properties.begin(); it != end; ++it) {


    }
    */
    std::cout << "Filling neighbor counts and undeformed positions" << std::endl;
    Scalar NI = static_cast<Scalar>(m_voxel_obj->NI());
    Scalar NJ = static_cast<Scalar>(m_voxel_obj->NJ());
    Scalar NK = static_cast<Scalar>(m_voxel_obj->NK());
    ActiveLatticeIterator a = m_voxel_obj->activeVertexIterator();
    for(ActiveLatticeIterator active = a; active; ++active) {
        LinearElasticVertexProperties & props = m_vertex_properties[active.value()];
        const Index3 & idx = active.index();
        std::cout << Vector3(static_cast<Scalar>(idx.i),static_cast<Scalar>(idx.j),static_cast<Scalar>(idx.k)).transpose() << std::endl;
        props.undeformedPos = m_voxel_obj->getTransform().cast<Scalar>() * Vector3(static_cast<Scalar>(idx.i),static_cast<Scalar>(idx.j),static_cast<Scalar>(idx.k)).homogeneous();
        //props.undeformedPos = Vector3(idx.i/(NI),idx.j/(NJ),idx.k/(NK));
        props.pos = props.undeformedPos;
        std::cout << props.pos.transpose() << std::endl;
        for(int32_t m=0; m < 2; ++m) {
            for(int32_t n=0; n < 2; ++n) {
                for(int32_t l=0; l < 2; ++l) {
                    //if((m & n & l) == 0) 
                    SIndex3 tmp = idx + SIndex3(m,n,l);
                    if(!m_voxel_obj->validVertexSIndex3(tmp)) continue;
                    if(m_voxel_obj->getVertex(tmp.i,tmp.j,tmp.k) != -1)
                        props.neighborCount += 1;

                }
            }
        }
    }
}

LinearElasticSolver::SparseMatrix LinearElasticSolver::getStiffnessMatrix(Vector & rhs, Scalar dt) {
    typedef Eigen::Triplet<Scalar> Triplet;
    typedef std::vector<Triplet> Triplets;
    Triplets triplets;
    /*
    for(ActiveLatticeIterator voxel = m_voxel_obj->activeVertexIterator(); voxel; ++voxel) {
        const Index3 & idx = voxel.index();
        std::cout << voxel.index() << ": " << voxel.value() << std::endl;
    }
    for(ActiveLatticeIterator voxel = m_voxel_obj->activeVoxelIterator(); voxel; ++voxel) {
        const Index3 & idx = voxel.index();
        std::cout << voxel.index() << ": " << voxel.value() << std::endl;
        for(VoxelNodeIterator alpha = m_voxel_obj->getVoxelNodeIterator(idx); alpha; ++alpha) {
            const Index3 & alpha_index = alpha.index();
            const int32_t alpha_value = alpha.value();
                std::cout << "    " << alpha_index << ": " << alpha_value << std::endl;
        }
    }
    */
    rhs.resize(3*m_voxel_obj->numDOFs());
    rhs.setZero();
    std::set<int32_t> used_vertex;
    StiffnessEntryStorage store(*m_voxel_obj,m_vertex_properties, integrator);
    for(ActiveLatticeIterator voxel = m_voxel_obj->activeVoxelIterator(); voxel; ++voxel) {
        std::cout << voxel.value() << "/" << m_voxel_obj->activeVoxels() << std::endl;
#ifdef HETEROGENEOUS_SYSTEM
        const LinearElasticVoxelProperties & prop = m_voxel_properties[voxel.value()];
        const Scalar lambda = prop.lambda;
        const Scalar mu = prop.mu;
#else
        const Scalar lambda = m_lambda;
        const Scalar mu = m_mu;
#endif
        const Index3 & idx = voxel.index();
        for(VoxelNodeIterator alpha = m_voxel_obj->getVoxelNodeIterator(idx); alpha; ++alpha) {
            const Index3 & alpha_index = alpha.index();
            const int32_t alpha_value = alpha.value();
            const LinearElasticVertexProperties & alpha_props = m_vertex_properties[alpha_value];
            //alpha sets the matrix, betas set the rhs values for alpha
            if(alpha_props.isConstrained) {
                if(used_vertex.find(alpha_value) == used_vertex.end()) {
                    used_vertex.insert(alpha_value);
                    const int32_t ind = mat_ind(alpha_value,0);
                    triplets.push_back(Triplet(ind,ind,1));
                    triplets.push_back(Triplet(ind+1,ind+1,1));
                    triplets.push_back(Triplet(ind+2,ind+2,1));
                }
            } else {
            rhs(3*alpha_value+0) += alpha_props.externalForce(0);
            rhs(3*alpha_value+1) += alpha_props.externalForce(1) * .125 * 1e-8;
            rhs(3*alpha_value+2) += alpha_props.externalForce(2);
            for(VoxelNodeIterator beta = m_voxel_obj->getVoxelNodeIterator(idx); beta; ++beta) {
                const Index3 & beta_index = beta.index();
                const int32_t beta_value = beta.value();
                const LinearElasticVertexProperties & beta_props = m_vertex_properties[beta_value];
                const Vector3 beta_externalForce = beta_props.externalForce;
                for(int32_t alphadim = 0; alphadim < 3; ++alphadim) {
                    const int32_t alpha_i = mat_ind(alpha_value,alphadim);
                    for(int32_t betadim = 0; betadim < 3; ++betadim) {
                        const int32_t beta_i = mat_ind(beta_value,betadim);
                        const Scalar v = store.genericTerm(alpha.pos(), beta.pos(), alphadim, betadim,lambda,mu);
                        if(beta_props.isConstrained) {
                            //rhs(beta_i) += v*externalForce;

                        } else {
                            triplets.push_back(Triplet(alpha_i,beta_i,
                                        (
                                         v
                                        )
                                        ));

                        }
                    }
                }
                /*
                   const SIndex3 rel_alpha = alpha_index - idx;
                   const int32_t ind_alpha = rel_alpha.i * 4 + rel_alpha.j * 2 + rel_alpha.k;

                   const SIndex3 rel_beta = beta_index - idx;
                   const int32_t ind_beta = rel_beta.i * 4 + rel_beta.j * 2 + rel_beta.k;
                   */
                //const Scalar diag = integrator.i(boost::bind(&StiffnessEntryFunctor::diagonalTermi
                const Scalar diag = store.diagonalTerm(alpha.pos(), beta.pos(), mu);
                for(int32_t dim = 0; dim < 3; ++dim) {
                    const int32_t ai = mat_ind(alpha_value,dim);
                    const int32_t bi = mat_ind(beta_value,dim);
                    triplets.push_back(Triplet(ai,bi,
                                (
                                 diag
                                )
                                ));

                }
            }
            }
        }
    }
    std::cout << std::endl;

    SparseMatrix ret(3*m_voxel_obj->numDOFs(), 3*m_voxel_obj->numDOFs());
    ret.setFromTriplets(triplets.begin(), triplets.end());
    ret.prune(1,Eigen::NumTraits<Scalar>::dummy_precision());
    /*
       for(int32_t i=0; i < 3*m_voxel_obj->numDOFs(); ++i) {
       for(int32_t j=0; j < 3*m_voxel_obj->numDOFs(); ++j) {
       Scalar & v = ret.coeffRef(i,j);
       if(std::abs(v) < 1e-8) {
       v = 0;
       }
       }
       }
       */
    return ret;
}

void LinearElasticSolver::
clearConstraints() {
    for(int32_t i=0; i < m_vertex_properties.size(); ++i) {
        m_vertex_properties[i].isConstrained = false;
    }
}

void LinearElasticSolver::
setGravity() {
    //.8 m/s => 80cm/s
    //dx = cm/vox
    Vector3 gravity(0,-80 / m_voxel_obj->dx(1),0);
    for(int i=0; i < m_vertex_properties.size(); ++i) {
        setExternalForce(i,gravity);
    }
}
void LinearElasticSolver::
setExternalForce(uint32_t i, uint32_t j, uint32_t k, const Vector3 & v) {
    int32_t idx = m_voxel_obj->getVertex(i,j,k);
    if(idx == -1) return;
    setExternalForce(idx,v);
}
void LinearElasticSolver::
setExternalForce(uint32_t idx, const Vector3 & v) {
    std::cout << v.transpose() << std::endl;
    m_vertex_properties[idx].externalForce = v;
}
void LinearElasticSolver::
setConstraint(uint32_t i, uint32_t j, uint32_t k, const Vector3 & v) {
    int32_t idx = m_voxel_obj->getVertex(i,j,k);
    if(idx == -1) return;
    m_vertex_properties[idx].isConstrained = true;
    m_vertex_properties[idx].pos += v;
}

void LinearElasticSolver::
makeCompressed() {
    //TODO: use the map generated to remap material properties
    m_voxel_obj->compactifyVoxels();
    std::map<int32_t,int32_t> propMap = m_voxel_obj->compactifyVertices();
    std::vector<LinearElasticVertexProperties> newprop(m_vertex_properties.size());
    for(std::map<int32_t,int32_t>::iterator p = propMap.begin(); p != propMap.end(); ++p) {
        newprop[p->second] = m_vertex_properties[p->first];
    }
    m_vertex_properties.swap(newprop);//Really should use a std::move if it were available..
}
VoxelRenderPackage LinearElasticSolver::getRenderData() const{

    std::cout << "Creating render data" << std::endl;
    uint32_t activeCount = m_voxel_obj->numDOFs();

    std::vector<Vector3f> colors(activeCount);
    std::vector<Vector3f> vertices(activeCount);

    Scalar max = 0;
    for(std::vector<LinearElasticVertexProperties>::const_iterator it=m_vertex_properties.begin(); it != m_vertex_properties.end(); ++it) {
        const Scalar v = (it->pos - it->undeformedPos).norm();
        max = std::max(v,max);
    }
    for(uint32_t i=0; i < activeCount; ++i) {
        vertices[i] = (m_vertex_properties[i].pos).cast<float>();
        //vertices[i] = (m_vertex_properties[i].undeformedPos).cast<float>();

        const Scalar v = (m_vertex_properties[i].pos - m_vertex_properties[i].undeformedPos).norm()/max;
        colors[i] = Vector3f(2*v-1,1-abs(2*v-1),1-2*v);
        //colors[i] = (m_vertex_properties[i].pos - m_vertex_properties[i].undeformedPos)/(2*max)+Vector3f(0.5,0.5,0.5);
        /*
           if(m_vertex_properties[i].isConstrained) {
           colors[i] = Vector3f(1,1,1);
           } else {
           colors[i] = Vector3f(0,200*(m_vertex_properties[i].pos - m_vertex_properties[i].undeformedPos).norm(),0);
           }
           */
    }


    /*
       for(int32_t i=0; i < activeCount; ++i) {
       vertices[activeCount + i] = (m_voxel_obj->getTransform() * m_vertex_properties[i].undeformedPos.homogeneous()).cast<float>();

       if(m_vertex_properties[i].isConstrained) {
       colors[activeCount + i] = Vector3f(1,1,1);
       } else {
       colors[activeCount + i] = Vector3f(.5,.5,.5);
       }
       colors[activeCount + i] = Vector3f(1,1,1);
       }
       */

    VoxelRenderPackage ret;
    ret.vertices.swap(vertices);
    ret.colors.swap(colors);
    std::cout << "Render indices" << std::endl;
    ret.indices.swap(m_voxel_obj->getRenderIndices());
    /*const uint32_t numind = ret.indices.size();
      ret.indices.insert(ret.indices.end(), ret.indices.begin(), ret.indices.end());

      for(int32_t i=numind; i < ret.indices.size(); ++i) {
      ret.indices[i] += activeCount;
      }
      */
    std::cout << "Have created render data" << std::endl;
    std::cout << ret.vertices.size() << " ";
    std::cout << ret.colors.size() << " ";
    std::cout << ret.indices.size() << std::endl;
    return ret;
}
