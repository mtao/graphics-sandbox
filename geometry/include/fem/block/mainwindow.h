#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <memory>
#include "qvoxelSolver.h"


class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget * parent=0);
        ~MainWindow();
    protected:
        void keyPressEvent(QKeyEvent * event);
    private:
        QVoxelSolver * solver;
        SideBar * sidebar;
};

#endif
