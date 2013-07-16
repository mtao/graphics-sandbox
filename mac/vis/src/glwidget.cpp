#include "glwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

GLWidget::GLWidget(QWidget * parent): QGLWidget(parent) {
}
void GLWidget::keyPressEvent(QKeyEvent *event) {
    if(!event->isAccepted()) {
        QGLWidget::keyPressEvent(event);
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if(!event->isAccepted()) {
        QGLWidget::mousePressEvent(event);
    }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    if(!event->isAccepted()) {
        QGLWidget::wheelEvent(event);
    }
}

void GLWidget::clearRenderables() {
    auto it = m_grids.begin();
    while(it != m_grids.end()) {
        if(!it.gridPtr.lock()) {
            auto to_del = it++;
            m_grids.erase(to_del);
        } else {
            ++it;
        }
    }

}

void GLWidget::addGrid(GridType::ptr ptr, mtao::GridEnum type, const QString & name) {
    m_grids.push_back(ptr,name,type);
}


void GLWidget::paintGL() {
}


void GLWidget::resizeGL(int w, int h) {
}
