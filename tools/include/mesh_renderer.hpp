#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP
//#include <GL/gl3w.h>
#include <GL/glew.h>
#include <memory>
#include "renderer.hpp"
#include <Eigen/Dense>

class MeshRenderer: public Renderer {
    public:
        MeshRenderer(const Eigen::MatrixXf& v, const Eigen::MatrixXi& i);
        ~MeshRenderer();
        void _render(int w, int h) override;

        GLfloat& scale() { return mScale; }

    private:
        Eigen::MatrixXf mVertices;
        Eigen::MatrixXi mIndices;
        GLuint mVBO, mVAO, mEBO;
        GLint mColorUniform;
        GLint mScaleUniform;
        GLfloat mScale = 1.0;
};

#endif//MESHRENDERER_HPP
