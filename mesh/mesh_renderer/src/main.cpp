#include <QApplication>
#include "mainwindow.h"


int main(int argc, char * argv[]) {
    QApplication a(argc,argv);
    MeshRenderMainWindow * mw = new MeshRenderMainWindow(); 
    QStringList args = a.arguments();
    if(args.size() > 1) {
        mw->openFile(args[1]);
    }
    mw->show();
    

    return a.exec();
}
