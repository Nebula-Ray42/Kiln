//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#include "format/gltf_parser.hpp"
#include "format/gltf_helper.hpp"
#include "core/file_io.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

namespace kiln::format {

std::expected<mesh::MeshData, std::string> parse_gltf(std::string_view filepath) noexcept {
    // 1. JSONの読み込み
    std::ifstream file{std::string(filepath), std::ios::in | std::ios::binary};
    if (not file.is_open()) return std::unexpected("エラー: ファイルを開けませんでした");

    nlohmann::json gltf_json;
    try {
        gltf_json = nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error& e) {
        return std::unexpected("エラー: JSONの形式が不正です");
    }

    // 2. メタデータの抽出 (POSITION)
    constexpr uint32_t GLTF_FLOAT = 5126;
    auto pos_meta_res = detail::extract_attribute_metadata(gltf_json, "POSITION", GLTF_FLOAT, "VEC3");
    if (not pos_meta_res.has_value()) return std::unexpected(pos_meta_res.error());

    const auto& pos_meta_opt = pos_meta_res.value();
    if (not pos_meta_opt.has_value()) return std::unexpected("エラー: 必須である POSITION が存在しません");
    const auto& pos_meta = pos_meta_opt.value();

    // TODO: ここに NORMAL を抽出する処理を足す
    auto norm_res = detail::extract_attribute_metadata(gltf_json, "NORMAL", GLTF_FLOAT, "VEC3");
    if (not norm_res.has_value()) return std::unexpected(norm_res.error());

    size_t norm_offset = 0;
    size_t norm_count = 0;
    const auto& norm_opt = norm_res.value();

    if (norm_opt.has_value()) {
        const auto& norm_meta = norm_opt.value();
        norm_offset = norm_meta.byte_offset;
        norm_count = norm_meta.byte_length / sizeof(float);
    }

    // TODO: ここに UV を抽出する処理を足す
    auto uv_res = detail::extract_attribute_metadata(gltf_json, "TEXCOORD_0", GLTF_FLOAT, "VEC2");
    if (not uv_res.has_value()) return std::unexpected(uv_res.error());

    size_t uv_offset = 0;
    size_t uv_count = 0;
    const auto& uv_opt = uv_res.value();

    if (uv_opt.has_value()) {
        const auto& uv_meta = uv_opt.value();
        uv_offset = uv_meta.byte_offset;
        uv_count = uv_meta.byte_length / sizeof(float);
    }

    // 3. バイナリのロード
    std::filesystem::path gltf_dir = std::filesystem::path(filepath).parent_path();
    std::filesystem::path bin_path = gltf_dir / pos_meta.uri;

    auto binary_result = kiln::core::read_binary_file(bin_path.string());
    if (not binary_result.has_value()) return std::unexpected(binary_result.error());

    std::vector<std::byte> binary_data = std::move(binary_result.value());
    if (binary_data.size() < pos_meta.byte_length) {
        return std::unexpected("エラー: バイナリファイルのサイズが足りません");
    }

    kiln::mesh::MeshData mesh_data{
        .raw_buffer = std::move(binary_data),
        .positions_byte_offset = pos_meta.byte_offset,
        .positions_float_count = pos_meta.byte_length / sizeof(float),
        .normals_byte_offset = norm_offset,
        .normals_float_count = norm_count,
        .uvs_byte_offset = uv_offset,
        .uvs_float_count = uv_count
    };

    std::cout << "[SUCCESS] POSITIONの抽出に成功しました\n";
    return mesh_data;
}

} // namespace kiln::format