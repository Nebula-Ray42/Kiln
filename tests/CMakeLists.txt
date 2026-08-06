add_executable(baker_tests
        baker_test.cpp
)

target_link_libraries(baker_tests PRIVATE
        baker_core
)

add_test(NAME BakerCoreTest COMMAND baker_tests)