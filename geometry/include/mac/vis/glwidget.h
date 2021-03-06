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
        GridRenderable(
                typename MACGrid<Scalar,embed_dim,1,0>::ptr ptr
                , typename MACGrid<Scalar,embed_dim,1,1>::ptr ptr2
                , const QString & name)
        : gridName(name)
          , gridType(mtao::UVGrid)
          , gridPtr(ptr)
          , gridPtr2(ptr2)
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
    GridType::weak_ptr gridPtr2;
    bool enabled=true;
};

class GLWidget: public QGLWidget {
    Q_OBJECT
    public:
        enum {embed_dim = 2};
        typedef float Scalar;
        typedef Grid<Scalar,embed_dim> GridType;
        typedef MACGridFactory<Scalar,embed_dim> GridFactoryType;
        GLWidget(QWidget * parent, const GridFactoryType::ptr&);
        void paintGL();
        void resizeGL(int w, int h);
        void initializeGL();
    protected:
        friend class MainWindow;
        void mousePressEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent *);
        void keyPressEvent(QKeyEvent *);
        std::list<GridRenderable> m_grids;
        public slots:
        void cleanRenderables();
        void addGrid(GridType::ptr, mtao::GridEnum type, const QString & name);
        void addVelocity(MACGrid<Scalar,embed_dim,1,0>::ptr
                ,MACGrid<Scalar,embed_dim,1,1>::ptr, const QString& name);
    private:
        void renderGridTypeSelector(const GridRenderable &);
        template <mtao::GridEnum Type>
        void renderGridByType(const GridRenderable & gr) {renderGrid(gr);}
        void renderGrid(const GridRenderable &);
        void renderVelocity(const GridRenderable &);
        GridFactoryType::weak_ptr m_factory;

};
#endif
