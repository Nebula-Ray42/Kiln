function(set_project_warnings project_name)
    get_target_property(target_type ${project_name} TYPE)

    if ("${target_type}" STREQUAL "INTERFACE_LIBRARY")
        return()
    endif()

    target_compile_options(${project_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
endfunction()