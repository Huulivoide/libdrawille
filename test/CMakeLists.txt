find_library(CMOCKA cmocka)

if (${CMOCKA_LIBRARY})
    enable_testing()

    add_executable(translate util.c translate.c)
    target_link_libraries(translate libdrawille ${CMOCKA})
    add_test(translations translate)

    add_executable(rotate util.c rotate.c)
    target_link_libraries(rotate libdrawille ${CMOCKA})
    add_test(rotations rotate)

    add_executable(scale util.c scale.c)
    target_link_libraries(scale libdrawille ${CMOCKA})
    add_test(scalings scale)

    add_executable(combine util.c combine.c)
    target_link_libraries(combine libdrawille ${CMOCKA})
    add_test(combinings combine)
endif()
