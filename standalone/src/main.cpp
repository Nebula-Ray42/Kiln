//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#include <iostream>

#include "format/gltf_parser.hpp"

int main() {
    auto result = kiln::format::parse_gltf("/Users/sakakibarayuto/Kiln/tests/assets/Box.gltf");

    if (result.has_value()) {
        std::cout << "パース完了\n";
    } else {
        std::cerr << "失敗: " << result.error() << "\n";
    }
    return 0;
}