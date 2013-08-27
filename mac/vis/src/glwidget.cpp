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

void GLWidget::cleanRenderables() {
    auto it = m_grids.begin();
    while(it != m_grids.end()) {
        if(!it->gridPtr.lock()) {
            auto to_del = it++;
            m_grids.erase(to_del);
        } else {
            ++it;
        }
    }

}

void GLWidget::addGrid(GridType::ptr ptr, mtao::GridEnum type, const QString & name) {
    m_grids.push_back({ptr,name,type});
    m_grids.back().enabled=true;
}

void GLWidget::renderGrid(const GridRenderable &gr) {
    glBegin(GL_POINTS);
    if(auto&& ptr = gr.gridPtr.lock()) {
        auto&& g = *(ptr);
        g.loop([&](const Eigen::Vector2i & c, Scalar v) {
                glColor3f(v,0,-v);
                Eigen::Vector2f p = g.indexToWorld(c);
                glVertex2f(p(0),p(1));
                });
    }
    glEnd();


}
void GLWidget::initializeGL() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    qglClearColor(QColor(0, 0, 0));
}

//some magical specialization code can go here in the future
template <>
void GLWidget::renderGridByType<mtao::CGrid>(const GridRenderable& gr) {
    renderGrid(gr);
}
template <>
void GLWidget::renderGridByType<mtao::UGrid>(const GridRenderable& gr) {
    renderGrid(gr);
}
template <>
void GLWidget::renderGridByType<mtao::VGrid>(const GridRenderable& gr) {
    renderGrid(gr);
}
template <>
void GLWidget::renderGridByType<mtao::NGrid>(const GridRenderable& gr) {
    renderGrid(gr);
}

void GLWidget::renderGridTypeSelector(const GridRenderable& gr) {
    switch(gr.gridType) {
        case mtao::CGrid: renderGridByType<mtao::CGrid>(gr);break;
        case mtao::UGrid: renderGridByType<mtao::UGrid>(gr);break;
        case mtao::VGrid: renderGridByType<mtao::VGrid>(gr);break;
        case mtao::NGrid: renderGridByType<mtao::NGrid>(gr);break;
        default: break;

    }
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,1,1);
    glBegin(GL_POINTS);
    glVertex2f(0,0);
    glEnd();
    for(auto&& gr: m_grids) {
        renderGridTypeSelector(gr);
    }

}


void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
}
