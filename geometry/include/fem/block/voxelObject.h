#ifndef VOXEL_OBJECT_H
#define VOXEL_OBJECT_H
#include "index.h"
#include "lattice.h"
#include "macros.h"
#include <Eigen/Sparse>
#include <Eigen/Geometry>

enum VoxelType {LINEAR};
namespace VoxelNS{//To avoid having stiffness become a global scope function

    //bitmask 
    enum class EdgeType: char{None=0,X=1,Y=2,Z=4};

    //This implementation of stiffness is inefficient / not to be used
    template <VoxelType A, VoxelType B>
        inline float stiffness(const Index & a, const Index & b, float h) {
            return stiffness<A,B>(a-b,h);
        }

    template <VoxelType A, VoxelType B>
        inline float stiffness(const SIndex & d, float h) {
            return stiffness<A,B>(d.i,d.j,d.k,h);
        }
    template <VoxelType A, VoxelType B>
        inline float stiffness(int i,int j,int k, float h) {
            return 0;
        }
    template <>
        inline float stiffness<LINEAR,LINEAR>(int i, int j, int k, float h) {
            int offset = std::abs(i) + std::abs(j) + std::abs(k);
            switch(offset) {

                case 0:
                    return h/3.;
                case 1:
                    return 0;
                case 2:
                    return -h/12.;
                case 3:
                    return -h/12.;
                default:
                    return 0;
            }

        }
    //This implementation of load is inefficient / not to be used
    template <VoxelType A, VoxelType B>
        inline float load(const Index & a, const Index & b, float h) {
            return load<A,B>(a-b,h);
        }

    template <VoxelType A, VoxelType B>
        inline float load(const SIndex & d, float h) {
            return load<A,B>(d.i,d.j,d.k,h);
        }
    template <VoxelType A, VoxelType B>
        inline float load(int i,int j,int k, float h) {
            return 0;
        }
    template <>
        inline float load<LINEAR,LINEAR>(int i, int j, int k, float h) {
            int offset = std::abs(i) + std::abs(j) + std::abs(k);
            switch(offset) {

                case 0:
                    return h*h*h/27.;
                case 1:
                    return h*h*h/54.;
                case 2:
                    return h*h*h/54.;
                case 3:
                    return h*h*h/27.;
                default:
                    return 0;
            }

        }

}

template <VoxelType VoxelType_>
class Voxel {
    public:
        static const VoxelType Type = VoxelType_;
        template <typename OtherVoxel>
            static inline float stiffness(const Index & a, const Index & b, float h) {
                return VoxelNS::stiffness<Type,OtherVoxel::Type>(a,b, h);
            }
        template <typename OtherVoxel>
            static inline float stiffness(const SIndex & d, float h) {
                return VoxelNS::stiffness<Type,OtherVoxel::Type>(d.i,d.j,d.k, h);
            }
        template <typename OtherVoxel>
            static inline float stiffness(int i, int j, int k, float h) {
                return VoxelNS::stiffness<Type,OtherVoxel::Type>(i,j,k, h);
            }
        template <typename OtherVoxel>
            static inline float load(const Index & a, const Index & b, float h) {
                return VoxelNS::load<Type,OtherVoxel::Type>(a,b, h);
            }
        template <typename OtherVoxel>
            static inline float load(const SIndex & d, float h) {
                return VoxelNS::load<Type,OtherVoxel::Type>(d.i,d.j,d.k, h);
            }
        template <typename OtherVoxel>
            static inline float load(int i, int j, int k, float h) {
                return VoxelNS::load<Type,OtherVoxel::Type>(i,j,k, h);
            }
};

struct VoxelData{
    EIGEN_DENSE_MATRIX_TYPEDEFS
    Vector3 vec;
    float val;

};
struct VoxelRenderPackage{
    std::vector<VoxelData> data;
    std::vector<unsigned int> indices;
};

class VoxelObjectBase {
    public:
        EIGEN_SPARSE_MATRIX_TYPEDEFS
            EIGEN_DENSE_MATRIX_TYPEDEFS
        VoxelObjectBase();
        virtual SparseMatrix getStiffnessMatrix(Vector & rhs) = 0;
        virtual inline size_t activeVoxels() const= 0;
        virtual inline size_t numDOFs() const= 0;
        virtual inline size_t NI() = 0;
        virtual inline size_t NJ() = 0;
        virtual inline size_t NK() = 0;
        virtual inline size_t N() = 0;
        virtual void setConstraint(size_t i, size_t j, size_t k, float v) = 0;
        virtual void clearConstraints() = 0;
        //virtual VoxelIterator begin() = 0;
        //virtual VoxelIterator end() = 0;
        //virtual ConstVoxelIterator cbegin() const = 0;
        //virtual ConstVoxelIterator cend() const = 0;

