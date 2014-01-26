#ifndef MASS_SPRING_SYSTEM_HPP
#define MASS_SPRING_SYSTEM_HPP
#include <array>
#include <eigen3/Eigen/Dense>

struct SpringProperties {
    float rest_length = 1.0f;
    float stiffness = 1.0f;
};

class MassSpringSystem {
    public:
        typedef Eigen::Vector3f Vec3f;
        typedef Eigen::MatrixXf Matrix;
        typedef std::array<size_t,2> Edge;

        MassSpringSystem(const std::vector<Vec3f>& vertices, const std::vector<float>& mass, const std::vector<Edge>& edges, const std::vector<SpringProperties>& edge_properties);
    private:
        std::vector<Vec3f> computeForces() const;

    private:
        std::vector<Vec3f> m_vertices;
        std::vector<float> m_mass;
        std::vector<Edge> m_edges;
        std::vector<SpringProperties> m_edge_properties;

};

#endif
