#!/usr/bin/env bash
set -e

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR="${ROOT_DIR}/cmake-build-debug"

echo "=== [Kiln] ビルドプロセスを開始します ==="

if [ -z "$VCPKG_ROOT" ]; then
    export VCPKG_ROOT="$HOME/vcpkg"
    echo "[*] VCPKG_ROOTを ${VCPKG_ROOT} に設定しました"
fi

cd "$ROOT_DIR"

if [ ! -d "$BUILD_DIR" ]; then
    echo "[*] CMake構成を初期化中..."
    cmake --preset kiln
fi

echo "[*] FlatBuffersスキーマのコード生成を実行中..."
cmake --build "$BUILD_DIR" --target kiln_schema_gen

echo "[*] メインプロジェクトのビルドを実行中..."
cmake --build "$BUILD_DIR" --parallel

echo "=== [Kiln] ビルド完了 ==="