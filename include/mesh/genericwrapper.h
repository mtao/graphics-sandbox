#ifndef MESH_GENERIC_WRAPPER_H
#define MESH_GENERIC_WRAPPER_H

namespace mtao {
    namespace internal {
        //Not going to conform to stl for these
        class VertexIteratorBase {
            public:
            virtual operator bool() const = 0;
            virtual mtao::Vec3f getF() const = 0;
            virtual mtao::Vec3d getD() const = 0;
            virtual void next();
        };
        template <typename T>
            VertexIteratorBaseScalar: virtual VertexIteratorBase {
                public:
                    typedef typename mtao::scalar_types<T>::Vec3 Vec3;
                    Vec3 get();
                    mtao::Vec3d getD() const;
                    mtao::Vec3f getF() const;
            };
        template <>
            mtao::Vec3f VertexIteratorBaseScalar<float>::getD() const {return get();}
        template <>
            mtao::Vec3d VertexIteratorBaseScalar<float>::getD() const {return get().cast<double>();}

        template <>
            mtao::Vec3f VertexIteratorBaseScalar<double>::getD() const {return get().cast<float>();}
        template <>
            mtao::Vec3d VertexIteratorBaseScalar<double>::getD() const {return get();}

        class TriangleIteratorBase {
            typedef std::array<size_t,3> triangle_type;
            virtual operator bool() const = 0;
            virtual triangle_type get() const = 0;
            virtual void next();
        };
    }
    class MeshWrapperNT {
        public:
            virtual VertexIteratorBase vertexIterator() = 0;
            virtual TriangleIteratorBase triangleIterator() = 0;
    };
    template <typename MeshType>
        class MeshWrapper: public internal::VertexIteratorBaseScalar<MeshType::Scalar> {
            public:
                VertexIteratorBase vertexIterator();
                TriangleIteratorBase triangleIterator();
            private:
                MeshType* mesh;
        };
};

#endif
