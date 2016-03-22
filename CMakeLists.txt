cmake_minimum_required(VERSION 3.4)
project(libdrawille VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
