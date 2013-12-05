//#ifndef HEADLESS_BUILD
#include "stdafx.h"
//#endif
#include "linearElasticSolver.h"
#include <boost/bind.hpp>
StiffnessEntryFunctor::
StiffnessEntryFunctor(Scalar lambda, Scalar mu, const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props, const LegendreCubeIntegrator & integrator)
      
    : lambda(lambda)
    , mu(mu)
      , obj(obj)
      , props(props) {
          const int32_t num_nodes = integrator.getDegree() + 1;
          const int32_t total_samples = num_nodes * num_nodes * num_nodes;
          jacobians.resize(total_samples);
          m_dldx.resize(total_samples);
          m_dldxdldx.resize(total_samples);
          std::fill(jacobians.begin(),jacobians.end(),0);
          const Scalar * nodes = integrator.getNodes();
          scale = static_cast<Scalar>((obj.NI() * obj.NJ() * obj.NK() ));

          /*
          for(int32_t i = 0 ; i < num_nodes; ++i) {
              for(int32_t j = 0 ; j < num_nodes; ++j) {
                  for(int32_t k = 0 ; k < num_nodes; ++k) {
                      const int ijk = 3*(3*i+j)+k;
                      const Vector3 v(nodes[i],nodes[j],nodes[k]);
                      jacobians[ijk] = jacobian(v,idx);
                      boost::array<boost::array<Scalar,3>, 8> & lx = m_dldx[ijk];//take over name in this scope
                      for(int l=0; l < 8; ++l) {
                          for(int m=0; m < 3; ++m) {
                              lx[l][m] = dldx(l,m,v);
                          }
                      }
                  }
              }
          }
          */
          for(int32_t i = 0 ; i < num_nodes; ++i) {
              for(int32_t j = 0 ; j < num_nodes; ++j) {
                  for(int32_t k = 0 ; k < num_nodes; ++k) {
                      const int ijk = integrator.ijk2idx(i,j,k,num_nodes);
                      const Vector3 v(nodes[i],nodes[j],nodes[k]);
                      boost::array<boost::array<Scalar,3>, 8> & lx = m_dldx[ijk];//take over name in this scope
                      for(int l=0; l < 8; ++l) {
                          for(int m=0; m < 3; ++m) {
                              lx[l][m] = dldx(l,m,v);
                          }
                      }
                      jacobians[ijk] = 1;
                      /*
                      for(int a=0; a < 8; ++a) {
                          for(int b=0; b < 8; ++b) {
                              for(int da = 0; da < 3; ++ da) {
                                  for(int db = 0; db < 3; ++ db) {
                                      std::cout << i << " " << j << " " << k << ": " << genericTerm(a,b,da,db,v)
                                          - genericTermi(a,b,da,db,ijk);
                                      std::cout << std::endl;
                                      std::cout << " " <<  lambda * dldx(a,da,v) * dldx(b,db,v) + mu * dldx(a,db,v) * dldx(b,da,v);
                                      std::cout << " " <<  lambda * lx[a][da] * lx[b][db] + mu * lx[a][db] * lx[b][da] << std::endl;
                                  }
                              }
                          }
                      }
                      */
                  }
              }
          }
          /*
             for(int32_t i = 0 ; i < num_nodes; ++i) {
             for(int32_t j = 0 ; j < num_nodes; ++j) {
             for(int32_t k = 0 ; k < num_nodes; ++k) {
             const int ijk = 3*(3*i+j)+k;
             const Vector3 v(nodes[i],nodes[j],nodes[k]);
             for(int l=0; l < 8; ++l) {
             for(int m=0; m < 3; ++m) {
             std::cout << m_dldx[ijk][l][m] << " " <<  dldx(l,m,v) << std::endl;
             }
             }
             }
             }
             }
             */

          for(int32_t i = 0; i <  total_samples; ++i) {
              for(int a=0; a < 8; ++a) {
                  for(int da = 0; da < 3; ++ da) {
                      for(int b=0; b < 8; ++b) {
                          for(int db = 0; db < 3; ++ db) {
                              m_dldxdldx[i][a][da][b][db] = m_dldx[i][a][da]*m_dldx[i][b][db];
                          }
                      }
                  }
              }
          }
      }

