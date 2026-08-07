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
    return std::unexpected("エラー: 'meshes' が空っぽ、または正しい配列形式ではありません");
  }

  const auto& first_mesh = gltf_json["meshes"][0];

  if (!first_mesh.contains("primitives") || !first_mesh["primitives"].is_array()) {
    return std::unexpected("エラー: メッシュの中に 'primitives' (頂点構造) が見つかりません");
  }

  [[maybe_unused]] const auto& primitive = first_mesh["primitives"][0];

  return mesh_data;

}

} // namespace kiln::format