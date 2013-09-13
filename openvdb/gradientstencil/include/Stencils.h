#ifndef MTAO_OPENVDB_VOXEL_STENCIL_HAS_BEEN_INCLUDED
#define MTAO_OPENVDB_VOXEL_STENCIL_HAS_BEEN_INCLUDED

#include <openvdb/math/Stencils.h> // General VDB Stencils, including my base classes

namespace openvdb {
OPENVDB_USE_VERSION_NAMESPACE
namespace OPENVDB_VERSION_NAME {
namespace math {




//////////////////////////////////////////////////////////////////////


/// This is a simple 7-point nearest neighbor stencil that supports
/// gradient by second-order central differencing, first-order upwinding,
/// Laplacian, closest-point transform and zero-crossing test.
///
/// @note For optimal random access performance this class
/// includes its own grid accessor.
template<typename GridType>
class InteriorGradStencil: public BaseStencil<GridType, InteriorGradStencil<GridType> >
{
public:
    typedef BaseStencil<GridType, InteriorGradStencil<GridType> > BaseType;
    typedef typename BaseType::BufferType                 BufferType;
    typedef typename GridType::ValueType                  ValueType;
    typedef math::Vec3<ValueType>                         Vec3Type;

    static const int SIZE = 9;

    InteriorGradStencil(const GridType& grid):
        BaseType(grid, SIZE),
        mInvDx(ValueType(1.0 / grid.voxelSize()[0])),
        mInvDx2(ValueType(1.0 * mInvDx * mInvDx))
    {
    }

    InteriorGradStencil(const GridType& grid, Real dx):
        BaseType(grid, SIZE),
        mInvDx(ValueType(1.0 / dx)),
        mInvDx2(ValueType(1.0 * mInvDx * mInvDx))
    {
    }


    /// @brief Return the gradient computed at the previously buffered
    /// location by second order central differencing or first order
    /// forward/backward differencing depending on the family of active 
    /// voxels
    ///
    /// @note This method should not be called until the stencil
    /// buffer has been populated via a call to moveTo(ijk).
    inline Vec3Type gradient() const
    {
        return Vec3Type(_dx(),
                        _dy(),
                        _dz());
    }


    //Left is the higher value
    inline ValueType _d(int l, int r) const {
        const bool leftA = mActive[l];
        const bool rightA = mActive[r];
        const int c = SevenPt< 0, 0, 0>::idx;
        std::cout << "(" << mStencil[l] << ", " << mStencil[c] << ", " << mStencil[r] << ")";
        if(leftA) {
            if(rightA) {
                std::cout << "[c]";
                return .5*mInvDx*(mStencil[l] - mStencil[r]);
            } else {
                std::cout << "[l]";
                return mInvDx*(mStencil[l] - mStencil[c]);
            }
        } else {
            if(rightA) {
                std::cout << "[r]";
                return mInvDx*(mStencil[c] - mStencil[r]);
            } else {
                return ValueType();
            }
        }
    }

    inline ValueType _dx() const {
        std::cout << "dx: ";
        return _d(SevenPt< 1, 0, 0>::idx, SevenPt< -1, 0, 0>::idx);
    }
    inline ValueType _dy() const {
        std::cout << "dy: ";
        return _d(SevenPt< 0, 1, 0>::idx, SevenPt<  0,-1, 0>::idx);
    }
    inline ValueType _dz() const {
        std::cout << "dz: ";
        return _d(SevenPt< 0, 0, 1>::idx, SevenPt<  0, 0,-1>::idx);
    }


private:
    
    inline void init(const Coord& ijk)
    {
        mStencil[SevenPt< 0, 0, 0>::idx] = mCache.getValue(ijk.offsetBy( 0,  0,  0));

        mStencil[SevenPt<-1, 0, 0>::idx] = mCache.getValue(ijk.offsetBy(-1,  0,  0));
        mStencil[SevenPt< 1, 0, 0>::idx] = mCache.getValue(ijk.offsetBy( 1,  0,  0));

        mStencil[SevenPt< 0,-1, 0>::idx] = mCache.getValue(ijk.offsetBy( 0, -1,  0));
        mStencil[SevenPt< 0, 1, 0>::idx] = mCache.getValue(ijk.offsetBy( 0,  1,  0));

        mStencil[SevenPt< 0, 0,-1>::idx] = mCache.getValue(ijk.offsetBy( 0,  0, -1));
        mStencil[SevenPt< 0, 0, 1>::idx] = mCache.getValue(ijk.offsetBy( 0,  0,  1));

        mActive[SevenPt< 0, 0, 0>::idx] = mCache.isValueOn(ijk.offsetBy( 0,  0,  0));
                                      
        mActive[SevenPt<-1, 0, 0>::idx] = mCache.isValueOn(ijk.offsetBy(-1,  0,  0));
        mActive[SevenPt< 1, 0, 0>::idx] = mCache.isValueOn(ijk.offsetBy( 1,  0,  0));
                                      
        mActive[SevenPt< 0,-1, 0>::idx] = mCache.isValueOn(ijk.offsetBy( 0, -1,  0));
        mActive[SevenPt< 0, 1, 0>::idx] = mCache.isValueOn(ijk.offsetBy( 0,  1,  0));
                                      
        mActive[SevenPt< 0, 0,-1>::idx] = mCache.isValueOn(ijk.offsetBy( 0,  0, -1));
        mActive[SevenPt< 0, 0, 1>::idx] = mCache.isValueOn(ijk.offsetBy( 0,  0,  1));
    }
    
    template<typename, typename> friend class BaseStencil; // allow base class to call init()
    using BaseType::mCache;
    using BaseType::mStencil;
    const ValueType mInvDx, mInvDx2;
    bool mActive[7];
}; // class GradStencil




} // end math namespace
} // namespace OPENVDB_VERSION_NAME
} // end openvdb namespace

#endif // OPENVDB_MATH_STENCILS_HAS_BEEN_INCLUDED

// Copyright (c) 2012-2013 DreamWorks Animation LLC
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
