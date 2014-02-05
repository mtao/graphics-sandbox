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

MeshRenderMainWindow::MeshRenderMainWindow(QWidget * parent): QMainWindow(parent) {
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
    connect(this,&MeshRenderMainWindow::meshLoaded, m_glwidget,&GLWidget::receiveMesh);
    setCentralWidget(m_glwidget);



    connect(this,&MeshRenderMainWindow::loadingNewMesh
            , m_glwidget,&GLWidget::unloadMesh);
    connect(this,&MeshRenderMainWindow::toggleDrawMode
            , m_glwidget,&GLWidget::toggleDrawMode);
    connect(this,&MeshRenderMainWindow::drawSmooth
            , m_glwidget,&GLWidget::drawSmooth);
    connect(this,&MeshRenderMainWindow::drawPoints
            , m_glwidget,&GLWidget::drawPoints);
    connect(this,&MeshRenderMainWindow::drawWire
            , m_glwidget,&GLWidget::drawWire);

}



void MeshRenderMainWindow::openFile(const QString & filename) {
    emit loadingNewMesh();
    m_mesh.reset(new Mesh());
    int err_code = vcg::tri::io::Importer<Mesh>::Open(*m_mesh,filename.toStdString().c_str());
    if(vcg::tri::io::Importer<Mesh>::ErrorCritical(err_code)){
        qDebug() << "Couldn't read mesh: " << vcg::tri::io::Importer<Mesh>::ErrorMsg(err_code);
        return;
    }
    initializeMesh();
}

void MeshRenderMainWindow::initializeMesh() {
    if(!m_mesh) return;
    auto&& bbox = m_mesh->bbox;
    for(auto&& p: m_mesh->vert) {
        bbox.Add(p.P());
    }
    auto&& dim = bbox.Dim();
    auto&& center = bbox.Center();
    double max = std::max({dim[0],dim[1],dim[2]});
    for(auto&& p: m_mesh->vert) {
        auto&& v = p.P();
        v = (v-center)/max;
    }
    bbox.SetNull();
    for(auto&& p: m_mesh->vert) {
        bbox.Add(p.P());
    }
    std::cout << "(";
    std::cout << bbox.min[0] << " ";
    std::cout << bbox.min[1] << " ";
    std::cout << bbox.min[2] << ")->(";
    std::cout << bbox.max[0] << " ";
    std::cout << bbox.max[1] << " ";
    std::cout << bbox.max[2] << ")\n";
    std::cout << "Building normals per vertex:" << std::endl;
    vcg::tri::UpdateNormal<Mesh>::PerVertexNormalizedPerFace(*m_mesh);
    std::cout << "Building normals per face:" << std::endl;
    vcg::tri::UpdateNormal<Mesh>::PerFaceNormalized(*m_mesh);
    std::cout << "Done with normals" << std::endl;
    emit meshLoaded(m_mesh);
}






void MeshRenderMainWindow::openFile() {
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

void MeshRenderMainWindow::keyPressEvent(QKeyEvent *event) {

    switch(event->key()){
        case Qt::Key_AsciiTilde:
            emit toggleDrawMode();
        case Qt::Key_1:
            emit drawSmooth();
            break;
        case Qt::Key_2:
            emit drawWire();
            break;
        case Qt::Key_3:
            emit drawPoints();
            break;
        case Qt::Key_B:
            emit toggleBBox();
            break;
    default:
        m_glwidget->keyPressEvent(event);
        QMainWindow::keyPressEvent(event);
        return;
    }
    event->accept();
}
