#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <glm/glm.hpp>

class GLWidget: public QGLWidget {
    public:
        GLWidget();
    protected:
        void resizeGL(int,int);
        void initializeGL();
        void paintGL();
    private:
        glm::vec3 eye = glm::vec3(0.0f,0.0f,-5.0f);
        glm::vec3 target = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
};

#endif
