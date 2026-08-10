#pragma once

#include <cstddef>
#include <expected>
#include <string>
#include <string_view>
#include <vector>

namespace kiln::core {

/**
 * @brief バイナリファイルを一括でメモリに読み込む関数
 *
 * @param filepath 読み込むファイルのパス
 * @return 成功時はバイト配列（std::vector<std::byte>）、失敗時はエラー文字列
 */
std::expected<std::vector<std::byte>, std::string> read_binary_file(std::string_view filepath) noexcept;

} // namespace kiln::core