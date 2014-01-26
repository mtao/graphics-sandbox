#include "mass_spring_system.hpp"

    MassSpringSystem::MassSpringSystem() {}
MassSpringSystem::MassSpringSystem(const std::vector<Vec3f>& vertices, const std::vector<float>& mass, const std::vector<Edge>& edges, const std::vector<SpringProperties>& edge_properties)
: m_vertices(vertices), m_velocities(vertices.size()), m_mass(mass), m_edges(edges), m_edge_properties(edge_properties) {

}
void MassSpringSystem::advect(float dt) {
    auto&& forces = computeForces();
    for(size_t i = 0; i < forces.size(); ++i) {
        m_velocities[i] += dt * (forces[i]/m_mass[i] + Vec3f(0,0.1,0));
        m_velocities[i] *= 0.999;
    }
    for(size_t i = 0; i < forces.size(); ++i) {
        if(m_mass[i] != 0.0) {
            m_vertices[i] += dt * m_velocities[i];
        }
    }
}

auto MassSpringSystem::computeForces() const -> std::vector<Vec3f> {
    
    std::vector<Vec3f> forces(m_vertices.size(),Vec3f(0,0,0));
    Matrix stiffness(m_vertices.size(),m_vertices.size());
    for(size_t i=0; i < m_edges.size(); ++i) {
        auto&& edge = m_edges[i];
        auto&& v0 = m_vertices[edge[0]];
        auto&& v1 = m_vertices[edge[1]];
        //auto&& m0 = m_mass[edge[0]];
        //auto&& m1 = m_mass[edge[1]];
        auto&& edge_props = m_edge_properties[i];
        Vec3f dir = (v1-v0);
        float f = (dir.norm()-edge_props.rest_length) * edge_props.stiffness;
        dir.normalize();
        stiffness(edge[0],edge[1]) = f;
        forces[edge[0]] += f * dir;
        forces[edge[1]] -= f * dir;

    }

    return forces;
}
