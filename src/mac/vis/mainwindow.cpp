#include "mainwindow.h"
#include "glwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>


MainWindow::MainWindow(const GridFactoryType::ptr & ptr): m_factory(ptr) {

    GLWidget * widget = new GLWidget(this,ptr);
    setCentralWidget(widget);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    static_cast<GLWidget*>(centralWidget())->keyPressEvent(event);
    if(!event->isAccepted()) {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    static_cast<GLWidget*>(centralWidget())->mousePressEvent(event);
    if(!event->isAccepted()) {
        QMainWindow::mousePressEvent(event);
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    static_cast<GLWidget*>(centralWidget())->wheelEvent(event);
    if(!event->isAccepted()) {
        QMainWindow::wheelEvent(event);
    }
}
