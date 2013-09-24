#ifndef QVOXELSOLVER_H
#define QVOXELSOLVER_H
#include <QObject>
#include "voxelSolver.h"
#include "package.h"
#include "sidebar.h"
class QVoxelSolver: public QObject, public VoxelSolver{
    public:
    Q_OBJECT
    public:
    QVoxelSolver(const std::shared_ptr<VoxelObjectBase> & voxelobj = 0, QObject * parent=0);
    
   signals:
       void objectChanged();
       void packageSet();
   public slots:
       void setPackageVBOs(Package & pkg);
        void setObject(const std::shared_ptr<VoxelObjectBase> & voxelobj);
        bool solve();
};
#endif
