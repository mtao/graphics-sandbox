#ifndef SHADER_SCENENODE_H
#define SHADER_SCENENODE_H
#include "render/internalnode.h"
#include <QOpenGLShaderProgram>
#include <memory>

namespace mtao{rendering{
class ShaderProgramNode: public InternalSceneNode {
    public:
        virtual void render() = 0;
        ShaderProgramNode();
        ShaderProgramNode(std::unique_ptr<QOpenGLShaderProgram>&& p);
        ~ShaderProgramNode();
    private:
        std::unique_ptr<QOpenGLShaderProgram> program;

};
}}


#endif
