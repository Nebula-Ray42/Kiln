//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

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