#ifndef MESH_SCENENODE_H
#define MESH_SCENENODE_H

#include "../scenenode.h"
#include <QOpenGLBuffer>
#include "mesh/vftrianglemesh.h"


namespace mtao{namespace rendering{
struct MeshSceneNode: public SceneNode {
    public:
        typedef std::shared_ptr<MeshSceneNode> Ptr;
        static Ptr create(const std::string& filename);
        mtao::BBox3f bbox() const {return m_bbox;}
        void render();
    private:
        MeshSceneNode(const QOpenGLBuffer& v, const QOpenGLBuffer indices, const QOpenGLBuffer& texture, size_t count, const mtao::BBox3f& bbox);
        QOpenGLBuffer m_vertices,m_indices,m_texture;
        size_t m_count;
        mtao::BBox3f m_bbox;
    mtao::VFTriangleMesh<float> mesh;
};
}}
#endif
