#pragma once
#include <vector>
#include <cstdint>

namespace kiln::mesh {

  struct Vertex {
    float position[3];
    float normal[3];
    float uv[2];
  };

  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<u_int32_t> indices;
  };

}  // namespace kiln::mesh