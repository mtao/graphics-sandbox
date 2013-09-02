#include <iostream>
#include "macgridfactory.h"
#include "rungekutta2.h"
#include "expliciteuler.h"

int main(int argc, char * argv[]) {
    Eigen::AlignedBox<float,3> m(Eigen::Vector3f(-.5,-.5,-0.5),Eigen::Vector3f(.5,.5,.5));

    auto&& factory = std::shared_ptr<MACGridFactory<float,3> >(new MACGridFactory<float,3>(Eigen::Vector3i(10,10,10),m));
    Eigen::Vector3f cent = factory->bbox().center();

    auto&& u = factory->createPtr<mtao::UGrid>();
    std::fill(u->begin(),u->end(),1);

    u->loop([&](const Eigen::Vector3i& c, double v)->double{
            
            return 10*((u->indexToWorld(c)-cent).norm()-.3);
            });


    auto&& v = factory->createPtr<mtao::VGrid>();
    std::fill(v->begin(),v->end(),-1);
    v->loop([&](const Eigen::Vector3i& c, double vl)->double{
            
            return 10*((v->indexToWorld(c)-cent).norm()-.3);
            });


    auto&& w = factory->createPtr<mtao::WGrid>();
    w->loop([&](const Eigen::Vector3i& c, double v)->double{
            
            return 10*((w->indexToWorld(c)-cent).norm()-.3);
            });
    mtao::advection::RungeKutta2<float,3,1> rk2(*u,*v,*w);
    for(int i=0; i < 100; ++i) {
        std::cout << i << ") " << cent.transpose() << "  Current velocity: " << rk2.velocity(cent).transpose() << std::endl;
        rk2.evalInPlace(cent,0.02);
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    cent = factory->bbox().center();
    mtao::advection::ExplicitEuler<float,3,1> ee(*u,*v,*w);
    for(int i=0; i < 100; ++i) {
        std::cout << i << ") " << cent.transpose() << "  Current velocity: " << ee.velocity(cent).transpose() << std::endl;
        ee.evalInPlace(cent,0.02);
    }
    return 0;
    
}
