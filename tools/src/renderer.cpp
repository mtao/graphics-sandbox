#include "renderer.hpp"
#include <glm/vec2.hpp>


#include <vector>
#include <iostream>
#include <array>
#include <memory>


Renderer::~Renderer() {

}

Renderer::Renderer() {
}





void Renderer::setProgram(const std::shared_ptr<ShaderProgram>& p) {
    mProgram = p;
}




void Renderer::render(int w, int h) {

    if(mProgram) {
        auto pg = mProgram->guard();
        _render(w,h);

    }

}
