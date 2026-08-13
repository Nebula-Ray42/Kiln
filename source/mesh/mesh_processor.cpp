// source/mesh/mesh_processor.cpp
//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#include "mesh/mesh_processor.hpp"
#include <algorithm>
#include <expected>
#include <string>

namespace kiln::mesh {

std::expected<void, std::string> validate_mesh(const MeshData& mesh) noexcept {

    const auto positions = mesh.get_positions();
    const auto uvs = mesh.get_uvs();
    const auto indices = mesh.get_indices();

    if (positions.empty()) {
        return std::unexpected("エラー: 頂点データ(positions)が空っぽです");
    }

    const size_t vertex_count = positions.size() / 3;

    if (not uvs.empty()) {
        const size_t uv_count = uvs.size() / 2;
        if (vertex_count != uv_count) {
            return std::unexpected("エラー: 頂点の数とUVの数が一致しません");
        }
    }

    if (not indices.empty()) {
        auto max_index = std::ranges::max(indices);
        if (max_index >= vertex_count) {
            return std::unexpected("エラー: 存在しない頂点を指しているインデックスがあります");
        }
    }

    return {};
}

} // namespace kiln::mesh