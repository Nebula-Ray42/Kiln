add_executable(kiln_main
        src/main.cpp
)

target_include_directories(kiln_main PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(kiln_main PRIVATE
        kiln_core
)