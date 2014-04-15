
template <typename T>
struct traits {
    typedef Eigen::Vector3d vector_type;
};
template <typename Derived>
class KDNodeBase {
    public:
        typedef typename traits<Derived>::tree_type tree_type;
        typedef typename traits<Derived>::vector_type vector_type;
        typedef typename traits<Derived>::stored_type stored_type;
        Derived& derived(){return *static_cast<Derived*>(this);}
        const Derived& derived()const {return *static_cast<const Derived*>(this);}
        KDNodeBase(const tree_type& tree);
        KDNodeBase(const Derived & parent);
        Derived& insert(const stored_type&);
        const vector_type& value()const {
            return derived().value();
        }
        const vector_type& value(const stored_type& v)const {
            return tree->value(v);
        }
        int axis=0;
        std::unique_ptr<Derived> left,right;
        const tree_type * tree;
};


    template <typename Derived>
    KDNodeBase<Derived>::KDNodeBase(const Derived& p)
: axis((p.axis+1)%3)
    , tree(p.tree) {
    }
    template <typename Derived>
    KDNodeBase<Derived>::KDNodeBase(const tree_type& p)
: tree(&p) {
    }
    template <typename Derived>
Derived& KDNodeBase<Derived>::insert(const typename traits<Derived>::stored_type & v) {
    auto&& nv = value(v);
    auto&& mv = value();
    if(nv(axis) < mv(axis)) {
        if(left) {
            return left->insert(v);
        } else {
            left = std::unique_ptr<Derived>(new Derived(derived(),v));
            return *left;
        }
    } else {
        if(right) {
            return right->insert(v);
        } else {
            right = std::unique_ptr<Derived>(new Derived(derived(),v));
            return *right;
        }
    }
}
template <typename Derived>
class KDTreeBase{
    public:
        typedef typename traits<Derived>::vector_type vector_type;
        typedef typename traits<Derived>::node_type node_type;
        typedef typename traits<node_type>::stored_type stored_type;
        Derived& derived(){return *static_cast<Derived*>(this);}
        const Derived& derived()const {return *static_cast<const Derived*>(this);}
        node_type& insert(const stored_type& v);
        const vector_type& value(const stored_type& v)const{
            return derived().value(v);
        }
        std::unique_ptr<node_type> root;
    Eigen::AlignedBox<typename vector_type::Scalar,Eigen::internal::traits<vector_type>::RowsAtCompileTime> bbox;

};
template <typename Derived>
auto KDTreeBase<Derived>::insert(const stored_type & v)->node_type&{
        bbox.extend(value(v));
    if(root) {
        return root->insert(v);
    } else {
        root = std::unique_ptr<node_type>(new node_type(derived(),v));
        return *root;
    }
}
