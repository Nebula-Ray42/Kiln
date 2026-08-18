# Kiln

> A modern, data-oriented 3D asset processing engine built with C++23.

---

## **Work in Progress & Disclaimer**

**Current Status: Experimental / Active Development**
This project is currently in the early stages of development. The architecture, APIs, and feature sets are highly volatile and subject to breaking changes without notice. It is **not** yet ready for production use.

---

## Overview
Kiln is a high-performance backend system designed for 3D asset parsing and processing (e.g., glTF). It emphasizes **Data-Oriented Design (DOD)** for memory efficiency and cache locality, combined with a **Functional Programming** approach using C++23 features like `std::expected` for robust, exceptionless error handling.

---

## Features
- **Modern C++23:** Leveraging the latest language features for safe and expressive code.
- **Data-Oriented Pipeline:** Strict separation of data and logic to maximize throughput.
- **glTF 2.0 Parsing:** Robust extraction and validation of meshes and nodes.
- **Seamless Build System:** Fully integrated with CMake Presets and `vcpkg` for cross-platform dependency management.

---

## Tech Stack
- **Language:** C++23
- **Build System:** CMake, Ninja
- **Package Manager:** vcpkg
- **Dependencies:** `nlohmann-json`, `flatbuffers`, `cgltf`, `CLI11`, `fmt`

---

## Prerequisites
- C++23 compatible compiler (e.g., Apple Clang 15+, GCC 13+)
- [CMake](https://cmake.org/) (v3.25 or newer)
- [vcpkg](https://github.com/microsoft/vcpkg)

---

## Build Instructions

1. **Build the project** (Automatically configures CMake, generates FlatBuffers schemas, and compiles the project):
```bash
./tool/build.sh
```

2. **Run the application**
```bash
./cmake-build-debug/bin/kiln_main
```

---

## Project Structure

* `include/` : Public headers for the core library.
* `source/` : Implementation of the core library (`baker_core`).
* `standalone/` : Main entry point for the CLI application (`kiln_main`).
* `schema/` : Data serialization schemas (FlatBuffers).

---

## License

[BSD-2-Clause Plus Patent License](LICENSE)
