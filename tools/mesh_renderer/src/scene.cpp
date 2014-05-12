#include "scene.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include "core/linereader.hpp"
#include <QOpenGLFunctions_3_0>
#include <iostream>
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
    mtao::FileParser fp(filename.toStdString());
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    std::array<float,3> t;
    std::array<std::string,3> s;
    std::array<GLuint,3> i;
    mtao::BBox3f bb;
//    std::array<int,3> uv;
    for(auto&& line: fp) {
        char v;
        std::stringstream ss(line);
        ss >> v;
        switch(v) {
            case 'v':
                ss >> t[0] >> t[1] >> t[2];
                for(auto&& v: t) {
                    vertices.push_back(v);
                }
                bb.extend(Eigen::Map<mtao::Vec3f>(t.begin()));
                break;
            case 'f':
                ss >> s[0] >> s[1] >> s[2];
                for(size_t m=0; m < 3; ++m) {
                    auto&& str = s[m];
                    auto it = str.begin();
                    for(; it != str.end(); ++it) {
                        if(*it == '/') {
                            break;
                        }
                    }
                    std::stringstream ss2(str.substr(0,std::distance(str.begin(),it)));
                    ss2 >> i[m];

                }
                for(auto&& v: i) {
                    indices.push_back(v-1);
                }
                break;
            default:
                break;
        }
    }
    float scaler = bb.sizes().maxCoeff();
    for(auto&& v: vertices) {
        v/=scaler;
    }
    bb.min() = bb.min() / scaler;
    bb.max() = bb.max() / scaler;
    QOpenGLBuffer vbo(QOpenGLBuffer::Type::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(),vertices.size()*sizeof(float));

    QOpenGLBuffer ibo(QOpenGLBuffer::Type::IndexBuffer);
    ibo.create();
    ibo.bind();
    size_t count = indices.size();
    ibo.allocate(indices.data(),indices.size()*sizeof(GLuint));
    std::cout << "Loaded mesh from file: (" << filename.toStdString() << ")BBox: " << bb.min().transpose() << " -> " << bb.max().transpose() << std::endl;
    return std::shared_ptr<MeshSceneNode>(new MeshSceneNode(vbo,ibo,QOpenGLBuffer(),count,bb));

}

MeshSceneNode::MeshSceneNode(const QOpenGLBuffer& v, const QOpenGLBuffer indices, const QOpenGLBuffer& texture, size_t count, const mtao::BBox3f& bbox): m_vertices(v), m_indices(indices), m_texture(texture), m_count(count), m_bbox(bbox) {
}
