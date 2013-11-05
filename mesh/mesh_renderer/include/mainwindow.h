#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "glwidget.h"
#include "mesh.h"


class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void openFile();
    virtual void openFile(const QString & filename);
    virtual void initializeMesh();
protected:
    GLWidget * m_glwidget;
    Mesh::shared_ptr m_mesh;
signals:
    void meshLoaded(const Mesh::shared_ptr&);
    void dataLoaded();
    void loadingNewMesh();
    void toggleDrawMode();
    void toggleBBox();
    void drawSmooth();
    void drawWire();
    void drawPoints();
};

#endif
