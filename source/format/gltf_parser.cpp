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

  if (!gltf_json["meshes"].is_array() || gltf_json.at("meshes").empty()) {
    return std::unexpected("エラー: 'meshes' が存在しない、または正しい配列形式ではありません");
  }

  const auto& first_mesh = gltf_json.at("meshes").at(0);

  if (!first_mesh.contains("primitives") || !first_mesh.at("primitives").is_array()) {
    return std::unexpected("エラー: メッシュの中に 'primitives' (頂点構造) が見つかりません");
  }

  const auto& primitive = first_mesh.at("primitives").at(0);

  if (!primitive.contains("attributes")) {
    return std::unexpected("エラー: glTFファイル内に 'attributes'が見つかりません");
  }

  const auto& attributes = primitive.at("attributes");

  if (!attributes.contains("POSITION")) {
    return std::unexpected("エラー: 'attributes'の中に 'POSITION'が見つかりません");
  }

  const size_t position_accessor_index = attributes.at("POSITION");

  if (!gltf_json.contains("accessors") || !gltf_json.at("accessors").is_array()) {
    return std::unexpected("エラー: 'accessors' が存在しない、または正しい配列形式ではありません");
  }

  const auto& accessors = gltf_json.at("accessors");

  if (position_accessor_index >= accessors.size()) {
    return std::unexpected("エラー: accessors 配列の要素数（サイズ）が、position_accessor_index より大きいか同等です");
  }

  const auto& position_accessor = accessors.at(position_accessor_index);

  if (!position_accessor.contains("componentType")) {
    return std::unexpected("エラー: 'componentType'が存在しません");
  }

  const uint32_t component_type = position_accessor.at("componentType");

  if (!position_accessor.contains("type")) {
    return std::unexpected("エラー: 'type'が存在しません");
  }

  const std::string accessor_type = position_accessor.at("type");

  if (constexpr uint32_t GLTF_FLOAT = 5126;
      component_type != GLTF_FLOAT || accessor_type != "VEC3") {
    return std::unexpected("エラー: POSITIONデータはFLOAT型のVEC3である必要があります");
  }

  if (!position_accessor.contains("bufferView")) {
    return std::unexpected("エラー: 'bufferView'が存在しません");
  }

  const size_t buffer_view_index = position_accessor.at("bufferView");
  const auto& buffer_views_array = gltf_json.at("bufferViews");

  if (!gltf_json.contains("bufferViews") || !buffer_views_array.is_array()) {
    return std::unexpected("エラー: 'bufferViews' が存在しない、または正しい配列形式ではありません");
  }

  if (buffer_view_index >= buffer_views_array.size()) {
    return std::unexpected("エラー: buffer_view_index が bufferViews 配列の範囲外です");
  }

  [[maybe_unused]] const auto& buffer_view = buffer_views_array.at(buffer_view_index);

  [[maybe_unused]] size_t size1 = get_component_size<float>();

  return mesh_data;
}

} // namespace kiln::format