#include "kdtree.h"

KDNodeP::KDNodeP(const KDTreeP& tree,int idx) 
    : KDNodeBase<KDNodeP>(tree)
      , p(idx)
{
}
    KDNodeP::KDNodeP(const KDNodeP & p,int idx)
    : KDNodeBase<KDNodeP>(p)
      , p(idx)
{
}






KDNode::KDNode(KDTree& tree,const Eigen::Vector3d & v) 
    : KDNodeBase<KDNode>(tree)
      , p(v)
{
}
    KDNode::KDNode(const KDNode & p,const Eigen::Vector3d & v)
    : KDNodeBase<KDNode>(p)
      , p(v)
{
}

KDTreeP::KDTreeP(const std::vector<Vector3>& v): verts(v) {
    for(int i=0; i < v.size(); ++i) {
        insert(i);
    }
}

const KDNodeP::Vector3& KDNodeP::value()const{return tree->value(p);}
