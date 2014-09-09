#include <GL/glew.h>
#include "render/mesh/meshnode.h"
#include "mesh/vftrianglemesh.h"
#include "mesh/loaders/objloader.h"

namespace mtao {namespace rendering{

void MeshSceneNode::render() {
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM,&program);

    int vertexLoc = glGetAttribLocation(program,"vertexPosition");

    glEnableVertexAttribArray( vertexLoc );

    m_vertices.bind();
    glVertexAttribPointer( vertexLoc, 3, GL_FLOAT, GL_TRUE, 0, 0 );
    m_indices.bind();
    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT,(void*)0);
    glUseProgram(0);
    glBegin(GL_TRIANGLES);
    for(auto&& t: mesh.triangles()) {
        for(auto&& i: t) {
            auto v = mesh.vertices()[i];
            glVertex3f(v(0),v(1),v(2));
        }
    }
    glEnd();

}



auto MeshSceneNode::create(const std::string& filename) -> Ptr{
    typedef mtao::VFTriangleMesh<float> MeshType;
    mtao::MeshConstructor<MeshType> mc;
    mtao::OBJLoader ml(&mc);
    ml.open(filename);
    auto mesh = mc.construct();

    mtao::BBox3f bb;
    for(auto&& v: mesh.vertices()) {
        bb.extend(v);
    }
//    std::array<int,3> uv;
    float scaler = bb.sizes().maxCoeff();
    for(auto&& v: mesh.vertices()) {
        v/=scaler;
    }
    auto& vertices = mesh.vertices();
    bb.min() = bb.min() / scaler;
    bb.max() = bb.max() / scaler;
    QOpenGLBuffer vbo(QOpenGLBuffer::Type::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(),vertices.size()*sizeof(float));

    QOpenGLBuffer ibo(QOpenGLBuffer::Type::IndexBuffer);
    ibo.create();
    ibo.bind();
    std::vector<GLuint> indices;
    auto& triangles = mesh.triangles();
    indices.resize(3*triangles.size());
    size_t i=0;
    for(auto& t: triangles) {
        for(size_t j=0;j<3;++j){
            indices[3*i + j] = t[j];
        }
    }
    size_t count = indices.size();
    ibo.allocate(indices.data(),indices.size()*sizeof(GLuint));
    std::cout << "Loaded mesh from file: (" << filename << ")BBox: " << bb.min().transpose() << " -> " << bb.max().transpose() << std::endl;
    auto ptr = new MeshSceneNode(vbo,ibo,QOpenGLBuffer(),count,bb);
    ptr->mesh = mesh;
    return std::shared_ptr<MeshSceneNode>(ptr);

}

MeshSceneNode::MeshSceneNode(const QOpenGLBuffer& v, const QOpenGLBuffer indices, const QOpenGLBuffer& texture, size_t count, const mtao::BBox3f& bbox): m_vertices(v), m_indices(indices), m_texture(texture), m_count(count), m_bbox(bbox) {

}
}}
