add_library(kiln_core STATIC
        baker_core.cpp
        ../include/mesh/mesh_types.hpp
        ../include/format/gltf_parser.hpp
)

target_include_directories(kiln_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)