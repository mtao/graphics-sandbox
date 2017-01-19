#include "mesh_renderer.hpp"
#include "shader_program.hpp"
#include <iostream>
#include <iomanip>


MeshRenderer::MeshRenderer(const std::shared_ptr<MeshWrapperBase>& m): mMesh(m) {
    // Shaders
    static const GLchar* linesVertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "uniform vec2 scale;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(scale*position,0, 1.0);\n"
        //"   gl_Position = vec4(position, 0.0, 1.0);\n"
        "}\n\0";
    static const GLchar* linesFragmentShaderSource = "#version 330 core\n"
        "out vec4 out_color;\n"
        "uniform vec3 color;\n"
        "void main()\n"
        "{\n"
        "out_color = vec4(color, 1.0f);\n"
        "}\n\0";

    ShaderProgram::Constructor program_constructor;
    program_constructor.addShader(GL_VERTEX_SHADER,linesVertexShaderSource);
    program_constructor.addShader(GL_FRAGMENT_SHADER,linesFragmentShaderSource);
    setProgram(program_constructor.construct());
    //mPosAttribute = program()->getAttribLocation("position");
    mScaleUniform = program()->getUniformLocation("scale");
    mColorUniform = program()->getUniformLocation("color");



    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, m->vertexSize() * sizeof(float), m->vertexData(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->facetSize() * sizeof(int), m->facetData(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    glPointSize(5);
    glLineWidth(5);


}

MeshRenderer::~MeshRenderer() {
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
}


void MeshRenderer::_render(int w, int h) {
    glClear( GL_COLOR_BUFFER_BIT );

    auto g = program()->guard();

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindVertexArray(mVAO); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            mMesh->dimension(),                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

    glUniform2f(mScaleUniform,mScale,mScale);
    glUniform3f(mColorUniform,1,1,1);
    glDrawArrays(GL_POINTS, 0, mMesh->numVertices()); // 3 indices starting at 0 -> 1 triangle
    //glDrawArrays(GL_POINTS, 0, mMesh->vertexSize()); // 3 indices starting at 0 -> 1 triangle
    glUniform3f(mColorUniform,1,0,0);
    glDrawElements(mMesh->renderEnum(), mMesh->facetSize(), GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);

}
