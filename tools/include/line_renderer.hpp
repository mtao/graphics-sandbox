#ifndef LINERENDERER_HPP
#define LINERENDERER_HPP
//#include <GL/gl3w.h>
#include <GL/glew.h>
#include <memory>
#include "renderer.hpp"

class LineRenderer: public Renderer {
    public:
        LineRenderer();
        ~LineRenderer();
        void _render(int w, int h) override;

        GLfloat& scale() { return mScale; }

    private:
        GLuint mVBO, mVAO;
        GLint mColorUniform;
        GLint mScaleUniform;
        GLfloat mScale = 1.0;
};

#endif//LINERENDERER_HPP
