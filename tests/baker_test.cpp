//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#include <iostream>
#include <fstream>
#include <vector>

// 自動生成された設計図を読み込む
#include "mesh_generated.h"

int main() {
    std::cout << "=== [Kiln] ゼロコピー読み込みテスト開始 ===" << std::endl;

    std::ifstream infile("../../test_output.bin", std::ios::binary);
    if (!infile) {
        std::cerr << "エラー: test_output.bin が見つかりません。" << std::endl;
        return 1;
    }

    infile.seekg(0, std::ios::end);
    size_t length = static_cast<size_t>(infile.tellg());
    infile.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(length);

    infile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(length));
    infile.close();

    auto mesh = kiln::schema::GetMesh(buffer.data());
    auto vertices = mesh->vertices();

    if (vertices) {
        std::cout << "[*] 読み込み成功！頂点数: " << vertices->size() << std::endl;

        for (uint32_t i = 0; i < vertices->size(); ++i) {
            auto v = vertices->Get(i);

            // 実際の設計図に合わせて px(), py(), pz() を呼び出す
            std::cout << "    頂点 " << i << ": ("
                      << v->px() << ", "
                      << v->py() << ", "
                      << v->pz() << ")" << std::endl;
        }
    } else {
        std::cout << "[!] 頂点データが空です。" << std::endl;
    }

    std::cout << "=== [Kiln] テスト完了 ===" << std::endl;
    return 0;
}