        //Optional function to allow for derived class to clean up their representations if necessary
        virtual void makeCompressed() {}
        virtual VoxelRenderPackage getRenderData() = 0;
        virtual Vector & getData() = 0;

    protected:
        Eigen::Transform<SCALAR_TYPE,3,Eigen::Affine> m_transform;

};

struct VertexProperties{
    bool isBound=false;
    int voxelCount = 0;
    bool isConstrained=false;
    float value=0.0;

};


template <typename LatticeType_>
class VoxelObject: public VoxelObjectBase {
    public:
        EIGEN_SPARSE_MATRIX_TYPEDEFS
            EIGEN_DENSE_MATRIX_TYPEDEFS
            typedef LatticeType_ LatticeType;
        virtual SparseMatrix getStiffnessMatrix(Vector & rhs) = 0;
        inline size_t NI(){return m_voxel_lattice.NI();}
        inline size_t NJ(){return m_voxel_lattice.NJ();}
        inline size_t NK(){return m_voxel_lattice.NK();}
        inline size_t N(){return m_voxel_lattice.N();}
        inline size_t activeVoxels() const {return m_voxel_lattice.activeCount();}
        inline size_t numVoxels() const {return m_voxel_lattice.activeCount();}
        inline LatticeType & getVoxels() {return m_voxel_lattice;}
        inline int addVoxel(size_t i, size_t j, size_t k);
        inline int addVoxel(const Index & idx);
        inline void addVertex(size_t i, size_t j, size_t k);
        inline void addVertex(const Index & idx);
        inline void addVertices(size_t i, size_t j, size_t k);
        inline void addVertices(const Index & idx);
        inline void setConstraint(size_t i, size_t j, size_t k, float v);
        LatticeType& lattice(){return m_voxel_lattice;}
        const LatticeType& lattice() const {return m_voxel_lattice;}
        virtual void makeCompressed();
        virtual void clearConstraints();
        virtual VoxelRenderPackage getRenderData();
        Vector & getData() {return m_data;}

        VoxelObject(size_t i, size_t j, size_t k);
        LatticeType& getDOFs();
        size_t numDOFs() const;

    protected:
        LatticeType m_voxel_lattice;
        LatticeType m_vertex_lattice;
        std::vector<VertexProperties> m_vertex_properties;
        Vector m_data;
        //        Eigen::Transform<SCALAR_TYPE,3,Eigen::Affine> m_tranform;
};



//Prototype to be specialized by various lattice/voxel combos
template <typename LatticeType_, typename VoxelType_>
class InstancedVoxelObjectBase: public VoxelObject<LatticeType_> {
    public:
        EIGEN_SPARSE_MATRIX_TYPEDEFS
            EIGEN_DENSE_MATRIX_TYPEDEFS
        typedef VoxelObject<LatticeType_> ParentType;
        typedef LatticeType_ LatticeType;
        typedef VoxelType_ Voxel;
        SparseMatrix getStiffnessMatrix(Vector & rhs);
        InstancedVoxelObjectBase(size_t i, size_t j, size_t k): ParentType(i,j,k) {}
        using ParentType::makeCompressed;
        using ParentType::NI;
        using ParentType::NJ;
        using ParentType::NK;
        using ParentType::N;
        using ParentType::getDOFs;
        using ParentType::numDOFs;
        using ParentType::getVoxels;
        using ParentType::numVoxels;
    protected:
        using ParentType::m_voxel_lattice;
        using ParentType::m_vertex_lattice;
        using ParentType::m_vertex_properties;
    private:
        inline size_t ijk2idx(size_t i,size_t j, size_t k) const ;
        inline size_t ijk2idx(const Index & idx) const ;
        inline int ijk2sidx(const SIndex & idx) const ;
        void addValue(SparseMatrix & A,const Index & idx, const SIndex & offset);
        Voxel m_voxel;



};



/*
template <typename VoxelType_>
class InstancedVoxelObjectBase<DenseLattice,VoxelType_>: public VoxelObject<DenseLattice>  {
    public:
        typedef VoxelObject<DenseLattice> ParentType;
        typedef DenseLattice LatticeType;
        typedef VoxelType_ Voxel;
        EIGEN_SPARSE_MATRIX_TYPEDEFS

        InstancedVoxelObjectBase(size_t i, size_t j, size_t k): ParentType(i,j,k) {}
        using VoxelObjectgetStiffnessMatrix(Vector & rhs);
    protected:
        using ParentType::m_voxel_lattice;
        Voxel m_voxel;

};
*/



#include "src/voxelObject.hpp"
#endif
