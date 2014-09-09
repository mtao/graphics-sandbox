#ifndef SCENENODE_H
#define SCENENODE_H
#include "types.h"
#include <memory>

namespace mtao{namespace rendering{
class SceneNode: public std::enable_shared_from_this<SceneNode> {
    public:
        typedef std::shared_ptr<SceneNode> Ptr;
        virtual void render() = 0;
        virtual mtao::BBox3f bbox() const = 0;
        virtual void renderBBox() const;
};
}}
#endif
