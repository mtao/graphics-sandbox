#ifndef KDTREE_H
#define KDTREE_H
#include <Eigen/Geometry>
#include <vector>
#include <memory>
#include "kdetreebase.h"
class KDTreeP;
class KDNodeP;
class KDTree;
class KDNode;

template <>
struct traits<KDTree> {
    typedef KDNode node_type;
    typedef Eigen::Vector3d vector_type;
};
template <>
struct traits<KDTreeP> {
    typedef KDNodeP node_type;
    typedef Eigen::Vector3d vector_type;
};
template <>
struct traits<KDNode> {
    typedef KDTree tree_type;
    typedef traits<tree_type>::vector_type vector_type;
    typedef vector_type stored_type;
};
template <>
struct traits<KDNodeP> {
    typedef KDTreeP tree_type;
    typedef traits<tree_type>::vector_type vector_type;
    typedef int stored_type;
};


class KDNodeP: public KDNodeBase<KDNodeP> {
    public:
    typedef traits<KDNode>::vector_type Vector3;
        typedef KDTreeP tree_type;
        KDNodeP(const KDNodeP & parent,int idx);
        KDNodeP(const KDTreeP & parent,int idx);
        const Vector3& value()const;
        int p=-1;
};

class KDTree;
class KDNode: public KDNodeBase<KDNode> {
    public:
        typedef typename traits<KDNode>::vector_type Vector3;
        KDNode(KDTree& tree, const Vector3 & v);
        KDNode(const KDNode & parent, const Vector3 & v);
        const Vector3& value()const {return p;}
        Vector3 p;
};


class KDTreeP: public KDTreeBase<KDTreeP> {
    typedef Eigen::Vector3d Vector3;
    public:
    typedef Vector3 vector_type;
    KDTreeP(const std::vector<Vector3>& verts);
    const Vector3& value(const int& v)const{return verts[v];}
    private:
    friend class KDNodeP;
    const std::vector<Vector3> & verts;
};

class KDTree: public KDTreeBase<KDTree> {
    typedef traits<KDTree>::vector_type Vector3;
    public:
    KDTree() {}
    const Vector3& value(const Vector3& v)const{return v;}

    private:
    friend class KDNode;
};
#endif
