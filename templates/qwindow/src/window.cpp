#include "window.h"
#include <QOpenGLFunctions>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include <algorithm>
#include <functional>
#include <QTimer>
#include <sstream>

Window::Window() : m_timer(new QTimer(this)) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);

    setMouseGrabEnabled(true);

    connect(m_timer,SIGNAL(timeout()),this,SLOT(update()));
    m_timer->start(16);

    initialize();


}

void Window::initialize() {
    m_t = 0;
    m_current_frame_recorded = false;
}

void Window::resizeGL(int w, int h) {
}

QString qPointToQStr(const QPointF& p) {
    return QObject::tr("QPointF(") + QString::number(p.x()) + QObject::tr(",") + QString::number(p.y()) + QObject::tr(")");
}



void Window::updateTransform() {
    m_transform.reset();
    m_transform.translate(width()/2.,height()/2.);

    m_transform.translate(-m_origin.x(),-m_origin.y());

    qreal radius = m_zoom * m_radius;
    m_transform.scale(radius,radius);
    m_transform.translate(-.5,-.5);
}


void Window::paintGL() {

    if(m_autostep) {
        step();
    }


    QString mpos;
    { 
        auto qp = m_transform.inverted().map(m_mouse_pos);
        int i,j;
        float di,dj;
        std::stringstream ss;
        ss << "(" << qp.x();
        ss << "," << qp.y()<<")";
        mpos = QString::fromStdString(ss.str());
    }


    QOpenGLFunctions * f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter p(this);


    p.setPen(Qt::blue);
    //p.translate(-midpoint/2);
    p.setPen(Qt::green);
    p.drawPoint(width()/2,height()/2);
    p.setPen(Qt::blue);

    QPen pen(Qt::blue);
    pen.setWidthF(0);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);
    updateTransform();

    p.setTransform(QTransform());
    p.setPen(Qt::red);
    QPointF bound(5,5);
    QPointF lb = m_mouse_pos - bound;
    QPointF ub = m_mouse_pos + bound;
    p.drawEllipse(QRectF(lb,ub));
    p.drawText(m_mouse_pos,mpos);


    if(m_recording && !m_current_frame_recorded) {
        m_current_frame_recorded = false;
        auto img = grabFramebuffer();
        static int framenum = 0;
        auto filename = QString::number(framenum++) + tr(".png");
        img.save(filename);
        m_current_frame_recorded = true;

    }
}











void Window::keyPressEvent(QKeyEvent* ev) {
        std::cout << ev->key() << std::endl;
    if(ev->modifiers() & Qt::ControlModifier) {
    switch(ev->key()) {
        default:
            break;
    }
    



    } else {
        switch(ev->key()) {
            case Qt::Key_Space:
                m_autostep ^= true;
                ev->accept();
                break;
            case Qt::Key_R:
                toggleRecording();
                break;
            default:
                break;

        }
    }
    QOpenGLWindow::keyPressEvent(ev);
}

void Window::mousePressEvent(QMouseEvent* ev) {
    if(ev->modifiers() == Qt::ShiftModifier) {
        m_mouseMoving = true;
    }
    else {
        //auto pt = m_transform.inverted().map(QPointF(ev->pos()));
        //m_custom_tracking_points.emplace_back(pt.x(),pt.y());
        //std::cout << m_custom_tracking_points.back().transpose()<< std::endl;
    }
    update();
}
void Window::mouseMoveEvent(QMouseEvent* ev) {
    if(m_mouseMoving) {
        auto newpos = ev->pos();
        m_origin -= (newpos - m_mouse_pos);
        m_mouse_pos = newpos;
    } else {
        m_mouse_pos = ev->pos();
    }
    update();
}
void Window::mouseReleaseEvent(QMouseEvent* ev) {
    m_mouseMoving = false;
}

void Window::wheelEvent(QWheelEvent* ev) {
    //this is absoultely stupid as it will always be a constant-ish function. oh well
    m_zoom *= std::exp(ev->angleDelta().y()/(120.0 * 10));
    update();

}

void Window::toggleRecording() {
    m_recording ^= true;
}
void Window::startRecording() {
    m_recording = true;
}
void Window::stopRecording() {
    m_recording = false;
}






void Window::step() {
    float& t = m_t;
    float& dt = m_dt;
    t+=dt;

    m_current_frame_recorded = false;
}


