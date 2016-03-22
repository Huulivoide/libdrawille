add_executable(rotate_square rotate_square.c)
target_link_libraries(rotate_square libdrawille)

set_property(TARGET rotate_square PROPERTY C_STANDARD 11)
set_property(TARGET rotate_square PROPERTY C_STANDARD_REQUIRED ON)
