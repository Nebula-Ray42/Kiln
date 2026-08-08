#include "format/gltf_parser.hpp"
#include <string>
#include <expected>
#include <fstream>
#include <nlohmann/json.hpp>

namespace kiln::format {

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

  if (!gltf_json["meshes"].is_array() || gltf_json["meshes"].empty()) {
    return std::unexpected("エラー: 'meshes' が存在しない、または正しい配列形式ではありません");
  }

  const auto& first_mesh = gltf_json["meshes"][0];

  if (!first_mesh.contains("primitives") || !first_mesh["primitives"].is_array()) {
    return std::unexpected("エラー: メッシュの中に 'primitives' (頂点構造) が見つかりません");
  }

  const auto& primitive = first_mesh["primitives"][0];

  if (!primitive.contains("attributes")) {
    return std::unexpected("エラー: glTFファイル内に 'attributes'が見つかりません");
  }

  const auto& attributes = primitive["attributes"];

  if (!attributes.contains("POSITION")) {
    return std::unexpected("エラー: 'attributes'の中に 'POSITION'が見つかりません");
  }

  const size_t position_accessor_index = attributes["POSITION"];

  if (!gltf_json.contains("accessors") || !gltf_json["accessors"].is_array()) {
    return std::unexpected("エラー: 'accessors' が存在しない、または正しい配列形式ではありません");
  }

  const auto& accessors = gltf_json["accessors"];

  if (position_accessor_index >= accessors.size()) {
    return std::unexpected("エラー: accessors 配列の要素数（サイズ）が、position_accessor_index より大きいです");
  }

  [[maybe_unused]] const auto& position_accessor = accessors[position_accessor_index];

  return mesh_data;
}

} // namespace kiln::format