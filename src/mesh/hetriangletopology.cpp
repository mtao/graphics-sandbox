#include "mesh/hetriangletopology.h"
#include <limits>

void mtao::HETriangleTopologyConstructor::add_triangle(size_t a, size_t b, size_t c) {
    ptr_type&& ab = create_edge(a,b);
    ptr_type&& bc = create_edge(b,c);
    ptr_type&& ca = create_edge(c,a);
    HalfEdge::set_triangle(ab,bc,ca);
    
    triangles.insert(ca);
}
auto mtao::HETriangleTopologyConstructor::create_edge(size_t a, size_t b) -> ptr_type{

    auto amapit = m_edgeMap.find(a);
    if( amapit != m_edgeMap.end()) {
        auto&& amap = amapit->second;
        auto bit = amap.find(b);
        if(bit != amap.end()) {
            return bit->second;
        } else {
            auto&& hep = HalfEdge::make_half_edge(a,b);
            amap.emplace(b,hep[0]);
            insert(hep[1]);
            return hep[0];
        }
    } else {
            auto&& hep = HalfEdge::make_half_edge(a,b);
            insert(hep[0]);
            insert(hep[1]);
            return hep[0];
    }
}

void mtao::HETriangleTopologyConstructor::insert(const ptr_type& ptr) {
    
    const size_t& b = ptr->head;
    const size_t& a = ptr->dual->head;
    auto amapit = m_edgeMap.find(a);
    if( amapit == m_edgeMap.end()) {
        amapit = m_edgeMap.emplace(a,std::map<size_t,ptr_type>()).first;
    }

    auto&& amap = amapit->second;
    amap.emplace(b,ptr);
}


auto mtao::HETriangleTopologyConstructor::toSet() const -> std::set<ptr_type> {
    std::set<ptr_type> ret_set;
    for(auto&& amappair: m_edgeMap) {
        for(auto&& bpair: amappair.second) {
            ret_set.insert(bpair.second);

        }
    }
    return ret_set;
}

//auto mtao::HETriangleTopologyConstructor::toVector() const -> std::vector<ptr_type> {
//    std::vector<ptr_type> ret_vec;
//    for(auto&& amappair: m_edgeMap) {
//        for(auto&& bpair: amappair.second) {
//            ret_vec.push_back(bpair.second);
//
//        }
//    }
//    return ret_vec;
//}


/*  /c\      /c\
// / | \    /   \
//b  |  d  b---->d
// \ v /    \   /
//  \a/      \a/
*/

void mtao::HETriangleTopologyOperators::flip(ptr_type& edge) {
    //a->b->c
    auto&& ca = edge;
    auto&& ab = ca->next;
    auto&& bc = ab->next;
    //a->c->d
    auto&& ac = edge->dual;
    auto&& cd = ac->next;
    auto&& da = ac->next;

    auto&& bd = ca;
    auto&& db = ac;

    //flip the indices
    bd->head = cd->head;
    db->head = ab->head;
    
    //flip rest of it

    bc->next = cd;
    cd->next = db;
    db->next = bc;

    db->next = da;
    da->next = ab;
    ab->next = bd;
}

/*  /c\        /c\
// / | \      / v \
//b  |  d    b--e--d 
// \ v /      \ v /  
//  \a/        \a/   
*/

void mtao::HETriangleTopologyOperators::split(ptr_type& ca, size_t e) {
    //a->b->c
    auto&& ab = ca->next;
    auto&& bc = ab->next;
    //a->c->d
    auto&& ac = ca->dual;
    auto&& cd = ac->next;
    auto&& da = ac->next;

    auto&& pr_eb = HalfEdge::make_half_edge(e,ab->head);
    auto&& pr_ea = HalfEdge::make_half_edge(e,da->head);
    auto&& pr_ed = HalfEdge::make_half_edge(e,cd->head);
    auto&& eb = pr_eb[0];
    auto&& be = pr_eb[1];
    auto&& ea = pr_ea[0];
    auto&& ae = pr_ea[1];
    auto&& ed = pr_ed[0];
    auto&& de = pr_ed[1];

    //contract ac/ca -> ec/ce
    ca->head = e;
    auto&& ce = ca;
    auto&& ec = ac;

    bc->next = ce;
    ce->next = eb;
    eb->next = bc;

    cd->next = de;
    de->next = ec;
    ec->next = cd;

    da->next = ae;
    ae->next = ed;
    ed->next = da;

    ab->next = be;
    be->next = ea;
    ea->next = ab;
}
/*--/c\--     \   / 
// / | \       \ / 
//b  |  d    ---d---    
// \ v /       / \
//--\a/--     /   \
*/

void mtao::HETriangleTopologyOperators::collapse(ptr_type& ca) {
    
    //a->b->c
    auto&& ab = ca->next;
    auto&& bc = ab->next;
    //a->c->d
    auto&& ac = ca->dual;
    auto&& cd = ac->next;
    auto&& da = ac->next;

    //d is the center
    size_t center = cd->head;
    //circulate a
    for(ptr_type e = ab->dual; e != da; e = e->circulateCW()) {
        e->head = center;
    }
    //circulate b
    for(ptr_type e = bc->dual; e != ab; e = e->circulateCW()) {
        e->head = center;
    }

    //circulate c
    for(ptr_type e = cd->dual; e != bc; e = e->circulateCW()) {
        e->head = center;
    }

    //remove bc/cb
    remove(bc);
    //remote da/ad
    remove(da);
}
void mtao::HETriangleTopologyOperators::remove(ptr_type& edge) {
    auto&& dual = edge->dual;
    //Because we use shared_ptr there's a strong chance the edge wont actually be removed, so we are nullifying the edges
    edge->head = -1;
    dual->head = -1;
    topology->halfedges().erase(edge);
    topology->halfedges().erase(dual);
}




auto mtao::HETriangleTopology::triangles() const -> std::set<ptr_type> {
    std::set<ptr_type> ret;
    for(auto&& he: halfedges()) {
        auto&& tri = triangle(he);
        if(tri) {
            ret.insert(tri);
        }
    }
           
    return ret;
}

auto mtao::HETriangleTopology::triangle(const ptr_type& he) const -> ptr_type {
    if(!he->next) {
        return ptr_type();
    }
    ptr_type min = he;
    for(ptr_type it = he->next; it && (it != he); it = it->next) {
        if(it->head < min->head) {
            min = it;
        }
    }
    return min;
}
