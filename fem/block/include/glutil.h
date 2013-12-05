#ifndef GLUTIL_H
#define GLUTIL_H

#include <GL/glew.h>
#include <vector>
#include <map>
#include <memory>

/*
 Standard use:
initializeGL()
{
ShaderProgram p;
VertexBufferObject vbo1;
VertexBufferObject vbo2;
VertexBufferObject vbo3;
p.addAttribute("vertex",&vbo1);
p.addAttribute("normal",&vbo2);
p.addAttribute("color",&vbo3);
VertexIndexObject vio;
}
paintGL()
{
program.bind();
vio.render();
program.release();
}

  */


struct VertexBufferObject
{
    /*
      Capabilities:
      create vbo
      load vbo
      */
    VertexBufferObject(): owner(false) {}
    VertexBufferObject(GLvoid *data, GLsizei size, GLenum usage=GL_STATIC_DRAW,
                       int tupleSize=1, GLenum type=GL_FLOAT, GLboolean normalized=GL_FALSE,
                       GLsizei stride=0, GLvoid * offset=0);
    void setData(GLvoid *data, GLsizei size, GLenum usage=GL_STATIC_DRAW,
                       int tupleSize=1, GLenum type=GL_FLOAT, GLboolean normalized=GL_FALSE,
                       GLsizei stride=0, GLvoid * offset=0);
    void bind(GLint attributeId);
    ~VertexBufferObject();
    GLuint vbo;
    int tupleSize;//Values stored redundantly between constructor but doesn't matter
    int size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    GLvoid * offset;
    bool owner=true;
};
struct VertexIndexObject
{
    /*
      Capabilities:
      create ibo
      render using ibo
      */
    VertexIndexObject(): owner(false) {}
    VertexIndexObject(GLvoid *data, GLsizei size, GLenum usage,
                      GLenum mode=GL_TRIANGLES, GLenum type=GL_UNSIGNED_INT, GLvoid * offset=0);
    void setData(GLvoid *data, GLsizei size, GLenum usage,
                      GLenum mode=GL_TRIANGLES, GLenum type=GL_UNSIGNED_INT, GLvoid * offset=0);
    ~VertexIndexObject();
    void render();
    GLuint ibo;
    GLenum mode;//Once again values stored redundantly between constructor but doesn't matter
    GLsizei size;
    GLenum type;
    GLvoid * offset;
    bool owner=true;
};

struct ShaderProgram
{
    /*
      Capabilities:
      Create shader program
      bind shader program (and perhaps all of the attributes it has bound)
      release shader program (and perhaps all of the attributes it has bound)

      optionally:
      check for attributes
      check for redundant attributes
      */
    ShaderProgram();
    ShaderProgram(GLuint programId);
    void init();
    void addAttribute(const char * name, std::shared_ptr<VertexBufferObject> vbo);
    GLint getAttribLocation(const char * name);
    std::shared_ptr<VertexBufferObject> getAttribute(const char * name);
    void bind(bool bindAttributes=true);
    void release();
    GLuint programId;
    bool attributesBound=false;
    //std::map<GLint, std::weak_ptr<VertexBufferObject> > attributes;
    std::map<GLint, std::shared_ptr<VertexBufferObject> > attributes;
    //std::vector< VertexBufferObject> attributes;
    bool owner=true;
};


struct ShaderCollection
{
    /*
      Capabilities:
      Holds a shader program, its attributes, and an ibo
      */
    ShaderCollection() {}
    void setProgram(ShaderProgram & program_) {program=program_; program.owner=false;}
    void setVertexIndexObject(VertexIndexObject & vio_) {vio=vio_; vio.owner=false;}
    void addVertexBufferObject(VertexBufferObject & vbo) {vbos.push_back(vbo); vbos.back().owner=false;}
    void render(){vio.render();}
    ShaderProgram program;
    VertexIndexObject vio;
    std::vector<VertexBufferObject> vbos;

};




#include <Eigen/Dense>

namespace GLMatrix{
template <typename Scalar>
    Eigen::Transform<Scalar,3,Eigen::Affine> lookAt(
            const Eigen::Ref<const Eigen::Matrix<Scalar,3,1> > & eye,
            const Eigen::Ref<const Eigen::Matrix<Scalar,3,1> > & center,
            const Eigen::Ref<const Eigen::Matrix<Scalar,3,1> > & up
            ) {
        typedef Eigen::Matrix<Scalar,3,1> Vec3;

        Vec3 f = (center-eye).normalized();
        Vec3 u = up.normalized();
        Vec3 s = f.cross(u).normalized();
        u = s.cross(f);
        Eigen::Transform<Scalar,3,Eigen::Affine> result;
        result.block(0,0,3,1) = s;
        result.block(0,1,3,1) = u;
        result.block(0,2,3,1) = -f;
        result(3,0) = -s.dot(eye);
        result(3,1) = -u.dot(eye);
        result(3,2) =  f.dot(eye);
        return result;
    }

template <typename Scalar>
    Eigen::Transform<Scalar,3,Eigen::Projective> perspective(Scalar fovy, Scalar aspect, Scalar zNear, Scalar zFar) {
        Scalar range = std::tan(fovy / Scalar(2)) * zNear;
        Scalar left = -range * aspect;
        Scalar right = -range * aspect;
        Scalar bottom = -range;
        Scalar top = -range;
        Eigen::Transform<Scalar,3,Eigen::Projective> result;
        result(0,0) = (Scalar(2) * zNear) / (right-left);
        result(1,1) = (Scalar(2) * zNear)/(top-bottom);
        result(2,2) = - (zFar + zNear) / (zFar - zNear);
        result(2,3) = - Scalar(1);
        result(3,2) = - (Scalar(2) * zFar * zNear) / (zFar - zNear);
        return result;
    }

}



#endif // GLUTIL_H
