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
        std::array<QOpenGLShaderProgram,RenderMode::END-1> m_shaders;
        size_t m_count;
        QOpenGLBuffer m_vertices,m_indices,m_texture;
        QOpenGLFunctions_4_3_Compatibility gl;
        RenderMode m_render_mode = RenderMode::WIREFRAME;
        QMatrix4x4 m_viewMat,m_perspectiveMat,m_MVP;


        
        



};

#endif
