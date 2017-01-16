#include "line_renderer.hpp"
#include "shader_program.hpp"
#include <iostream>


LineRenderer::LineRenderer() {
    // Shaders
    static const GLchar* linesVertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "uniform vec2 scale;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(scale*position, 0.0, 1.0);\n"
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



    GLfloat vertices[] = {
        -1.f, -1.0f,
        1.f, -1.0f,
        0.f,  1.0f,
    };

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    glLineWidth(5);

}

LineRenderer::~LineRenderer() {
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}


void LineRenderer::_render(int w, int h) {
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    auto g = program()->guard();

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindVertexArray(mVAO); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );



    // Draw the triangle !
    //glUniform3f(mColorUniform,1,1,1);
    //glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
    static GLfloat cols[7][3] = {
        {0,0,0},
        {1,0,1},
        {1,1,0},
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {0,0,0},
    };
    glUniform2f(mScaleUniform,mScale,mScale);
    glUniform3f(mColorUniform,1,1,1);
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
    float s2 = mScale;
    for(auto&& c: cols) {
        glUniform2f(mScaleUniform,s2,s2);
        glUniform3fv(mColorUniform,1,c);
        glDrawArrays(GL_LINE_LOOP, 0, 3); // 3 indices starting at 0 -> 1 triangle
        s2 *= .8;
    }

    glDisableVertexAttribArray(0);

}
