#include "trellis_generator.hpp"
#include <openvdb/math/Stencils.h>
#include <memory>
#include <random>
#include <iostream>

TrellisGenerator::TrellisGenerator() {
}

Trellis TrellisGenerator::generate() const {
    auto&& vertices = this->generate_vertices();
    std::vector<Edge> edges;
    for(size_t i=0; i < vertices.size(); ++i) {
        for(size_t j=i+1; j < vertices.size(); ++j) {
            auto&& vi = vertices[i];
            auto&& vj = vertices[j];
            if((vi-vj).length() < max_edge_length) {
                edges.emplace_back(Edge{{i,j}});
            }
        }
    }
    return Trellis{vertices,edges};
}

TrellisGenerator::Vertex randomVertex(std::uniform_real_distribution<>& dis, std::mt19937& gen) {
  return {dis(gen),dis(gen),dis(gen)};

}

auto TrellisGenerator::generate_vertices(const openvdb::DoubleGrid::Ptr& grid) const -> std::vector<Vertex>{
    bool have_grid = bool(grid);
    std::unique_ptr<openvdb::math::BoxStencil<openvdb::DoubleGrid> > stencil;
    if(have_grid) {
        stencil.reset(new openvdb::math::BoxStencil<openvdb::DoubleGrid>(*grid));
    }
    auto volume = bbox.volume();
    int num_vertices = density * volume;
    std::vector<Vertex> vertices;
    vertices.resize(num_vertices, Vertex{0,0,0});

    
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dis(0,1);


int pos=0;
    for(auto&& v: vertices) {
        if(have_grid) {
            while(true) {
                stencil->moveTo(v);
                v = randomVertex(dis,gen);
                if(stencil->interpolation(v) < 0)
                    break;
                if(stencil->interpolation(v) < 0.1) {
                    v = v + stencil->gradient(v);
                }
            }

        } else {
            for(int c = 0; c < 30; c++) {

                v = randomVertex(dis,gen);
                bool tooClose = false;
                for(int i=0; i < pos; ++i) {
                    auto&& v2 = vertices[i];
                    if((v2 - v).length() < min_vertex_dist) {
                        tooClose = true;
                        break;
                    }
                }
                if(!tooClose) {
                    break;
                }
            }
        }
        pos = pos+1;
    }

    return std::move(vertices);

}
