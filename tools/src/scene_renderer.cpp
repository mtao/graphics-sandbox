#include "scene_renderer.hpp"
#include <iostream>
#include <iomanip>


SceneRenderer::SceneRenderer() {
}

SceneRenderer::~SceneRenderer() {
}


void SceneRenderer::_render(int w, int h) {
    glClear( GL_COLOR_BUFFER_BIT );

    for(auto&& mp: mObjects) {
    }


}
