add_library(baker_core STATIC
        baker_core.cpp
)

target_include_directories(baker_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)