#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <memory>
#include "voxelObject.h"
#include <functional>
#include <vector>
struct VoxelMode{
    QString name;
    std::function<std::shared_ptr<VoxelObjectBase>(size_t,size_t,size_t)> func;
};
struct ConstraintMode{
    QString name;
    std::function<void(VoxelObjectBase&)> func;
};
class QTreeWidget;
class QComboBox;
class SideBar: public QWidget{
    Q_OBJECT
    public:
        SideBar(QWidget * parent=0);
    protected:
    std::shared_ptr<VoxelObjectBase> obj;
    QComboBox * voxelSelection;
    QComboBox * constraintSelection;
    std::vector<VoxelMode> voxel_modes;
    std::vector<ConstraintMode> constraint_modes;
    private:
    bool voxelSet=false;
    bool constraintSet=false;

    public slots:
void setVoxelConfiguration(const QString & text);
void setConstraintConfiguration(const QString & text);
    void checkIfSolveValid();
signals:
void newVoxelConfiguration(std::shared_ptr<VoxelObjectBase>);
    void newConstraintsSet();
    void runSolve();

};



#endif
