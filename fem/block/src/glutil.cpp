#include "../include/glutil.h"
#include <iostream>
#include <algorithm>
#include <set>


void bufferSize(GLuint buffer, GLenum type=GL_ARRAY_BUFFER)
{
    glBindBuffer(type,buffer);
    GLsizei datasize;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &datasize);
    std::cout << "Buffer["<<buffer<<"] size: " << datasize << " "<< std::endl;
}
GLint GLenum2size(GLenum type)
{
    switch(type)
    {
    case GL_BYTE:
        return sizeof(GLbyte);
    case GL_UNSIGNED_BYTE:
        return sizeof(GLubyte);
    case GL_SHORT:
        return sizeof(GLshort);
    case GL_UNSIGNED_SHORT:
        return sizeof(GLushort);
    case GL_INT:
        return sizeof(GLint);
    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    case GL_HALF_FLOAT:
        return sizeof(GLhalf);
    case GL_FLOAT:
        return sizeof(GLfloat);
    case GL_DOUBLE:
        return sizeof(GLdouble);
    }
    return 0;
}
ShaderProgram::ShaderProgram(): programId(glCreateProgram())
{
}
ShaderProgram::ShaderProgram(GLuint programId): programId(programId)
{
}
void ShaderProgram::init()
{
    programId = glCreateProgram();
}

GLint ShaderProgram::getAttribLocation(const char * name)
{
    GLint attributeId = glGetAttribLocation(programId, name);
    if(attributeId == -1)
    {
        std::cout << "Could not bind attribute: " << name << " returning -1 anyway" << std::endl;
    }
    return attributeId;
}

void ShaderProgram::addAttribute(const char * name, std::shared_ptr<VertexBufferObject> vbo)
{
    GLint attributeId = getAttribLocation(name);
    attributes[attributeId] = vbo;
}

std::shared_ptr<VertexBufferObject> ShaderProgram::getAttribute(const char * name)
{
    GLint attributeId = getAttribLocation(name);
    //return attributes[attributeId].lock();
    return attributes[attributeId];

}
void ShaderProgram::bind(bool bindAttributes)
{
    glUseProgram(programId);
    if(bindAttributes)
    {
        std::for_each(attributes.begin(), attributes.end(), [](decltype(attributes)::value_type & pair)
        {

            //if(auto vbo = pair.second.lock())
            //        vbo->bind(pair.first);
            pair.second->bind(pair.first);
        });
        attributesBound = true;
    }
}

void ShaderProgram::release()
{
    if(attributesBound)
    {
        std::for_each(attributes.begin(), attributes.end(), [](decltype(attributes)::value_type & pair)
        {
            glDisableVertexAttribArray(pair.first);
        });
        attributesBound = false;
    }
    glUseProgram(0);
}

VertexBufferObject::VertexBufferObject(GLvoid *data, GLsizei size, GLenum usage,
                                       int tupleSize, GLenum type, GLboolean normalized, GLsizei stride, GLvoid * offset):
    tupleSize(tupleSize), size(size), type(type), normalized(normalized), stride(stride), offset(offset)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,GLenum2size(type)*size*tupleSize,data, usage);
}

void VertexBufferObject::setData(GLvoid *data, GLsizei size_, GLenum usage,
                                       int tupleSize_, GLenum type_, GLboolean normalized_, GLsizei stride_, GLvoid * offset_) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,GLenum2size(type)*size*tupleSize,data, usage);
    tupleSize=tupleSize_;
    size=size_;
    type=type_;
    normalized=normalized_;
    stride=stride_;
    offset=offset_;
}
void VertexBufferObject::bind(GLint attributeId)
{
    glEnableVertexAttribArray(attributeId);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if(type == GL_INT || type == GL_UNSIGNED_INT)
    {
        glVertexAttribIPointer( attributeId,
                                tupleSize,
                                type,
                                stride,
                                offset
                                );
    }
    else
    {
        glVertexAttribPointer( attributeId,
                               tupleSize,
                               type,
                               normalized,
                               stride,
                               offset
                               );
    }

}
#include <iostream>
VertexBufferObject::~VertexBufferObject()
{
    if(owner){
        glDeleteBuffers(1,&vbo);
    }
}

VertexIndexObject::VertexIndexObject(GLvoid *data, GLsizei size, GLenum usage,
                                     GLenum mode, GLenum type, GLvoid * offset):
    mode(mode), size(size), type(type), offset(offset)
{
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,GLenum2size(type)*size,data, usage);
}

void VertexIndexObject::setData(GLvoid *data_, GLsizei size_, GLenum usage_,
                                     GLenum mode_, GLenum type_, GLvoid * offset_)
{
    mode=(mode_);
    size=(size_);
    type=(type_);
    offset=(offset_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,GLenum2size(type)*size,data_, usage_);
}
void VertexIndexObject::render()
{
    //bufferSize(ibo, GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(mode, size, type, offset);


}

VertexIndexObject::~VertexIndexObject()
{
    if(owner) {
        glDeleteBuffers(1,&ibo);
    }
}
