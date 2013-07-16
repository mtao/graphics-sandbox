#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "mac.h"
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
        MainWindow();
    protected:
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    GLWidget & glwidget(){return *static_cast<GLWidget*>(centralWidget());}
    private:
    //std::unique_ptr<MACGridFactory<Scalar,embed_dim> > m_factory;
};
#endif