void StiffnessEntryFunctor::setJacobians(const Index3 & idx) {
        const Vector3 & origin = props[obj.getVertex(idx)].pos;
        for(int i=0; i < jacobians.size(); ++i) {
                      jacobians[i] = jacobian(i,idx,origin);
          }
}

LinearElasticSolver::Scalar StiffnessEntryFunctor::
dldx(unsigned char case_, int32_t dim, const Vector3 & v) const {
    //for each dim, if it is the dim for the partial, choose +1 -1 appropriately
    //otherwise it should just pick its dimension
    //// dp_x = yz => dim=0 case_ = 7
    Scalar x = (dim==0)?(case_&4?1:-1):(case_&4?v(0):(1-v(0)));
    Scalar y = (dim==1)?(case_&2?1:-1):(case_&2?v(1):(1-v(1)));
    Scalar z = (dim==2)?(case_&1?1:-1):(case_&1?v(2):(1-v(2)));
    return x*y*z;
}
LinearElasticSolver::Scalar StiffnessEntryFunctor::
jacobian(int ijk, const Index3 & idx, const Vector3 & origin) {
    Matrix3 m;
    m.setZero();
    //const Vector3 p = v - origin;
    //add up, for each basis function, the evaluation of 
    for(VoxelNodeIterator alpha = obj.getVoxelNodeIterator(idx); alpha; ++alpha) {
        //Recenter the position int32_to [0,1]^3 (aka barycentric coords?)
        const Vector3 & pos = props[alpha.value()].pos-origin;
        for(int32_t i=0; i <3; ++i) {
            m.row(i) += pos * m_dldx[ijk][alpha.pos()][i];
        }
    }
    return m.determinant();
}
LinearElasticSolver::Scalar StiffnessEntryFunctor::
genericTerm(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, const Vector3 &v) const {

    return lambda * dldx(alpha_case,di,v) * dldx(beta_case,dj,v) + mu * dldx(alpha_case,dj,v) * dldx(beta_case,di,v);
}

LinearElasticSolver::Scalar StiffnessEntryFunctor::
diagonalTerm(unsigned char alpha_case, unsigned char beta_case, const Vector3 &v) const {
    Scalar val = 0;
    for(int32_t dim =0; dim < 3; ++dim) {
        val += dldx(alpha_case,dim,v) * dldx(beta_case,dim,v);
    }
    return mu * val;

}

LinearElasticSolver::Scalar StiffnessEntryFunctor::
genericTermi(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, int ijk) const {

    /*
    const boost::array<boost::array<Scalar,3>, 8> & dldx = m_dldx[ijk];//take over name in this scope
    return lambda * m_dldx[ijk][alpha_case][di]* m_dldx[ijk][beta_case][dj] + mu * m_dldx[ijk][alpha_case][dj] * m_dldx[ijk][beta_case][di];
    */
       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       return lambda * dldxdldx[alpha_case][di][beta_case][dj] + mu * dldxdldx[alpha_case][dj][beta_case][di];
}

