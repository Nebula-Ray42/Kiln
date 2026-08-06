#pragma once
#include "mesh/mesh_types.hpp"

#include <expected>
#include <filesystem>

namespace kiln::format {
[[nodiscard]] auto parse_gltf(const std::filesystem::path& file_path)
    -> std::expected<mesh::Mesh, std::string>;
}