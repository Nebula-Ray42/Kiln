function(set_project_warnings project_name)
    target_compile_options(${project_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
endfunction()