#ifndef _SPHERE_MESH_GEN_H_
#define _SPHERE_MESH_GEN_H_
#include <array>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <Eigen/Dense>
#include <cmath>
template <typename Scalar>
class SphereMesh{
    typedef std::array<int, 3> Face;
    typedef std::array<int, 2> Edge;
    public:
    using scalar_type = Scalar;
    typedef typename Eigen::Matrix<scalar_type,3,1> Vec3;
    using Vec2 = typename Eigen::Matrix<scalar_type,2,1>;
    using Mat3X = typename Eigen::Matrix<scalar_type,3,Eigen::Dynamic>;
    using Mat2X = typename Eigen::Matrix<scalar_type,2,Eigen::Dynamic>;
    SphereMesh(int depth=3);
    void triforce(const Face & f, int depth, int& cur_vertex_count);
    int add_edge(Edge e, int& cur_vertex_count);
    void write(const std::string & filename, const Mat3X& vertices)const;
    void write(std::ostream & outstream, const Mat3X& vertices)const;
    void write(const std::string & filename) const{
        return write(filename,m_vertices);
    }
    void write(std::ostream & outstream)const {
        return write(outstream,m_vertices);
    }
    const std::vector<Face>& faces() const {return m_faces;}
    const Mat3X& vertices() const {return m_vertices;}

    Mat3X static spherical_coordinates(const Mat3X& coords) ;
    Mat3X spherical_coordinates() const {
        return spherical_coordinates(m_vertices);
    }
    Mat3X static cartesian_coordinates(const Mat3X& coords) ;



    private:
    const int m_depth = 0;
    Mat3X m_vertices;
    std::vector<Face> m_faces;
    std::map<Edge,  int> m_edges;

};



template <typename T>
SphereMesh<T>::SphereMesh(int depth): m_depth(depth) {
    //Create icosahedron base

    scalar_type gr = .5 * (1 + std::sqrt(scalar_type(5)));
    //Depth Faces Edges Verts
    //0     20    30    12
    //1     80    120   42
    //2     320   80    162
    m_vertices.resize(3,2 + 10* std::pow<int>(4,depth));

    m_vertices.col( 0) = Vec3(     0,    - 1,     gr);
    m_vertices.col( 1) = Vec3(    gr,      0,      1);
    m_vertices.col( 2) = Vec3(    gr,      0,    - 1);
    m_vertices.col( 3) = Vec3(   -gr,      0,    - 1);
    m_vertices.col( 4) = Vec3(   -gr,      0,      1);
    m_vertices.col( 5) = Vec3(   - 1,     gr,      0);
    m_vertices.col( 6) = Vec3(     1,     gr,      0);
    m_vertices.col( 7) = Vec3(     1,    -gr,      0);
    m_vertices.col( 8) = Vec3(   - 1,    -gr,      0);
    m_vertices.col( 9) = Vec3(     0,    - 1,    -gr);
    m_vertices.col(10) = Vec3(     0,      1,    -gr);
    m_vertices.col(11) = Vec3(     0,      1,     gr);
    m_vertices.colwise().normalize();
    int cur_vertex_count = 12;

    triforce({{ 1 ,  2 ,  6}},depth, cur_vertex_count); 
    triforce({{ 1 ,  7 ,  2}},depth, cur_vertex_count); 
    triforce({{ 3 ,  4 ,  5}},depth, cur_vertex_count); 
    triforce({{ 4 ,  3 ,  8}},depth, cur_vertex_count); 
    triforce({{ 6 ,  5 , 11}},depth, cur_vertex_count); 
    triforce({{ 5 ,  6 , 10}},depth, cur_vertex_count); 
    triforce({{ 9 , 10 ,  2}},depth, cur_vertex_count); 
    triforce({{10 ,  9 ,  3}},depth, cur_vertex_count); 
    triforce({{ 7 ,  8 ,  9}},depth, cur_vertex_count); 
    triforce({{ 8 ,  7 ,  0}},depth, cur_vertex_count); 
    triforce({{11 ,  0 ,  1}},depth, cur_vertex_count); 
    triforce({{ 0 , 11 ,  4}},depth, cur_vertex_count); 
    triforce({{ 6 ,  2 , 10}},depth, cur_vertex_count); 
    triforce({{ 1 ,  6 , 11}},depth, cur_vertex_count); 
    triforce({{ 3 ,  5 , 10}},depth, cur_vertex_count); 
    triforce({{ 5 ,  4 , 11}},depth, cur_vertex_count); 
    triforce({{ 2 ,  7 ,  9}},depth, cur_vertex_count); 
    triforce({{ 7 ,  1 ,  0}},depth, cur_vertex_count); 
    triforce({{ 3 ,  9 ,  8}},depth, cur_vertex_count); 
    triforce({{ 4 ,  8 ,  0}},depth, cur_vertex_count); 



}
template <typename T>
void SphereMesh<T>::triforce(const Face & f, int depth, int& cur_vertex_count) {
    if(depth <= 0) {
        m_faces.push_back(f);
    } else {
        int e01 = add_edge({{f[0],f[1]}}, cur_vertex_count);
        int e12 = add_edge({{f[1],f[2]}}, cur_vertex_count);
        int e02 = add_edge({{f[0],f[2]}}, cur_vertex_count);
        triforce({{f[0],e01,e02}},depth-1, cur_vertex_count);
        triforce({{f[1],e12,e01}},depth-1, cur_vertex_count);
        triforce({{f[2],e02,e12}},depth-1, cur_vertex_count);
        triforce({{e01 ,e12,e02}},depth-1, cur_vertex_count);
    }

}

