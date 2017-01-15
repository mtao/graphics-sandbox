#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "glutil.h"
#include <QGLWidget>
#include <memory>
#include "package.h"
#include <Eigen/Geometry>
#include <glm/glm.hpp>
#include <QMouseEvent>

#include <QVector3D>
#include <QTime>
class GLWidget: public QGLWidget {
    Q_OBJECT
    public:
        GLWidget(QWidget * parent);
        GLWidget(const QGLFormat & format, QWidget * parent=0);

    protected:
        void initializeGL();
        void initializeTextures();
        void resizeGL(int w, int h);
        void paintGL();
    private:
        void init();
        //Alternate rendering to diff vbo
        Package m_data[2];
        bool m_vboState;
        //cur as in currently rendering
        inline Package & curPkg();
        inline Package & otherPkg();
        GLuint compileShader(GLenum shaderType, const QString & filename);

        QTimer * m_timer;
        QTime m_time;

        std::unique_ptr<ShaderProgram> m_pointProgram;


        GLuint m_dataTex;
        /*
        Eigen::Transform<float,3,Eigen::Projective> m_projectionMatrix;
        Eigen::Transform<float,3,Eigen::Affine> m_viewMatrix;
        Eigen::Vector3f rotation;
        */
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_modelMatrix;
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        int m_lastTime=0;
        int m_mouseEventTime;
        float m_distance=1.4f;
        QPointF lastPos;
        QPointF pos;
        QVector3D m_rotation;
        QVector3D m_angularMomentum;
        QVector3D m_accumulatedMomentum;





signals:
        void modifiableVBO(Package & pkg);

        public slots:
            void vboRequest();
        void swapVBO();
};

inline Package & GLWidget::
curPkg() {
    return m_data[m_vboState?1:0];
}

inline Package & GLWidget::
otherPkg() {
    return m_data[m_vboState?0:1];
}

#endif
