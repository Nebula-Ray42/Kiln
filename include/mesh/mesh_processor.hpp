//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

#pragma once
#include "mesh_types.hpp"

#include <expected>

namespace kiln::mesh {


// 副作用のない純粋なバリデーション関数
[[nodiscard]] std::expected<void, std::string> validate_mesh(const MeshData& mesh) noexcept;

[[nodiscard]] std::expected<MeshData, std::string> optimize_mesh(MeshData mesh) noexcept;

}