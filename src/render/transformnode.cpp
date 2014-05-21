#include <GL/glew.h>
#include "render/transformnode.h"


namespace mtao{namespace rendering{
    void TransformSceneNode::render() {
        glPushMatrix();
        glMultMatrixd(m_transform.data());
        InternalSceneNode::render();

        glPopMatrix();
    }
        void TransformSceneNode::setTransform(const TransformType& t) {
            m_transform = t;
        }
}}
