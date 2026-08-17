find_package(flatbuffers CONFIG REQUIRED)

set(SCHEMA_FILE "${CMAKE_CURRENT_SOURCE_DIR}/mesh.fbs")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(MAKE_DIRECTORY "${GENERATED_DIR}")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated/mesh_generated.h
        COMMAND flatc --cpp --python -o ${CMAKE_CURRENT_SOURCE_DIR}/generated ${CMAKE_CURRENT_SOURCE_DIR}/mesh.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mesh.fbs
        COMMENT "Compiling FlatBuffers schema: mesh.fbs (C++ and Python)"
)

add_custom_target(kiln_schema_gen ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generated/mesh_generated.h
)

add_library(kiln_schema INTERFACE)
target_include_directories(kiln_schema INTERFACE "${GENERATED_DIR}")
target_link_libraries(kiln_schema INTERFACE flatbuffers::flatbuffers)

target_sources(kiln_schema INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/generated/mesh_generated.h")