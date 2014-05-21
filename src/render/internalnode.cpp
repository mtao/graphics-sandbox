#include "render/internalnode.h"
#include <iostream>

namespace mtao{namespace rendering{
void InternalSceneNode::render() {
    for(auto&& ptr: children) {
        ptr->render();
    }

}
mtao::BBox3f InternalSceneNode::bbox() const {
    mtao::BBox3f bb;
    for(auto&& ptr: children) {
        bb.extend(ptr->bbox());
    }
    return bb;

}
void InternalSceneNode::add(const SceneNode::Ptr& ptr) {
    children.push_back(ptr);
}
}}
