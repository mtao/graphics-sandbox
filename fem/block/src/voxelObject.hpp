
#include <algorithm>
template <typename LatticeType>
VoxelObject<LatticeType>::
    VoxelObject(size_t i, size_t j, size_t k)
    : m_voxel_lattice(i,j,k)
      , m_vertex_lattice(i+1,j+1,k+1)
{
    m_transform = Eigen::Translation<float,3>(-.5,-.5,-.5) * Eigen::Scaling<float>(1.0f/i,1.0/j,1.0/k);
}


template <typename LatticeType>
void VoxelObject<LatticeType>::
clearConstraints() {
    std::fill(m_vertex_properties.begin(),m_vertex_properties.end(),VertexProperties());
}
template <typename LatticeType>
void VoxelObject<LatticeType>::
makeCompressed() {
    //TODO: use the map generated to remap material properties
    m_voxel_lattice.compactifyIndices();
    std::map<int,int> propMap = m_vertex_lattice.compactifyIndices();
    //std::cout << "Reordering things..." << std::endl;
    std::vector<VertexProperties> newprop(m_vertex_properties.size());
    for(const std::pair<int,int> & p: propMap) {
        //std::cout << p.first << "->" << p.second << ":    " << m_vertex_properties[p.first].voxelCount << std::endl;
        newprop[p.second] = m_vertex_properties[p.first];
    }
    //std::cout << "Reordering things is DONE" << std::endl;
    m_vertex_properties.swap(newprop);//Really should use a std::move if it were available..
}
template <typename LatticeType>
void VoxelObject<LatticeType>::
addVertex(size_t i, size_t j, size_t k) {
    bool b;
      //  std::cout << Index{i,j,k} << std::endl;
    int v = m_vertex_lattice.add(i,j,k, b);
    if(b) {
      //  std::cout << "Added vertex! " << v << ": " << Index{i,j,k} << std::endl;
        m_vertex_properties.push_back(VertexProperties());
        m_vertex_properties[v].voxelCount=1;

    } else {
      //  std::cout << "Vertex was already there!" << v <<": " << Index{i,j,k} << std::endl;
        m_vertex_properties[v].voxelCount++;
        m_vertex_properties[v].isBound=true;
    }
}

template <typename LatticeType>
void VoxelObject<LatticeType>::
addVertex(const Index & idx) {
    addVertex(idx.i,idx.j,idx.k);
}
template <typename LatticeType>
void VoxelObject<LatticeType>::
addVertices(size_t i, size_t j, size_t k) {
    //Assumes not bound
    for(int m=0; m<2; ++m)
        for(int o=0; o<2; ++o)
            for(int l=0; l<2; ++l)
            {
                //std::cout << Index{i+m,j+o,k+l} << std::endl;
                addVertex(i+m,j+o,k+l);
            }
}

template <typename LatticeType>
void VoxelObject<LatticeType>::
addVertices(const Index & idx) {
    addVertices(idx.i,idx.j,idx.k);
}

template <typename LatticeType>
int VoxelObject<LatticeType>::
addVoxel(size_t i, size_t j, size_t k) {
    bool nv;
    //std::cout << "Adding voxel?" << Index{i,j,k} << std::endl;
    //std::cout << m_voxel_lattice.NI() << " " << m_voxel_lattice.NJ() << " " << m_voxel_lattice.size() << std::endl;
    int idx = m_voxel_lattice.add(i,j,k, nv);
    //std::cout << "Added voxel! " << idx << " " << nv<< std::endl;
    if(nv)
        addVertices(i,j,k);
    return idx;

}
template <typename LatticeType>
int VoxelObject<LatticeType>::
addVoxel(const Index & idx) {
    addVoxel(idx.i,idx.j,idx.k);
}
template <typename LatticeType>
void VoxelObject<LatticeType>::
setConstraint(size_t i, size_t j, size_t k, float v) {
    VertexProperties & p= m_vertex_properties[m_vertex_lattice(i,j,k)];
    p.isConstrained = true;
    p.value = v;

}