LinearElasticSolver::Scalar StiffnessEntryFunctor::
diagonalTermi(unsigned char alpha_case, unsigned char beta_case, int ijk) const {
    /*
    const boost::array<boost::array<Scalar,3>, 8> & dldx = m_dldx[ijk];//take over name in this scope
    Scalar val = 0;
    for(int32_t dim =0; dim < 3; ++dim) {
        val += dldx[alpha_case][dim] * dldx[beta_case][dim];
    }
    return mu * val;
    */

       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       Scalar val = 0;
       for(int32_t dim =0; dim < 3; ++dim) {
       val += dldxdldx[alpha_case][dim][beta_case][dim];
       }
       return mu * val;
}
StiffnessEntryStorage::
StiffnessEntryStorage(const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props, const LegendreCubeIntegrator & integrator) {
          const int32_t num_nodes = integrator.getDegree() + 1;
          const int32_t total_samples = num_nodes * num_nodes * num_nodes;
          m_dldx.resize(total_samples);
          m_dldxdldx.resize(total_samples);
          const Scalar * nodes = integrator.getNodes();

          for(int32_t i = 0 ; i < num_nodes; ++i) {
              for(int32_t j = 0 ; j < num_nodes; ++j) {
                  for(int32_t k = 0 ; k < num_nodes; ++k) {
                      const int ijk = integrator.ijk2idx(i,j,k,num_nodes);
                      const Vector3 v(nodes[i],nodes[j],nodes[k]);
                      boost::array<boost::array<Scalar,3>, 8> & lx = m_dldx[ijk];//take over name in this scope
                      for(int l=0; l < 8; ++l) {
                          for(int m=0; m < 3; ++m) {
                              lx[l][m] = dldx(l,m,v);
                              //std::cout << "dldx for (" << ijk << "," << l << "," << m << "): " << lx[l][m] << std::endl;
                          }
                      }
                  }
              }
          }
          setScale(obj,props);
          for(int32_t i = 0; i <  total_samples; ++i) {
              for(int a=0; a < 8; ++a) {
                  for(int da = 0; da < 3; ++ da) {
                      for(int b=0; b < 8; ++b) {
                          for(int db = 0; db < 3; ++ db) {
                              m_dldxdldx[i][a][da][b][db] = m_dldx[i][a][da]*m_dldx[i][b][db];
                              /*
                              std::cout << "dldxdldx for (" << i 
                                  << "," << a << "," << da
                                  << "," << b << "," << db
                                  << "): " << m_dldxdldx[i][a][da][b][db] << std::endl;
                              */
                          }
                      }
                  }
              }
          }
          for(int a=0; a < 8; ++a) {
              for(int b=0; b < 8; ++b) {
                  for(int da = 0; da < 3; ++ da) {
                      for(int db = 0; db < 3; ++ db) {
                          m_genericTerms[a][da][b][db] = std::make_pair(
                              m_scale * integrator.i(boost::bind(
                                       &StiffnessEntryStorage::_genericTermLefti,this,
                                       a,b,da,db,_1
                                      ))
                              ,
                              m_scale * integrator.i(boost::bind(
                                       &StiffnessEntryStorage::_genericTermRighti,this,
                                       a,b,da,db,_1
                                      ))
                              );
                              std::cout << "generic term for ("
                                  << a << "," << da
                                  << "," << b << "," << db
                                  << "): " << m_genericTerms[a][da][b][db].first 
                                  << ", " << m_genericTerms[a][da][b][db].second
                                              << std::endl;
                      }
                  }
              }
          }
          for(int a=0; a < 8; ++a) {
              for(int b=0; b < 8; ++b) {
                  m_diagonalTerms[a][b] = m_scale * integrator.i(boost::bind(
                              &StiffnessEntryStorage::_diagonalTermCenteri,this,
                              a,b,_1));
                              std::cout << "diagonal term for ("
                                  << a 
                                  << "," << b 
                                  << "): " << m_diagonalTerms[a][b] << std::endl;
              }
          }
      }

