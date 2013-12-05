#ifndef PACKAGE_H
#define PACKAGE_H

#include "glutil.h"
#include <memory>

struct Package {
    std::shared_ptr<VertexBufferObject> activeData;
    std::shared_ptr<VertexIndexObject> indices;
};


#endif
