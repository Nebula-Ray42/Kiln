add_executable(kiln_tests
        baker_test.cpp
)

target_link_libraries(kiln_tests PRIVATE
        kiln_core
)

add_test(NAME KilnCoreTest COMMAND kiln_tests)