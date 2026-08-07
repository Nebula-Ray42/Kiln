#include <iostream>
#include <fstream>
#include <format/gltf_parser.hpp>

int main() {
  // 1. テスト用のダミーglTF(JSON)ファイルを作る
  // （R"(...)" は、改行やダブルクォーテーションをそのまま書けるC++の便利な機能です）
  const std::string test_filepath = "dummy_mesh.gltf";
  std::ofstream out_file(test_filepath);
  out_file << R"({
        "meshes": [
            {
                "primitives": [
                    { "attributes": { "POSITION": 0 } }
                ]
            }
        ]
    })";
  out_file.close();

  // 2. 作ったダミーファイルを、自作のパーサーに読ませる
  std::cout << "--- パース開始 ---\n";
  auto result = kiln::format::parse_gltf(test_filepath);

  // 3. 結果の確認（C++23 std::expected の使い方）
  if (result.has_value()) {
    std::cout << "✅ 成功: JSONのパースとデータ抽出が完了しました！\n";
  } else {
    // エラーだった場合は、中のエラーメッセージ（std::string）を取り出して表示
    std::cout << "❌ 失敗: " << result.error() << "\n";
  }

  return 0;
}