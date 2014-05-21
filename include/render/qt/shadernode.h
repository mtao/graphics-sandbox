#ifndef SHADER_SCENENODE_H
#define SHADER_SCENENODE_H
#include "render/internalnode.h"
#include <QOpenGLShaderProgram>
#include <memory>

namespace mtao{namespace rendering{
class ShaderProgramNode: public InternalSceneNode, public QOpenGLShaderProgram {
    public:
        virtual void render();

};
}}


#endif
