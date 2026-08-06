cmake_minimum_required(VERSION 3.28)
project(AssetBaker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)

add_subdirectory(source)
add_subdirectory(standalone)

enable_testing()
add_subdirectory(tests)

set_project_warnings(baker_core)
set_project_warnings(asset_baker)
set_project_warnings(baker_tests)