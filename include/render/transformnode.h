#ifndef TRANSFORM_SCENENODE_H
#define TRANSFORM_SCENENODE_H

#include "internalnode.h"

namespace mtao{namespace rendering{
class TransformSceneNode: public InternalSceneNode {
    public:
        typedef Eigen::Transform<double,3,Eigen::AffineCompact> TransformType;
        virtual void render() ;
        void setTransform(const TransformType& t);
    private:
         TransformType m_transform;
};
}}

#endif
