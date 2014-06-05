#ifndef INTERNAL_SCENENODE_H
#define INTERNAL_SCENENODE_H

#include "scenenode.h"

namespace mtao{namespace rendering{
class InternalSceneNode: public SceneNode {
    public:
        virtual void render() ;
        virtual mtao::BBox3f bbox() const;
        virtual void renderBBox() const;
        void add(const SceneNode::Ptr& ptr) ;
    private:
    std::vector<SceneNode::Ptr> children;
};
}}

#endif
