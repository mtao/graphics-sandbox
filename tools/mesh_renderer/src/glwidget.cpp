#include "glwidget.h"
#include <QResource>
#include <array>
#include <vector>
#include <iostream>

GLWidget::GLWidget(QWidget * parent ): QGLWidget(parent) {
}
void GLWidget::initializeGL() {
    gl.initializeOpenGLFunctions();
    initializeShaders();

    for(auto&& shader: m_shaders) {
        if(!shader.isLinked()) continue;
        shader.bind();
        shader.setUniformValue("color", .2f,.5f,1.f,1.0f);
        shader.setUniformValue("lightPosition", 10.f,10.f,10.f);
        shader.release();
    }


    m_eyePos = QVector3D(0,0,2);
    m_focusPos = QVector3D(0,0,0);
    m_upDir = QVector3D(0,1,0);
    updateView();

    gl.glEnable(GL_ALPHA_TEST);
    gl.glEnable(GL_DEPTH_TEST);
    gl.glEnable(GL_CULL_FACE);
    gl.glEnable(GL_MULTISAMPLE);
    openMesh("armadillo.obj");
    m_render_mode = RenderMode::FLAT;
    //openMesh("Test.obj");
}

bool GLWidget::useAdvancedPipeline() const {
    int major_version = this->format().majorVersion();
    int minor_version = this->format().minorVersion();
    return major_version > 3 && minor_version > 0;
}

QString GLWidget::getResourceLocation(const QString& filename) const {
    QString prefix(":/shaders/");
    if(useAdvancedPipeline()) {
        prefix += "410/";
    } else {
        prefix += "120/";
    }
    return prefix + filename;
}


void GLWidget::initializeShaders() {
    int major_version = this->format().majorVersion();
    int minor_version = this->format().minorVersion();
    std::cout << "OpenGL Version: " << major_version << "." << minor_version << std::endl;

    QString rcc_prefix("tools/mesh_renderer/");
    if(useAdvancedPipeline()) {
        rcc_prefix += "410";
    } else {
        rcc_prefix += "120";
    }
    QString rcc_filepath = rcc_prefix + "_shaders.rcc";

    //if(!QResource::registerResource(rcc_filepath)) {//TODO: doublecheck this
    //    std::cout << "Could not open resource!" << std::endl;
    //}


    QOpenGLShader * basic_vertex = makeShader(QOpenGLShader::Vertex, getResourceLocation("basic.v.glsl"));
 //   QOpenGLShader * flat_fragment = makeShader(QOpenGLShader::Fragment, ":/flat.f.glsl");
//    QOpenGLShader * phong_fragment = makeShader(QOpenGLShader::Fragment, ":/phong.f.glsl");
    std::cout << "Done making shaders" << std::endl;

    /*
    QOpenGLShader * texture_vertex = makeShader(QOpenGLShader::Vertex, "texture.v.glsl");
    QOpenGLShader * texture_fragment = makeShader(QOpenGLShader::Fragment, "texture.f.glsl");
    
    */

    m_shaders[RenderMode::FLAT].addShader(basic_vertex);
//    m_shaders[RenderMode::FLAT].addShader(phong_fragment);
    m_shaders[RenderMode::FLAT].addShaderFromSourceFile(QOpenGLShader::Fragment, getResourceLocation("phong.f.glsl"));
    if(useAdvancedPipeline()) {
        m_shaders[RenderMode::FLAT].addShaderFromSourceFile(QOpenGLShader::Geometry, getResourceLocation("flat.g.glsl"));
    }

    m_shaders[RenderMode::SMOOTH].addShader(basic_vertex);
    //m_shaders[RenderMode::SMOOTH].addShader(phong_fragment);
    //m_shaders[RenderMode::SMOOTH].addShaderFromSourceFile(QOpenGLShader::Fragment, ":/phong.f.glsl");
    std::cout << "Done adding shaders" << std::endl;

    m_shaders[RenderMode::WIREFRAME].addShader(basic_vertex);
    m_shaders[RenderMode::WIREFRAME].addShaderFromSourceFile(QOpenGLShader::Fragment, getResourceLocation("wireframe.f.glsl"));
    if(useAdvancedPipeline()) {
        m_shaders[RenderMode::WIREFRAME].addShaderFromSourceFile(QOpenGLShader::Geometry, getResourceLocation("wireframe.g.glsl"));
    }
    /*
    m_shaders[RenderMode::TEXTURE].addShader(texture_vertex);
    m_shaders[RenderMode::TEXTURE].addShader(texture_fragment);
    */

    for(auto&& shader: m_shaders) {
        shader.link();
    }
}

QOpenGLShader * GLWidget::makeShader(QOpenGLShader::ShaderType, const QString& filename) {
        qDebug() << "About to compile shader file: " << filename;
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
void GLWidget::updateView() {
    m_viewMat.setToIdentity();
    m_viewMat.lookAt(m_eyePos, m_focusPos, m_upDir);
    updateViewPlanes();
    for(auto&& shader: m_shaders) {
        if(!shader.isLinked()) continue;
        shader.bind();
        shader.setUniformValue("eyePosition", m_eyePos);
        shader.release();
    }

}
void GLWidget::updateViewPlanes() {
    std::array<mtao::Vec3f,2> bb;
    bb[0] = m_bbox.min();
    bb[1] = m_bbox.max();
    m_view_near = std::numeric_limits<float>::max();
    m_view_far = std::numeric_limits<float>::min();
    Eigen::Map<const Eigen::Matrix<float,4,4,Eigen::ColMajor> > map(m_viewMat.constData());
    for(int a=0; a < 2; ++a) {
        for(int b=0; b < 2; ++b) {
            for(int c=0; c < 2; ++c) {
                auto vec = (map*Eigen::Matrix<float,4,1>(bb[a].x(),bb[b].y(),bb[c].z(),1)).eval();
                float z = -vec.z() / vec.w();
                m_view_near = std::min(z,m_view_near);
                m_view_far = std::max(z,m_view_far);
            }
        }
    }
    m_view_near = std::max(0.00001f,m_view_near);
    m_view_far = std::min(1000.0f,m_view_far);
    std::cout << "New cut planes: [" << m_view_near << "," << m_view_far << "]" << std::endl;
}

void GLWidget::updateBBox() {
    m_bbox = m_scene.bbox();
    updateViewPlanes();
}

void GLWidget::openMesh(const QString& filename) {
    auto n = MeshSceneNode::create(filename);

    if(n) {
        m_scene.add(n);
    }
    updateBBox();
}

void GLWidget::resizeGL(int w, int h) {
    gl.glViewport(0,0,w,h);
    float aspectRatio = float(w)/float(h);
    m_perspectiveMat.setToIdentity();
    m_perspectiveMat.perspective(40.0,aspectRatio,m_view_near, m_view_far);
    m_MVP = m_perspectiveMat * m_viewMat;
    for(auto&& shader: m_shaders) {
        if(!shader.isLinked()) continue;
        shader.bind();
        shader.setUniformValue("WIN_SCALE", GLfloat(width()), GLfloat(height()));
        shader.setUniformValue("MVP", m_MVP);
        shader.setUniformValue("normalMatrix", m_MVP.normalMatrix());
        shader.release();
    }
}

void GLWidget::paintGL() {

    gl.glClearColor(0,0,0,0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    return;
    m_shaders[m_render_mode].bind();
    m_scene.render(&m_shaders[m_render_mode], gl);
    m_shaders[m_render_mode].release();
}

