#include "macgridfactory.h"
#include <iostream>


struct A {
    typedef MACGridFactory<float,2> FactoryType;
    A(Eigen::AlignedBox<float,2> bbox, Eigen::Vector2i v)
        : m_factory(v,bbox)
          , u(m_factory.create<mtao::UGrid>()) 
    {}
    FactoryType m_factory;
    FactoryType::Selector<mtao::UGrid>::type u;
};
struct B {
    typedef MACGridFactory<float,2> FactoryType;
    B(Eigen::AlignedBox<float,2> bbox, Eigen::Vector2i v)
        : m_factory(v,bbox)
          , u(m_factory.createPtr<mtao::UGrid>()) 
    {}
    FactoryType m_factory;
    FactoryType::Selector<mtao::UGrid>::type::ptr u;
};

int main() {
    //The grid will be fined by an axis aligned bounding box...
    Eigen::AlignedBox<double,3> m(Eigen::Vector3d(0,0,0),Eigen::Vector3d(1,1,1));
    //In order to have all grids created be consistent wit heach other you need to make them
    //from a factory
    MACGridFactory<double,3> factory(Eigen::Vector3i(10,10,10),m);

    //After you have a factory you can create a grid rather easily!
    auto&& g = factory.create<mtao::UGrid>();
    std::cout << g.size() << std::endl;
    std::cout << g.N().transpose() << std::endl;
    std::cout << g.origin().transpose() << std::endl;
    std::cout << g.dx().transpose() << std::endl;

    //Here's a vertex-centered one (VGrid is already taken so I use N for nodal)
    auto&& g2 = factory.create<mtao::NGrid>();
    std::cout << g2.size() << std::endl;
    std::cout << g2.N().transpose() << std::endl;
    std::cout << g2.indexToWorld(0,0,0).transpose() << std::endl;
    std::cout << g2.dx().transpose() << std::endl;

    //Individual elements can be accessed like this:
    g2(0,0,0) = 1;
    //Interpolating is done through lerp on a world space vector
    auto&& v = g2.lerp(Eigen::Vector3d(0.01,0.01,0.01));
    std::cout << "Value" << v << std::endl;
    g2(0,0,0) = 1;
    g2(0,1,0) = 1;
    g2(0,1,1) = 1;
    g2(0,0,1) = 1;
    v = g2.lerp(Eigen::Vector3d(0.05,0.05,0.05));
    std::cout << "Value" << v << std::endl;

    Eigen::AlignedBox<double,2> m2(Eigen::Vector2d(0,0),Eigen::Vector2d(1,1));
    MACGridFactory<double,2> factory2(Eigen::Vector2i(10,10),m2);
    auto&& g3 = factory2.create<mtao::CGrid>();

    std::cout << g3.size() << std::endl;
    std::cout << g3.N().transpose() << std::endl;
    std::cout << g3.origin().transpose() << std::endl;
    std::cout << g3.dx().transpose() << std::endl;
    //Can use fill because we forward std::vector iterators!
    std::fill(g3.begin(),g3.end(),3);
    g3(3,4) = 20;
    //Range for loops work too!
    for(auto&& v: g3) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    g3.loop([](double v)->double{
            return 2*v;
            });
    for(auto&& v: g3) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    Eigen::Vector2d center = factory2.bbox().center();
    g3.loop([&](const Eigen::Vector2i& c, double v)->double{
            
            return (g3.indexToWorld(c)-center).norm()-.3;
            });
    for(auto&& v: g3) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    auto&& a = factory2.createManagedPtr<mtao::CGrid>();
    auto&& b = factory2.createManagedPtr<mtao::CGrid>();
    auto&& c = factory2.createManagedPtr<mtao::NGrid>();
    auto&& d = factory2.createManagedPtr<mtao::UGrid>();
    Eigen::Vector2i coord(2,2);
    a->indexToWorld(coord);
    b->indexToWorld(coord);
    c->indexToWorld(coord);
    d->indexToWorld(coord);
    std::cout << factory2.create<mtao::CGrid>().indexToWorld(coord).transpose() << std::endl;
    std::cout << factory2.create<mtao::UGrid>().indexToWorld(coord).transpose() << std::endl;
    std::cout << factory2.create<mtao::VGrid>().indexToWorld(coord).transpose() << std::endl;
    std::cout << factory2.create<mtao::NGrid>().indexToWorld(coord).transpose() << std::endl;

    std::cout << std::endl << "Resize test: " << std::endl;
    std::cout << a->indexToWorld(coord).transpose() << std::endl;
    std::cout << b->indexToWorld(coord).transpose() << std::endl;
    std::cout << c->indexToWorld(coord).transpose() << std::endl;
    std::cout << d->indexToWorld(coord).transpose() << std::endl;
    std::cout << std::endl;
    factory2.resize(Eigen::Vector2i(30,30));
    std::cout << a->indexToWorld(coord).transpose() << std::endl;
    std::cout << b->indexToWorld(coord).transpose() << std::endl;
    std::cout << c->indexToWorld(coord).transpose() << std::endl;
    std::cout << d->indexToWorld(coord).transpose() << std::endl;
    auto&& f1 = factory2.indexToWorldIntegralFunction<mtao::CGrid>();
    std::cout << f1(coord) << std::endl;
}
