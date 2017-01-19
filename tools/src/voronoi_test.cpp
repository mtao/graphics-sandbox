// ImGui - standalone example application for Glfw + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl.h"
#include <stdio.h>
#include <iostream>
//#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.hpp"
#include <sstream>
#include <numeric>
#include <thread>
#include <mutex>
#include <vector>
#include "mesh_renderer.hpp"
#include <random>
#include <Eigen/Dense>
#include <voro++/voro++.hh>

void prepareImgui();
void updateLogic();


Eigen::MatrixXi indices;

Eigen::MatrixXf vertices;


static void error_callback(int error, const char* description);



ImVec4 clear_color = ImColor(114, 144, 154);
std::unique_ptr<MeshRenderer> renderer;


std::mutex metasMutex;
std::unique_ptr<char[]> imgname_string;

int current_item = 0;
int old_item = -1;
bool show_settings = true;



GLFWwindow* window;

int setup() {
   
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,4);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    int major_version, minor_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);
    std::cout << "OpenGL version: " <<major_version << "." << minor_version << std::endl;

    glEnable(GL_MULTISAMPLE);

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);


    //renderer = std::make_unique<Renderer>();

    /*
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<double> uniform_dist(-1,1);
    */

    vertices = Eigen::MatrixXf::Random(2,100);
    //vertices.row(2).array() = 0;

    indices.resize(2,100);
    for(int i = 0; i < 100; ++i) {
        indices.col(i) = Eigen::Vector2i(i,(i+1)%100);
    }
    int n_x=3;
    int n_y=3;
    int n_z=3;
    voro::container con(-1,1,-1,1,-1,1,n_x,n_y,n_z,false,false,false,0);
    std::cout << indices << std::endl;

    using Mesh = mtao::geometry::mesh::SimplexMesh<1>;


    auto mesh = std::make_shared<Mesh>(vertices, indices);
    renderer = std::make_unique<MeshRenderer>(mesh);

    return 0;
}


void updateFilelist() {
}




int main(int argc, char* argv[])
{

    if(setup() == 1) {
        return 1;
    }





    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        prepareImgui();



        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);



        renderer->render(display_w, display_h);



        ImGui::Render();
        glfwSwapBuffers(window);

        updateLogic();
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}


void updateLogic() {
    
}

void keyboard_inputs();

void prepareImgui() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Settings", &show_settings);

    float& scale = renderer->scale();
    ImGui::InputFloat("scale", &scale, 0.01, 10.0);
    //ImGui::Text("MouseWheel: (%g)", io.MouseWheel);
    scale += .1 * io.MouseWheel;
    scale = std::max<float>(scale,0);
    //ImGui::SliderFloat("scale", &renderer->scale(), 0.01, 10.0, "%.4f");
    //if(ImGui::Button("Reset Scale")) { renderer->scale() = 1; }

    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    ImGui::Text("MousePos: (%g, %g)", io.MousePos.x, io.MousePos.y);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    keyboard_inputs();
}




void keyboard_logic(int key) {
    if(key == GLFW_KEY_DOWN || key == GLFW_KEY_RIGHT || key == GLFW_KEY_PAGE_DOWN) {
        current_item++;
    }
    if(key == GLFW_KEY_UP || key == GLFW_KEY_LEFT || key == GLFW_KEY_PAGE_UP) {
        current_item--;
    }
}


#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))
void keyboard_inputs() {
    ImGuiIO& io = ImGui::GetIO();
    std::vector<int> keys_pressed;
    std::vector<int> mods_presssed;
    for(int i = 0; i < IM_ARRAYSIZE(io.KeysDown); ++i) {
        if(ImGui::IsKeyPressed(i)) {
            keyboard_logic(i);
        }
    }
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}
