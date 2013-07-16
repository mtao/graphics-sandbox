#include <iostream>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char * argv[]) {
    QApplication app(argc,argv);
    MainWindow * window = new MainWindow();
    window->show();
    //The grid will be fined by an axis aligned bounding box...
    Eigen::AlignedBox<float,2> m(Eigen::Vector2f(-.5,-.5),Eigen::Vector2f(.5,.5));

    m_factory = std::unique_ptr<MACGridFactory<float,2> >(new MACGridFactory<float,2>(Eigen::Vector2i(50,50),m));

    auto&& g = factory.createPtr<mtao::UGrid>();
    window->add
    return app.exec();
    
}
