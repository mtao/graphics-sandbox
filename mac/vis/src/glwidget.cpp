#include "glwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

GLWidget::GLWidget(QWidget * parent, const GridFactoryType::ptr& ptr): QGLWidget(parent), m_factory(ptr) {
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
void GLWidget::addVelocity(MACGrid<Scalar,embed_dim,1,0>::ptr ptr
        , MACGrid<Scalar,embed_dim,1,1>::ptr ptr2, const QString& name) {
    m_grids.push_back(GridRenderable{ptr,ptr2,name});
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

void GLWidget::renderVelocity(const GridRenderable &gr) {
    GridFactoryType::ptr ptr;
    GridType::ptr uptr,vptr;
    if(
            (ptr = m_factory.lock())
            && (uptr = gr.gridPtr.lock())
            && (vptr = gr.gridPtr2.lock())
            ) {
        auto&& i2w_ = ptr->indexToWorldIntegralFunction<mtao::CGrid>();
        auto&& i2w = [&](int x, int y) -> Eigen::Vector2f {
            return i2w_(Eigen::Vector2i(x,y));
        };
        Eigen::Vector2i n = ptr->extraCells<mtao::CGrid>();
        int nx = n(0);
        int ny = n(1);
        Eigen::Vector2f p;
        float& px = p(0);
        float& py = p(1);
        Eigen::Vector2f v;
        float& vx = v(0);
        float& vy = v(1);
        glBegin(GL_LINES);
        glColor4f(1,0,0,1);
        for(int i=0; i < nx; ++i) {
            for(int j=0; j < ny; ++j) {
                p = i2w(i,j);
                glVertex2f(px,py);
                vx = uptr->lerp(p);
                vy = vptr->lerp(p);
                v = 0.01 * v;
                glVertex2f(vx+px,vy+py);
            }
        }
        glEnd();
    }

}

void GLWidget::initializeGL() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPointSize(4);
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
        case mtao::UVGrid: renderVelocity(gr);break;
        default: break;

    }
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,1,1);
    glBegin(GL_POINTS);
    glVertex2f(0,0);
    glEnd();
    if(auto&& ptr = m_factory.lock()) {
        auto&& i2w_ = ptr->indexToWorldIntegralFunction<mtao::NGrid>();
        auto&& i2w = [&](int x, int y) -> Eigen::Vector2f {
            return i2w_(Eigen::Vector2i(x,y));
        };
        Eigen::Vector2i n = ptr->extraCells<mtao::NGrid>();
        int& nx = n(0);
        int& ny = n(1);


        glColor4f(1.0,1.0,1.0,0.4);
        Eigen::Vector2f val;
        float& x = val(0);
        float& y = val(1);
        glBegin(GL_LINES);
        for(int i=0; i < nx; ++i) {
            val = i2w(i,0);
            glVertex2f(x,y);
            val = i2w(i,ny-1);
            glVertex2f(x,y);
        }
        for(int j=0; j < ny; ++j) {
            val = i2w(0,j);
            glVertex2f(x,y);
            val = i2w(nx-1,j);
            glVertex2f(x,y);
        }
        glEnd();

    }

    for(auto&& gr: m_grids) {
        renderGridTypeSelector(gr);
    }

}


void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
}
