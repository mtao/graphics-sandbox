#include <QApplication>
#include "window.h"


int main(int argc, char * argv[]) {



    QApplication app(argc, argv);

    auto window = new Window();
    window->show();


    return app.exec();
}
