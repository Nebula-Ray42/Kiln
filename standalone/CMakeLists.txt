add_executable(asset_baker
        main.cpp
)

target_link_libraries(asset_baker PRIVATE
        baker_core
)