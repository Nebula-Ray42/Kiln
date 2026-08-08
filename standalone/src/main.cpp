#include <iostream>
#include <fstream>
#include <format/gltf_parser.hpp>

int main() {
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

  std::cout << "--- パース開始 ---\n";
  auto result = kiln::format::parse_gltf(test_filepath);


  if (result.has_value()) {
    std::cout << "成功: JSONのパースとデータ抽出が完了しました！\n";
  } else {
    std::cout << "失敗: " << result.error() << "\n";
  }

  return 0;
}