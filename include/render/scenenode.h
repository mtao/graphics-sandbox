#ifndef SCENENODE_H
#define SCENENODE_H
#include "types.h"
#include <memory>

namespace mtao{namespace rendering{
class SceneNode {
    public:
        typedef std::shared_ptr<SceneNode> Ptr;
        virtual void render() = 0;
        virtual mtao::BBox3f bbox() const = 0;
};
}}
#endif
