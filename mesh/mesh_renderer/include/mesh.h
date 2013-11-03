#ifndef MESH_H
#define MESH_H
#include <vcg/complex/complex.h>
#include <vector>
#include <memory>

class Vertex; class Edge; class Face;
struct UsedTypes: public vcg::UsedTypes<
                  vcg::Use<Vertex>  ::AsVertexType,
                  vcg::Use<Edge>  ::AsEdgeType,
                  vcg::Use<Face>  ::AsFaceType
                  > {};
class Vertex    : public vcg::Vertex<UsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags >{};
class Face      : public vcg::Face<UsedTypes, vcg::face::VertexRef, vcg::face::Normal3f, vcg::face::BitFlags > {};
class Edge      : public vcg::Edge<UsedTypes> {};

class Mesh      : public vcg::tri::TriMesh< std::vector<Vertex>, std::vector<Face>, std::vector<Edge> >{
    public:
        typedef std::shared_ptr<Mesh> ptr;
        typedef std::shared_ptr<Mesh> shared_ptr;
        typedef std::weak_ptr<Mesh> weak_ptr;
        typedef std::unique_ptr<Mesh> unique_ptr;
};
#endif
