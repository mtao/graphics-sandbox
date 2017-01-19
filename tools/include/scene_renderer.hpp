#ifndef SCENE_RENDERER_HPP
#define SCENE_RENDERER_HPP
//#include <GL/gl3w.h>
#include <GL/glew.h>
#include <memory>
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <vector>

class SceneRenderer: public Renderer {
    public:
        using Object = std::pair<std::shared_ptr<Renderer>, glm::mat4>;
        SceneRenderer();
        ~SceneRenderer();
        void _render(int w, int h);


    private:
        std::vector<Object> mObjects;

};

#endif//SCENE_RENDERER_HPP
