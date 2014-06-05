#include <GL/glew.h>
#include "render/scenenode.h"


namespace mtao{ namespace rendering{
    void SceneNode::renderBBox() const {
        auto&& bb = bbox();
        std::array<mtao::Vec3f,2> m = {{bb.min(),bb.max()}};
        mtao::Vec3f s;
        glBegin(GL_LINES);
        for(int dim = 0; dim < 3; ++dim) {
            int d0 = dim;
            int d1 = (dim+1)%3;
            int d2 = (dim+2)%3;
            for(int i=0; i < 2; ++i) {
                for(int j=0; j < 2; ++j) {
                    s(d0) = m[i](d0);
                    s(d1) = m[j](d1);
                    s(d2) = m[0](d2);
                    glVertex3f(s(0),s(1),s[2]);
                    s(d2) = m[1](d2);
                    glVertex3f(s(0),s(1),s[2]);
                }
            }
        }
        glEnd();
    }
    
}}

