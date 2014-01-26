#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <glm/glm.hpp>
#include "mass_spring_system.hpp"
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
    private:
        glm::vec3 eye = glm::vec3(0.0f,0.0f,-5.0f);
        glm::vec3 target = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
        QTimer* m_timer;
        MassSpringSystem system;

};

#endif
