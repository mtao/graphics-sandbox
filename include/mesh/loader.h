#ifndef LOADER_H
#define LOADER_H
#include <string>

class MeshLoader {
    public:
        MeshLoader(MeshConstructorBase* constructor): m_constructor(constructor) {}
        virtual void open(const std::string& str) = 0;
        virtual void save(const std::string& str) = 0;
    protected:
        MeshConstructorBase * m_constructor;


};

#endif