StiffnessEntryStorage::
StiffnessEntryStorage(float scale, const LegendreCubeIntegrator & integrator) {
    m_scale = scale;
          const int32_t num_nodes = integrator.getDegree() + 1;
          const int32_t total_samples = num_nodes * num_nodes * num_nodes;
          m_dldx.resize(total_samples);
          m_dldxdldx.resize(total_samples);
          const Scalar * nodes = integrator.getNodes();

          for(int32_t i = 0 ; i < num_nodes; ++i) {
              for(int32_t j = 0 ; j < num_nodes; ++j) {
                  for(int32_t k = 0 ; k < num_nodes; ++k) {
                      const int ijk = integrator.ijk2idx(i,j,k,num_nodes);
                      const Vector3 v(nodes[i],nodes[j],nodes[k]);
                      boost::array<boost::array<Scalar,3>, 8> & lx = m_dldx[ijk];//take over name in this scope
                      for(int l=0; l < 8; ++l) {
                          for(int m=0; m < 3; ++m) {
                              lx[l][m] = dldx(l,m,v);
                              //std::cout << "dldx for (" << ijk << "," << l << "," << m << "): " << lx[l][m] << std::endl;
                          }
                      }
                  }
              }
          }
          //setScale();
          for(int32_t i = 0; i <  total_samples; ++i) {
              for(int a=0; a < 8; ++a) {
                  for(int da = 0; da < 3; ++ da) {
                      for(int b=0; b < 8; ++b) {
                          for(int db = 0; db < 3; ++ db) {
                              m_dldxdldx[i][a][da][b][db] = m_dldx[i][a][da]*m_dldx[i][b][db];
                              /*
                              std::cout << "dldxdldx for (" << i 
                                  << "," << a << "," << da
                                  << "," << b << "," << db
                                  << "): " << m_dldxdldx[i][a][da][b][db] << std::endl;
                              */
                          }
                      }
                  }
              }
          }
          for(int a=0; a < 8; ++a) {
              for(int b=0; b < 8; ++b) {
                  for(int da = 0; da < 3; ++ da) {
                      for(int db = 0; db < 3; ++ db) {
                          m_genericTerms[a][da][b][db] = std::make_pair(
                              m_scale * integrator.i(boost::bind(
                                       &StiffnessEntryStorage::_genericTermLefti,this,
                                       a,b,da,db,_1
                                      ))
                              ,
                              m_scale * integrator.i(boost::bind(
                                       &StiffnessEntryStorage::_genericTermRighti,this,
                                       a,b,da,db,_1
                                      ))
                              );
                          /*
                              std::cout << "generic term for ("
                                  << a << "," << da
                                  << "," << b << "," << db
                                  << "): " << m_genericTerms[a][da][b][db].first 
                                  << ", " << m_genericTerms[a][da][b][db].second
                                              << std::endl;*/
                      }
                  }
              }
          }
          for(int a=0; a < 8; ++a) {
              for(int b=0; b < 8; ++b) {
                  m_diagonalTerms[a][b] = m_scale * integrator.i(boost::bind(
                              &StiffnessEntryStorage::_diagonalTermCenteri,this,
                              a,b,_1));
                  /*
                              std::cout << "diagonal term for ("
                                  << a 
                                  << "," << b 
                                  << "): " << m_diagonalTerms[a][b] << std::endl;
                                  */
              }
          }
      }
LinearElasticSolver::Scalar StiffnessEntryStorage::
dldx(unsigned char case_, int32_t dim, const Vector3 & v) const {
    //for each dim, if it is the dim for the partial, choose +1 -1 appropriately
    //otherwise it should just pick its dimension
    //// dp_x = yz => dim=0 case_ = 7
    Scalar x = (dim==0)?(case_&4?1:-1):(case_&4?v(0):(1-v(0)));
    Scalar y = (dim==1)?(case_&2?1:-1):(case_&2?v(1):(1-v(1)));
    Scalar z = (dim==2)?(case_&1?1:-1):(case_&1?v(2):(1-v(2)));
    return x*y*z;
}
void StiffnessEntryStorage::setScale(const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props  ) {
    //pick the first valid voxel
    ActiveLatticeIterator it = obj.activeVoxelIterator();

    //use origin to simplify the search
    const Vector3 & origin = props[obj.getVertex(it.index())].pos;
    m_scale = jacobian(0,it.index(),origin,obj,props);
}
LinearElasticSolver::Scalar StiffnessEntryStorage::
jacobian(int ijk, const Index3 & idx, const Vector3 & origin, const VoxelObjectBase & obj, const std::vector<LinearElasticVertexProperties> & props ) {
    Matrix3 m;
    m.setZero();
    //const Vector3 p = v - origin;
    //add up, for each basis function, the evaluation of 
    for(VoxelNodeIterator alpha = obj.getVoxelNodeIterator(idx); alpha; ++alpha) {
        //Recenter the position int32_to [0,1]^3 (aka barycentric coords?)
        const Vector3 & pos = props[alpha.value()].pos-origin;
        for(int32_t i=0; i <3; ++i) {
            std::cout << m_dldx[ijk][alpha.pos()][i] << ": " << pos.transpose() << std::endl;
            m.row(i) += pos * m_dldx[ijk][alpha.pos()][i];
        }
    }
    return m.determinant();
}

