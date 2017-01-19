#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP
//#include <GL/gl3w.h>
#include <GL/glew.h>
#include <memory>
#include "renderer.hpp"
#include <Eigen/Dense>
#include "mesh/simplexmesh.h"
#include <glm/glm.hpp>

class MeshRenderer: public Renderer {
    public:
        class MeshWrapperBase;
        template <int D>
            class SimplexMeshWrapper;
        MeshRenderer(const std::shared_ptr<MeshWrapperBase>& m);
        template <int D>
        MeshRenderer(const std::shared_ptr<mtao::geometry::mesh::SimplexMesh<D>>& m): MeshRenderer(std::make_shared<SimplexMeshWrapper<D>>(m)) {}
        ~MeshRenderer();
        void _render(int w, int h) override;

        GLfloat& scale() { return mScale; }

    private:
        std::shared_ptr<MeshWrapperBase> mMesh;
        GLuint mVBO, mVAO, mEBO;
        GLint mColorUniform;
        GLint mScaleUniform;
        GLfloat mScale = 1.0;
};



class MeshRenderer::MeshWrapperBase {
    public:
        virtual float* vertexData() const = 0;
        virtual int vertexSize() const = 0;
        virtual int * facetData() const = 0;
        virtual int facetSize() const = 0;
        virtual int numVertices() const = 0;
        virtual int numFacets() const = 0;
        virtual int verticesPerFacet() const = 0;
        virtual int dimension() const = 0;
        GLenum renderEnum() {
            switch(verticesPerFacet()) {
                case 1: return GL_POINTS;
                case 2: return GL_LINES;
                case 3: return GL_TRIANGLES;
                default: return GL_POINTS;
            }
        }
};


template <int D>
class MeshRenderer::SimplexMeshWrapper: public MeshRenderer::MeshWrapperBase {
    public:
        using SimplexMesh = mtao::geometry::mesh::SimplexMesh<D>;
        using SMPtr = std::shared_ptr<SimplexMesh>;

        SimplexMeshWrapper(const SMPtr& p): sm(p) {}

        float* vertexData() const {
            return sm->vertices().data();
        }

        int vertexSize() const {
            return sm->vertices().size();
        }
        int numVertices() const {
            return sm->vertices().cols();
        }

        int * facetData() const {
            return sm->simplices().data();
        }

        int facetSize() const {
            return sm->simplices().size();
        }
        int numFacets() const {
            return sm->simplices().cols();
        }

        int verticesPerFacet() const {
            return D+1;
        }
        int dimension() const {
            return sm->vertices().rows();
        }

    private:
        SMPtr sm;
};

#endif//MESHRENDERER_HPP
