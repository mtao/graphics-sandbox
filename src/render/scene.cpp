#include <GL/glew.h>
#include "render/scene.h"


namespace mtao { namespace rendering {
    Scene::Scene() {
        glewInit();
    }
    mtao::BBox3f Scene::bbox() const {
        return root->bbox();
    }

    void Scene::render() {
    }
}
}
