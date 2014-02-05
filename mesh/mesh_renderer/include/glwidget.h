#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <GL/glew.h>
#include <QGLWidget>
#include <glm/glm.hpp>
#include <memory>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include "mesh.h"
#include <wrap/gl/trimesh.h>


class MeshRenderMainWindow;
class GLWidget: public QGLWidget
{
    Q_OBJECT
public:
    friend class MeshRenderMainWindow;
    GLWidget(const QGLFormat&  format, QWidget * parent=0);
    void getVels(const std::vector<Eigen::Vector3f> & vel) {
        velocitytuples = vel;
        do_vel = true;
    }
    std::vector<Eigen::Vector3f> velocitytuples;
    bool do_vel = false;
protected:
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent * wheelEvent);
    virtual void keyPressEvent(QKeyEvent *event);
private:
    vcg::GlTrimesh<Mesh> m_glWrap;
    Mesh::shared_ptr m_mesh;
    QTimer* m_timer = 0;
    bool m_doRender = false;
    glm::mat4 mat_mvp, mat_mv, mat_m, mat_v, mat_p, mat_normal_transform;
    float m_aspectRatio = 1;

    QTime m_time;
    int m_lastTime = 0;
    int m_mouseEventTime;
    float m_distance = 1.4f;
    QPointF lastPos;
    QVector3D m_rotation;
    QVector3D m_angularMomentum;
    QVector3D m_accumulatedMomentum;
    vcg::GLW::DrawMode m_drawMode = vcg::GLW::DMSmooth;
    bool m_drawBBox = true;
    signals:
    void doneInitializingGL();
public slots:
    void disableRendering(){m_doRender =false;}
    void enableRendering(){m_doRender = true;}
    void receiveMesh(const Mesh::shared_ptr& ptr);
    void unloadMesh();
    void toggleDrawMode();
    void toggleBBox();
    void drawSmooth();
    void drawWire();
    void drawPoints();
};
#endif
