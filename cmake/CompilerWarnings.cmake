function(set_project_warnings kiln)
    target_compile_options(${kiln} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
endfunction()