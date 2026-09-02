//------------------------------------------------//
// Copyright (c) 2026 Nebula-Ray42.               //
// SPDX-License-Identifier: BSD-2-Clause-Patent   //
//------------------------------------------------//

// source/format/gltf_helper.hpp
#pragma once

#include <nlohmann/json_fwd.hpp>
#include <expected>
#include <optional>
#include <string>
#include <string_view>

namespace kiln::format::detail {

struct BufferMetadata {
    size_t byte_offset = 0;
    size_t byte_length = 0;
    std::string uri;
    uint32_t component_type = 0;
};

std::expected<std::optional<BufferMetadata>, std::string> extract_attribute_metadata(
    const nlohmann::json& gltf_json,
    std::string_view attribute_name,
    uint32_t expected_component_type,
    std::string_view expected_type
) noexcept;

std::expected<std::optional<BufferMetadata>, std::string> extract_index_metadata(
    const nlohmann::json& gltf_json) noexcept;

} // namespace kiln::format::detail
