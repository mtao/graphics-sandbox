#define USE_IGL
#define USE_VIEWER
#include <iostream>
#include <Eigen/Dense>
#include <iomanip>
#ifdef USE_IGL
#ifdef USE_VIEWER
#include <igl/viewer/Viewer.h>
#include <igl/parula.h>
#include <igl/jet.h>
#endif
#include <igl/read_triangle_mesh.h>
#include <igl/centroid.h>
#endif
#include <Eigen/Sparse>
#include <iostream>
#include <queue>
#include "sphere.hpp"
#include "spherical_harmonics.hpp"

Eigen::MatrixXd V;
Eigen::MatrixXi F;
Eigen::MatrixXd Vmesh;
Eigen::MatrixXi Fmesh;
Eigen::MatrixXd Vsh;
Eigen::MatrixXi Fsh;


constexpr static int SHDim = 4;

std::unique_ptr<SphericalHarmonics<SHDim,Eigen::VectorXd>> sh;

void init() {

    SphereMesh<double> s(8);
    auto sc = s.spherical_coordinates();
    sh.reset(new SphericalHarmonics<SHDim,Eigen::VectorXd>(sc.topRows(2)));

    Vsh = s.vertices();
    auto&& faces = s.faces();
    Fsh.resize(faces.size(),3);
    for(int i = 0; i < faces.size(); ++i) {

        for(int j = 0; j < 3; ++j) {
            Fsh(i,j) = faces[i][j];
        }
    }
}


int main(int argc, char * argv[]) {
    using Mat2 = SphereMesh<double>::Mat2X;
    using Mat3 = SphereMesh<double>::Mat3X;

    init();

    if(argc < 2) {

        SphereMesh<double> s(3);
        Vmesh = s.vertices().transpose();

        Mat3 rad = SphereMesh<double>::spherical_coordinates(Vmesh.transpose());
        SphericalHarmonics<SHDim,Eigen::VectorXd> mesh_harmonics(rad.topRows(2));

        Eigen::VectorXd true_data;
        double size = mesh_harmonics.size();
        true_data = Eigen::VectorXd::Random(size);
        true_data(0) = 2*true_data.norm();
        auto data = mesh_harmonics.eval(true_data);
        

        Vmesh = (Vmesh.array().colwise() *data.array().cast<double>());


        auto faces = s.faces();
        Fmesh.resize(faces.size(),3);
        for(int i = 0; i < faces.size(); ++i) {

            for(int j = 0; j < 3; ++j) {
                Fmesh(i,j) = faces[i][j];
            }
        }
    } else {
#ifdef USE_IGL
        igl::read_triangle_mesh(argv[1], Vmesh, Fmesh);
        Eigen::RowVector3d centroid;

        igl::centroid(Vmesh,Fmesh,centroid);
        Vmesh = Vmesh.rowwise() - centroid;
#else
        return 0;
#endif
    }


    Mat3 rad = SphereMesh<double>::spherical_coordinates(Vmesh.transpose());



    SphericalHarmonics<SHDim,Eigen::VectorXd> mesh_harmonics(rad.topRows(2));

    auto CFS = mesh_harmonics.values();

    Eigen::VectorXd coeffs = rad.row(2);
    Eigen::MatrixXd A = CFS.transpose() * CFS;
    Eigen::VectorXd b = CFS.transpose() * coeffs;

    Eigen::VectorXd x = A.ldlt().solve(b);


    std::cout << "Error: " << (coeffs - CFS * x).norm() << std::endl;


    coeffs = sh->eval(x);


    F = Fsh;

#ifdef USE_IGL
#ifdef USE_VIEWER
    igl::viewer::Viewer viewer;



    auto toggle_mesh = [&]() {
        static bool mesh = true;
        if(mesh) {
            std::cout << "Switching to original mesh!" << std::endl;
            V = Vmesh;
            F = Fmesh;
        } else {
            std::cout << "Using internal SH representation!" << std::endl;
            //V = Vsh;
            V = (Vsh.array().transpose().colwise() *coeffs.array().cast<double>());
            F = Fsh;
        }
        mesh = !mesh;
        std::cout << "V: " << V.rows() << " " << V.cols() << std::endl;
        std::cout << "F: " << F.rows() << " " << F.cols() << std::endl;
        viewer.data.clear();
        viewer.data.set_mesh(V,F);
        viewer.data.compute_normals();
    };

    toggle_mesh();
    viewer.callback_key_down = [&](igl::viewer::Viewer & viewer,unsigned char key,int)->bool
    {
        switch(key) {
            case ' ':
                toggle_mesh();
                break;
            default:
                break;
        }
        return false;
    };
  viewer.callback_pre_draw = 
    [&](igl::viewer::Viewer &)->bool
  {
    if(viewer.core.is_animating)
    {
        //igl::jet(coeffs.cast<double>(),true,C);
        //viewer.data.set_colors(C);
    }
    return false;
  };
  viewer.callback_key_down(viewer,' ',0);
  viewer.core.show_lines = false;
  viewer.core.is_animating = true;
  viewer.launch();
#endif
#endif

}
