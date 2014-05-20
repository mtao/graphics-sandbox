#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <vector>
#include "types.h"
#include "scenenode.h"

namespace mtao { namespace rendering{
class Scene: public SceneNode {
    public:
        Scene();
        void render();
        mtao::BBox3f bbox() const;
    private:
        std::unique_ptr<SceneNode> root;
};
}}


#endif
