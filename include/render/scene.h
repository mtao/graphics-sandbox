#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <vector>
#include "types.h"
#include "internalnode.h"

namespace mtao { namespace rendering{
class Scene: public InternalSceneNode {
    public:
        Scene();
        void render();
    private:
};
}}


#endif
