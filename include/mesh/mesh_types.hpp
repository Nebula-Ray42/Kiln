//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#pragma once
#include <vector>
#include <span>

namespace kiln::mesh {

struct MeshData {
    // 1. 唯一のデータ所有者（巨大なバイナリ）
    std::vector<std::byte> raw_buffer;

    // 2. メタデータ（どこに・何個あるか）
    size_t positions_byte_offset = 0;
    size_t positions_float_count = 0;

    size_t normals_byte_offset = 0;
    size_t normals_float_count = 0;

    size_t uvs_byte_offset = 0;
    size_t uvs_float_count = 0;

    size_t indices_byte_offset = 0;
    size_t indices_count = 0; // 今回は uint32_t 固定と仮定

    // 3. 必要な時だけゼロコピーで窓枠を作る（メソッド化）
    [[nodiscard]] std::span<const float> get_positions() const noexcept {
        if (positions_float_count == 0) return {};
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const auto* ptr = reinterpret_cast<const float*>(raw_buffer.data() + positions_byte_offset);
        return {ptr, positions_float_count};
    }

    [[nodiscard]] std::span<const float> get_normals() const noexcept {
        if (normals_float_count == 0) return {};
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const auto* ptr = reinterpret_cast<const float*>(raw_buffer.data() + normals_byte_offset);
        return {ptr, normals_float_count};
    }

    [[nodiscard]] std::span<const float> get_uvs() const noexcept {
        if (uvs_float_count == 0) return {};
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const auto* ptr = reinterpret_cast<const float*>(raw_buffer.data() + uvs_byte_offset);
        return {ptr, uvs_float_count};
    }

    [[nodiscard]] std::span<const uint32_t> get_indices() const noexcept {
        if (indices_count == 0) return {};
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const auto* ptr = reinterpret_cast<const uint32_t*>(raw_buffer.data() + indices_byte_offset);
        return {ptr, indices_count};
    }
};

}  // namespace kiln::meshln::mesh