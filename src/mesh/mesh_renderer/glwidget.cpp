#include "mesh/mesh_renderer/glwidget.h"
#include "core/linereader.hpp"
#include <QResource>
#include <array>
#include <vector>
#include <iostream>

GLWidget::GLWidget(QWidget * parent ): QGLWidget(parent) {
}
void GLWidget::initializeGL() {
    gl.initializeOpenGLFunctions();
    initializeShaders();
    openMesh("bunny.obj");

    m_viewMat.setToIdentity();
    m_viewMat.lookAt(QVector3D(0,0,1), QVector3D(0,0,0), QVector3D(0,1,0));

    gl.glEnable(GL_ALPHA_TEST);
    gl.glEnable(GL_DEPTH_TEST);
    gl.glEnable(GL_CULL_FACE);
    gl.glEnable(GL_MULTISAMPLE);
}

void GLWidget::initializeShaders() {

    QResource::registerResource("mesh_renderer.rcc");//TODO: doublecheck this


    QOpenGLShader * basic_vertex = makeShader(QOpenGLShader::Vertex, ":/basic.v.glsl");
// //   QOpenGLShader * flat_fragment = makeShader(QOpenGLShader::Fragment, ":/flat.f.glsl");
//    QOpenGLShader * smooth_fragment = makeShader(QOpenGLShader::Fragment, ":/smooth.f.glsl");
    std::cout << "Done making shaders" << std::endl;

    /*
    QOpenGLShader * texture_vertex = makeShader(QOpenGLShader::Vertex, ":/texture.v.glsl");
    QOpenGLShader * texture_fragment = makeShader(QOpenGLShader::Fragment, ":/texture.f.glsl");
    
    */

//    m_shaders[RenderMode::FLAT].addShader(basic_vertex);
//    m_shaders[RenderMode::FLAT].addShader(flat_fragment);

    m_shaders[RenderMode::SMOOTH].addShader(basic_vertex);
//    m_shaders[RenderMode::SMOOTH].addShader(smooth_fragment);
    m_shaders[RenderMode::SMOOTH].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/smooth.f.glsl");
    std::cout << "Done adding shaders" << std::endl;

    m_shaders[RenderMode::WIREFRAME].addShader(basic_vertex);
    m_shaders[RenderMode::WIREFRAME].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/wireframe.f.glsl");
    m_shaders[RenderMode::WIREFRAME].addShaderFromSourceFile(QOpenGLShader::Geometry, ":/wireframe.g.glsl");

    /*
    m_shaders[RenderMode::TEXTURE].addShader(texture_vertex);
    m_shaders[RenderMode::TEXTURE].addShader(texture_fragment);
    */

    for(auto&& shader: m_shaders) {
        shader.link();
    }
}

QOpenGLShader * GLWidget::makeShader(QOpenGLShader::ShaderType, const QString& filename) {
    QOpenGLShader * shader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    if(shader->compileSourceFile(filename)) {
        return shader;
    } else {
        qDebug() << "Could not compile shader from file: " << filename;
        qDebug() << shader->log();
        delete shader;
        return nullptr;
    }

}

void GLWidget::openMesh(const QString& filename) {
    mtao::FileParser fp(filename.toStdString());
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    std::array<float,3> t;
    std::array<std::string,3> s;
    std::array<GLuint,3> i;
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
    m_vertices = QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    m_vertices.create();
    m_vertices.bind();
    m_vertices.allocate(vertices.data(),vertices.size()*sizeof(float));
    for(auto&& shader: m_shaders) {
        if(!shader.isLinked()) continue;
        shader.enableAttributeArray( "vertexPosition" );
        shader.setAttributeBuffer( "vertexPosition", GL_FLOAT, 0, 3 );
    }

    m_indices = QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    m_indices.create();
    m_indices.bind();
    m_count = indices.size();
    m_indices.allocate(indices.data(),indices.size()*sizeof(GLuint));

}

void GLWidget::resizeGL(int w, int h) {
    gl.glViewport(0,0,w,h);
    float aspectRatio = float(w)/float(h);
    m_perspectiveMat.setToIdentity();
    m_perspectiveMat.perspective(40.0,aspectRatio,0.01,100.0);
    m_MVP = m_perspectiveMat * m_viewMat;
    for(auto&& shader: m_shaders) {
        if(!shader.isLinked()) continue;
        shader.bind();
        shader.setUniformValue("WIN_SCALE", GLfloat(width()), GLfloat(height()));
        shader.setUniformValue("MVP", m_MVP);
        shader.setUniformValue("color", .2f,.5f,1.f,1.0f);
        shader.release();
    }
}

void GLWidget::paintGL() {

    gl.glClearColor(0,0,0,0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    m_shaders[m_render_mode].bind();
    m_shaders[m_render_mode].enableAttributeArray( "vertexPosition" );
    m_vertices.bind();
    m_shaders[m_render_mode].setAttributeBuffer( "vertexPosition", GL_FLOAT, 0, 3 );
    m_indices.bind();
    gl.glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT,(void*)0);
    m_shaders[m_render_mode].release();
}

