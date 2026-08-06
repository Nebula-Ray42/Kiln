#!/usr/bin/env bash
set -e

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR="${ROOT_DIR}/cmake-build-debug"

echo "kilnを起動します"

if [ ! -d "$BUILD_DIR" ]; then
    echo "CMake構成を初期化中..."
    cmake --preset kiln
fi

echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.zshrc
source ~/.zshrc

echo "ビルドを実行中..."
cmake --build "$BUILD_DIR" --parallel

echo "ビルド完了"