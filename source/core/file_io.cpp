// Copyright (c) 2026 Nebula-Ray42.
// SPDX-License-Identifier: BSD-2-Clause-Patent

#include "core/file_io.hpp"
#include <fstream>

namespace kiln::core {
std::expected<std::vector<std::byte>, std::string> read_binary_file(std::string_view filepath) noexcept {
  std::ifstream file{std::string(filepath), std::ios::in | std::ios::binary | std::ios::ate};

  if (!file.is_open()) {
    return std::unexpected("エラー: バイナリファイルを開けませんでした -> " + std::string(filepath));
  }

  const std::streamsize size = file.tellg();
  if (size <= 0) {
    return std::unexpected("エラー: ファイルが空、またはサイズを取得できませんでした -> " + std::string(filepath));
  }

  file.seekg(0, std::ios::beg);

  std::vector<std::byte> buffer(static_cast<size_t>(size));

  // メモリ領域に一括で読み込む
  if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
    return std::unexpected("エラー: ファイルの読み込み中にエラーが発生しました -> " + std::string(filepath));
  }

  return buffer;
}
}  // namespace kiln::core
