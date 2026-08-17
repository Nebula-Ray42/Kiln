add_executable(kiln_tests
        baker_test.cpp
)

add_dependencies(kiln_tests kiln_schema_gen)

find_package(flatbuffers CONFIG REQUIRED)

target_link_libraries(kiln_tests PRIVATE flatbuffers::flatbuffers)

target_include_directories(kiln_tests PRIVATE ${CMAKE_SOURCE_DIR}/schema/generated)

add_test(NAME KilnCoreTest COMMAND kiln_tests)