#ifndef CONTROLS_H
#define CONTROLS_H
#include <memory>
#include <QWidget>

namespace mtao { namespace qt {
class ControlWidgetBase: public QWidget {
    virtual void update() = 0;
};

template <typename T>
class ControlWidget;

class ControlWidgetRegistry {
    public:
        static ControlWidgetRegistry& get();
        typedef std::shared_ptr<ControlWidgetBase> CtrlPtr;
        typedef std::weak_ptr<ControlWidgetBase> CtrlWeakPtr;
        struct ControlData {
            CtrlWeakPtr ptr;
            bool dirty=true;
        };
    private:
        ControlWidgetRegistry() {}
        std::map<void*,ControlData> ctrl_map;
};


}}


#endif
