#include "mesh/loaders/objloader.h"

void OBJLoader::open(const std::string& str) {
    mtao::FileParser fp(filename.toStdString());
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    std::array<float,3> t;
    std::array<std::string,3> s;
    std::array<GLuint,3> i;
    //mtao::BBox3f bb;
//    std::array<int,3> uv;
    for(auto&& line: fp) {
        char v;
        std::stringstream ss(line);
        ss >> v;
        switch(v) {
            case 'v':
                ss >> t[0] >> t[1] >> t[2];
                add_vertex(t[0],t[1],t[2]);
                //bb.extend(Eigen::Map<mtao::Vec3f>(t.begin()));
                break;
            case 'f':
                ss >> s[0] >> s[1] >> s[2];
                for(size_t m=0; m < 3; ++m) {
                    auto&& str = s[m];
                    auto it = str.begin();
                    for(; it != str.end(); ++it) {
                        if(*it == '/') {
                            break;
                        }
                    }
                    std::stringstream ss2(str.substr(0,std::distance(str.begin(),it)));
                    ss2 >> i[m];

                }
                add_triangle(i[0],i[1],i[2]);
                break;
            default:
                break;
        }
    }
    float scaler = bb.sizes().maxCoeff();
    for(auto&& v: vertices) {
        v/=scaler;
    }
    bb.min() = bb.min() / scaler;
    bb.max() = bb.max() / scaler;
    QOpenGLBuffer vbo(QOpenGLBuffer::Type::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(),vertices.size()*sizeof(float));

    QOpenGLBuffer ibo(QOpenGLBuffer::Type::IndexBuffer);
    ibo.create();
    ibo.bind();
    size_t count = indices.size();
    ibo.allocate(indices.data(),indices.size()*sizeof(GLuint));
    std::cout << "Loaded mesh from file: (" << filename.toStdString() << ")BBox: " << bb.min().transpose() << " -> " << bb.max().transpose() << std::endl;
    return std::shared_ptr<MeshSceneNode>(new MeshSceneNode(vbo,ibo,QOpenGLBuffer(),count,bb));
}
void OBJLoader::save(const std::string& str) {
}
