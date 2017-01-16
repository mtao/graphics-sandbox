#ifndef RENDERER_HPP
#define RENDERER_HPP
//#include <GL/gl3w.h>
#include <GL/glew.h>
#include <memory>
#include "shader_program.hpp"

class Renderer {
    public:
        Renderer();
        ~Renderer();
        virtual void _render(int w, int h) = 0;
        void render(int w, int h);


        void setProgram(const std::shared_ptr<ShaderProgram>& p);
        const auto& program() const { return mProgram; }
    private:
        std::shared_ptr<ShaderProgram> mProgram = nullptr;

};

#endif//RENDERER_HPP