template <typename T>
int SphereMesh<T>::add_edge(Edge e, int& cur_vertex_count) {
    if(e[0] > e[1]) {
        int tmp = e[0];
        e[0] = e[1];
        e[1] = tmp;
    }
    auto it = m_edges.find(e);
    if(it != m_edges.end()) {
        return it->second;
    } else {
        m_edges[e] = cur_vertex_count;
        m_vertices.col(cur_vertex_count++) = 
                (m_vertices.col(e[0]) + m_vertices.col(e[1])).normalized()
                ;
        return cur_vertex_count - 1;
    }


}


template <typename T>
void SphereMesh<T>::write(const std::string & filename, const Mat3X& vertices) const{
    std::ofstream outstream(filename.c_str());
    write(outstream, vertices);
}

template <typename T>
void SphereMesh<T>::write(std::ostream & outstream, const Mat3X& vertices) const{
    outstream << "#Icosahedral subdivision to depth " << m_depth << std::endl;

    for(int i = 0; i < m_vertices.cols(); ++i) {
        outstream << "v " << vertices.col(i).transpose() << std::endl;
    }

    for(auto&& f: m_faces) {
        outstream << "f " << f[0]+1 << " " << f[1]+1 << " " << f[2]+1 << std::endl;
    }
}

template <typename T>
auto SphereMesh<T>::spherical_coordinates(const Mat3X& coords) -> Mat3X {
    Mat3X ret(3,coords.cols());
    ret.setZero();
    auto&& x = coords.row(0).array();
    auto&& y = coords.row(1).array();
    auto&& z = coords.row(2).array();
    auto&& t = ret.row(0).array();
    auto&& p = ret.row(1).array();
    auto&& r = ret.row(2).array();
    r = coords.colwise().norm();
    auto xy = coords.topRows(2).colwise().norm();
    for(int i = 0; i < ret.cols(); ++i) {
        t(i) = std::atan2(y(i),x(i));
        //p(i) = std::atan2(r(i),z(i));
        p(i) = std::atan2(xy(i),z(i));
    }
    return ret;
}
template <typename T>
auto SphereMesh<T>::cartesian_coordinates(const Mat3X& coords) -> Mat3X {
    Mat3X ret(3,coords.cols());
    auto&& x = ret.row(0).array();
    auto&& y = ret.row(1).array();
    auto&& z = ret.row(2).array();
    auto&& t = coords.row(0).array();
    auto&& p = coords.row(1).array();
    auto&& r = coords.row(2).array();

    auto sp = Eigen::sin(p).eval();
    x = r * sp * Eigen::cos(t);
    y = r * sp * Eigen::sin(t);
    z = r * Eigen::cos(p);

    return ret;
}




#endif



/*
#include <iostream>

int main(int argc, char * argv[]) {
    //SphereMesh<float>().write(std::cout);
    SphereMesh<float> s(0);
    std::cout << s.vertices() << std::endl;
    std::cout << std::endl;
    SphereMesh<float>::Mat2X sc = s.spherical_coordinates();
    std::cout << sc << std::endl;

    std::cout << std::endl;
    std::cout << s.cartesian_coordinates(sc) << std::endl;
}
*/
