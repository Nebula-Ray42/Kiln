#!/usr/bin/env bash
set -e

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
DATA_DIR="${ROOT_DIR}/data"

echo "=== [Kiln] Blender アセットベイクを開始します ==="

mkdir -p "$DATA_DIR"

if command -v blender &> /dev/null; then
    BLENDER_CMD="blender"
elif [ -f "/Applications/Blender.app/Contents/MacOS/Blender" ]; then
    BLENDER_CMD="/Applications/Blender.app/Contents/MacOS/Blender"
else
    echo "エラー: Blenderの実行ファイルが見つかりません。"
    echo "Blenderが /Applications にインストールされているか確認してください。"
    exit 1
fi

"$BLENDER_CMD" --background --python "${ROOT_DIR}/tool/baker.py" -- "${DATA_DIR}/test_output.bin"

echo "=== [Kiln] ベイク完了 ==="