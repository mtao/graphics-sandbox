#include <GL/glew.h>
#include "render/scene.h"


namespace mtao { namespace rendering {
    Scene::Scene() {
        glewInit();
    }

    void Scene::render() {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        InternalSceneNode::render();
        glPopMatrix();
    }
}
}
