#ifndef INTERNAL_SCENENODE_CONTROLS_H
#ifndef INTERNAL_SCENENODE_CONTROLS_H
#include "../controls.h"
#include "render/internalnode.h"

namespace mtao { namespace qt {
    template <>
        class ControlWidget<InternalSceneNode>: public ControlWidgetBase {
            Q_OBJECT
            public:
                typedef ControlWidget<InternalSceneNode> Type;
                Type(const std::shared_ptr<InternalSceneNode>& ptr);
                virtual void update();
            private:
                std::shared_ptr<InternalSceneNode> node;

        };
}}

#endif
