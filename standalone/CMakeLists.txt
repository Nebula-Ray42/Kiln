add_executable(kiln_main
        src/main.cpp
)

target_link_libraries(kiln_main PRIVATE
        baker_core
)