LinearElasticSolver::Scalar StiffnessEntryStorage::
_genericTerm(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, const Vector3 &v, Scalar lambda, Scalar mu) const {

    return lambda * dldx(alpha_case,di,v) * dldx(beta_case,dj,v) + mu * dldx(alpha_case,dj,v) * dldx(beta_case,di,v);
}

LinearElasticSolver::Scalar StiffnessEntryStorage::
_diagonalTerm(unsigned char alpha_case, unsigned char beta_case, const Vector3 &v, Scalar mu) const {
    Scalar val = 0;
    for(int32_t dim =0; dim < 3; ++dim) {
        val += dldx(alpha_case,dim,v) * dldx(beta_case,dim,v);
    }
    return mu * val;

}
LinearElasticSolver::Scalar StiffnessEntryStorage::
_genericTermi(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, int ijk, Scalar lambda, Scalar mu) const {

       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       return lambda * dldxdldx[alpha_case][di][beta_case][dj] + mu * dldxdldx[alpha_case][dj][beta_case][di];
}
LinearElasticSolver::Scalar StiffnessEntryStorage::
_diagonalTermi(unsigned char alpha_case, unsigned char beta_case, int ijk, Scalar mu) const {
       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       Scalar val = 0;
       for(int32_t dim =0; dim < 3; ++dim) {
       val += dldxdldx[alpha_case][dim][beta_case][dim];
       }
       return mu * val;
}





LinearElasticSolver::Scalar StiffnessEntryStorage::
diagonalTerm(unsigned char alpha_case, unsigned char beta_case, Scalar mu) const {
    Scalar val = 0;
    return mu * m_diagonalTerms[alpha_case][beta_case];

}
LinearElasticSolver::Scalar StiffnessEntryStorage::
genericTerm(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, Scalar lambda, Scalar mu) const {

    const std::pair<Scalar,Scalar> & a = m_genericTerms[alpha_case][di][beta_case][dj];
    return lambda * a.first + mu * a.second;
}









LinearElasticSolver::Scalar StiffnessEntryStorage::
_genericTermLefti(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, int ijk) const {

       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       return dldxdldx[alpha_case][di][beta_case][dj] ;
}
LinearElasticSolver::Scalar StiffnessEntryStorage::
_genericTermRighti(unsigned char alpha_case,unsigned char beta_case, int32_t di, int32_t dj, int ijk) const {

       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       return dldxdldx[alpha_case][dj][beta_case][di];
}

LinearElasticSolver::Scalar StiffnessEntryStorage::
_diagonalTermCenteri(unsigned char alpha_case, unsigned char beta_case, int ijk) const {
       const boost::array<boost::array<boost::array<boost::array<Scalar,3>, 8>,3>,8> & dldxdldx = m_dldxdldx[ijk];//take over name in this scope
       Scalar val = 0;
       for(int32_t dim =0; dim < 3; ++dim) {
       val += dldxdldx[alpha_case][dim][beta_case][dim];
       }
       return val;
}
