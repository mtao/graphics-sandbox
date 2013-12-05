#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QKeyEvent>
#include <QDockWidget>
#include <iostream>
#include <random>
#include <wrap/io_trimesh/import.h>
#include <vcg/complex/algorithms/update/normal.h>

MainWindow::MainWindow(QWidget * parent): QMainWindow(parent) {
    setMenuBar(new QMenuBar(this));
    QMenu *fileMenu = menuBar() -> addMenu(tr("&File"));
    QAction *openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    // Quit action
    QAction *quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(openAct);
    fileMenu->addAction(quitAct);

    QGLFormat glFormat;
    std::cout << glFormat.openGLVersionFlags() << std::endl;
    std::cout << (QGLFormat::OpenGL_Version_3_0 <= glFormat.openGLVersionFlags()) << std::endl;
    if(QGLFormat::OpenGL_Version_3_3 & glFormat.openGLVersionFlags())
    {
        glFormat.setVersion( 3,3 );
    }
    else
    {
        glFormat.setVersion( 2, 1 );
    }
    std::cout << "GL Version: " << glFormat.majorVersion() << " " << glFormat.minorVersion() << std::endl;
    glFormat.setProfile( QGLFormat::CompatibilityProfile );
    glFormat.setSampleBuffers( true );
    m_glwidget = new GLWidget(glFormat,this);
    connect(this,&MainWindow::meshLoaded, m_glwidget,&GLWidget::receiveMesh);
    setCentralWidget(m_glwidget);



    connect(this,&MainWindow::loadingNewMesh
            , m_glwidget,&GLWidget::unloadMesh);

}



void MainWindow::openFile(const QString & filename) {
    emit loadingNewMesh();
    m_mesh.reset(new Mesh());
    if(vcg::tri::io::Importer<Mesh>::Open(*m_mesh,filename.toStdString().c_str()) != 0) {
        qDebug() << "Couldn't read mesh";
        return;
    }
    initializeMesh();
}

void MainWindow::initializeMesh() {
    if(!m_mesh) return;
    vcg::tri::UpdateNormal<Mesh>::PerVertexNormalizedPerFace(*m_mesh);
    vcg::tri::UpdateNormal<Mesh>::PerFaceNormalized(*m_mesh);
    emit meshLoaded(m_mesh);
}






void MainWindow::openFile() {
    QFileDialog::Options options(QFileDialog::HideNameFilterDetails);
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Choose file or directory"),
                QDir::homePath(),
                //tr("OBJ Files (*.obj)")
                tr("All (*)")
                );

    if (filename.isNull()) {
        return;
    }
    openFile(filename);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    switch(event->key()){
    default:
        m_glwidget->keyPressEvent(event);
        QMainWindow::keyPressEvent(event);
        return;
    }
    event->accept();
}
