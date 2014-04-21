#include "mesh/mesh_renderer/glwidget.h"
#include <QApplication>

int main(int argc, char * argv[]) {
    QApplication app(argc,argv);
    GLWidget *widget = new GLWidget();
    widget->show();
    return app.exec();
}