template <typename LatticeType>
VoxelRenderPackage VoxelObject<LatticeType>::
getRenderData() {
    if(m_data.rows() != numDOFs())
    m_data = Vector::Random(getDOFs().activeCount());
    std::vector<VoxelData> result(getDOFs().activeCount());
    float min=m_data.minCoeff();
    float max=m_data.maxCoeff();
    float range = max - min;
    for(auto&& it=m_vertex_lattice.activeIterator(); it; ++it) {
        Vector3 val(it.index().i, it.index().j, it.index().k);
        val = m_transform * val.homogeneous();
        result[it.value()] = 
        {
            val
                ,
                (m_data(it.value())-min)/range
        };
    }
    std::vector<unsigned int> indices;
    for(auto&& it=m_voxel_lattice.activeIterator(); it; ++it) {
        const Index & idx = it.index();
        unsigned int i000 = (m_vertex_lattice(idx+SIndex{0,0,0}));
        unsigned int i001 = (m_vertex_lattice(idx+SIndex{0,0,1}));
        unsigned int i010 = (m_vertex_lattice(idx+SIndex{0,1,0}));
        unsigned int i011 = (m_vertex_lattice(idx+SIndex{0,1,1}));
        unsigned int i100 = (m_vertex_lattice(idx+SIndex{1,0,0}));
        unsigned int i101 = (m_vertex_lattice(idx+SIndex{1,0,1}));
        unsigned int i110 = (m_vertex_lattice(idx+SIndex{1,1,0}));
        unsigned int i111 = (m_vertex_lattice(idx+SIndex{1,1,1}));
        unsigned int reorder[8] = {i001,i101,i111,i011,i000,i100,i110,i010};

        indices.push_back(reorder[0]);
        indices.push_back(reorder[1]);
        indices.push_back(reorder[2]);

        indices.push_back(reorder[2]);
        indices.push_back(reorder[3]);
        indices.push_back(reorder[0]);

        indices.push_back(reorder[1]);
        indices.push_back(reorder[5]);
        indices.push_back(reorder[6]);

        indices.push_back(reorder[6]);
        indices.push_back(reorder[2]);
        indices.push_back(reorder[1]);

        indices.push_back(reorder[7]);
        indices.push_back(reorder[6]);
        indices.push_back(reorder[5]);

        indices.push_back(reorder[5]);
        indices.push_back(reorder[4]);
        indices.push_back(reorder[7]);

        indices.push_back(reorder[4]);
        indices.push_back(reorder[0]);
        indices.push_back(reorder[3]);

        indices.push_back(reorder[3]);
        indices.push_back(reorder[7]);
        indices.push_back(reorder[4]);

        indices.push_back(reorder[4]);
        indices.push_back(reorder[5]);
        indices.push_back(reorder[1]);

        indices.push_back(reorder[1]);
        indices.push_back(reorder[0]);
        indices.push_back(reorder[4]);

        indices.push_back(reorder[3]);
        indices.push_back(reorder[2]);
        indices.push_back(reorder[6]);

        indices.push_back(reorder[6]);
        indices.push_back(reorder[7]);
        indices.push_back(reorder[3]);
    };
    return {result,indices};
}


template <typename LatticeType>
LatticeType & VoxelObject<LatticeType>::
getDOFs() {
    return m_vertex_lattice;
    
}

template <typename LatticeType>
size_t VoxelObject<LatticeType>::
numDOFs() const{
    return m_vertex_lattice.activeCount();
    
}

//Index functions from the lattice type
template <typename LatticeType, typename VoxelType>
inline size_t InstancedVoxelObjectBase<LatticeType, VoxelType>::
ijk2idx(size_t i, size_t j, size_t k) const
{
    return m_voxel_lattice.ijk2idx(i,j,k);
}
template <typename LatticeType, typename VoxelType>
inline size_t InstancedVoxelObjectBase<LatticeType, VoxelType>::
ijk2idx(const Index & idx) const
{
    return m_voxel_lattice.ijk2idx(idx);
}
template <typename LatticeType, typename VoxelType>
inline int InstancedVoxelObjectBase<LatticeType, VoxelType>::
ijk2sidx(const SIndex & idx) const
{
    return m_voxel_lattice.ijk2sidx(idx);
}

