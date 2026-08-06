find_package(flatbuffers CONFIG REQUIRED)

set(SCHEMA_FILE "${CMAKE_CURRENT_SOURCE_DIR}/mesh.fbs")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(MAKE_DIRECTORY "${GENERATED_DIR}")

add_custom_command(
        OUTPUT "${GENERATED_DIR}/mesh_generated.h"
        COMMAND flatbuffers::flatc -c -o "${GENERATED_DIR}" "${SCHEMA_FILE}"
        DEPENDS "${SCHEMA_FILE}"
        COMMENT "Compiling FlatBuffers schema: mesh.fbs"
)

add_library(kiln_schema INTERFACE)
target_include_directories(kiln_schema INTERFACE "${GENERATED_DIR}")
target_link_libraries(kiln_schema INTERFACE flatbuffers::flatbuffers)

target_sources(kiln_schema INTERFACE "${GENERATED_DIR}/mesh_generated.h")