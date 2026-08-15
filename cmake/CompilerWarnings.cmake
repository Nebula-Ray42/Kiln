function(set_project_warnings project_name)
    get_target_property(target_type ${project_name} TYPE)

    if ("${target_type}" STREQUAL "INTERFACE_LIBRARY")
        return()
    endif()

    if(MSVC)
        target_compile_options(${project_name} PRIVATE
                /W4
                /WX
                /w14242
                /w14254
                /w14265
        )
    else()
        target_compile_options(${project_name} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Werror
                -Wconversion
        )
    endif()
endfunction()