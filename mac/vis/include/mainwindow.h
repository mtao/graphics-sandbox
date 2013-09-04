#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "mac.h"
#include "glwidget.h"
#include <memory>


class QWheelEvent;
class QKeyEvent;
class QMouseEvent;

class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        enum {embed_dim = 2};
        typedef float Scalar;
        typedef Grid<Scalar,embed_dim> GridType;
        typedef MACGridFactory<Scalar,embed_dim> GridFactoryType;
        MainWindow(const GridFactoryType::ptr& ptr = 0);
    GLWidget & glwidget(){return *static_cast<GLWidget*>(centralWidget());}
    protected:
        void mousePressEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent *);
        void keyPressEvent(QKeyEvent *);
    private:
        GridFactoryType::ptr m_factory;
};
#endif
