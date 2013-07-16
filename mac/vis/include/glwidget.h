#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QGLWidget>
#include "macgridfactory.h"

struct GridRenderable{
        enum {embed_dim = 2};
        typedef float Scalar;
        typedef Grid<Scalar,embed_dim> GridType;
    template <int FormDim, int WhichForm>
        GridRenderable(typename MACGrid<Scalar,embed_dim,FormDim,WhichForm>::ptr ptr, const QString & name)
        : gridName(name)
          , gridType(mtao::internal::GridEnumSelector<embed_dim,FormDim,WhichForm>::value)
          , gridPtr(ptr)
          , enabled(false)
    {}
        GridRenderable(GridType::ptr ptr, const QString & name, mtao::GridEnum type)
        : gridName(name)
          , gridType(type)
          , gridPtr(ptr)
          , enabled(false)
    {}
    QString gridName;
    mtao::GridEnum gridType=mtao::NGrid;
    GridType::weak_ptr gridPtr;
    bool enabled=false;
};

class GLWidget: public QGLWidget {
    Q_OBJECT
    public:
        enum {embed_dim = 2};
        typedef float Scalar;
        typedef Grid<Scalar,embed_dim> GridType;
        GLWidget(QWidget * parent);
        void paintGL();
        void resizeGL(int w, int h);
    protected:
        friend class MainWindow;
        void mousePressEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent *);
        void keyPressEvent(QKeyEvent *);
        std::list<GridRenderable> m_grids;
        public slots:
        void cleanRenderables();
        void addGrid(GridType::ptr, mtao::GridEnum type, const QString & name);
};
#endif
