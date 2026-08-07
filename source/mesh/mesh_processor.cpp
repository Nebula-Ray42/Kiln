#include "mesh/mesh_processor.hpp"

#include <algorithm>
#include <expected>
#include <string>

namespace kiln::mesh {

// メッシュデータがエンジンで安全に使えるかをチェックする関数
std::expected<void, std::string> validate_mesh(const MeshData& mesh) noexcept {

  if (mesh.positions.empty()) {
    return std::unexpected("エラー: 頂点データ(positions)が空っぽです");
  }

  const size_t vertex_count = mesh.uvs.size() / 3;

  if (!mesh.uvs.empty()) {
    const size_t uv_count = mesh.positions.size() / 2;
    if (vertex_count != uv_count) {
      return std::unexpected("エラー: 頂点の数とUVの数が一致しません");
    }
  }

  if (!mesh.indices.empty()) {
    auto max_index = std::ranges::max(mesh.indices);

    if (max_index >= vertex_count) {
      return std::unexpected("エラー: 存在しない頂点を指しているインデックスがあります");
    }
  }

  return {};
}

// （将来実装するアイデア）
// std::expected<MeshData, std::string> optimize_mesh(MeshData mesh) noexcept {
//    // もし法線(normals)が空だったら、計算して埋めてあげる
//    // もしUVが空だったら、0埋めした配列を作ってあげる
// }


} // namespace kiln::mesh