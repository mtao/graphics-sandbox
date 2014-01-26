#include "glwidget.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
GLWidget::GLWidget() {}


void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);

    float viewingAngle = 40.0;
    float nearPlane = 0.1;
    float farPlane = 100.0;
    h = std::max(h,1);
    float aspectRatio = float(w) / float(h);
    glMatrixMode(GL_PROJECTION);
    glm::mat4 projection = glm::perspective(viewingAngle, aspectRatio, nearPlane, farPlane);
    glLoadMatrixf(glm::value_ptr(projection));


}

void GLWidget::initializeGL() {
    glewInit();
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL() {
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glLoadIdentity();
        glm::mat4 view = glm::lookAt(eye, target, up);
        glLoadMatrixf(glm::value_ptr(view));
        {
            glPushMatrix();
            glLoadIdentity();

            glBegin(GL_POINTS);
            glColor3f(1,1,1);
            glVertex3f(0,0,0);
            glVertex3f(0.1,0,0);
            glEnd();


            glPopMatrix();
        }

        glPopMatrix(); 
    }
}
