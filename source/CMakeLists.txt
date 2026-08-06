add_library(kiln_core STATIC
        baker_core.cpp
)

target_include_directories(kiln_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)