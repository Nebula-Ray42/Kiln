#!/usr/bin/env bash
set -e

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR="${ROOT_DIR}/cmake-build-debug"

echo "テストを実行します..."
cd "$BUILD_DIR"
ctest --output-on-failure