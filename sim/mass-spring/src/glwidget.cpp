#include "glwidget.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTimer>
#include <QMouseEvent>
GLWidget::GLWidget(): m_timer(new QTimer(this)) {
    connect(m_timer, &QTimer::timeout, this, &QGLWidget::updateGL);
    m_timer->start(17);
    setMouseTracking(true);



    std::vector<Vec3f> verts;
    std::vector<float> mass;
    std::vector<bool> rigid;
    std::vector<Edge> edges;
    std::vector<SpringProperties> edge_props;
    size_t rows=50,cols=50;
    size_t numel = rows * cols;
    size_t idx = 0;
    for(size_t i=0; i < rows; ++i) {
        for(size_t j=0; j < cols; ++j) {
            verts.push_back(Vec3f(i,j,0));
//                mass.push_back(10/(cols - j+1.0));
                mass.push_back(1);
            if(idx % rows == 0) {
                rigid.push_back(true);
            } else {
                rigid.push_back(false);
            }
            if(idx +1 < numel && idx%rows != rows-1) {
                edges.push_back(Edge{idx,idx+1});
                edge_props.push_back(SpringProperties{1.f,9.f});
            }
            if(idx +rows < numel) {
                edges.push_back(Edge{idx,idx+rows});
                edge_props.push_back(SpringProperties{1.f,9.f});
            }
            if(idx%2==0) {
            if(idx +1+rows < numel && idx%rows != rows-1) {
                edges.push_back(Edge{idx,idx+1+rows});
                edge_props.push_back(SpringProperties{std::sqrt(2.0f),3.f});
            }
            if(idx -1+rows < numel && idx%rows != 0) {
                edges.push_back(Edge{idx,idx-1+rows});
                edge_props.push_back(SpringProperties{std::sqrt(2.0f),5.f});
            }
            }
            idx++;
        }
    }
    system = MassSpringSystem(verts,mass,edges,edge_props,rigid);
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
    glLineWidth(3.0);


}

void GLWidget::initializeGL() {
    glewInit();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 view = glm::lookAt(eye, target, up);
    glm::mat4 model = glm::scale(glm::mat4(1.0f),glm::vec3(-0.05f));
    model = glm::translate(model,glm::vec3(-25.f,-25.f,0.0f));
    glLoadMatrixf(glm::value_ptr(view * model));


    auto&& verts = system.vertices();
    auto&& edges = system.edges();
    glBegin(GL_LINES);
    glColor3f(0,0.3,0.3);
    for(auto&& e: edges) {
        size_t dist = std::distance(&edges[0], &e);
        if(system.spring_properties()[dist].stiffness == 0) continue;
        glVertex3fv(verts[e[0]].data());
        glVertex3fv(verts[e[1]].data());
    }
    glEnd();
    glBegin(GL_POINTS);
    /*
    glColor3f(1,1,1);
    for(auto&& v: verts) {
        glVertex3fv(v.data());

    }
    */
    glColor3f(1,0,0);
    glVertex3fv(mousePos.data());
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(0.1,0,0,0.6);
    size_t rows=50,cols=50;
    size_t idx = 0;
    for(size_t i=0; i < rows; ++i) {
        for(size_t j=0; j < cols-1 && idx+rows+1 < verts.size(); ++j) {
    if(idx%rows != rows-1) {
        glVertex3fv(verts[idx].data());
        glVertex3fv(verts[idx+rows].data());
        glVertex3fv(verts[idx+rows+1].data());
        glVertex3fv(verts[idx+1].data());
    }

            idx++;
        }
    }
    glEnd();
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_POINTS);
    glColor3f(0,0,1);
    for(auto&& si: selectedVertices) {
        glVertex3fv(verts[si].data());
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);

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


    system.advect(0.09f);
}
auto GLWidget::getMousePos(QMouseEvent* event) -> Vec3f{
    return getMousePos(event->x(),event->y());
}
auto GLWidget::getMousePos(int x, int y) -> Vec3f{
    glm::ivec4 viewport;
    glm::mat4 modelview, projection;
    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelview));
    glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
    glGetIntegerv( GL_VIEWPORT, glm::value_ptr(viewport) );
    glm::vec3 win;
    win.x = x;
    win.y = float(viewport.w) - float(y);
    glReadPixels(win.x, int(win.y), 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);
    glm::vec3 pos = glm::unProject(win,modelview,projection, viewport);
    return Vec3f(pos.x,pos.y,pos.z);


}
#include <iostream>
void GLWidget::mousePressEvent(QMouseEvent * event) {
    auto&& pos = getMousePos(event);
    mousePos = pos;
    startPos = mousePos;
    auto&& verts = system.vertices();
    for(size_t i = 0; i < verts.size(); ++i) {
        auto&& v = verts[i];
        //std::cout << v.transpose() << " - " << pos.transpose() << " " << (v - pos).topRows<2>().norm() << std::endl;
        if((v - pos).topRows<2>().norm() < 2.0) {
            selectedVertices.insert(i);
            system.setRigid(i);
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent * event) {
    auto&& pos = getMousePos(event);
    mousePos = pos;
    if(!selectedVertices.empty()) {
        auto change = mousePos - startPos;
        if(change.topRows<2>().norm() > 100) return;
        auto&& verts = system.vertices();
        for(auto&& i: selectedVertices) {
            verts[i].topRows<2>() += change.topRows<2>();
        }
        startPos = mousePos;


    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * event) {
    if(!selectedVertices.empty()) {
        for(auto&& i: selectedVertices) {
            if(i%50 != 0)
            system.unsetRigid(i);
        }
        selectedVertices.clear();

    }
}
