find_package(nlohmann_json CONFIG REQUIRED)

add_library(kiln_core STATIC
        baker_core.cpp
        ../include/mesh/mesh_types.hpp
        ../include/format/gltf_parser.hpp
        mesh/mesh_processor.cpp
        ../include/mesh/mesh_processor.hpp
        format/gltf_parser.cpp
)


target_include_directories(kiln_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(kiln_core PRIVATE nlohmann_json::nlohmann_json)