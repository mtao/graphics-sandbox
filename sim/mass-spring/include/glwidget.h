#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <glm/glm.hpp>
#include "mass_spring_system.hpp"
#include <set>
class QTimer;

class GLWidget: public QGLWidget {
    public:
        GLWidget();
        typedef MassSpringSystem::Vec3f Vec3f;
        typedef MassSpringSystem::Edge Edge;
    protected:
        void resizeGL(int,int);
        void initializeGL();
        void paintGL();
        void mousePressEvent(QMouseEvent * event);
        void mouseMoveEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent * event);
    private:
        glm::vec3 eye = glm::vec3(0.0f,0.0f,-5.0f);
        glm::vec3 target = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
        QTimer* m_timer;
        MassSpringSystem system;
        std::set<size_t> selectedVertices;
        Vec3f getMousePos(QMouseEvent* event);
        Vec3f getMousePos(int x, int y);
        Vec3f mousePos = Vec3f();
        Vec3f startPos;

};

#endif
