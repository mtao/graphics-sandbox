#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP
#include <GL/glew.h>
#include <memory>
#include <set>

class ShaderProgram {
    public:
        struct Guard;
        class Constructor;

        friend class Constructor;

        ~ShaderProgram();
        Guard guard() const;
        void use() const;


        GLuint handle() const { return mHandle; }

        GLint getAttribLocation(const GLchar* name) const;
        GLint getUniformLocation(const GLchar* name) const;

        GLint getAttribLocation(const std::string& name) const { return getAttribLocation(name.c_str()); }
        GLint getUniformLocation(const std::string& name) const { return getUniformLocation(name.c_str()); }


    private:
        GLuint mHandle;

        ShaderProgram(GLuint handle);
};

struct ShaderProgram::Guard {
    Guard(const ShaderProgram* program);
    ~Guard();

};


class ShaderProgram::Constructor {
    public:
        class Shader;
        struct tags {
            struct file {};
            struct source {};

        };
        std::shared_ptr<ShaderProgram> construct();
        template <typename... Args>
        void addShader(Args&&... args) {mShaders.emplace(std::forward<Args>(args)...,tags::source());}
        template <typename... Args>
        void addShaderFile(Args&&... args) {mShaders.emplace(std::forward<Args>(args)...,tags::file());}
    private:
        std::set<Shader> mShaders;

};


class ShaderProgram::Constructor::Shader{
    public:

        Shader(GLenum shaderType, const std::string& filename, tags::file);
        Shader(GLenum shaderType, const char* source, tags::source);

        GLuint handle() const { return mHandle; }


        bool operator<(const Shader& other) const {
            return handle() < other.handle();
        }
    private:
        void compile(GLenum shaderType, const char* source);
        GLuint mHandle;
};

#endif//SHADER_PROGRAM_HPP
