foreach(example rotate_square rotate_filled_pentagon)
    add_executable("${example}" "${example}.c")
    target_link_libraries("${example}" libdrawille)
endforeach()
