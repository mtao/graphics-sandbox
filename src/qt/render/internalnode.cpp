#include "qt/render/internalnode.h"

//TODO: create a registry of pointers to controls

namespace mtao { namespace qt {
    template<>
        ControlWidget<InternalSceneNode>::ControlWidget(const std::shared_ptr<InternalSceneNode>& ptr): node(ptr) {
            update();
        }
    template<>
        ControlWidget<InternalSceneNode>::update() {
        }
}}