#include <iostream>
//How to add values
template <typename LatticeType, typename VoxelType>
void InstancedVoxelObjectBase<LatticeType, VoxelType>::
addValue(SparseMatrix & A, const Index & rowidx, const SIndex & offset) {
    int i=rowidx.i+offset.i, j=rowidx.j+offset.j, k=rowidx.k+offset.k;
    if(
            i < 0 || i > NI()
            ||j < 0 || j > NJ()
            ||k < 0 || k > NK()
      )
        return;
    //If there isn't anything there dont add it to the matrix
    if(m_vertex_lattice(rowidx+offset)<0) return;
    size_t row = m_vertex_lattice(rowidx);
    size_t col = m_vertex_lattice(rowidx+offset);

    /*
       size_t row = ijk2idx(rowidx);
       size_t col = row + ijk2sidx(offset);
       */
    float value = Voxel::template stiffness<Voxel>(offset, (1./NI()+1./NJ()+1./NK())/3.);
#ifdef USE_BOUNDARY
    bool itself = offset == SIndex{0,0,0};
    //Two cases:
    //  -We are integrating with ourself, which means we divide by the number of corners we are part of
    //  -We are integrating with something next to us, so we have to divide if we share an edge
    /*
    if((itself || offset.i != 0) && (rowidx.i == 0 || rowidx.i == NI())) value /= 2;
    if((itself || offset.j != 0) && (rowidx.j == 0 || rowidx.j == NJ())) value /= 2;
    if((itself || offset.k != 0) && (rowidx.k == 0 || rowidx.k == NK())) value /= 2;
    */
    
#endif

    A.coeffRef(row,col) = value;
}
template <typename LatticeType, typename VoxelType>
auto InstancedVoxelObjectBase<LatticeType, VoxelType>::
getStiffnessMatrix(Vector & rhs) -> SparseMatrix{
    makeCompressed();
    SparseMatrix result(numDOFs(),numDOFs());
    rhs = Vector::Zero(numDOFs());
    //Prereserve the places where we will have fillin
    std::vector<size_t> reserveSizes(numDOFs(),0);
    std::transform(m_vertex_properties.begin(),m_vertex_properties.end(), reserveSizes.begin(),[](const VertexProperties & v) -> int {
            return v.voxelCount;
            });
    result.reserve(reserveSizes);


    /*
    for(auto&& a: m_vertex_lattice) {
        std::cout << a << " ";
    }
    std::cout <<std::endl;
    */
    const float spacing = (1./NI()+1./NJ()+1./NK())/3.;
#define ITERATE_VOXELS
#ifdef ITERATE_VOXELS
//#define USE_TRIPLETS
#ifdef USE_TRIPLETS
    typedef Eigen::Triplet<float> Triplet;
    typedef std::vector<Triplet> Triplets;
    Triplets triplets;
    std::vector<bool> used_constraints(numDOFs());
#endif
    for(ActiveLatticeIterator<LatticeType> active = getVoxels().activeIterator(); active; ++active) {
        //Figure out the stuff to add into this element
        const Index & idx = active.index();
        for(int m=0; m < 2; ++m) {
            for(int n=0; n < 2; ++n) {
                for(int l=0; l < 2; ++l) {
                    const int row = m_vertex_lattice(idx.i+m,idx.j+n,idx.k+l);
                    VertexProperties & rp = m_vertex_properties[row];
                    if(rp.isConstrained) {
                        rhs(row) = rp.value;
#ifdef USE_TRIPLETS
                        if(!used_constraints[row]) {
                            triplets.push_back(Triplet(row,row,1));
                            used_constraints[row] = true;
                        }
#else
                        result.coeffRef(row,row) = 1;
#endif
                    }
                    for(int m2=0; m2 < 2; ++m2) {
                        for(int n2=0; n2 < 2; ++n2) {
                            for(int l2=0; l2 < 2; ++l2) {

                                const int col = m_vertex_lattice(idx.i+m2,idx.j+n2,idx.k+l2);
                                VertexProperties & cp = m_vertex_properties[col];
                                const float value = Voxel::template stiffness<Voxel>(m-m2,n-n2,l-l2, spacing);
                                if(cp.isConstrained) continue;
                                if(rp.isConstrained) {
                                    if(row != col)
                                        rhs(col) -= value * rp.value;
                                } else {
#ifdef USE_TRIPLETS
                                    triplets.push_back(Triplet(row,col,value));
#else
                                    result.coeffRef(row,col) += value;
                                    #endif
                                    //rhs(col) += spacing*spacing*spacing/27;//Voxel::template load<Voxel>(m-m2,n-n2,l-l2,spacing);
                                }


                            }
                        }
                    }
                }
            }
        }
    }
#ifdef USE_TRIPLETS
    result.setFromTriplets(triplets.begin(),triplets.end());
#endif
#else//Iterate DOFs
    for(ActiveLatticeIterator<LatticeType> active = getDOFs().activeIterator(); active; ++active) {
        SparseMatrix::InnerIterator rowit(A,active.value());
        const Index & idx = active.index();
        for(int l=-1; l < 2; ++l) {
            for(int m=-1; m < 2; ++m) {
                for(int n=-1; n < 2; ++n) {

                    const int col = m_vertex_lattice(idx.i+l,idx.j+m,idx.k+n);
                    /*
                    if(col != rowit.col()) {
                        std::cout << "Checking the wrong col!" << std::endl;
                    }
                    */
                    VertexProperties & cp = m_vertex_properties[col];
                    const float value = Voxel::template stiffness<Voxel>(l,m,n, spacing);
                    if(cp.isConstrained) continue;
                    if(rp.isConstrained) {
                        if(row != col)
                            rhs(col) -= value * rp.value;
                    } else {
                        result.insert(row,col) += value;
                        //rhs(col) += spacing*spacing*spacing/27;//Voxel::template load<Voxel>(m-m2,n-n2,l-l2,spacing);
                    }
                    ++rowit;



                }
            }
        }
    }
#endif
    return result;
}



typedef InstancedVoxelObjectBase<DenseLattice, Voxel<LINEAR> > LinearHomgeneousVoxelGrid;
