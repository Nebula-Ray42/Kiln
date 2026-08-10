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
  if (!gltf_json.contains("meshes")) return std::unexpected("エラー: glTFファイル内に 'meshes' が見つかりません");
  if (!gltf_json["meshes"].is_array() || gltf_json["meshes"].empty()) { return std::unexpected("エラー: 'meshes' が存在しないか不正です"); }

  const auto& first_mesh = gltf_json["meshes"][0]; // NOLINT
  if (!first_mesh.contains("primitives") || !first_mesh["primitives"].is_array()) { return std::unexpected("エラー: 'primitives' が見つかりません"); }

  const auto& primitive = first_mesh["primitives"][0]; // NOLINT
  if (!primitive.contains("attributes")) { return std::unexpected("エラー: 'attributes'が見つかりません"); }

  const auto& attributes = primitive["attributes"]; // NOLINT
  if (!attributes.contains("POSITION")) { return std::unexpected("エラー: 'POSITION'が見つかりません"); }

  return attributes["POSITION"]; // NOLINT
}

// 2. Accessor を検証し、BufferView ID を抽出する関数
std::expected<size_t, std::string> get_buffer_view_index(const nlohmann::json& gltf_json, size_t accessor_index) noexcept {
  if (!gltf_json.contains("accessors") || !gltf_json["accessors"].is_array()) { return std::unexpected("エラー: 'accessors' が不正です"); }

  const auto& accessors = gltf_json["accessors"]; // NOLINT
  if (accessor_index >= accessors.size()) { return std::unexpected("エラー: accessors 配列の範囲外です"); }

  const auto& position_accessor = accessors[accessor_index]; // NOLINT
  if (!position_accessor.contains("componentType") || !position_accessor.contains("type")) {
    return std::unexpected("エラー: 型情報(componentType/type)が存在しません");
  }

  const uint32_t component_type = position_accessor["componentType"]; // NOLINT
  const std::string accessor_type = position_accessor["type"]; // NOLINT

  constexpr uint32_t GLTF_FLOAT = 5126;
  if (component_type != GLTF_FLOAT || accessor_type != "VEC3") {
    return std::unexpected("エラー: POSITIONデータはFLOAT型のVEC3である必要があります");
  }

  if (!position_accessor.contains("bufferView")) { return std::unexpected("エラー: 'bufferView'が存在しません"); }

  return position_accessor["bufferView"]; // NOLINT
}

// 3. BufferView から Buffer ID を抽出する関数
std::expected<size_t, std::string> get_buffer_index(const nlohmann::json& gltf_json, size_t buffer_view_index) noexcept {
  if (!gltf_json.contains("bufferViews") || !gltf_json["bufferViews"].is_array()) { return std::unexpected("エラー: 'bufferViews' が不正です"); }

  const auto& buffer_views = gltf_json["bufferViews"]; // NOLINT
  if (buffer_view_index >= buffer_views.size()) { return std::unexpected("エラー: bufferViews 配列の範囲外です"); }

  const auto& buffer_view = buffer_views[buffer_view_index]; // NOLINT
  if (!buffer_view.contains("buffer")) { return std::unexpected("エラー: 'buffer' が存在しません"); }

  return buffer_view["buffer"]; // NOLINT
}

} // namespace

// --- メインのパース関数 ---
std::expected<kiln::mesh::MeshData, std::string> parse_gltf(std::string_view filepath) noexcept {
  std::ifstream file{std::string(filepath), std::ios::in | std::ios::binary};
  if (!file.is_open()) return std::unexpected("エラー: ファイルを開けませんでした -> " + std::string(filepath));

  nlohmann::json gltf_json;
  try {
    gltf_json = nlohmann::json::parse(file);
  } catch (const nlohmann::json::parse_error& e) {
    return std::unexpected("エラー: JSONの形式が不正です -> " + std::string(e.what()));
  }


  // 1. Accessor ID の取得
  auto accessor_idx_res = get_position_accessor_index(gltf_json);
  if (!accessor_idx_res) { return std::unexpected(accessor_idx_res.error()); }

  // 2. BufferView ID の取得
  auto buffer_view_idx_res = get_buffer_view_index(gltf_json, *accessor_idx_res);
  if (!buffer_view_idx_res) { return std::unexpected(buffer_view_idx_res.error()); }

  // 3. Buffer ID の取得
  auto buffer_idx_res = get_buffer_index(gltf_json, *buffer_view_idx_res);
  if (!buffer_idx_res) { return std::unexpected(buffer_idx_res.error()); }

  // --- 最終データの抽出 ---
  if (!gltf_json.contains("buffers") || !gltf_json["buffers"].is_array()) {
    return std::unexpected("エラー: 'buffers' が不正です");
  }
  const auto& buffers = gltf_json["buffers"]; // NOLINT
  if (*buffer_idx_res >= buffers.size()) { return std::unexpected("エラー: buffer 配列の範囲外です"); }

  [[maybe_unused]] const auto& buffer = buffers[*buffer_idx_res]; // NOLINT

  if (!buffer.contains("uri") || !buffer.contains("byteLength")) {
    return std::unexpected("エラー: buffer に 'uri' または 'byteLength' が存在しません");
  }

  const std::string uri = buffer["uri"]; // NOLINT
  const size_t byte_length = buffer["byteLength"]; // NOLINT

  auto binary_result = kiln::core::read_binary_file(uri);
  if (!binary_result) {
    return std::unexpected(binary_result.error());
  }

  std::vector<std::byte> binary_data = std::move(binary_result.value());

  if (binary_data.size() < byte_length) {
    return std::unexpected("エラー: バイナリファイルのサイズがJSONの 'byteLength' 指定より小さいです");
  }

  [[maybe_unused]] size_t float_size = get_component_size<float>();

  kiln::mesh::MeshData mesh_data;
  return mesh_data;
}

} // namespace kiln::format