#include "format/gltf_parser.hpp"
#include <string>
#include <expected>
#include <fstream>
#include <nlohmann/json.hpp>
#include <concepts>

namespace kiln::format {

template <typename T>
concept GltfComponent = std::same_as<T, float> || std::same_as<T, uint16_t> || std::same_as<T, uint32_t> || std::same_as<T, uint8_t>;

template <typename T>
requires GltfComponent<T>
size_t get_component_size() {
  return sizeof(T);
}

std::expected<kiln::mesh::MeshData, std::string> parse_gltf(std::string_view filepath) noexcept {

  std::ifstream file{std::string(filepath), std::ios::in | std::ios::binary};
  if (!file.is_open()) {
    return std::unexpected("エラー: ファイルを開けませんでした -> " + std::string(filepath));
  }

  nlohmann::json gltf_json;

  try {
    gltf_json = nlohmann::json::parse(file);
  }
  catch (const nlohmann::json::parse_error& e) {
    return std::unexpected("エラー: JSONの形式が不正です -> " + std::string(e.what()));
  }

  kiln::mesh::MeshData mesh_data;

  if (!gltf_json.contains("meshes")) {
    return std::unexpected("エラー: glTFファイル内に 'meshes' (メッシュデータ) が見つかりません");
  }

  if (!gltf_json["meshes"].is_array() || gltf_json["meshes"].empty()) { // NOLINT: Existence checked by contains() above
    return std::unexpected("エラー: 'meshes' が存在しない、または正しい配列形式ではありません");
  }

  const auto& first_mesh = gltf_json["meshes"][0]; // NOLINT: Bounds checked by empty() above

  if (!first_mesh.contains("primitives") || !first_mesh["primitives"].is_array()) { // NOLINT: Existence checked by contains()
    return std::unexpected("エラー: メッシュの中に 'primitives' (頂点構造) が見つかりません");
  }

  const auto& primitive = first_mesh["primitives"][0]; // NOLINT: Existence of array verified above

  if (!primitive.contains("attributes")) {
    return std::unexpected("エラー: glTFファイル内に 'attributes'が見つかりません");
  }

  const auto& attributes = primitive["attributes"]; // NOLINT: Existence checked by contains() above

  if (!attributes.contains("POSITION")) {
    return std::unexpected("エラー: 'attributes'の中に 'POSITION'が見つかりません");
  }

  const size_t position_accessor_index = attributes["POSITION"]; // NOLINT: Existence checked by contains() above

  if (!gltf_json.contains("accessors") || !gltf_json["accessors"].is_array()) { // NOLINT: Safe access with contains() check
    return std::unexpected("エラー: 'accessors' が存在しない、または正しい配列形式ではありません");
  }

  const auto& accessors = gltf_json["accessors"]; // NOLINT: Existence checked by contains() above

  if (position_accessor_index >= accessors.size()) {
    return std::unexpected("エラー: accessors 配列の要素数（サイズ）が、position_accessor_index より大きいか同等です");
  }

  const auto& position_accessor = accessors[position_accessor_index]; // NOLINT: Bounds strictly checked against size() above

  if (!position_accessor.contains("componentType")) {
    return std::unexpected("エラー: 'componentType'が存在しません");
  }

  const uint32_t component_type = position_accessor["componentType"]; // NOLINT: Existence checked by contains() above

  if (!position_accessor.contains("type")) {
    return std::unexpected("エラー: 'type'が存在しません");
  }

  const std::string accessor_type = position_accessor["type"]; // NOLINT: Existence checked by contains() above

  if (constexpr uint32_t GLTF_FLOAT = 5126;
      component_type != GLTF_FLOAT || accessor_type != "VEC3") {
    return std::unexpected("エラー: POSITIONデータはFLOAT型のVEC3である必要があります");
  }

  if (!position_accessor.contains("bufferView")) {
    return std::unexpected("エラー: 'bufferView'が存在しません");
  }

  const size_t buffer_view_index = position_accessor["bufferView"]; // NOLINT: Existence checked by contains() above

  if (!gltf_json.contains("bufferViews") || !gltf_json["bufferViews"].is_array()) { // NOLINT: Safe access with contains() check
    return std::unexpected("エラー: 'bufferViews' が存在しない、または正しい配列形式ではありません");
  }

  const auto& buffer_views_array = gltf_json["bufferViews"]; // NOLINT: Existence checked by contains() above

  if (buffer_view_index >= buffer_views_array.size()) {
    return std::unexpected("エラー: buffer_view_index が bufferViews 配列の範囲外です");
  }

  [[maybe_unused]] const auto& buffer_view = buffer_views_array[buffer_view_index]; // NOLINT: Bounds strictly checked against size() above

  [[maybe_unused]] size_t size1 = get_component_size<float>();

  return mesh_data;
}

} // namespace kiln::format