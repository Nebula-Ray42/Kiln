#pragma once
#include <vector>

namespace kiln::mesh {

struct MeshData {
  std::vector<float> positions; // x, y, z, x, y, z...
  std::vector<float> normals;   // x, y, z...
  std::vector<float> uvs;       // u, v, u, v...
  std::vector<uint32_t> indices;
};

}  // namespace kiln::mesh