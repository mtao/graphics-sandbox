#include "glwidget.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTimer>
GLWidget::GLWidget(): m_timer(new QTimer(this)) {
    connect(m_timer, &QTimer::timeout, this, &QGLWidget::updateGL);
    m_timer->start(17);
    std::vector<Vec3f> verts;
    std::vector<float> mass;
    std::vector<Edge> edges;
    std::vector<SpringProperties> edge_props;
    size_t rows=50,cols=50;
    size_t numel = rows * cols;
    for(size_t i=0; i < rows; ++i) {
        for(size_t j=0; j < cols; ++j) {
            verts.push_back(Vec3f(i,j,0));
            size_t idx = verts.size() - 1;
            if(idx % rows == 0) {
                mass.push_back(0.0);
            } else {
                mass.push_back(10/(cols - j+1.0));
            }
            if(idx +1 < numel && idx%rows != rows-1) {
                edges.push_back(Edge{idx,idx+1});
                edge_props.push_back(SpringProperties{1.f,9.f});
            }
            if(idx +rows < numel) {
                edges.push_back(Edge{idx,idx+rows});
                edge_props.push_back(SpringProperties{1.f,2.f});
            }
        }
    }
    verts[numel - 1] = Vec3f(-1,-1,0);
    system = MassSpringSystem(verts,mass,edges,edge_props);
}


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
    glPointSize(3.0f);


}

void GLWidget::initializeGL() {
    glewInit();
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glm::mat4 view = glm::lookAt(eye, target, up);
        glm::mat4 model = glm::scale(glm::mat4(1.0f),glm::vec3(-0.05f));
        model = glm::translate(model,glm::vec3(-25.f,-25.f,0.0f));
        glLoadMatrixf(glm::value_ptr(view * model));


        auto&& verts = system.vertices();
        auto&& edges = system.edges();
        glBegin(GL_LINES);
        glColor3f(0,0.1,0.1);
        for(auto&& e: edges) {
            glVertex3fv(verts[e[0]].data());
            glVertex3fv(verts[e[1]].data());
        }
        glEnd();
        glBegin(GL_POINTS);
        glColor3f(1,1,1);
        for(auto&& v: verts) {
            glVertex3fv(v.data());

        }
        glEnd();


        if(0) {
        auto&& forces = system.computeForces();
        glBegin(GL_LINES);
        for(size_t i = 0; i < verts.size(); ++i) {
            auto&& start = verts[i];
            auto end = (start + forces[i]).eval();
            glColor3f(1,0,1);
            glVertex3fv(start.data());
            glColor3f(1,0,0);
            glVertex3fv(end.data());
        }
        glEnd();
        }


        glPopMatrix(); 
    }
    system.advect(0.09f);
}
