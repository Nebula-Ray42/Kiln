//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#pragma once
#include "mesh/mesh_types.hpp"

#include <expected>
#include <filesystem>

namespace kiln::format {


[[nodiscard]] std::expected<kiln::mesh::MeshData, std::string> parse_gltf(std::string_view filepath) noexcept;
}  // namespace kiln::format