//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#pragma once
#include <expected>
#include <string>

#include "mesh_types.h"

namespace kiln::mesh {

[[nodiscard]] std::expected<void, std::string> validate_mesh(const MeshData& mesh) noexcept;

[[nodiscard]] std::expected<MeshData, std::string> optimize_mesh(MeshData mesh) noexcept;

}