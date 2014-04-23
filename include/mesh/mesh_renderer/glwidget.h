#ifndef GLWIDGET_H
#define GLWIDGET_H
//Core GLWidget functionality I will expect
#include <QGLWidget>
#include <array>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_3_Compatibility>
#include <QMatrix4x4>
#include "mesh/mesh_renderer/scene.h"
class GLWidget: public QGLWidget {
    Q_OBJECT
    public:
        enum RenderMode : char { FLAT,SMOOTH,WIREFRAME, TEXTURE, END };
        GLWidget(QWidget * parent = 0);
        QOpenGLShader * makeShader(QOpenGLShader::ShaderType, const QString& filename);
        void openMesh(const QString& file);
        void openMesh();
    protected:
        void initializeShaders();
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);
        void updateBBox();
        void updateView();
        void updateViewPlanes();
        std::array<QOpenGLShaderProgram,RenderMode::END-1> m_shaders;
        QOpenGLFunctions_4_3_Compatibility gl;
        RenderMode m_render_mode = RenderMode::WIREFRAME;
        QVector3D m_eyePos, m_focusPos, m_upDir;
        QMatrix4x4 m_viewMat,m_perspectiveMat,m_MVP;
        InternalSceneNode m_scene;
        float m_view_near = 0.001f, m_view_far = 100.0f;
        mtao::BBox3f m_bbox;


        
        



};

#endif
