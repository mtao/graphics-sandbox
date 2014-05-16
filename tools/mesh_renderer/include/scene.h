#ifndef MESH_SCENE_H
#define MESH_SCENE_H
class QOpenGLShaderProgram;
#include <memory>
#include <vector>
#include <QOpenGLBuffer>
#include "types.h"
#include <QOpenGLFunctions_3_0>

class SceneNode {
    public:
        typedef std::shared_ptr<SceneNode> Ptr;
        virtual void render(QOpenGLShaderProgram* program, QOpenGLFunctions_3_0&) = 0;
    virtual mtao::BBox3f bbox() const = 0;
};
class InternalSceneNode {
    public:
        virtual void render(QOpenGLShaderProgram* program, QOpenGLFunctions_3_0&) ;
        virtual mtao::BBox3f bbox() const;
        void add(const SceneNode::Ptr& ptr) ;
    private:
    std::vector<SceneNode::Ptr> children;
};

struct MeshSceneNode: public SceneNode {
    public:
        typedef std::shared_ptr<MeshSceneNode> Ptr;
        static Ptr create(const QString& filename);
        mtao::BBox3f bbox() const {return m_bbox;}
        void render(QOpenGLShaderProgram* program, QOpenGLFunctions_3_0&) ;
    private:
        MeshSceneNode(const QOpenGLBuffer& v, const QOpenGLBuffer indices, const QOpenGLBuffer& texture, size_t count, const mtao::BBox3f& bbox);
        QOpenGLBuffer m_vertices,m_indices,m_texture;
        size_t m_count;
        mtao::BBox3f m_bbox;
};

#endif
