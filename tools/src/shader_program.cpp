#include "shader_program.hpp"
#include <iostream>
#include <vector>
#include <fstream>

ShaderProgram::ShaderProgram(GLuint handle): mHandle(handle) {
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(handle());
}


void ShaderProgram::use() const {
    glUseProgram(handle());
}


auto ShaderProgram::guard() const -> Guard
{
    return Guard(this);
}




GLint ShaderProgram::getAttribLocation(const GLchar* name) const {
    GLint r = glGetAttribLocation(handle(), name);
    if(r == -1) {
        std::cerr << "Couldnt find attribute [" << name << "]" << std::endl;
    }
    return r;
}
GLint ShaderProgram::getUniformLocation(const GLchar* name) const {
    GLint r = glGetUniformLocation(handle(), name);
    if(r == -1) {
        std::cerr << "Couldnt find uniform [" << name << "]" << std::endl;
    }
    return r;
}




std::shared_ptr<ShaderProgram> ShaderProgram::Constructor::construct() {

    GLuint program;
    GLint link_ok = GL_FALSE;

    program = glCreateProgram();
    for(auto&& shader: mShaders) {
        glAttachShader(program,shader.handle());
    }
    glLinkProgram(program);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        std::cerr << "Error in glLinkProgram" << std::endl;
    }
    return std::shared_ptr<ShaderProgram>(new ShaderProgram(program));
}

ShaderProgram::Constructor::Shader::Shader(GLenum shaderType, const char* source, tags::source) {
    compile(shaderType,source);
}
void ShaderProgram::Constructor::Shader::compile(GLenum shaderType, const char* source) {

    GLint compile_ok = GL_FALSE;
    GLenum shader = glCreateShader(shaderType);

    glShaderSource(shader,1,&source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);

    if(!compile_ok) {

        auto shaderT = [](GLenum t) -> std::string {
            switch(t) {
                case GL_VERTEX_SHADER: return "Vertex";
                case GL_FRAGMENT_SHADER: return "Fragment";
                case GL_GEOMETRY_SHADER: return "Geometry";
                default:return "UNKONWN";
            }
        };

        std::cout << "Compilation of " <<shaderT(shaderType) << "shader failed" << std::endl;

        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::string error(errorLog.data());
        std::cout << error << std::endl;

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
    }
    mHandle = shader;

}


ShaderProgram::Constructor::Shader::Shader(GLenum shaderType, const std::string& filename, tags::file) {
    std::ifstream file(filename);
    std::string str((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

    compile(shaderType,str.c_str());


}



ShaderProgram::Guard::Guard(const ShaderProgram* program) {
    program->use();
}
ShaderProgram::Guard::~Guard() {
    glUseProgram(0);
}
