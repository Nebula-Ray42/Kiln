// Copyright (c) 2026 Nebula-Ray42.
// SPDX-License-Identifier: BSD-2-Clause-Patent

#include "format/gltf_parser.hpp"
#include "core/file_io.hpp"
#include <string>
#include <expected>
#include <fstream>
#include <nlohmann/json.hpp>
#include <concepts>
#include <iostream>

namespace kiln::format {

// --- コンセプトと型制約 ---
template <typename T>
concept GltfComponent = std::same_as<T, float> || std::same_as<T, uint16_t> ||
                        std::same_as<T, uint32_t> || std::same_as<T, uint8_t>;

template <typename T>
requires GltfComponent<T>
size_t get_component_size() {
  return sizeof(T);
}

namespace {

// 1. POSITION の Accessor ID を抽出する関数
std::expected<size_t, std::string> get_position_accessor_index(const nlohmann::json& gltf_json) noexcept {
  const bool has_meshes = gltf_json.contains("meshes");
  if (!has_meshes) return std::unexpected("エラー: glTFファイル内に 'meshes' が見つかりません");

  const bool meshes_is_array = gltf_json["meshes"].is_array();
  const bool meshes_not_empty = meshes_is_array && !gltf_json["meshes"].empty();
  if (!meshes_not_empty) { return std::unexpected("エラー: 'meshes' が存在しないか不正です"); }

  const auto& first_mesh = gltf_json["meshes"][0]; // NOLINT

  const bool has_primitives = first_mesh.contains("primitives");
  const bool primitives_is_array = has_primitives && first_mesh["primitives"].is_array();
  if (!primitives_is_array) { return std::unexpected("エラー: 'primitives' が見つかりません"); }

  const auto& primitive = first_mesh["primitives"][0]; // NOLINT

  const bool has_attributes = primitive.contains("attributes");
  if (!has_attributes) { return std::unexpected("エラー: 'attributes'が見つかりません"); }

  const auto& attributes = primitive["attributes"]; // NOLINT

  const bool has_position = attributes.contains("POSITION");
  if (!has_position) { return std::unexpected("エラー: 'POSITION'が見つかりません"); }

  return attributes["POSITION"]; // NOLINT
}

// 2. Accessor を検証し、BufferView ID を抽出する関数
std::expected<size_t, std::string> get_buffer_view_index(const nlohmann::json& gltf_json, size_t accessor_index) noexcept {
  const bool has_accessors = gltf_json.contains("accessors");
  const bool accessors_is_array = has_accessors && gltf_json["accessors"].is_array();
  if (!accessors_is_array) { return std::unexpected("エラー: 'accessors' が不正です"); }

  const auto& accessors = gltf_json["accessors"]; // NOLINT

  const bool accessor_in_range = accessor_index < accessors.size();
  if (!accessor_in_range) { return std::unexpected("エラー: accessors 配列の範囲外です"); }

  const auto& position_accessor = accessors[accessor_index]; // NOLINT

  const bool has_component_and_type = position_accessor.contains("componentType") && position_accessor.contains("type");
  if (!has_component_and_type) {
    return std::unexpected("エラー: 型情報(componentType/type)が存在しません");
  }

  const uint32_t component_type = position_accessor["componentType"]; // NOLINT
  const std::string accessor_type = position_accessor["type"]; // NOLINT

  constexpr uint32_t GLTF_FLOAT = 5126;
  const bool is_float_vec3 = (component_type == GLTF_FLOAT) && (accessor_type == "VEC3");
  if (!is_float_vec3) {
    return std::unexpected("エラー: POSITIONデータはFLOAT型のVEC3である必要があります");
  }

  const bool has_buffer_view = position_accessor.contains("bufferView");
  if (!has_buffer_view) { return std::unexpected("エラー: 'bufferView'が存在しません"); }

  return position_accessor["bufferView"]; // NOLINT
}

// 3. BufferView から Buffer ID を抽出する関数
std::expected<size_t, std::string> get_buffer_index(const nlohmann::json& gltf_json, size_t buffer_view_index) noexcept {
  const bool has_buffer_views = gltf_json.contains("bufferViews");
  const bool buffer_views_is_array = has_buffer_views && gltf_json["bufferViews"].is_array();
  if (!buffer_views_is_array) { return std::unexpected("エラー: 'bufferViews' が不正です"); }

  const auto& buffer_views = gltf_json["bufferViews"]; // NOLINT

  const bool buffer_view_in_range = buffer_view_index < buffer_views.size();
  if (!buffer_view_in_range) { return std::unexpected("エラー: bufferViews 配列の範囲外です"); }

  const auto& buffer_view = buffer_views[buffer_view_index]; // NOLINT

  const bool has_buffer = buffer_view.contains("buffer");
  if (!has_buffer) { return std::unexpected("エラー: 'buffer' が存在しません"); }

  return buffer_view["buffer"]; // NOLINT
}

} // namespace

// --- メインのパース関数 ---
std::expected<kiln::mesh::MeshData, std::string> parse_gltf(std::string_view filepath) noexcept {
  std::ifstream file{std::string(filepath), std::ios::in | std::ios::binary};
  const bool file_open = file.is_open();
  if (!file_open) return std::unexpected("エラー: ファイルを開けませんでした -> " + std::string(filepath));

  nlohmann::json gltf_json;
  try {
    gltf_json = nlohmann::json::parse(file);
  } catch (const nlohmann::json::parse_error& e) {
    return std::unexpected("エラー: JSONの形式が不正です -> " + std::string(e.what()));
  }

  // 1. Accessor ID の取得
  auto accessor_idx_res = get_position_accessor_index(gltf_json);
  const bool accessor_ok = static_cast<bool>(accessor_idx_res);
  if (!accessor_ok) { return std::unexpected(accessor_idx_res.error()); }

  // 2. BufferView ID の取得
  auto buffer_view_idx_res = get_buffer_view_index(gltf_json, *accessor_idx_res);
  const bool buffer_view_ok = static_cast<bool>(buffer_view_idx_res);
  if (!buffer_view_ok) { return std::unexpected(buffer_view_idx_res.error()); }

  // 3. Buffer ID の取得
  auto buffer_idx_res = get_buffer_index(gltf_json, *buffer_view_idx_res);
  const bool buffer_idx_ok = static_cast<bool>(buffer_idx_res);
  if (!buffer_idx_ok) { return std::unexpected(buffer_idx_res.error()); }

  // --- 最終データの抽出 ---
  const bool has_buffers = gltf_json.contains("buffers");
  const bool buffers_is_array = has_buffers && gltf_json["buffers"].is_array();
  if (!buffers_is_array) {
    return std::unexpected("エラー: 'buffers' が不正です");
  }

  const auto& buffers = gltf_json["buffers"]; // NOLINT

  const bool buffer_index_in_range = *buffer_idx_res < buffers.size();
  if (!buffer_index_in_range) { return std::unexpected("エラー: buffer 配列の範囲外です"); }

  [[maybe_unused]] const auto& buffer = buffers[*buffer_idx_res]; // NOLINT

  const bool buffer_has_uri = buffer.contains("uri");
  const bool buffer_has_byteLength = buffer.contains("byteLength");
  if (!(buffer_has_uri && buffer_has_byteLength)) {
    return std::unexpected("エラー: buffer に 'uri' または 'byteLength' が存在しません");
  }

  const std::string uri = buffer["uri"]; // NOLINT
  const size_t byte_length = buffer["byteLength"]; // NOLINT

  auto binary_result = kiln::core::read_binary_file(uri);
  const bool binary_ok = static_cast<bool>(binary_result);
  if (!binary_ok) {
    return std::unexpected(binary_result.error());
  }

  std::vector<std::byte> binary_data = std::move(binary_result.value());

  const bool binary_size_ok = binary_data.size() >= byte_length;
  if (!binary_size_ok) {
    return std::unexpected("エラー: バイナリファイルのサイズがJSONの 'byteLength' 指定より小さいです");
  }

  [[maybe_unused]] size_t float_size = get_component_size<float>();

  kiln::mesh::MeshData mesh_data;
  return mesh_data;
}

} // namespace kiln::format