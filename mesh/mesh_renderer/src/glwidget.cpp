#include "glwidget.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QMouseEvent>
#include <QDebug>
#include <QResource>
#include <memory>
#include <utility>
#include <qmath.h>
#include <QFileInfo>
#include <QDir>

extern int qInitResources_shaders();
GLWidget::GLWidget(const QGLFormat & format, QWidget * parent): QGLWidget(format, parent), m_timer(new QTimer(this)) {

    connect( m_timer, SIGNAL( timeout() ), SLOT( update() ) );

    m_timer->start( 16 );
    //QResource::registerResource("../qrc_shaders.cxx");
    qInitResources_shaders();
    QDir d(":/shaders/");

}

void GLWidget::resizeGL(int w, int h) {
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, h );

    // Setup an orthogonal projection matrix
    float viewingAngle = 40.0;
    float nearPlane = 0.1;
    float farPlane = 10000.0;
    h = std::max( h, 1 );
    m_aspectRatio = float( w ) / float( h );
    mat_p = glm::perspective( viewingAngle, m_aspectRatio, nearPlane, farPlane );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(mat_p));
    //mat_mvp = mat_p * mat_mv;
}

void GLWidget::initializeGL() {
    glewInit();
    glm::vec3 eyePosition(0.0f,0.0f,1.0f);
    glm::vec3 targetPosition(0.0f,0.0f,0.0f);
    glm::vec3 upDirection(0.0f,1.0f,0.0f);
    mat_v = glm::lookAt(eyePosition,targetPosition,upDirection);



    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    //glLineWidth(2.0);
    GLfloat white[] = {0.3,0.3,0.3};
    GLfloat red[] = {1,0,0};
    glLightfv(GL_LIGHT0,GL_SPECULAR, white);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0,GL_AMBIENT, white);
    GLfloat ambient[] = {.19225,.19225,.19225};
    GLfloat diffuse[] = {.50754,.504754,.50754};
    GLfloat specular[] = {.508273,.508273,.508273};
    GLfloat shininess = 0.4;
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shininess);
}




void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    const int delta = m_time.elapsed() - m_lastTime;
    m_rotation += m_angularMomentum * (delta / 1000.0);
    m_lastTime += delta;

    // Setup the modelview matrix
    mat_m = glm::translate(glm::mat4(1.0f),glm::vec3(0.f,0.f,-m_distance));
    mat_m = glm::rotate(mat_m, (float)m_rotation.x(), glm::vec3(1.f,0.f,0.f));
    mat_m = glm::rotate(mat_m, (float)m_rotation.y(), glm::vec3(0.f,1.f,0.f));
    mat_m = glm::rotate(mat_m, (float)m_rotation.z(), glm::vec3(0.f,0.f,1.f));
    mat_mv = mat_v * mat_m;
    mat_mvp = mat_p * mat_mv;
    mat_normal_transform = glm::inverse(glm::transpose(mat_mv));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(mat_mv));

    m_glWrap.Draw<vcg::GLW::DMSmooth, vcg::GLW::CMNone, vcg::GLW::TMNone> ();
}


void GLWidget::receiveMesh(const Mesh::shared_ptr& ptr) {
    m_mesh = ptr;
    m_glWrap.m = ptr.get();
}


void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    default: QGLWidget::keyPressEvent(event); return;
    }
    //If I haven't used the key I'll have already returned, so I should accept the input
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->pos() - lastPos;
        lastPos=event->pos();
        const QVector3D angularImpulse = QVector3D(delta.y(), delta.x(), 0) * 0.1;

        m_rotation += angularImpulse;
        m_accumulatedMomentum += angularImpulse;

        event->accept();
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    m_mouseEventTime = m_time.elapsed();
    m_angularMomentum = m_accumulatedMomentum = QVector3D();
    event->accept();
    lastPos=event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

    const int delta = m_time.elapsed() - m_mouseEventTime;
    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
    event->accept();
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{

    m_distance *= qPow(1.2, -event->delta() / 120);
    event->accept();
    update();
}

void GLWidget::unloadMesh() {
    m_mesh.reset();
    m_glWrap.m = 0;
}



