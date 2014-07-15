#include "sampling/poisson_bridson.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

std::vector<mtao::Vec2f> vec; 

void render() {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mtao::BridsonSampler<float,2> bs;
    vec = bs.sample();

    glColor3f(1,1,1);
    glBegin(GL_POINTS);
    for(auto&& v: vec) {
        glVertex2f(v(0)-.5,v(1)-.5);
    }
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char * argv[]) {
    for(auto&& v: vec) {
        std::cout << v.transpose() << std::endl;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Hello World");
    glutDisplayFunc(&render);
    glutMainLoop();
    //glutIdleFunc(&update_fade_factor);

}
