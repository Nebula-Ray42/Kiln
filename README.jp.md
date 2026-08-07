# Kiln (日本語)

> C++23で構築された、データ指向のモダンな3Dアセット処理エンジン

---

## 概要

Kilnは、glTFなどの3Dアセットをパース・処理するためのハイパフォーマンスなバックエンドシステムです。メモリ効率とキャッシュ局所性を最大化する**データ指向設計 (DOD)** を中核に据え、C++23の `std::expected` 等を用いた**関数型プログラミング**のアプローチにより、例外に依存しない堅牢なエラーハンドリングを実現しています。

---

## 主な機能

* **モダン C++23:** 安全で表現力豊かな最新の言語機能をフル活用。
* **データ指向パイプライン:** データとロジックを厳格に分離し、スループットを最適化。
* **glTF 2.0 パース:** メッシュやノードの堅牢な抽出とバリデーション。
* **モダンなビルド環境:** CMake Presets と `vcpkg` の統合による、スムーズな依存関係管理。

---

## 技術スタック

* **言語:** C++23
* **ビルドシステム:** CMake, Ninja
* **パッケージ管理:** vcpkg
* **依存ライブラリ:** `nlohmann-json`, `flatbuffers`, `cgltf`, `CLI11`, `fmt`

---

## 開発環境の要件

* C++23 対応コンパイラ (Apple Clang 15+, GCC 13+ など)
* [CMake](https://cmake.org/) (v3.25 以降)
* [vcpkg](https://github.com/microsoft/vcpkg)

## ビルド手順

1. 提供されている CMake プリセットを使用して**プロジェクトを構成**します:
```bash
cmake --preset kiln
```


2. **ビルドを実行**します:
```bash
cmake --build cmake-build-debug --target kiln_main -j 6
```

3. **アプリケーションを実行**します:
```bash
./cmake-build-debug/standalone/kiln_main
```
---

## プロジェクト構成

* `include/` : コアライブラリの公開ヘッダ
* `source/` : コアライブラリの実装 (`baker_core`)
* `standalone/` : CLIアプリケーションのエントリーポイント (`kiln_main`)
* `schema/` : データシリアライゼーション用のスキーマ (FlatBuffers)

---

## ライセンス

[Apache License](LICENSE)