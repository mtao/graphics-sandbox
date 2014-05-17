#ifndef OFF_LOADER_H
#define OFF_LOADER_H
#include "../loader.h"

namespace mtao {
class OFFLoader: public MeshLoader {
    public:
        OFFLoader() {}
        OFFLoader(MeshConstructorBase* constructor): MeshLoader(constructor) {}
    void open(const std::string& str);
    void save(const std::string& str);


};
}

#endif
