#include "../include/mainwindow.h"
#include "../include/glwidget.h"
#include "../include/voxelSolver.h"
#include <vector>
#include <string>
#include <QKeyEvent>
#include <QDebug>
#include <QDockWidget>

MainWindow::
    MainWindow(QWidget * parent)
: QMainWindow(parent)
{



    /*
    std::shared_ptr<VoxelObject> obj = 
        std::make_shared<LinearHomgeneousVoxelGrid >(10,10,10);
    static_cast<LinearHomgeneousVoxelGrid *>(obj.get())->lattice().fill();
    */
    solver = new QVoxelSolver(0,this);
    GLWidget * glwidget = new GLWidget(this);


    //Connections to make VBOs swap
    //Note: This definitely only works with one solver per GLWidget
    // -first solver lets the glwidget it wants to refresh
    connect(solver,SIGNAL(objectChanged()),glwidget,SLOT(vboRequest()));
    // -second glwidget sends the unused vbo package
    connect(glwidget,SIGNAL(modifiableVBO(Package &)),solver,SLOT(setPackageVBOs(Package &)));
    // -third solver finishes writing to the vbo and tells the glwidget it can stop
    connect(solver,SIGNAL(packageSet()), glwidget,SLOT(swapVBO()));



    sidebar = new SideBar(this);
    QDockWidget * dock = new QDockWidget(tr("Form Chooser"),this);
    dock->setWidget(sidebar);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(sidebar,SIGNAL(newVoxelConfiguration(std::shared_ptr<VoxelObjectBase>)), solver,SLOT(setObject(const std::shared_ptr<VoxelObjectBase> )));
    sidebar->show();
    connect(sidebar,SIGNAL(runSolve()),solver,SLOT(solve()));

    setCentralWidget(glwidget);
}
MainWindow::
~MainWindow() {
}





void MainWindow::
keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_S) 
        solver->solve();
    else
    QMainWindow::keyPressEvent(event);
}
