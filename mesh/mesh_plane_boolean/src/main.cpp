#include "trianglemesh.h"
#include "plane.h"
#include <iostream>
#include <typeinfo>
#include <Eigen/Eigenvalues>

int main() {
    TriangleMesh m(TriangleMesh::readObj("a.obj"));
    m.writeObj("b.obj");
    Eigen::Matrix3d distrib = Eigen::Matrix3d::Zero();
    Eigen::Vector3d center = Eigen::Vector3d::Zero();
    for(auto&& v: m.vertices) {
        center += v;
    }
    center /= static_cast<double>(m.vertices.size());
    for(auto&& v: m.vertices) {
        auto t = v - center;
        distrib += t*t.transpose();
    }
    distrib /= static_cast<double>(m.vertices.size());

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(distrib,Eigen::ComputeEigenvectors);
    int maxcoeff;
    es.eigenvalues().maxCoeff(&maxcoeff);
    Eigen::Vector3d lambda_0 = es.eigenvectors().col(maxcoeff);

    lambda_0 = Eigen::Vector3d(0,1,0);
    center = Eigen::Vector3d(0,0,0);
    Plane p{{center,lambda_0}};
    //Plane p{Eigen::Vector3d(0,0,0),Eigen::Vector3d(1,0,0)};

    p.cut(m).writeObj("cutMesh.obj");
}
