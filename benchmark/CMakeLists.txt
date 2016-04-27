foreach(example fill_triangle_bench)
    add_executable("${example}" "${example}.c")
    target_link_libraries("${example}" libdrawille)

    set_property(TARGET "${example}" PROPERTY C_STANDARD 11)
    set_property(TARGET "${example}" PROPERTY C_STANDARD_REQUIRED ON)
endforeach()
