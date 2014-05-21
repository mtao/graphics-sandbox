#include "render/qt/shadernode.h"

namespace mtao{namespace rendering{
    void ShaderProgramNode::render() {
        bind();
        InternalSceneNode::render();
        release();

    }
}}
