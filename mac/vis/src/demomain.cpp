#include <iostream>
#include <QApplication>
#include "mainwindow.h"
#include "macgridfactory.h"

int main(int argc, char * argv[]) {
    QApplication app(argc,argv);
    MainWindow * window = new MainWindow();
    window->show();
    //The grid will be fined by an axis aligned bounding box...
    Eigen::AlignedBox<float,2> m(Eigen::Vector2f(-.5,-.5),Eigen::Vector2f(.5,.5));

    auto&& factory = std::unique_ptr<MACGridFactory<float,2> >(new MACGridFactory<float,2>(Eigen::Vector2i(10,10),m));

    auto&& u = factory->createPtr<mtao::UGrid>();
    window->glwidget().addGrid(u,mtao::UGrid,"u");
    std::fill(u->begin(),u->end(),1);



    auto&& v = factory->createPtr<mtao::VGrid>();
    window->glwidget().addGrid(v,mtao::VGrid,"v");
    std::fill(v->begin(),v->end(),-1);

    auto&& center = factory->createPtr<mtao::CGrid>();
    window->glwidget().addGrid(center,mtao::UGrid,"c");
    Eigen::Vector2f cent = factory->bbox().center();
    center->loop([&](const Eigen::Vector2i& c, double v)->double{
            
            return 10*((center->indexToWorld(c)-cent).norm()-.3);
            });
    return app.exec();
    
}
