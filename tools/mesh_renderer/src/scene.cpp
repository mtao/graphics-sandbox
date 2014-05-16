#include "scene.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include "linereader.hpp"
#include <QOpenGLFunctions_3_0>
#include <iostream>
#include "mesh/vftrianglemesh.h"
#include "mesh/loaders/objloader.h"
void InternalSceneNode::render(QOpenGLShaderProgram* program, QOpenGLFunctions_3_0& gl) {
    for(auto&& ptr: children) {
        ptr->render(program,gl);
    }

}
mtao::BBox3f InternalSceneNode::bbox() const {
    mtao::BBox3f bb;
    for(auto&& ptr: children) {
        bb.extend(ptr->bbox());
    }
    return bb;

}
void InternalSceneNode::add(const SceneNode::Ptr& ptr) {
    children.push_back(ptr);
}


void MeshSceneNode::render(QOpenGLShaderProgram* program, QOpenGLFunctions_3_0&gl) {
    program->bind();
    program->enableAttributeArray( "vertexPosition" );
    m_vertices.bind();
    program->setAttributeBuffer( "vertexPosition", GL_FLOAT, 0, 3 );
    m_indices.bind();
    gl.glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT,(void*)0);

}



auto MeshSceneNode::create(const QString& filename) -> Ptr{
    typedef mtao::VFTriangleMesh<float> MeshType;
    mtao::MeshConstructor<MeshType> mc;
    mtao::OBJLoader ml(&mc);
    ml.open(filename.toStdString());
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
    std::cout << "Loaded mesh from file: (" << filename.toStdString() << ")BBox: " << bb.min().transpose() << " -> " << bb.max().transpose() << std::endl;
    return std::shared_ptr<MeshSceneNode>(new MeshSceneNode(vbo,ibo,QOpenGLBuffer(),count,bb));

}

MeshSceneNode::MeshSceneNode(const QOpenGLBuffer& v, const QOpenGLBuffer indices, const QOpenGLBuffer& texture, size_t count, const mtao::BBox3f& bbox): m_vertices(v), m_indices(indices), m_texture(texture), m_count(count), m_bbox(bbox) {
}
