#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "../loader.h"

namespace mtao {
class OBJLoader: public MeshLoader {
    public:
        OBJLoader() {}
        OBJLoader(MeshConstructorBase* constructor): MeshLoader(constructor) {}
    void open(const std::string& str);
    void save(const std::string& str);


};
}

#endif
