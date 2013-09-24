#include "include/sidebar.h"
#include <QGroupBox>
#include <QTreeWidget>
#include <QComboBox>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QPushButton>

SideBar::SideBar(QWidget * parent): QWidget(parent) {
    setLayout(new QVBoxLayout);
    QGroupBox * groupbox = new QGroupBox(tr("Voxel Configuration Selector"));
    layout()->addWidget(groupbox);

    groupbox->setLayout(new QVBoxLayout);

    voxel_modes.push_back(
            {QString("Cube")
            ,
            [](size_t ni, size_t nj, size_t nk) 
            -> std::shared_ptr<VoxelObjectBase >
            {
            auto&& obj = 
            std::make_shared<InstancedVoxelObjectBase<SparseLattice,Voxel<LINEAR> > >(ni,nj,nk) ;
            for(int i=0; i < ni; ++i) {
            for(int j=0; j < nj; ++j) {
            for(int k=0; k < nk; ++k) {
            obj->addVoxel(i,j,k);
            }
            }
            }
            return obj;

            }
            }
            );
    voxel_modes.push_back(
            {QString("Heatsink")
            ,
            [](size_t ni, size_t nj, size_t nk) 
            -> std::shared_ptr<VoxelObjectBase >
            {
            auto&& obj = 
            std::make_shared<InstancedVoxelObjectBase<SparseLattice,Voxel<LINEAR> > >(ni,nj,nk) ;
            for(int i=0; i < ni; ++i,++i) {
            for(int j=0; j < nj; ++j) {
            for(int k=0; k < nk; ++k) {
            obj->addVoxel(i,j,k);
            }
            }
            }
            std::cout << "pipes!\n";
            for(int i=0; i < ni; ++i) {
            obj->addVoxel(i,nj-1,nk-1);
            obj->addVoxel(i,nj-1,0);
            obj->addVoxel(i,0,nk-1);
            obj->addVoxel(i,0,0);
            }
            return obj;

            }
            }
    );

    constraint_modes.push_back(
            ConstraintMode{QString("Corners")
            ,
            [](VoxelObjectBase& obj) -> void {

            for(int l=0; l < 2; ++l) {
            for(int m=0; m < 2; ++m) {
            for(int n=0; n < 2; ++n) {
            obj.setConstraint(obj.NI()-1+l,obj.NJ()-1+m,obj.NK()-1+n, -1);
            obj.setConstraint(l,m,n,1);
            }
            }
            }
            }
            }
            );
    constraint_modes.push_back(
            {QString("Near Sides")
            ,
            [](VoxelObjectBase& obj) -> void{
            for(int i=0; i <= obj.NI(); ++i) {
            for(int j=1; j <= obj.NJ(); ++j) {
            obj.setConstraint(i,j,0,-1);

            }
            }
            for(int j=1; j <= obj.NJ(); ++j) {
            for(int k=0; k <= obj.NK(); ++k) {
            obj.setConstraint(0,j,k,1);

            }
            }
            }
            }
            );
    voxelSelection = new QComboBox(this);
    constraintSelection = new QComboBox(this);
    voxelSelection->addItem("");
    for(auto&& vm: voxel_modes) {
        voxelSelection->addItem(vm.name);
    }
    constraintSelection->addItem("");
    for(auto&& vm: constraint_modes) {
        constraintSelection->addItem(vm.name);
    }
    groupbox->layout()->addWidget(voxelSelection);
    groupbox->layout()->addWidget(constraintSelection);

    QPushButton * button = new QPushButton(tr("Solve!"),this);
    connect(button,SIGNAL(clicked()),this,SLOT(checkIfSolveValid()));
    groupbox->layout()->addWidget(button);

    connect(voxelSelection,SIGNAL(currentIndexChanged(const QString & )), this, SLOT(setVoxelConfiguration(const QString &)));
    connect(constraintSelection,SIGNAL(currentIndexChanged(const QString & )), this, SLOT(setConstraintConfiguration(const QString &)));

}
void SideBar::
checkIfSolveValid() {
    if(voxelSet && constraintSet)
        emit runSolve();
}


void SideBar::setVoxelConfiguration(const QString & text) {
    obj.reset();
    voxelSet = false;
   if(text == "") return;
   int dim = 15;
    for(auto&& vm: voxel_modes) {
        if(vm.name == text) {
            obj = vm.func(dim,dim,dim);
            emit newVoxelConfiguration(obj);
   voxelSet = true;
            return;
        }
    }
}

void SideBar::setConstraintConfiguration(const QString & text) {
    obj->clearConstraints();
    constraintSet = false;
   if(text == "") return;
    for(auto&& vm: constraint_modes) {
        if(vm.name == text) {
            vm.func(*obj);
            obj->makeCompressed();
            emit newConstraintsSet();
    constraintSet =true;
            return;
        }
    }


}
