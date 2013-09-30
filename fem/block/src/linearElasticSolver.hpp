#include "linearElasticSolver.h"

template <typename LatticeType>
void VDBLinearFEMSolverModule<LatticeType>::setObject(ObjectPtr obj){
    VDBVoxelSolverModule<LatticeType>::setObject(obj);

}



template <typename LatticeType>
void VDBLinearFEMSolverModule<LatticeType>::updateStencil() {

    if(!m_obj) {
        std::cout << "No object set... shouldn't be updating stencil" << std::endl;
        return;
    }

    const LatticeType& vL = m_obj->getDOFs();
    m_vertex_neighbors.resize(3*vL.activeCount());
    std::vector<int>::iterator begin=m_vertex_neighbors.begin();
    std::vector<int>::iterator end=m_vertex_neighbors.end();
    std::fill(begin,end,0);
    ActiveLatticeIterator a = m_obj->activeVertexIterator();

    int totalReserve = 0;
    for(ActiveLatticeIterator active = a; active; ++active) {
        for(int32_t m=-1; m < 2; ++m) {
            for(int32_t n=-1; n < 2; ++n) {
                for(int32_t l=-1; l < 2; ++l) {
                    //if((m & n & l) == 0) 
                    SIndex3 tmp = active.index() + SIndex3(m,n,l);
                    //if(!m_obj->validVertexIndex3(tmp)) continue;
                    if(m_obj->getVertex(tmp.i,tmp.j,tmp.k) != -1) {
                        m_vertex_neighbors[3*active.value()] += 1;
                        m_vertex_neighbors[3*active.value()+1] += 1;
                        m_vertex_neighbors[3*active.value()+2] += 1;
                        totalReserve+=3;
                    }

                }
            }
        }
    }
    int numDOFs = 3*vL.activeCount();
    int32_t NI=vL.NI();
    int32_t NJ=vL.NJ();
    int32_t NK=vL.NK();
    std::cout << "Allocating sparse matrix of size: " << numDOFs << std::endl;
    M = SparseMatrix(numDOFs,numDOFs);
    rhs = Vector::Zero(numDOFs);
    //Pre-reserve the places where we will have fillin
    M.reserve(m_vertex_neighbors);


    typedef Eigen::Triplet<Scalar> Triplet;
    typedef std::vector<Triplet> Triplets;
    Triplets triplets;
    triplets.reserve(totalReserve);
    std::vector<bool> used_constraints(numDOFs);
    //rms::ScalarGrid::ConstAccessor accessor = GetDistanceGrid()->getConstAccessor();
    rms::VectorGridPtr velocityFieldPtr = m_obj->getVectorField();
    rms::ScalarGridPtr rigidFieldPtr = m_obj->getRigidField();
    rms::ScalarGridPtr heatFieldPtr = m_obj->getHeatField();
    rms::RGBAGridPtr materialFieldPtr = m_obj->getMaterialField();
    rms::ScalarGrid::ConstAccessor heatAccessor = heatFieldPtr->getConstAccessor();
    rms::ScalarGrid::ConstAccessor rigidAccessor = rigidFieldPtr->getConstAccessor();
    rms::RGBAGrid::ConstAccessor materialAccessor = materialFieldPtr->getConstAccessor();
    rms::VectorGrid::ConstAccessor velocityAccessor = velocityFieldPtr->getConstAccessor();
    int count=0;

    std::cout << "About to look at heat field" << std::endl;
    Scalar scale = heatFieldPtr->transform().voxelVolume();
    std::set<int32_t> used_vertex;
    StiffnessEntryStorage store(scale,integrator);//TODO: need to fix this
    std::cout << "Mincoord: " << m_minCoord << std::endl;
    vdb::CoordBBox bbox = m_obj->getDistanceField()->evalActiveVoxelBoundingBox();
    std::cout << bbox.min() << " " << bbox.max() << std::endl;
    m_minCoord = bbox.min();
    for(rms::ScalarGrid::ValueOnCIter it =  m_obj->getDistanceField()->cbeginValueOn(); it; ++it) {
        if(it.getValue() >= 0) continue;
        /*
        if(count++ % 1000 == 0) {
            std::cout << count << "/" << numDOFs << std::endl;
        }
        */
        std::cout << "Working in voxel: " << it.getCoord() << std::endl;
        Index3 idx = VDBtoLatticeCoord(it.getCoord());
        //std::cout << "True index: " << idx << " " << it.getCoord() << std::endl;
        /*
           if(m_obj->getVertex(idx.i,idx.j,idx.k) == -1) {
           std::cout << "bottomleftinner lattice index doesn't exist, clearly lattice just isn't populated"<< std::endl;
           }
         */
        const vdb::Coord vdbcoord = m_minCoord + vdb::Coord(idx.i,idx.j,idx.k);

        vdb::Vec4f material = materialAccessor.getValue(vdbcoord);
        vdb::Vec3f velocity = velocityAccessor.getValue(vdbcoord);
        float lambda = material(0);
        float mu = material(1);
        float density = material(3) * 0.125;//Divide because each basis function only takes on 1/8th of a whole cube
        for(VDBVoxelNodeIterator<LatticeType> alpha(m_obj->getDOFs(),idx); alpha; ++alpha) {
            const Index3 & alpha_index = alpha.index();
            const vdb::Coord alpha_vdbcoord = m_minCoord + vdb::Coord(alpha_index.i,alpha_index.j,alpha_index.k);
            const int32_t alpha_value = alpha.value();
            const int32_t alphaind = mat_ind(alpha_value,0);
            //const LinearElasticVertexProperties & alpha_props = m_vertex_properties[alpha_value];
            //alpha sets the matrix, betas set the rhs values for alpha
            std::cout << alpha_vdbcoord << ": ";
            if(rigidAccessor.getValue(alpha_vdbcoord) < 0) {
                std::cout << "Rigid!" << std::endl;
                if(used_vertex.find(alpha_value) == used_vertex.end()) {
                    used_vertex.insert(alpha_value);
                    const int32_t ind = alphaind;
                    triplets.push_back(Triplet(ind,ind,1));
                    triplets.push_back(Triplet(ind+1,ind+1,1));
                    triplets.push_back(Triplet(ind+2,ind+2,1));
                    std::cout << rhs(alphaind+0) << " ";
                    std::cout << rhs(alphaind+1) << " ";
                    std::cout << rhs(alphaind+2) << std::endl;
                }
            } else {
                std::cout << "Elastic!" << std::endl;
                rhs(alphaind+0) += velocity(0)*density;
                rhs(alphaind+1) += velocity(1)*density;
                rhs(alphaind+2) += velocity(2)*density;
                //std::cout << velocity << std::endl;
                for(VDBVoxelNodeIterator<LatticeType> beta(m_obj->getDOFs(),idx); beta; ++beta) {
                    const Index3 & beta_index = beta.index();
                    const int32_t beta_value = beta.value();
                    /*
                       if(alpha_value < 0 || beta_value < 0) {
                       std::cout << "WTF Shouldn't be finding vertices out of nowhere..." << alpha_value << " " << beta_value << ": " << it.getCoord()<< ":A" << alpha_index << ":B" << beta_index << std::endl;
                       std::cout << idx << " " << it.getCoord() << std::endl;
                       }*/

                    //const LinearElasticVertexProperties & beta_props = m_vertex_properties[beta_value];
                    //const Vector3 beta_externalForce = beta_props.externalForce;
                    const vdb::Coord beta_vdbcoord = m_minCoord + vdb::Coord(beta_index.i,beta_index.j,beta_index.k);
                    if(rigidAccessor.getValue(beta_vdbcoord) > 0) {
                        for(int32_t alphadim = 0; alphadim < 3; ++alphadim) {
                            const int32_t alpha_i = mat_ind(alpha_value,alphadim);
                            for(int32_t betadim = 0; betadim < 3; ++betadim) {
                                const int32_t beta_i = mat_ind(beta_value,betadim);
                                const Scalar v = store.genericTerm(alpha.pos(), beta.pos(), alphadim, betadim,lambda,mu);

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
    //std::cout << rhs.transpose() << std::endl;
    std::cout << "Number of matrix etdntries (with duplicates): " << triplets.size() << std::endl;

    M.setFromTriplets(triplets.begin(),triplets.end());
    M.prune(1,Eigen::NumTraits<Scalar>::dummy_precision());
    //std::cout << M << std::endl;
}


template <typename LatticeType>
void VDBLinearFEMSolverModule<LatticeType>::solve() {

    Eigen::PardisoLDLT<SparseMatrix,Eigen::Lower> solver(M);
    x = solver.compute(M).solve(rhs);
    //std::cout << "#iterations : " << solver.iterations() << std::endl;
    //std::cout << "error: " << solver.error() << std::endl;
    //std::cout << x.transpose() << std::endl;
    std::cout << "L2 difference: " << (rhs - M*x).norm()/rhs.norm() << std::endl;
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

    rms::VectorGridPtr velocityFieldPtr = m_obj->getVectorField();
    rms::ScalarGridPtr rigidFieldPtr = m_obj->getRigidField();
    rms::ScalarGridPtr heatFieldPtr = m_obj->getHeatField();
    rms::RGBAGridPtr materialFieldPtr = m_obj->getMaterialField();
    rms::ScalarGrid::ConstAccessor heatAccessor = heatFieldPtr->getConstAccessor();
    rms::ScalarGrid::ConstAccessor rigidAccessor = rigidFieldPtr->getConstAccessor();
    rms::RGBAGrid::ConstAccessor materialAccessor = materialFieldPtr->getConstAccessor();
    rms::VectorGrid::ConstAccessor velocityAccessor = velocityFieldPtr->getConstAccessor();
    int count=0;

    std::cout << "About to look at heat field" << std::endl;
    Scalar scale = heatFieldPtr->transform().voxelVolume();
    StiffnessEntryStorage store(scale,integrator);//TODO: need to fix this
    std::cout << "Mincoord: " << m_minCoord << std::endl;
    vdb::CoordBBox bbox = m_obj->getDistanceField()->evalActiveVoxelBoundingBox();
    std::cout << bbox.min() << " " << bbox.max() << std::endl;
    m_minCoord = bbox.min();
    for(rms::ScalarGrid::ValueOnCIter it =  m_obj->getDistanceField()->cbeginValueOn(); it; ++it) {
        if(it.getValue() >= 0) continue;
        Index3 idx = VDBtoLatticeCoord(it.getCoord());
        //std::cout << "True index: " << idx << " " << it.getCoord() << std::endl;
        /*
           if(m_obj->getVertex(idx.i,idx.j,idx.k) == -1) {
           std::cout << "bottomleftinner lattice index doesn't exist, clearly lattice just isn't populated"<< std::endl;
           }
         */
        const vdb::Coord vdbcoord = m_minCoord + vdb::Coord(idx.i,idx.j,idx.k);

        for(VDBVoxelNodeIterator<LatticeType> alpha(m_obj->getDOFs(),idx); alpha; ++alpha) {
            const Index3 & alpha_index = alpha.index();
            const vdb::Coord alpha_vdbcoord = m_minCoord + vdb::Coord(alpha_index.i,alpha_index.j,alpha_index.k);
            const int32_t alpha_value = alpha.value();
            const int32_t alphaind = mat_ind(alpha_value,0);
            //const LinearElasticVertexProperties & alpha_props = m_vertex_properties[alpha_value];
            //alpha sets the matrix, betas set the rhs values for alpha
            if(rigidAccessor.getValue(alpha_vdbcoord) < 0) {
                std::cout << alpha_vdbcoord << ": ";
                std::cout << "Rigid!" << std::endl;
                std::cout << rhs(alphaind+0) << " ";
                std::cout << rhs(alphaind+1) << " ";
                std::cout << rhs(alphaind+2) << std::endl;
                std::cout << x(alphaind+0) << " ";
                std::cout << x(alphaind+1) << " ";
                std::cout << x(alphaind+2) << std::endl;
            }
        }
    }
}






template <typename LatticeType>
rms::VectorGridPtr VDBLinearFEMSolverModule<LatticeType>::writeVectorResult(int mode) {

    rms::VectorGridPtr ret = rms::VectorGrid::create();
    ret->setTransform(m_obj->getDistanceField()->transformPtr() );
    rms::VectorGrid::Accessor accessor = ret->getAccessor();
    vdb::Coord coord;
    for(ActiveLatticeIterator it(m_obj->getDOFs());it; ++it) {
        int s = 3 * it.value();
        openvdb::Vec3d v(s,s+1,s+2);
        std::cout << __FUNCTION__ << ": " << latticeToVDBCoord(it.index()) << std::endl;
        accessor.setValue(latticeToVDBCoord(it.index()), v);
    }
    /*
       for(rms::ScalarGrid::ValueOnCIter it =  m_obj->getDistanceField()->cbeginValueOn(); it; ++it) {
       if(it.getValue() > 0) continue;
       auto&& c = it.getCoord();
       int32_t i = m_obj->getVertex(c.x()-m_minCoord.x(),c.y()-m_minCoord.y(),c.z()-m_minCoord.z());
       if(i >= 0)
       accessor.setValue(it.getCoord(),vdb::Vec3d(x(3*i),x((3*i)+1),x((3*i)+2)));

       }
     */
    return ret;
}
