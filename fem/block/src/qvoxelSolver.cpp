#include "include/qvoxelSolver.h"
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

QVoxelSolver::
QVoxelSolver(const std::shared_ptr<VoxelObjectBase> & voxelobj, QObject * parent)
    : QObject(parent),VoxelSolver(voxelobj)
{
    auto&& qargs = QCoreApplication::arguments();
    for(auto&& arg: qargs) {
        qWarning() << arg;
    }
}

void QVoxelSolver::
setObject(const std::shared_ptr<VoxelObjectBase> & voxelobj) {
    VoxelSolver::setObject(voxelobj);
    emit objectChanged();
}

bool QVoxelSolver::
solve() {
    qWarning() << "Solving!";
    bool ret = VoxelSolver::solve();
    emit objectChanged();
    return ret;
}

void QVoxelSolver::
setPackageVBOs(Package & pkg) {
    VoxelRenderPackage vp = m_voxel_obj->getRenderData();
    if(!pkg.activeData) {
    pkg.activeData = std::make_shared<VertexBufferObject>(vp.data.data(),vp.data.size(),GL_STATIC_DRAW,4);
    pkg.indices = std::make_shared<VertexIndexObject>(vp.indices.data(),vp.indices.size(), GL_STATIC_DRAW,GL_TRIANGLES);
    } else {
    pkg.activeData->setData(vp.data.data(),vp.data.size(),GL_STATIC_DRAW,4);
    pkg.indices->setData(vp.indices.data(),vp.indices.size(), GL_STATIC_DRAW,GL_TRIANGLES);


    }
    emit packageSet();
}

