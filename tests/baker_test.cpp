//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#include <cassert>
#include <iostream>

int main() {
    int expected = 42;
    int actual = 42;

    assert(expected == actual && "テスト失敗時のメッセージ");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}