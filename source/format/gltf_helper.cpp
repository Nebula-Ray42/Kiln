//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#include "format/gltf_helper.hpp"
#include <nlohmann/json.hpp>

namespace kiln::format::detail {

std::expected<std::optional<BufferMetadata>, std::string> extract_attribute_metadata(
    const nlohmann::json& gltf_json,
    std::string_view attribute_name,
    uint32_t expected_component_type,
    std::string_view expected_type) noexcept {

  const bool has_meshes = gltf_json.contains("meshes") and gltf_json["meshes"].is_array();
  if (not has_meshes || gltf_json["meshes"].empty()) return std::unexpected("エラー: meshesが不正です");

  const auto& first_mesh = gltf_json["meshes"][0];
  const bool has_primitives = first_mesh.contains("primitives") and first_mesh["primitives"].is_array();
  if (not has_primitives || first_mesh["primitives"].empty()) return std::unexpected("エラー: primitivesが不正です");

  const auto& first_primitive = first_mesh["primitives"][0];
  const bool has_attributes = first_primitive.contains("attributes") and first_primitive["attributes"].is_object();
  if (not has_attributes) return std::unexpected("エラー: attributesが不正です");

  const auto& attributes = first_primitive["attributes"];

  if (not attributes.contains(attribute_name)) {
      return std::nullopt;
  }

  const size_t accessor_idx = attributes[attribute_name];

  const auto& accessors = gltf_json["accessors"];
  if (accessor_idx >= accessors.size()) return std::unexpected("エラー: accessorのインデックスが範囲外です");

  const auto& accessor = accessors[accessor_idx];
  if (accessor["componentType"] != expected_component_type || accessor["type"] != expected_type) {
      return std::unexpected(std::string(attribute_name) + " の型が期待値と異なります");
  }

  const size_t buffer_view_idx = accessor["bufferView"];

  const auto& buffer_view = gltf_json["bufferViews"][buffer_view_idx];
  const size_t buffer_idx = buffer_view["buffer"];
  const auto& buffer = gltf_json["buffers"][buffer_idx];

  BufferMetadata meta;
  meta.byte_offset = buffer_view.contains("byteOffset") ? buffer_view["byteOffset"].get<size_t>() : 0;
  meta.byte_length = buffer["byteLength"];
  meta.uri = buffer["uri"];

  return meta;
}

} // namespace kiln::format::detail